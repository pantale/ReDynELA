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
  Class Contact definition
*/

#include <Node.h>
#include <NodalField.h>
#include <SideFace.h>
#include <Element.h>
#include <Boundary.h>
#include <Contact.h>
#include <lagMotion.h>

//-----------------------------------------------------------------------------
Contact::Contact()
//-----------------------------------------------------------------------------
{
  pside = NULL;
}

//-----------------------------------------------------------------------------
Contact::Contact(const Contact &X)
//-----------------------------------------------------------------------------
{
  cerr << "erreur Contact copy not implemented" << X.node << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
Contact::~Contact()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Indice
Contact::getMotion()
//-----------------------------------------------------------------------------
{
  return Con_NodeMotion;
}

//-----------------------------------------------------------------------------
Boolean
Contact::attachSideFace(SideFace *pf)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(pside == NULL);
#endif

  pside = pf;
  return (Success);
}

//-----------------------------------------------------------------------------
Boolean
Contact::detachSideFace()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(pside != NULL);
#endif

  pside = NULL;
  return (Success);
}

//-----------------------------------------------------------------------------
void Contact::Create()
//-----------------------------------------------------------------------------
{
  //   cout << "Creating contact Motion condition between slave node "<<node->number;
  //   cout << " and master element "<<pside->element->number<<endl;
}

//!Mise � jour de la position des noeuds pour le cas d'un contact
/*!
Cette m�thode effectue la mise � jour de la position d'un noeud de la structure. Les nouvelles coordonn�es sont donn�es par la relation: \f[ x_1=x_0+u_1 \f]
*/
//-----------------------------------------------------------------------------
void Contact::updateNode(Real timeStep)
//-----------------------------------------------------------------------------
{
  // update node coordinates
  node->coords += node->New->delta_disp;
}

//-----------------------------------------------------------------------------
void Contact::computeForces(Real timeStep)
//-----------------------------------------------------------------------------
{
  Vec3D Ft;
  Real fn;
  Vec3D force;
  double delta;

  if (pside->element->isNodeinElement(node) == True)
    delta = pside->computeDistanceWithPoint(node->coords);
  else
    delta = -1.0;

  if (delta < 0.0)
  {

    // cout << delta << endl;
    //  if (node->New->fe.dot(pside->normal)<0.) {
    //    cout << "release of node "<<node->number<<endl;
    //    exit(0);
    //  node->New->mat_speed=0.;
    //  node->New->dmat_speed=0.;

    // detachement de l'ancien controle de mouvement
    node->detachNodeMotion();

    // allocation d'un contact control
    lagMotion *mot = new lagMotion;

    // initialisation du contact
    mot->attachNode(node);

    // creation de la reference croisee
    node->attachNodeMotion(mot);
    return;
  }

  //  penetration+=delta;

  // calcul de la composante normale de la force
  //  force=(node->mass*delta/SQ(timeStep))*pside->normal;
  //  Fn=(node->mass*delta/SQ(timeStep))*pside->normal;
  fn = 2. * node->mass * delta / SQ(timeStep);

  computetangentialForce(fn, Ft);

  //  cout << fn*pside->normal<<endl;
  //  cout << Ft<<endl;

  force = fn * pside->normal + Ft;
  //  cout << force << endl;

  // calcul de la composante tangentielle de la force
  /*

     A FAIRE 

   */

  // update node coordinates
  // renvoi aux anciennes coordonn�es
  //  node->coords-=node->New->delta_disp;

  // correction de l'acc�l�ration
  node->New->dmat_speed += force / node->mass;
  return;

  // correction de la vitesse
  //  node->New->mat_speed=node->New->mat_speed-(node->New->mat_speed.dot(pside->normal))*pside->normal;
  node->New->mat_speed =
      node->New->mat_speed -
      ((node->New->mat_speed /*-pside->nodes(0)->New->mat_speed*/).dot(pside->normal)) * pside->normal;
  //  cout <<(pside->nodes(0)->New->mat_speed.dot(pside->normal))*pside->normal<<endl;

  //  node->boundary->applyConstant_V0(node);

  // correction du deplacement
  //   node->New->delta_disp=timeStep*node->Current->mat_speed+
  //    (SQ(timeStep)/2.)* ((1.-2.0*Global_Domain->ALE->displacementParameter)*node->New->dmat_speed+
  //      2.0*Global_Domain->ALE->displacementParameter*node->New->dmat_speed);
  cout << node->New->delta_disp << ",";
  node->New->delta_disp += delta * pside->normal;
  //  cout << Global_Domain << node->New->delta_disp << "\n";

  node->boundary->applyConstant_V0(node);

  // update node coordinates and grid speed
  //  node->New->grid_speed=node->New->mat_speed;
  node->coords += node->New->delta_disp;
}
