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
