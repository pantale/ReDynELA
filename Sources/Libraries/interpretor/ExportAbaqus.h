/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2004                                                *
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
	Class ExportAbaqus definition
*/

#ifndef __ExportAbaqus_h__
#define __ExportAbaqus_h__

#include <io_Structure.h>

/** @dia:pos 7,-23 */
/** @dia:route io_Structure;v,5.075,-30.2,-26,13.95,-23 */
class ExportAbaqus : public io_Structure
{

  FILE *pfile;

public:
  // constructeurs
  ExportAbaqus();
  ExportAbaqus(const ExportAbaqus &X);
  ~ExportAbaqus();

  // fonctions membres
  void structure_read();
  void structure_write();
  void headingWrite();
  void nodesWrite();
  void elementsWrite();
  void materialsWrite();
  void groupsWrite();
  void solidsWrite();
  void boundariesWrite();
  void stepWrite();
  void boudariesStepWrite();
  void interfacesWrite();
};

#endif
