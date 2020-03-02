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

// begin date : 22/05/1997

/*
	Class Side definition
*/

#ifndef __Side_h__
#define __Side_h__

#include <List.h>

/* #include <SideFace.h> */
class SideFace;
class NodeSet;
class lagMotion;
class Node;
class Interface;

/* #incluclass Node;
de <Domain.h> */

/**
*/
/** @dia:pos 289.8,97.8 */
class Side
{
  //protected:
public:
  /// type de la surface (master ou slave)
  Boolean type;
  /// liste des SideFaces qui composent la surface de contact
  /** @dia:route 29,13;h,306.05,107.9,315.75,186.5,250.25 */
  List<SideFace *> sides;
  /// ListIde des noeuds qui composent la surface de contact
  /** @dia:route 45,10;h,247.15,87.3,260.75,100.7,289.8 */
  ListId<Node *> nodes;

public:
  /** @dia:route 25,11;h,302.85,84.3,308.75,100.7,306.05 */
  Interface *interface;

public:
  // constructeurs
  Side();
  Side(const Side &X);
  ~Side();

  // fonctions membres
  Boolean &isMaster();
  Boolean addSideFace(SideFace *pSideFace);
  void computeScanVolume();
  void computeSideFacesNormals();
  void Init();
  Indice scanIncludeNodes(Side *scanned_side);
  void toFile(FILE *pfile);
  Boolean check();
  void addNodeSet(NodeSet *nset);
};

#endif
