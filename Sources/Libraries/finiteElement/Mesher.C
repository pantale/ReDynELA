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
	Class Mesher implementation
*/

#include <Mesher.h>
#include <Structure.h>

/*!
  \class Mesher Mesher.C
  \brief Class 
  \ingroup basicTools
  \version 1.0.0
  \date 1997-2002
  \author Olivier PANTALE


*/
//!default constructor of the class Mesher
/*!

*/
//-----------------------------------------------------------------------------
Mesher::Mesher()
//-----------------------------------------------------------------------------
{
  structure = NULL;
  elementType = NULL;
}

//!destructor of the class Mesher
/*!

*/
//-----------------------------------------------------------------------------
Mesher::~Mesher()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Mesher::link(Structure *_str)
//-----------------------------------------------------------------------------
{
  structure = _str;
}

//-----------------------------------------------------------------------------
void Mesher::setElement(Element *pel)
//-----------------------------------------------------------------------------
{
  elementType = pel;
}

//-----------------------------------------------------------------------------
BoxMesher::BoxMesher()
//-----------------------------------------------------------------------------
{
}

//!destructor of the class BoxMesher
/*!

*/
//-----------------------------------------------------------------------------
BoxMesher::~BoxMesher()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void BoxMesher::rectangle(Real lx, Real ly, Indice nx, Indice ny)
//-----------------------------------------------------------------------------
{
  if (structure == NULL)
  {
    fatalError("BoxMesher::rectangle", "Must link structure to mesher before calling meshing procedure\n");
  }
  if (elementType == NULL)
  {
    fatalError("BoxMesher::rectangle", "Must select element type before calling meshing procedure\n");
  }

  Indice nbNodes = structure->getNodesNumber() + 1;
  Real dxLargeur = lx / (nx);
  Real dxHauteur = ly / (ny);

  for (Indice j = 0; j <= ny; j += 1)
    for (Indice i = 0; i <= nx; i += 1)
    {
      structure->createNode(nbNodes, i * dxLargeur, j * dxHauteur, 0);
      nbNodes++;
    }

  structure->setDefaultElement(elementType);
  Indice nbElements = structure->getElementsNumber() + 1;
  Indice x1;
  Indice x2;
  Indice x3;
  Indice x4;
  for (Indice j = 0; j < ny; j++)
    for (Indice i = 0; i < nx; i++)
    {
      x1 = (i + (j * (nx + 1)) + 1);
      x2 = (i + (j * (nx + 1)) + 2);
      x3 = (i + ((j + 1) * (nx + 1)) + 2);
      x4 = (i + ((j + 1) * (nx + 1)) + 1);
      structure->createElement(nbElements, x1, x2, x3, x4);
      nbElements++;
    }
}
