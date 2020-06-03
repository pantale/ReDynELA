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
