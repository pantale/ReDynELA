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

// begin date : 04/03/1997
// revision date : 18/07/1997

#ifndef __SampleArchive_h__
#define __SampleArchive_h__
#include <String.h>
#include <List.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/** @dia:pos 96,12 */
class SampleFile
{
public:
  /** @dia:route 0,21;h,96,12,93.4501,15.1,79.35 */
  String name;
  Indice size;
  mode_t mode;
  char *data;
  SampleFile() { data = NULL; };
  ~SampleFile(){};
  void fromSource(String nm);
  void toSource();
  void fromFile(FILE *pfile);
  void toFile(FILE *pfile);
};

/** @dia:pos 92,2 */
class SampleArchive
{
public:
  /** @dia:route 8,15;h,92,3.9,86,12.7,79.35 */
  String name;
  /** @dia:route 10,17;h,92,4.9,88,13.5,79.35 */
  String description;
  /** @dia:route 3,13;h,92,2.7,84,11.9,79.35 */
  String readme;
  Indice size;
  /** @dia:route 4,3;h,106.85,2.7,110,12.7,96 */
  List<SampleFile *> files;
  SampleArchive(){};
  ~SampleArchive(){};
  void toFile(String);
  void fromFile(String, Boolean load = True);
  void toSource();
};

#endif
