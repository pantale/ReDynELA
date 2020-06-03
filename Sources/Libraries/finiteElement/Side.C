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

// begin date : 22/05/1997

/*
	Class Side definition
*/

#include <Side.h>
#include <SideFace.h>
#include <Node.h>
#include <Element.h>
#include <Select.h>

//-----------------------------------------------------------------------------
Side::Side()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Side::Side(const Side &X)
//-----------------------------------------------------------------------------
{
  cerr << "erreur Side copy not implemented" << X.sides.size() << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
Side::~Side()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Boolean &Side::isMaster()
//-----------------------------------------------------------------------------
{
  return (type);
}

//-----------------------------------------------------------------------------
Boolean
Side::addSideFace(SideFace *pSideFace)
//-----------------------------------------------------------------------------
{
  sides << pSideFace;

  pSideFace->side = this;

  return (Success);
}

//-----------------------------------------------------------------------------
void Side::computeScanVolume()
//-----------------------------------------------------------------------------
{
  // calcul des volumes de recherche
  for (Indice i = 0; i < sides.size(); i++)
    sides(i)->computeScanVolume();
}

//-----------------------------------------------------------------------------
void Side::computeSideFacesNormals()
//-----------------------------------------------------------------------------
{
  Indice i;

  // reinit a zero des normales des vertex
  for (i = 0; i < nodes.size(); i++)
    nodes(i)->normal = 0.;

  // calcul des normales des SideFaces
  for (i = 0; i < sides.size(); i++)
    sides(i)->computeNormal();

  // calcul des normales des vertex
  for (i = 0; i < sides.size(); i++)
    sides(i)->computeNormalVertex();

  // normalisation des normales des vertex
  for (i = 0; i < nodes.size(); i++)
    nodes(i)->normal.normalize();
}

//-----------------------------------------------------------------------------
void Side::Init()
//-----------------------------------------------------------------------------
{
  Element *pel;
  Node *pnd;
  Indice i, j, k, l, toHave, getted;
  Boolean ok;
  List<Element *> lel;
  SideFace *psf;

  for (i = 0; i < nodes.size(); i++)
  {
    // the first node
    pnd = nodes(i);
    for (j = 0; j < pnd->elements.size(); j++)
    {
      pel = pnd->elements(j);

      // si mod�le plan
      if (pel->getNumberOfFaces() != 1)
        fatalError("Side::Init", "not for 3D elements\n");
      // test if complete
      //	  for (k=0;k<pel->noSideFaces;k++)
      for (k = 0; k < pel->getNumberOfSideFaces(); k++)
      {
        toHave = pel->getNumberOfNodesOnSideFace(k);
        getted = 0;
        ok = False;
        for (l = 0; l < toHave; l++)
        {
          // face contains node
          if (pel->getNodeOnSideFace(k, l) == pnd)
          {
            ok = True;
            break;
          }
        }

        // check if all nodes are present
        if (ok)
        {
          for (l = 0; l < toHave; l++)
          {
            // face contains node
            if (nodes.AppN(pel->getNodeOnSideFace(k, l)->Id) != NULL)
              getted++;
          }
        }

        if (getted == toHave)
        {
          // test if already in list
          if (lel.contains(pel) == False)
          {
            // add the element
            lel << pel;

            if ((pel->getFamily() == Element::Bidimensional) || (pel->getFamily() == Element::Axisymetric))
              psf = new SideFace2D;
            else
              psf = new SideFace3D;
            psf->attachElement(pel);
            for (l = toHave - 1; l >= 0; l--)
            {
              psf->addNode(pel->getNodeOnSideFace(k, l));
              // face contains node
            }
            sides << psf;
          }
        }
      }
    }
  }

  lel.flush();
}

//-----------------------------------------------------------------------------
Indice
Side::scanIncludeNodes(Side *scanned_side)
//-----------------------------------------------------------------------------
{
  Indice i, j, nb;
  Node *pnd;

  // initialisation a zero du nombre de noeuds
  // trouv�s comme �tant inclus dans un �l�ment
  nb = 0;

  // recherche des noeuds inclus
  // boucle sur les SidesFaces du Side
  for (i = 0; i < sides.size(); i++)
  {

    // boucle sur les noeuds du sideface en face
    // on recherche les inclusions des noeuds en face
    for (j = 0; j < scanned_side->nodes.size(); j++)
    {

      // noeud du Side en face
      pnd = scanned_side->nodes(j);

      // ce noeud est-il dans mon SideFace courant
      if (sides(i)->isNodeinSideFace(this, pnd))
      {
        nb++;
      }
    }
  }
  //  cout << nb<<" penetrating nodes found\n";

  // retour de la valeur
  return nb;
}

/*//-----------------------------------------------------------------------------
void Side::computeSpeeds(Real timeStep)
//-----------------------------------------------------------------------------
{
  Node* pnd;
  Indice i;
  Contact* pcontact;

  // boucle sur les noeuds
  for (i=0;i<nodes.size();i++) {

    // recuperation du pointeur du noeud
    pnd=nodes(i);
    
    // test si noeud en contact
    if (pnd->motion->getMotion()==Con_NodeMotion) {

      // recup de la condition de contact
      pcontact=(Contact*)pnd->motion;

      // rammener le noeud a la surface maitre	
//      pcontact->computeContact(timeStep);
//      pcontact->computeSpeeds(timeStep);
    }
  } 
}*/

//-----------------------------------------------------------------------------
void Side::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < sides.size(); i++)
    sides(i)->toFile(pfile);
}

//-----------------------------------------------------------------------------
Boolean
Side::check()
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < sides.size(); i++)
  {
    if (sides(i)->check() == False)
      return False;
  }

  return True;
}

//-----------------------------------------------------------------------------
void Side::addNodeSet(NodeSet *nset)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(nset != NULL);
#endif

  for (Indice i = 0; i < nset->size(); i++)
  {
    nodes << nset->getNode(i);
  }
}
