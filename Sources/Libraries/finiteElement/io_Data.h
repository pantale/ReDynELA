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

// begin date : 19/05/1998

/*
	Class io_Data definition
*/

#ifndef __io_Data_h__
#define __io_Data_h__

#include <io_Structure.h>

/**
*/
/** @dia:pos 171.065,46.2794 */
/** @dia:route io_Structure;v,175.09,40.0794,44.25,177.265,46.2794 */
class io_Data : public io_Structure
{
  //Boolean binary;

public:
  // constructeurs
  io_Data();
  ~io_Data();

  //  Boolean& binaryFile() {return binary;}

  // fonctions membres
  void structure_read();
  void structure_write();
  void nodesRead(FILE *pfile);
  void nodesWrite(FILE *pfile);
  void elements_read(FILE *pfile);
  void elements_write(FILE *pfile);
  void datasWrite(FILE *pfile);
  void datasRead(FILE *pfile);
  Real getReadTimeData();
};

#endif
