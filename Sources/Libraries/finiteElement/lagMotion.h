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
	Class lagMotion definition
*/

#ifndef __lagMotion_h__
#define __lagMotion_h__

//#include <Arithm.h>

#include <NodeMotion.h>
class NodeMotion;

/*<lagMotion:*/
/**
*/
/** @dia:pos 248.8,119.8 */
/** @dia:route NodeMotion;v,233.575,113.2,115.85,255.175,119.8 */
class lagMotion : public NodeMotion
{

public:
  // constructeurs
  lagMotion();
  lagMotion(const lagMotion &X);
  ~lagMotion();

  // fonctions membres
  Indice getMotion()
  {
    return Lag_NodeMotion;
  }
  void updateNode(Real timeStep);
  void computeForces(Real timeStep);
};

/*>lagMotion:*/

#endif

// ---------- Documentation automatique ----------

/*Class:lagMotion
NAME: lagMotion - 

SYNTAX: lagMotion

KEYWORDS:

DESCRIPTION:

CONSTRUCTORS AND INITIALIZATION:
	
MEMBER FUNCTIONS:
 	
EXAMPLES:

SEE ALSO:

DEVELOPED BY:
	Equipe C.M.A.O 
	Laboratoire G\'enie de production
	Ecole Nationale d'Ing\'enieurs de Tarbes
	BP 1629 - 65016 TARBES cedex

AUTHOR:
	Olivier PANTALE
	
End:
*/
