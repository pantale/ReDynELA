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
	Class Mesher definition
*/

#ifndef __Mesher_h__
#define __Mesher_h__

#include <Elements.h>
class Structure;

/*!
  \class Mesher Mesher.h
  \brief Class 
  \ingroup basicTools
  \version 1.0.0
  \date 1997-2002
  \author Olivier PANTALE


*/
/** @dia:pos 151.065,34.2794 */
class Mesher
{

protected:
  /** @dia:route 11,13;h,149.465,76.9794,167.065,38.1794,163.115 */
  Element *elementType; //!< Element par d�faut
  /** @dia:route 3,65;h,151.065,34.9794,145.065,24.7794,141.965 */
  Structure *structure;

public:
  // constructor
  Mesher();
  virtual ~Mesher();

  // member functions
  void setElement(Element *pel);
  void link(Structure *structure);
};

/** @dia:pos 145.065,46.2794 */
/** @dia:route Mesher;v,157.09,39.4794,42.2794,154.59,46.2794 */
class BoxMesher : public Mesher
{

public:
  // constructor
  BoxMesher();
  ~BoxMesher();

  // member functions
  void rectangle(Real lx, Real ly, Indice nx, Indice ny);
};

#endif
