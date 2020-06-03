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
  Class NodeMotion definition
*/

#ifndef __NodeMotion_h__
#define __NodeMotion_h__

#include <upFemSolver.h>

class Node;

enum
{
  Unk_NodeMotion = 0,
  Lag_NodeMotion,
  Giu_NodeMotion,
  Con_NodeMotion
};

/**
*/
/** @dia:pos 225.8,104.8 */
class NodeMotion
{
protected:
  /** @dia:route 57,9;h,247.15,92.1,254.75,107.1,241.35 */
  Node *node;

public:
  // constructeurs
  NodeMotion();
  NodeMotion(const NodeMotion &X);
  virtual ~NodeMotion();

  // fonctions virtuelles
  virtual void updateNode(Real timeStep) = 0;
  virtual Indice getMotion() = 0;
  virtual void computeForces(Real timeStep) = 0;
  //  virtual int Type()=0;
  //  virtual void computeContact(Real timeStep)=0;

  // fonctions membres
  void attachNode(Node *pnd);
  void detachNode();
};

#endif
