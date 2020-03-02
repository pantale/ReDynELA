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
