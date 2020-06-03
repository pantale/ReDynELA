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

// begin date : 08/04/1997

/*
  Class lagMotion definition
*/

#include <lagMotion.h>
#include <Node.h>
#include <NodalField.h>

//-----------------------------------------------------------------------------
lagMotion::lagMotion()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
lagMotion::lagMotion(const lagMotion &X)
//-----------------------------------------------------------------------------
{
  cerr << "erreur lagMotion copy not implemented" << X.node << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
lagMotion::~lagMotion()
//-----------------------------------------------------------------------------
{
}

//!Mise � jour de la position des noeuds pour le cas d'un contact
/*!
Cette m�thode effectue la mise � jour de la position d'un noeud de la structure. Les nouvelles coordonn�es sont donn�es par la relation: \f[ x_1=x_0+u_1 \f]
*/
//-----------------------------------------------------------------------------
void lagMotion::updateNode(Real timeStep)
//-----------------------------------------------------------------------------
{
  // update node coordinates
  node->coords += node->New->delta_disp;
}

//-----------------------------------------------------------------------------
void lagMotion::computeForces(Real timeStep)
//-----------------------------------------------------------------------------
{
}
