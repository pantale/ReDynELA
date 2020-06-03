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

// begin date :
// revision date :

/*
	Class HistoryFile definition
*/

#ifndef __HistoryFile_h__
#define __HistoryFile_h__

#include <upFemSolver.h>
/* #include <Select.h> */
class NodeSet;
class Node;

/*!
  \file History.h
  \brief fichier .h de d�finition des noeuds du maillage
  \ingroup femLibrary

  Ce fichier sert � la d�finition des fichiers d'historique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/

/*!
  \class HistoryFileItem
  \brief Classe de d�finition des fichiers d'historique sur DynELA.
  \ingroup femLibrary

  Cette classe permet de d�finir et de manipuler les fichier d'historiques sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/
/** @dia:pos 201.8,76.8 */
class HistoryFileItem
{
private:
  friend class HistoryFile;
  /** @dia:route 0,2;h,44.7353,4.5206,-40.6853,-172.85,-95.25 */
  String name;
  /** @dia:route 0,2;h,44.7353,4.5206,-40.6853,-172.85,-95.25 */
  String field;
  Indice component;
  /** @dia:route 13,10;h,217,80.3,223.75,73.5,228.8 */
  Node *node;

public:
  // constructor
  HistoryFileItem();
  HistoryFileItem(const HistoryFileItem &X);
  ~HistoryFileItem();
  Real get();
};

/*class HistoryFileAtom {
public:
  NodeSet* ns;
  String str;
  Indice comp;
  HistoryFileAtom (){}
  ~HistoryFileAtom (){}
};*/

/*!
  \class HistoryFile
  \brief Classe de d�finition des fichiers d'historique sur DynELA.
  \ingroup femLibrary

  Cette classe permet de d�finir et de manipuler les fichier d'historiques sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/
/** @dia:pos 198.8,47.8 */
class HistoryFile
{
private:
  FILE *pfile;
  Real nextTime;
  /** @dia:route 10,3;h,198.8,50.5,194.75,77.5,201.8 */
  List<HistoryFileItem *> list;
  Real stopTime;
  Real startTime;
  Real saveTime;
  /** @dia:route 0,2;h,41.7353,-24.4794,-37.6853,-172.85,-95.25 */
  String fileName;
  /** @dia:pos 191.065,135.529 */
  typedef struct HistoryFileAtom
  {
    /** @dia:route 4,21;h,197.915,136.229,203.75,126.429,200.365 */
    NodeSet *ns;
    /** @dia:route 0,2;h,34,63.25,-29.95,-172.85,-95.25 */
    String str;
    Indice comp;
  };
  List<HistoryFileAtom *> historyFileAtom;

public:
  /** @dia:route 0,2;h,41.7353,-24.4794,-37.6853,-172.85,-95.25 */
  String name;

public:
  // constructor
  HistoryFile();
  HistoryFile(const HistoryFile &X);
  ~HistoryFile();
  void open();
  void setName(String name);
  void setSaveTime(Real st);
  void setSaveTime(Real sta, Real end, Real st);
  Real getSaveTime();
  Real getStartTime();
  Real getStopTime();
  void close();
  void save(Real);
  void headerWrite();
  void append(HistoryFileItem *item);
  void append(NodeSet *ns, String str, Indice comp = 0);
  String getName();
  Indice getFieldSize();
  NodeSet *getNodeSet(Indice);
  String getField(Indice);
  Indice getComponent(Indice);
};

#endif
