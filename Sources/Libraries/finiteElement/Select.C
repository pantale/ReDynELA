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
 **************************************************************************/

// begin date :
// revision date :

/*
  Class Set implementation
*/

/*!
  \file Select.C
  \brief fichier .C de d�finition des groupes de noeuds et �lements
  \ingroup femLibrary

  Ce fichier sert � la d�finition des groupes de noeuds et �lements de la structure.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#include <Select.h>
#include <Node.h>
#include <Element.h>
#include <Domain.h>

#include <Structure.h>

//!constructeur de la classe NodeSet
/*!
  Cette m�thode construit un NodeSet vide.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
NodeSet::NodeSet()
//-----------------------------------------------------------------------------
{
}

//!constructeur par recopie de la classe NodeSet
/*!
  Cette m�thode construit un NodeSet vide par recopie d'un autre. 
  Elle n'est pas finalis�e.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
NodeSet::NodeSet(const NodeSet &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely NodeSet class" << /*(int) */ &X << endl;
  exit(-1);
}

//!destructeur de la classe NodeSet
/*!
  Cette m�thode d�truit un NodeSet.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
NodeSet::~NodeSet()
//-----------------------------------------------------------------------------
{
}

//!ajoute un noeud � un NodeSet
/*!
  Cette m�thode ajoute un noeud existant � un NodeSet. Le noeud est directement sp�cifi� par son pointeur, aucune v�rification n'est effectu�e par cette m�thode.
  \param nd pointeur sur un noeud
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
void NodeSet::add(Node *nd)
//-----------------------------------------------------------------------------
{
  nodes << nd;
}

//!r�cup�re un pointeur sur le i�me noeud d'un node set
/*!
  Cette m�thode renvoie un pointeur sur le i�me noeud d'une liste de noeuds contenue dans un NodeSet.
  \param i place du noeud dans la liste.
  \return pointeur sur le noeud
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
Node *
NodeSet::getNode(Indice i)
//-----------------------------------------------------------------------------
{
  return (nodes(i));
}

//!r�cup�re le num�ro du i�me noeud d'un node set
/*!
  Cette m�thode renvoie le num�ro du i�me noeud d'une liste de noeuds contenue dans un NodeSet.
  \param i place du noeud dans la liste.
  \return pointeur sur le noeud
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
Indice
NodeSet::getNodeNumber(Indice i)
//-----------------------------------------------------------------------------
{
  return (nodes(i)->number);
}

//!nombre de noeuds dans la liste
/*!
  Cette m�thode renvoie le nombre de noeuds dans un NodeSet
  \return nombre de noeuds de la liste
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
Indice
NodeSet::size()
//-----------------------------------------------------------------------------
{
  return nodes.size();
}

//!ajoute un noeud � un NodeSet
/*!
  Cette m�thode ajoute un noeud existant � un NodeSet. Le noeud est sp�cifi� par son num�ro d'identification. Il est alors recherch� dans la grille courante du domaine courant de la structure.
  \param num num�ro du noeud � ajouter (il doit �tre pr�sent dans la grille courante du domaine courant).
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
void NodeSet::add(Indice num)
//-----------------------------------------------------------------------------
{
  Node *pnd;

  // check the exitance of the node
  if ((pnd = Global_Structure->getNodeByNum(num)) == NULL)
  {
    fatalError("NodeSet::add", "Node %d not exist in current grid and domain", num);
  }

  // add the node
  nodes << pnd;
}

//!ajoute un ensemble de noeuds � un NodeSet
/*!
  Cette m�thode ajoute un ensemble de noeuds existant � un NodeSet. Les noeuds sont sp�cifi�s par leurs num�ros d'identification donn� dans un intervalle de variation (num�ro de d�but, num�ro de fin et incr�ment). Les noeuds sont alors recherch�s dans la grille courante du domaine courant de la structure.
  \warning Les noeuds ajout�s doivent �tre pr�sents dans la grille courante du domaine courant.
  \param sNum num�ro de d�part dans la liste
  \param eNum num�ro de fin dans la liste
  \param inc incr�ment sur les num�ros (par d�faut = 1)
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
void NodeSet::add(Indice sNum, Indice eNum, Indice inc)
//-----------------------------------------------------------------------------
{
  Node *pnd;
  for (Indice i = sNum; i <= eNum; i += inc)
  {
    if ((pnd = Global_Structure->getNodeByNum(i)) == NULL)
    {
      fatalError("NodeSet::add", "Node %d not exist in current grid and domain", i);
    }

    // add the node
    nodes << pnd;
  }
}

//!ajoute un NodeSet � un NodeSet
/*!
  Cette m�thode ajoute un NodeSet existant � un NodeSet.
  \param ns NodeSet a fusionner
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void NodeSet::add(NodeSet *ns)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < ns->nodes.size(); i++)
  {
    nodes << ns->nodes(i);
  }
}

//-----------------------------------------------------------------------------
void NodeSet::intersect(NodeSet *es)
//-----------------------------------------------------------------------------
{
  Boolean ok;
  for (Indice i = nodes.size() - 1; i >= 0; i--)
  {
    ok = False;
    for (Indice j = 0; j < es->nodes.size(); j++)
    {
      if (nodes(i) == es->nodes(j))
      {
        ok = True;
        break;
      }
    }
    if (!ok)
      nodes.del(i);
  }
}

//-----------------------------------------------------------------------------
void NodeSet::substract(NodeSet *es)
//-----------------------------------------------------------------------------
{
  for (Indice i = nodes.size() - 1; i >= 0; i--)
  {
    for (Indice j = 0; j < es->nodes.size(); j++)
    {
      if (nodes(i) == es->nodes(j))
      {
        nodes.del(i);
        break;
      }
    }
  }
}

//-----------------------------------------------------------------------------
void NodeSet::clear()
//-----------------------------------------------------------------------------
{
  nodes.flush();
}

//-----------------------------------------------------------------------------
void NodeSet::surface(ElementSet *eset)
//-----------------------------------------------------------------------------
{
  List<LateralSurface *> lat;
  Element *pel;
  Indice i, j, k;
  LateralSurface *sr1;
  LateralSurface *sr2;

  for (i = 0; i < eset->size(); i++)
  {
    // get the element
    pel = eset->elements(i);

    for (j = 0; j < pel->getNumberOfSideFaces(); j++)
    {
      // create surfaces
      LateralSurface *sr = new LateralSurface;

      for (k = 0; k < pel->getNumberOfNodesOnSideFace(j); k++)
      {
        sr->tab[k] = pel->getNodeOnSideFace(j, k)->number;
      }

      // sort the table
      qsort(sr->tab, pel->getNumberOfNodesOnSideFace(j), sizeof(Indice), compareIndice);

      sr->pel = pel;
      sr->face = j;

      lat << sr;
    }
  }

  // sort the whole list
  lat.sort(compareLateralSurfaces);

  // now detect doubles
  Boolean ok;
  for (i = 1; i < lat.size(); i++)
  {
    sr1 = lat(i - 1);
    sr2 = lat(i);
    Indice sdd1 = sr1->pel->getNumberOfNodesOnSideFace(sr1->face);
    Indice sdd2 = sr2->pel->getNumberOfNodesOnSideFace(sr2->face);

    if (sdd1 == sdd2)
    {
      ok = True;
      for (j = 0; j < sdd1; j++)
      {
        if ((sr1->tab[j] != sr2->tab[j]) || (sr1->pel->material != sr2->pel->material))
        {
          ok = False;
          break;
        }
      }
      if (ok)
      {
        delete lat(i - 1);
        delete lat(i);
        lat.del(i - 1, i);
        i--;
      }
    }
  }

  nodes.flush();

  // cr�er la liste des noeuds
  for (i = 0; i < lat.size(); i++)
  {
    sr1 = lat(i);
    for (j = 0; j < sr1->pel->getNumberOfNodesOnSideFace(sr1->face); j++)
    {
      nodes << sr1->pel->getNodeOnSideFace(sr1->face, j);
    }
  }

  // sort the list
  nodes.sort();

  // suppress doubles
  for (i = 1; i < nodes.size(); i++)
  {
    if (nodes(i - 1) == nodes(i))
    {
      nodes.del(i - 1, i - 1);
      i--;
    }
  }

  // detruire la liste des surfaces temporaires
  for (i = 0; i < lat.size(); i++)
  {
    delete lat(i);
  }

  lat.flush();
}

//!constructeur de la classe ElementSet
/*!
  Cette m�thode construit un ElementSet vide.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
ElementSet::ElementSet()
//-----------------------------------------------------------------------------
{
}

//!constructeur par recopie de la classe ElementSet
/*!
  Cette m�thode construit un ElementSet vide par recopie d'un autre. 
  Elle n'est pas finalis�e.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
ElementSet::ElementSet(const ElementSet &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely ElementSet class" << /*(int) */ &X << endl;
  exit(-1);
}

//!destructeur de la classe ElementSet
/*!
  Cette m�thode d�truit un ElementSet.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
ElementSet::~ElementSet()
//-----------------------------------------------------------------------------
{
}

//!ajoute un �l�ment � un ElementSet
/*!
  Cette m�thode ajoute un �l�ment existant � un ElementSet. L'�l�ment est directement sp�cifi� par son pointeur, aucune v�rification n'est effectu�e par cette m�thode.
  \param el pointeur sur un �l�ment
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
void ElementSet::add(Element *el)
//-----------------------------------------------------------------------------
{
  elements << el;
}

//!r�cup�re un pointeur sur le i�me �l�ment d'un ElementSet
/*!
  Cette m�thode renvoie un pointeur sur le i�me �l�ment d'une liste d'�l�ments contenue dans un ElementSet.
  \param i place de l'�l�ment dans la liste.
  \return pointeur sur le �l�ment
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
Element *
ElementSet::getElement(Indice i)
//-----------------------------------------------------------------------------
{
  return (elements(i));
}

//!nombre d'�l�ments dans la liste
/*!
  Cette m�thode renvoie le nombre d'�l�ments dans un ElementSet
  \return nombre d'�l�ments de la liste
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
Indice
ElementSet::size()
//-----------------------------------------------------------------------------
{
  return elements.size();
}

//!ajoute un �l�ment � un ElementSet
/*!
  Cette m�thode ajoute un �l�ment existant � un ElementSet. L'�l�ment est sp�cifi� par son num�ro d'identification. Il est alors recherch� dans la grille courante du domaine courant de la structure.
  \param num num�ro de l'�l�ment � ajouter (il doit �tre pr�sent dans la grille courante du domaine courant).
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
void ElementSet::add(Indice num)
//-----------------------------------------------------------------------------
{
  Element *pel;

  // check the exitance of the element
  if ((pel = Global_Structure->getElementByNum(num)) == NULL)
  {
    fatalError("ElementSet::add", "Element %d not exist in current grid and domain", num);
  }

  // add the element
  elements << pel;
}

//!ajoute un ensemble d'�l�ments � un ElementSet
/*!
  Cette m�thode ajoute un ensemble d'�l�ments existant � un ElementSet. Les �l�ments sont sp�cifi�s par leurs num�ros d'identification donn� dans un intervalle de variation (num�ro de d�but, num�ro de fin et incr�ment). Les �l�ments sont alors recherch�s dans la grille courante du domaine courant de la structure.
  \warning Les �l�ments ajout�s doivent �tre pr�sents dans la grille courante du domaine courant.
  \param sNum num�ro de d�part dans la liste
  \param eNum num�ro de fin dans la liste
  \param inc incr�ment sur les num�ros (par d�faut = 1)
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
void ElementSet::add(Indice sNum, Indice eNum, Indice inc)
//-----------------------------------------------------------------------------
{
  Element *pel;
  for (Indice num = sNum; num <= eNum; num += inc)
  {
    if ((pel = Global_Structure->getElementByNum(num)) == NULL)
    {
      fatalError("ElementSet::add", "Element %d not exist in current grid and domain", num);
    }

    // add the element
    elements << pel;
  }
}

//!ajoute un ElementSet � un ElementSet
/*!
  Cette m�thode ajoute un ElementSet existant � un ElementSet.
  \param ns ElementSet a fusionner
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void ElementSet::add(ElementSet *es)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < es->elements.size(); i++)
  {
    elements << es->elements(i);
  }
}

//-----------------------------------------------------------------------------
void ElementSet::intersect(ElementSet *es)
//-----------------------------------------------------------------------------
{
  Boolean ok;
  for (Indice i = elements.size() - 1; i >= 0; i--)
  {
    ok = False;
    for (Indice j = 0; j < es->elements.size(); j++)
    {
      if (elements(i) == es->elements(j))
      {
        ok = True;
        break;
      }
    }
    if (!ok)
      elements.del(i);
  }
}

//-----------------------------------------------------------------------------
void ElementSet::substract(ElementSet *es)
//-----------------------------------------------------------------------------
{
  for (Indice i = elements.size() - 1; i >= 0; i--)
  {
    for (Indice j = 0; j < es->elements.size(); j++)
    {
      if (elements(i) == es->elements(j))
      {
        elements.del(i);
        break;
      }
    }
  }
}

//-----------------------------------------------------------------------------
void ElementSet::clear()
//-----------------------------------------------------------------------------
{
  elements.flush();
}

//!constructeur de la classe Select
/*!
  Cette m�thode construit un Select vide.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
Select::Select()
//-----------------------------------------------------------------------------
{
}

//!constructeur par recopie de la classe NodeSet
/*!
  Cette m�thode construit un NodeSet vide par recopie d'un autre. 
  Elle n'est pas finalis�e.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
Select::Select(const Select &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely Select class" << /*(int) */ &X << endl;
  exit(-1);
}

//!destructeur de la classe NodeSet
/*!
  Cette m�thode d�truit un NodeSet.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
Select::~Select()
//-----------------------------------------------------------------------------
{
}

//!ajoute un NodeSet
/*!
  Cette m�thode ajoute un NodeSet existant. Le NodeSet est directement sp�cifi� par son pointeur, aucune v�rification n'est effectu�e par cette m�thode.
  \param ns pointeur sur un NodeSet
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
void Select::add(NodeSet *ns)
//-----------------------------------------------------------------------------
{
  nodesSet << ns;
}

//!r�cup�re un NodeSet en fonction de son nom
/*!
  Cette m�thode renvoie un NodeSet existant. Le NodeSet est directement sp�cifi� par son nom.
  \param name nom du NodeSet
  \return pointeur sur le NodeSet
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
NodeSet *
Select::getNodeSet(String name)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < nodesSet.size(); i++)
  {
    if (name == nodesSet(i)->name)
    {
      return (nodesSet(i));
    }
  }

  cerr << "Node Set " << name << " doesn't exist\n";
  exit(-1);
  return NULL;
}

//!ajoute un ElementSet
/*!
  Cette m�thode ajoute un ElementSet existant. L'ElementSet est directement sp�cifi� par son pointeur, aucune v�rification n'est effectu�e par cette m�thode.
  \param ns pointeur sur un ElementSet
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
void Select::add(ElementSet *ns)
//-----------------------------------------------------------------------------
{
  elementsSet << ns;
}

//!r�cup�re un ElementSet en fonction de son nom
/*!
  Cette m�thode renvoie un ElementSet existant. Le ElementSet est directement sp�cifi� par son nom.
  \param name nom du ElementSet
  \return pointeur sur le ElementSet
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
ElementSet *
Select::getElementSet(String name)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < elementsSet.size(); i++)
  {
    if (name == elementsSet(i)->name)
    {
      return (elementsSet(i));
    }
  }

  cerr << "Element Set " << name << " doesn't exist\n";
  exit(-1);
  return NULL;
}
