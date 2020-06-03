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
