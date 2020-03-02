/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2006                                                *
 *                                                                         *
 *      Equipe C.M.A.O                                                     *
 *      Laboratoire Genie de production                                    *
 *      Ecole Nationale d'Ingenieurs de Tarbes                             *
 *      BP 1629 - 65016 TARBES cedex                                       *
 *                                                                         *
 *                                                                         *
 *  Main Author: Olivier PANTALE                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

// begin date :
// revision date :

/*
	Class Structure implementation
*/

#include <io_Data.h>
#include <Material.h>
#include <HistoryFile.h>
#include <dirent.h>
#include <Solver.h>
#include <Interface.h>
#include <Structure.h>
#include <Domain.h>
#include <io_Structure.h>
#include <Elements.h>
#include <Node.h>
#include <Select.h>
#include <Boundary.h>

#define nodeDisplayOnlineFrequency 100
#define elementDisplayOnlineFrequency 100

Structure *Global_Structure = NULL; // initialisation par défaut sur NULL
extern String parsedFileName;
extern CPUrecord recordTimes;

/*!
  \file Structure.C
  \brief fichier .C de définition des structures éléments finis
  \ingroup femLibrary

  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/

//!constructeur par défaut de la classe Structure
/*!
  Cette méthode est le constructeur par défaut de la classe Structure. En pratique, ici, on ne fait rien de plus que les allocations de mémoire nécessaires.
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Structure::Structure()
//-----------------------------------------------------------------------------
{
  // on construit ici un domaine par défaut et on l'ajoute

  name = "defaultStructure";
  domains << new Domain;

  // créer les fichiers de sauvegarde
  resultFile = new io_Data;
  String inter_name = parsedFileName.before(sourceFileExtension);
  resultFile->binaryFile() = True;
  resultFile->link(inter_name);
  resultFile->link(this);

  // on crée également un fichier log pour la lecture des données
  logFile = new LogFile(inter_name + ".log");

  // attacher la structure globale à celle-ci
  if (Global_Structure != NULL)
  {
    fatalError("Structure::Structure", "Only One structure is allowed for a Finite Element program");
  }
  Global_Structure = this;

  // initialisations par défaut
  startSaveTime = 0;
  endSaveTime = 0;
  saveTime = 0;
  nextSaveTime = 0;
  previousSaveTime = -1;
  currentTime = 0;
  defaultElement = NULL;

  // Creates and start a global timer
  cpuTimes.timer("Global")->start();
  cpuTimes.add(new Timer("Global:Solver"));
  cpuTimes.add(new Timer("Solver:computeInternalMatrices"));
  cpuTimes.add(new Timer("Solver:computeInternalForces"));
    cpuTimes.add(new Timer("Solver:computeTimeStep"));

}

//!constructeur par recopie de la classe Structure
/*!
  Cette méthode est le constructeur par recopie de la classe Structure. En pratique, ici, on ne fait rien de plus que les allocations de mémoire nécessaires.
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
Structure::Structure(const Structure &X)
//-----------------------------------------------------------------------------
{
}

//!destructeur de la classe Structure
/*!
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
Structure::~Structure()
//-----------------------------------------------------------------------------
{
  // on détruit le logFile
  delete logFile;
}

//!création d'un noeud et ajout à la structure
/*!
  Cette méthode crée un nouveau noeud et l'ajoute à la fois à la liste des noeuds de la structure et à la liste des noeuds de la grille courante du domaine courant.
  \param num numéro du nouveau noeud à créer
  \param x coordonnée x du noeud à créer
  \param y coordonnée y du noeud à créer
  \param z coordonnée z du noeud à créer
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
void Structure::createNode(Indice num, Real x, Real y, Real z)
//-----------------------------------------------------------------------------
{
  // pointeur sur le nouveau noeud
  Node *pnd = new Node(num);

  // chargement des coordonnees
  pnd->coords(0) = x;
  pnd->coords(1) = y;
  pnd->coords(2) = z;

  // l'ajouter à la grille courante du domaine courant
  domains.current()->add(pnd);

  // add the node to the grid
  nodes << pnd;

  if (nodes.size() % nodeDisplayOnlineFrequency == 0)
    displayOnline();

  // logFile
  char st[80];
  sprintf(st, "%ld [%lf,%lf,%lf]", pnd->number, pnd->coords(0), pnd->coords(1), pnd->coords(2));
  (*logFile) << "Node : " << st << " added to structure\n";
}

//!recherche d'un noeud dans la structure en fonction de son numéro
/*!
  Cette méthode recherche un noeud dans la structure en fonction de son numéro et renvoie un pointeur sur celui-ci, ou NULL si celui-ci n'existe pas dans la structure. Le noeud est recherché sur la grille courante du domaine courant.
  \param num numéro du noeud à rechercher
  \return pointeur sur le noeud trouvé ou NULL en cas d'échec de recherche
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
Node *Structure::getNodeByNum(Indice num)
//-----------------------------------------------------------------------------
{
  // pehaps it's just the last one (often assumed)
  if (getCurrentDomain()->nodes.size() > 0)
  {
    if (getCurrentDomain()->nodes.last()->number == num)
      return getCurrentDomain()->nodes.last();
  }

  // no so search for it
  return getCurrentDomain()->nodes.dichotomySearch(diffNodeNumber, num);
}

//!création d'un élément et ajout à la structure
/*!
  Cette méthode crée un nouvel élément et l'ajoute à la fois à la liste des éléments de la structure et à la liste des éléments de la grille courante du domaine courant. Pour faire appel à cette méthode, il faut au préalable avoir au moins une fois fait appel à la méthode setDefaultElement() qui permet de définir la nature de l'élément par défaut à utiliser par la suite dans la création des nouveaux éléments.

Cette méthode doit être modifiée à chaque fois que l'on crée un nouveau type d'élément fini. Elle sert de point d'entrée pour la lecture des éléments dans les fichier .dyn de deuxième génération.
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
void Structure::createElement(Indice num, ...)
//-----------------------------------------------------------------------------
{
  Indice nbNodes;
  Indice nNodes[50];

  // test if the default type is set
  if (defaultElement == NULL)
  {
    fatalError("Structure::createElement", "Element type not set\n");
  }

  nbNodes = defaultElement->getNumberOfNodes();

  // A place to store the list of arguments
  va_list arguments;

  //Initializing arguments to store all values
  va_start(arguments, num);

  for (Indice i = 0; i < nbNodes; i++)
  {
    nNodes[i] = va_arg(arguments, int);
  }

  // nouveau pointeur
  Element *pel = NULL;

  switch (defaultElement->getType())
  {
  case Element::ElQua4n2D:
    pel = new ElQua4n2D(num);
    break;
  case Element::ElTri3n2D:
    pel = new ElTri3n2D(num);
    break;
  case Element::ElQua4nAx:
    pel = new ElQua4nAx(num);
    break;
  case Element::ElHex8n3D:
    pel = new ElHex8n3D(num);
    break;
  case Element::ElTet4n3D:
    pel = new ElTet4n3D(num);
    break;
  case Element::ElTet10n3D:
    pel = new ElTet10n3D(num);
    break;
  default:
    fatalError("Structure::createElement", "Unknown element type");
  }

  domains.current()->createElement(pel, nNodes);

  // add the element to the grid
  elements << pel;

  String str;
  (*logFile) << "Element " << pel->getName() << " : " << str.convert(pel->number) << " [";
  for (Indice i = 0; i < nbNodes; i++)
  {
    if (i != 0)
      (*logFile) << ", ";
    (*logFile) << str.convert(pel->nodes(i)->number);
  }
  (*logFile) << "] added to structure\n";

  //Cleans up the list
  va_end(arguments);

  if (elements.size() % elementDisplayOnlineFrequency == 0)
    displayOnline();
}

//!recherche d'un élément dans la structure en fonction de son numéro
/*!
  Cette méthode recherche un élément dans la structure en fonction de son numéro et renvoie un pointeur sur celui-ci, ou NULL si celui-ci n'existe pas dans la structure. L'élément est recherché sur la grille courante du domaine courant.
  \param num numéro de l'élément à rechercher
  \return pointeur sur l'élément trouvé ou NULL en cas d'échec de recherche
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
Element *Structure::getElementByNum(Indice num)
//-----------------------------------------------------------------------------
{
  // pehaps it's just the last one (often assumed)
  if (getCurrentDomain()->elements.size() > 0)
  {
    if (getCurrentDomain()->elements.last()->number == num)
      return getCurrentDomain()->elements.last();
  }

  // no so search for it
  return getCurrentDomain()->elements.dichotomySearch(diffElementNumber, num);
}

//!Initialise les structures mémoire pour la résolution
/*!
  Cette méthode est utilisée pour initialiser les structures mémoire après lecture des données dans le fichier source. C'est la phase de préprocessing de la structure. Un ensemble de vérifications sont effectuées dans cette méthode.

  \return True si la méthode n'a généré aucune erreur
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean Structure::initSolve()
//-----------------------------------------------------------------------------
{
  Indice i;

  cout << "\nPreProcessing of the structure ...\n";

  // log file
  logFile->separatorWrite();
  (*logFile) << "\nInitialisation of solver phase ........\n\nStructure architecture\n";

  // log file
  (*Global_Structure->logFile) << elements.size() << " elements\n"
                               << nodes.size() << " nodes\n";

  // initialisation du temps
  currentTime = 0;
  // initialisation des domaines
  for (i = 0; i < domains.size(); i++)
  {
    (*Global_Structure->logFile) << "\nVerification of domain " << i << " ...\n";
    if (domains(i)->initSolve() == False)
    {
      // domain has to be deleted
      (*Global_Structure->logFile) << "Void Domain " << i << " has been deleted ...\n";
      delete domains(i);
      domains.del(i);
      i--;
    }
  }
  (*Global_Structure->logFile) << "\nPhysic contains " << i << " domain" << (i > 1 ? "s" : "") << " ...\n\n";

  // si pas de dommaines, alors pas de structure
  if (domains.size() == 0)
    return (False);

  // verification de la coh�rence des mat�riaux
  for (i = 0; i < materials.size(); i++)
  {
    (*logFile) << "Verification of material " << i << " named " << materials(i)->name << " ...\n";
    materials(i)->checkValues();
  }

  // calcul des materiaux
  for (i = 0; i < materials.size(); i++)
  {
    (*logFile) << "Initialisation of material " << i << " named " << materials(i)->name << " ...\n";
    materials(i)->compute_C();
  }

  (*logFile) << "\nEnd of initialisation of solver phase ........\n";

  // preparation des save times
  if (startSaveTime == 0)
    startSaveTime = saveTime;
  nextSaveTime = startSaveTime;

  nodes.sort();
  elements.sort();

  return (True);
}

//!recherche si un matériau existe actuellement
/*!
  Cette méthode est utilisée pour récupérer un matériau donné à partir de son nom dans une structure. Elle cherche un matériau donné en fonction de son nom et renvoie un pointeur sur ce matériau (s'il existe) ou NULL dans le cas où le matériau n'a pu être trouvé dans la liste des matériaux de la structure.

  \param name nom du matériau à récupérer
  \return un pointeur sur ce matériau (s'il existe) ou NULL dans le cas où le matériau n'a pu être trouvé dans la liste des matériaux de la structure.
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Material *Structure::getMaterial(String name)
//-----------------------------------------------------------------------------
{
  // balayage de la liste des matériaux
  for (Indice i = 0; i < materials.size(); i++)
  {
    // matériau trouvé
    if (materials(i)->name == name)
      return materials(i);
  }

  // matériau non trouvé
  return NULL;
}

//!ajoute un matériau à la structure
/*!
  Cette méthode ajoute un nouveau matériau à la structure. Le nouveau matériau est initialisé et renseigné de manière externe. Cette méthode est uniquement destinée à le stocker au niveau de la stucture. Il est ensuite possible d'y faire référence à partir nu nom de ce matériau. Cette méthode vérifie que lorsque l'on ajoute un matériau, si le nom est déclaré, un autre matériau portant le même nom n'est pas déje présent dans la liste des matériaux. Dans ce cas, une erreur est alors générée.

  \param pmat pointeur sur le nouveau matériau à ajouter à la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::addMaterial(Material *pmat)
//-----------------------------------------------------------------------------
{
  Indice i;
  if (pmat->name != "")
  {
    for (i = 0; i < materials.size(); i++)
    {
      if (materials(i)->name == pmat->name)
      {
        fatalError("Structure::addMaterial", "Material %s already exist\n", pmat->name.chars());
      }
    }
  }

  // ajout à la liste courante
  materials << pmat;

  // logFile
  (*logFile) << "Material " << (pmat->name == "" ? "WITHOUT NAME" : pmat->name) << " added to structure\n";
}

//!affecte un matériau à un ensemble d'éléments
/*!
  Cette méthode affecte un matériau à un ensemble d'éléments de la structure.

  \param mat matériau à utiliser
  \param els ElementSet à utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::attachMaterialToElements(Material *mat, ElementSet *els)
//-----------------------------------------------------------------------------
{
  Indice i;
  Boolean already = False;

  // attach all elements of the group
  for (i = 0; i < els->size(); i++)
  {
    els->elements(i)->attachMaterial(mat);

    // logFile
    (*logFile) << "Material " << (mat->name == "" ? "WITHOUT NAME" : mat->name) << " attached to element " << els->elements(i)->number << "\n";
  }

  // attach the group to the material
  mat->elementsSet << els;

  // attach the material to the structure if not already done
  for (i = 0; i < materials.size(); i++)
  {
    if (materials(i) == mat)
    {
      already = True;
      i = materials.size();
    }
  }
  if (already == False)
    addMaterial(mat);
}

//!affecte des conditions aux limites à un ensemble de noeuds
/*!
  Cette méthode affecte des conditions aux limites à un ensemble de noeuds de la structure.

  \param BC condition limite à utiliser
  \param nds NodeSet à utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::attachBCToNodes(BoundaryCondition *BC, NodeSet *nds)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < nds->size(); i++)
  {
    nds->nodes(i)->boundary = BC;

    // logFile
    (*logFile) << "A boundary condition has been attached to node " << nds->nodes(i)->number << "\n";
  }
}

//!affecte des conditions aux limites initiales à un ensemble de noeuds
/*!
  Cette méthode affecte des conditions aux limites à un ensemble de noeuds de la structure.

  \param bnd condition limite à utiliser
  \param nds NodeSet à utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::attachInitialBC(Boundary *bnd, NodeSet *nds)
//-----------------------------------------------------------------------------
{
  BoundaryCondition *BC;

  for (Indice i = 0; i < nds->size(); i++)
  {

    // test if boundary exists already
    if ((BC = nds->nodes(i)->boundary) == NULL)
    {

      // no, we have to create it
      BC = new BoundaryCondition;
      nds->nodes(i)->boundary = BC;
      (*logFile) << "Boundary condition created for node " << nds->nodes(i)->number << "\n";
    }

    // add the boundary condition
    BC->addInitial(bnd);

    // logFile
    (*logFile) << "Initial boundary condition attached to node " << nds->nodes(i)->number << "\n";
  }

  // ajout du node set de reference
  bnd->nodeSet << nds;
  Boolean tp;
  tp = 0;
  bnd->typeSet << tp;
}

//!affecte des conditions aux limites constantes à un ensemble de noeuds
/*!
  Cette méthode affecte des conditions aux limites à un ensemble de noeuds de la structure.

  \param bnd condition limite à utiliser
  \param nds NodeSet à utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::attachConstantBC(Boundary *bnd, NodeSet *nds)
//-----------------------------------------------------------------------------
{
  BoundaryCondition *BC;

  for (Indice i = 0; i < nds->size(); i++)
  {

    // test if boundary exists already
    if ((BC = nds->nodes(i)->boundary) == NULL)
    {

      // no, we have to create it
      BC = new BoundaryCondition;
      nds->nodes(i)->boundary = BC;
      (*logFile) << "Boundary condition created for node " << nds->nodes(i)->number << "\n";
    }

    // add the boundary condition
    BC->addConstant(bnd);

    // logFile
    (*logFile) << "Constant boundary condition attached to node " << nds->nodes(i)->number << "\n";
  }

  // ajout du node set de reference
  bnd->nodeSet << nds;
  Boolean tp;
  tp = 1;
  bnd->typeSet << tp;
}

//!ajoute un solveur à la structure
/*!
  Cette méthode ajoute un solveur à la structure. Le solveur est ajouté à la liste des solveurs du domaine courant de la structure.

  \param solver solveur à utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::addSolver(Solver *solver)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(solver != NULL);
#endif

  // ajouter le solveur au domaine courant
  domains.current()->solvers << solver;

  // logFile
  (*logFile) << "Solver " << solver->name << " linked to current domain\n";
}

//-----------------------------------------------------------------------------
void Structure::addHistoryFile(HistoryFile *historyFile)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(historyFile != NULL);
#endif

  // ajouter le solveur au domaine courant
  domains.current()->historyFiles << historyFile;

  // logFile
  (*logFile) << "HistoryFile " << historyFile->name << " linked to current domain\n";
}

//!renvoie le nombre de noeuds de la structure
/*!
  Cette méthode renvoie le nombre total de noeuds de la structure

  \return nombre de noeuds de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Indice Structure::getNodesNumber()
//-----------------------------------------------------------------------------
{
  return nodes.size();
}

//!renvoie le nombre d'éléments de la structure
/*!
  Cette méthode renvoie le nombre total d'éléments de la structure

  \return nombre d'éléments de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Indice Structure::getElementsNumber()
//-----------------------------------------------------------------------------
{
  return elements.size();
}

//!renvoie le ième noeud de la structure
/*!
  Cette méthode renvoie le ième noeud de la structure

  \param i indice du noeud
  \return ième noeud de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Node *Structure::getNode(Indice i)
//-----------------------------------------------------------------------------
{
  return nodes(i);
}

//!renvoie le ième élément de la structure
/*!
  Cette méthode renvoie le renvoie le ième élément de la structure

  \param i indice de l'élément
  \return ième élément de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Element *Structure::getElement(Indice i)
//-----------------------------------------------------------------------------
{
  return elements(i);
}

//!renvoie un noeud en fonction de son Id
/*!
  Cette méthode renvoie un noeud en fonction de son Id

  \param i Id du noeud
  \return ième noeud de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Node *Structure::getNodeById(Indice i)
//-----------------------------------------------------------------------------
{
  return nodes.AppN(i);
}

//!renvoie un élément en fonction de son Id
/*!
  Cette méthode renvoie un élément en fonction de son Id

  \param i Id de l'élément
  \return ième noeud de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Element *Structure::getElementById(Indice i)
//-----------------------------------------------------------------------------
{
  return elements.AppN(i);
}

//!sélectionne une physique dans la structure
/*!
  Cette méthode permet de créer un nouveau domaine dans la structure ou de sélectionner un autre domaine pour le domaine courant de la structure.

  \param domain pointeur sur le domaine à sélectionner
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::setDomain(Domain *domain)
//-----------------------------------------------------------------------------
{
  //  Indice ind;
  // si le domaine existe déja, on n'a pas à le créer
  if (!domains.contains(domain))
  {
    domains << domain;
  }

  // change the current index
  domains(domains.getIndex(domain));
  (*logFile) << "Structure: " << name << " domain: " << domains.current()->name << " selected\n";
}
//!calcule les coordonnées mini et maxi de l'ensemble des noeuds d'une structure
/*!
  Cette méthode calcule l'enveloppe mini et maxi des coordonnées des points d'uns structure complète

  \param min coordonnée minimale
  \param max coordonnée maximale
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::getGlobalBox(Vec3D &min, Vec3D &max)
//-----------------------------------------------------------------------------
{
  Vec3D coords;
  Indice i, j;
  // affectation par defaut au commencement
  max = min = nodes(0)->coords;

  // boucle de recherche
  for (i = 1; i < nodes.size(); i++)
  {
    coords = nodes(i)->coords;

    for (j = 0; j < 3; j++)
    {
      if (coords(j) < min(j))
        min(j) = coords(j);
      if (coords(j) > max(j))
        max(j) = coords(j);
    }
  }
}

//!affiche pendant la lecture des données un état d'avancement sur la console
/*!
  Cette méthode affiche pendant la lecture des données un état d'avancement sur la console du remplissage mémoire concernant les nombres de noeuds, éléments et domaines de la structure complète.

  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::displayOnline()
//-----------------------------------------------------------------------------
{
  printf("\rRead %ld domain%s %ld node%s %ld element%s %ld material%s",
         domains.size(), (domains.size() > 1 ? "s" : ""),
         nodes.size(), (nodes.size() > 1 ? "s" : ""),
         elements.size(), (elements.size() > 1 ? "s" : ""),
         materials.size(), (materials.size() > 1 ? "s" : ""));

  fflush(stdout);
}

//-----------------------------------------------------------------------------
void Structure::saveResults()
//-----------------------------------------------------------------------------
{
  if ((currentTime >= nextSaveTime) && (currentTime != previousSaveTime))
  {

    // transfer the datas
    resultFile->transfert();

    // increment saveTimes
    previousSaveTime = currentTime;
    nextSaveTime += saveTime;
  }
}

//!lancement du solveur général
/*!
  Cette méthode lance la procédure de solveur général de la structure. Elle prend en compte tous les types de solveurs possible et gère aussi bien la résolution mono-domaine que la résolution multi-domaine. C'est le point d'entrée de tout solveur.

  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Structure::solve()
//-----------------------------------------------------------------------------
{
  Indice i;
  Real nextTime;
  Boolean cont;
  Real endTime;

  // affichage
  cout << "\nProcessing structure ...\n";
  logFile->separatorWrite();
  (*logFile) << "Lanching Structure Solver phase ........\n\n";

  // Start Solver timer
  cpuTimes.timer("Solver")->start();

  // temps de la structure
  endTime = domains(0)->getEndSolveTime();
  for (i = 1; i < domains.size(); i++)
  {
    endTime = Min(domains(i)->getEndSolveTime(), endTime);
  }

  nextTime = 0;

  // Only one domain
  if (domains.size() == 1)
  {
    while (nextTime < endTime)
    {
      cont = False;

      // what is the next time
      nextTime = Global_Structure->nextSaveTime;

      // lancement du solveur
      if (domains(0)->solve(nextTime) == False)
      {
        Global_Structure->saveResults();
        fatalError("Solver Error", "Unable to solve problem upto time = %10.3E\n", nextTime);
      }

      // r�cup�ration du temps de la structure
      Global_Structure->currentTime = domains(0)->currentTime;

      // test pour la sauvegarde d'un fichier interm�diaire
      Global_Structure->saveResults();
    }
  }

  else
  {
    cont = True;
    while (cont)
    {
      cont = False;

      // what is the next time
      nextTime = Global_Structure->currentTime + Global_Structure->saveTime / 500;

      for (i = 0; i < domains.size(); i++)
      {
        if (domains(i)->solve(nextTime) == True)
          cont = True;
      }

      // Synchronize times
      Global_Structure->currentTime = Min(domains(0)->currentTime, domains(0)->currentTime);

      Global_Structure->saveResults();
    }
  }

  // final result if available
  Global_Structure->saveResults();

  cout << "end of solve\n";
  // print the CPU times
  recordTimes.report("CPU-TIMES");

  // Stop the timer for the solver
  cpuTimes.timer("Solver")->stop();

  // Stop all time logs
  cpuTimes.stop();
  cout << "RECORD TIMES\n";

  cpuTimes.saveReport("CPU-TIMES-2.txt");
}

//-----------------------------------------------------------------------------
Real Structure::getReadTimeData(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  Real tim;

  if (checkBinaryVersion(pfile, 0) != Ok)
    fatalError("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read((char *)&tim, sizeof(Real));

  return tim;
}

//-----------------------------------------------------------------------------
void Structure::readData(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  Indice i;

  if (checkBinaryVersion(pfile, 0) != Ok)
    fatalError("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read((char *)&currentTime, sizeof(Real));

  // load the domains
  for (i = 0; i < domains.size(); i++)
  {
    domains(i)->readData(pfile);
    if (checkBinaryVersion(pfile, 1) != Ok)
      fatalError("domain datas", "Read error");
  }
}

//-----------------------------------------------------------------------------
void Structure::writeData(ofstream &pfile)
//-----------------------------------------------------------------------------
{
  Indice i;

  checkBinaryVersionWrite(pfile, 0);

  // save current time
  pfile.write((char *)&currentTime, sizeof(Real));

  // load the domains
  for (i = 0; i < domains.size(); i++)
  {
    domains(i)->writeData(pfile);
    checkBinaryVersionWrite(pfile, 1);
  }
}

//-----------------------------------------------------------------------------
Boolean
Structure::checkBinaryVersion(ifstream &pfile, int top)
//-----------------------------------------------------------------------------
{
  Vec3D version, check;

  if (top == 0)
  {
    pfile >> version;
    pfile >> check;

    if (check(0) != versionCheckDefine)
    {
      cerr << "File recorded by DynELA v." << (int)version(0) << "." << (int)version(1) << "." << (int)version(2) << "-" << (int)check(1) << endl;
      cerr << endl
           << "This recording format is not compatible with ";
      cerr << "DynELA v." << VERSION << "." << RELEASE << "." << UNDER_RELEASE << "-" << ALPHA_VERSION << endl;
      cerr << "\nYou MUST delete previous results and rerun the solver\n";
      return Fail;
    }
    return Ok;
  }
  else
  {
    pfile >> check;
    if (check(0) != separatorCheckDefine)
    {
      cerr << "\n May be file is trucated\n";
      return Fail;
    }
    return Ok;
  }
}

//-----------------------------------------------------------------------------
void Structure::checkBinaryVersionWrite(ofstream &pfile, int top)
//-----------------------------------------------------------------------------
{
  Vec3D version, check;

  version(0) = (Real)VERSION;
  version(1) = (Real)RELEASE;
  version(2) = (Real)UNDER_RELEASE;
  check(0) = versionCheckDefine;
  check(1) = ALPHA_VERSION;

  if (top == 0)
  {
    check(0) = versionCheckDefine;
    pfile << version;
    pfile << check;
  }
  else
  {
    check(0) = separatorCheckDefine;
    pfile << check;
  }
}

//-----------------------------------------------------------------------------
void Structure::setSaveTimes(Real startST, Real stopST, Real ST)
//-----------------------------------------------------------------------------
{
  startSaveTime = startST;
  endSaveTime = stopST;
  saveTime = ST;
}

//-----------------------------------------------------------------------------
void Structure::sortElementsAndNodes()
//-----------------------------------------------------------------------------
{
  nodes.compact();
  elements.compact();
}

//-----------------------------------------------------------------------------
void Structure::setDefaultElement(Element *pel)
//-----------------------------------------------------------------------------
{
  defaultElement = pel;
}

//-----------------------------------------------------------------------------
void Structure::addInterface(Interface *pinter)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(pinter != NULL);
#endif

  domains.current()->addInterface(pinter);
  (*logFile) << "Interface : " << pinter->name << " added to structure\n";
}

//-----------------------------------------------------------------------------
void Structure::translate(Vec3D vec, NodeSet *pns)
//-----------------------------------------------------------------------------
{
  if (pns != NULL)
    for (Indice i = 0; i < pns->size(); i++)
      pns->nodes(i)->coords += vec;
  else
    for (Indice i = 0; i < nodes.size(); i++)
      nodes(i)->coords += vec;
}

//-----------------------------------------------------------------------------
void Structure::scale(Real val, NodeSet *pns)
//-----------------------------------------------------------------------------
{
  if (pns != NULL)
    for (Indice i = 0; i < pns->size(); i++)
      pns->nodes(i)->coords *= val;
  else
    for (Indice i = 0; i < nodes.size(); i++)
      nodes(i)->coords *= val;
}

//-----------------------------------------------------------------------------
void Structure::scale(Vec3D val, NodeSet *pns)
//-----------------------------------------------------------------------------
{
  if (pns != NULL)
    for (Indice i = 0; i < pns->size(); i++)
      for (Indice c = 0; c < 3; c++)
        pns->nodes(i)->coords(c) *= val(c);
  else
    for (Indice i = 0; i < nodes.size(); i++)
      for (Indice c = 0; c < 3; c++)
        nodes(i)->coords(c) *= val(c);
}

//-----------------------------------------------------------------------------
void Structure::rotate(String set, Real val, NodeSet *pns)
//-----------------------------------------------------------------------------
{
  Boolean flag = False;
  Tensor2 Mat;
  Mat = 0;

  // rotation autour de X
  if (set == "X" || set == "x")
  {
    Mat(0, 0) = 1;
    Mat(1, 1) = cos(val * DegToRad);
    Mat(1, 2) = sin(val * DegToRad);
    Mat(2, 1) = -sin(val * DegToRad);
    Mat(2, 2) = cos(val * DegToRad);
    flag = True;
  }

  // rotation autour de Y
  if (set == "Y" || set == "y")
  {
    Mat(0, 0) = cos(val * DegToRad);
    Mat(0, 2) = -sin(val * DegToRad);
    Mat(1, 1) = 1;
    Mat(2, 0) = sin(val * DegToRad);
    Mat(2, 2) = cos(val * DegToRad);
    flag = True;
  }

  // rotation autour de Z
  if (set == "Z" || set == "z")
  {
    Mat(0, 0) = cos(-val * DegToRad);
    Mat(0, 1) = sin(-val * DegToRad);
    Mat(1, 0) = -sin(-val * DegToRad);
    Mat(1, 1) = cos(-val * DegToRad);
    Mat(2, 2) = 1;
    flag = True;
  }

  if (pns != NULL)
    for (Indice i = 0; i < pns->size(); i++)
      pns->nodes(i)->coords = Mat * pns->nodes(i)->coords;
  else
    for (Indice i = 0; i < nodes.size(); i++)
      nodes(i)->coords = Mat * nodes(i)->coords;
}

//-----------------------------------------------------------------------------
void Structure::rotate(Vec3D axis, Real val, NodeSet *pns)
//-----------------------------------------------------------------------------
{
  Tensor2 Mat;

  Real r0 = val * DegToRad;

  // normalisation de l'axe
  axis.normalize();

  Mat(0, 0) = SQ(axis(0)) + cos(r0) * (-SQ(axis(0)) + 1);
  Mat(0, 1) = axis(0) * axis(1) - axis(2) * sin(r0) - axis(0) * axis(1) * cos(r0);
  Mat(0, 2) = axis(0) * axis(2) + axis(1) * sin(r0) - axis(0) * axis(2) * cos(r0);
  Mat(1, 0) = axis(0) * axis(1) + axis(2) * sin(r0) - axis(0) * axis(1) * cos(r0);
  Mat(1, 1) = SQ(axis(1)) + cos(r0) * (-SQ(axis(1)) + 1);
  Mat(1, 2) = axis(1) * axis(2) - axis(0) * sin(r0) - axis(1) * axis(2) * cos(r0);
  Mat(2, 0) = axis(0) * axis(2) - axis(1) * sin(r0) - axis(0) * axis(2) * cos(r0);
  Mat(2, 1) = axis(1) * axis(2) + axis(0) * sin(r0) - axis(1) * axis(2) * cos(r0);
  Mat(2, 2) = SQ(axis(2)) + cos(r0) * (-SQ(axis(2)) + 1);

  if (pns != NULL)
    for (Indice i = 0; i < pns->size(); i++)
      pns->nodes(i)->coords = Mat * pns->nodes(i)->coords;
  else
    for (Indice i = 0; i < nodes.size(); i++)
      nodes(i)->coords = Mat * nodes(i)->coords;
}

//-----------------------------------------------------------------------------
void Structure::compact()
//-----------------------------------------------------------------------------
{
  Indice i, j, k, l;
  Domain *pdomain;
  Indice elementNum, nodeNum;

  for (j = 0; j < domains.size(); j++)
  {
    pdomain = domains(j);
    elementNum = 1;
    nodeNum = 1;
    for (l = 0; l < pdomain->nodes.size(); l++)
    {
      pdomain->nodes(l)->number = nodeNum++;
    }
    for (l = 0; l < pdomain->elements.size(); l++)
    {
      pdomain->elements(l)->number = elementNum++;
    }
  }
}

//-----------------------------------------------------------------------------
void Structure::readResultFile(Indice num)
//-----------------------------------------------------------------------------
{
  // get the old mode
  char oldMode = resultFile->getMode();

  // set mode to read mode
  resultFile->setMode(Read);

  // set the increment
  resultFile->incrementalFile() = True;
  resultFile->startCurrentSave(num);

  // get the transfert
  resultFile->transfert();

  // rest the mode
  resultFile->setMode(oldMode);

  // logFile
  (*logFile) << "Result datafile " << resultFile->getFileName() << " loaded\n";
}

//-----------------------------------------------------------------------------
Real Structure::getCurrentTime()
//-----------------------------------------------------------------------------
{
  return currentTime;
}

//-----------------------------------------------------------------------------
void Structure::getDataFileBounds(Indice &min, Indice &max)
//-----------------------------------------------------------------------------
{
  dirent *file;
  DIR *direct;
  direct = opendir(".");
  String sub;
  Boolean v0;
  Indice val;
  String filename = resultFile->getFileName();

  // skip file extension if exists
  if (filename.contains(sourceFileExtension))
    filename = filename.before(sourceFileExtension);

  v0 = False;
  Boolean ok = False;

  if (!direct)
  {
    fatalError("readDirectory", "can't read directory");
  }

  while ((file = readdir(direct)))
  {
    String name(file->d_name);
    if ((name.contains(filename)) && (name.contains(resultFileExtension)))
    {
      sub = name.before(resultFileExtension);
      sub = sub.after(filename);
      sub = sub.after(".");
      val = sub.getInt();
      if (!v0)
      {
        min = max = val;
        v0 = True;
      }
      if (val < min)
        min = val;
      if (val > max)
        max = val;
      ok = True;
    }
  }
  if (!ok)
    min = max = 0;
}

//-----------------------------------------------------------------------------
Indice Structure::getLastDataFileNumber()
//-----------------------------------------------------------------------------
{
  Indice min, max;
  getDataFileBounds(min, max);
  return max;
}

//-----------------------------------------------------------------------------
Indice Structure::getFirstDataFileNumber()
//-----------------------------------------------------------------------------
{
  Indice min, max;
  getDataFileBounds(min, max);
  return min;
}

//-----------------------------------------------------------------------------
Real Structure::getNodalValue(Indice nod, String field, Indice component)
//-----------------------------------------------------------------------------
{
  return getNodeByNum(nod)->getNodalValue(field, component);
}

//-----------------------------------------------------------------------------
Vec3D Structure::getNodalVector(Indice nod, String field)
//-----------------------------------------------------------------------------
{
  return getNodeByNum(nod)->getNodalVector(field);
}

//-----------------------------------------------------------------------------
Tensor2 Structure::getNodalTensor(Indice nod, String field)
//-----------------------------------------------------------------------------
{
  return getNodeByNum(nod)->getNodalTensor(field);
}

//-----------------------------------------------------------------------------
void Structure::setResultFile(String file)
//-----------------------------------------------------------------------------
{
  resultFile->getFileName() = file;
}

//-----------------------------------------------------------------------------
void Structure::mergeDomains()
//-----------------------------------------------------------------------------
{
  // if only one grid;
  if (domains.size() == 1)
    return;

  Indice nnum = 1, elnum = 1;
  Indice i, j;
  Domain *pdomaind, *pdomaino;

  pdomaind = domains(0);

  for (j = 0; j < pdomaind->elements.size(); j++)
  {
    pdomaind->elements(j)->number = elnum++;
  }
  for (j = 0; j < pdomaind->nodes.size(); j++)
  {
    pdomaind->nodes(j)->number = nnum++;
  }

  for (i = 1; i < domains.size(); i++)
  {
    pdomaino = domains(i);

    // copy elements
    for (j = 0; j < pdomaino->elements.size(); j++)
    {
      pdomaind->elements << pdomaino->elements(j);
      pdomaino->elements(j)->number = elnum++;
    }

    // copy nodes
    for (j = 0; j < pdomaino->nodes.size(); j++)
    {
      pdomaind->nodes << pdomaino->nodes(j);
      pdomaino->nodes(j)->number = nnum++;
    }
  }

  domains.del(1, domains.size() - 1);
}
