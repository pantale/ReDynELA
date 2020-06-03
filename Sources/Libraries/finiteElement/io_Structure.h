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

/*
  Class io_Structure definition
*/

#ifndef __io_Structure_h__
#define __io_Structure_h__

#include <upFemSolver.h>

class io_Structure;
class Structure;
/* #include <Structure.h> */

/**
*/
/** @dia:pos 169.065,22.2794 */
class io_Structure
{
public:
  /** @dia:route 0,2;h,-120.6,-172.85,78,-47.4794,24.7853 */
  String name; // nom du fichier
  /** @dia:route 0,2;h,-120.6,-172.85,78,-47.4794,24.7853 */
  String filename; // nom du fichier
  /** @dia:route 55,10;h,141.965,20.7794,160.75,24.9794,169.065 */
  Structure *pstructure; // pointeur sur la structure

protected:
  char action; // action
  Boolean binary;
  Boolean incremental;
  Indice currentSave;

public:
  // constructeurs
  io_Structure();
  virtual ~io_Structure();

  // fonctions d'attache
  void link(String name);
  void link(Structure *structure);
  String &getFileName();

  // fonctions de reglage
  char getMode();
  void transfert();
  void setMode(char mode);
  void startCurrentSave(Indice i)
  {
    currentSave = i;
  }
  Boolean &binaryFile()
  {
    return binary;
  }
  Boolean &incrementalFile()
  {
    return incremental;
  }

  virtual void structure_read() = 0;
  virtual void structure_write() = 0;

  void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, io_Structure &structure);
};

#endif
