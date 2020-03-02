/***************************************************/
/*                                                 */
/*  DynELA Project                                 */
/*                                                 */
/*  (c) Copyright 1997-2004                        */
/*                                                 */
/*	Equipe C.M.A.O                             */
/*	Laboratoire Genie de production            */
/*	Ecole Nationale d'Ingenieurs de Tarbes     */
/*	BP 1629 - 65016 TARBES cedex               */
/*                                                 */
/*                                                 */
/*  Main Author: Olivier PANTALE                   */
/*                                                 */
/***************************************************/

// begin date :
// revision date :

/*
	Class ExportDynELA definition
*/

#ifndef __ExportDynELA_h__
#define __ExportDynELA_h__

#include <io_Structure.h>

/** @dia:pos 20,-34.75 */
/** @dia:route io_Structure;v,18.7603,-31.2794,42.05,26.55,-34.75 */
class ExportDynELA : public io_Structure
{

  FILE *pfile;

public:
  // constructeurs
  ExportDynELA();
  ExportDynELA(const ExportDynELA &X);
  ~ExportDynELA();

  // fonctions membres
  void sepWrite(String str);
  void structure_read();
  void structure_write();
  void headingWrite();
  void memoryVariablesWrite();
  void structureWrite();
  void nodesWrite();
  void elementsWrite();
  void materialsWrite();
  void nodeAndElementSetWrite();
  void solversWrite();
  void functionsWrite();
  void boundaryWrite();
  void boundaryRestrainWrite();
  void boundaryDisplacementWrite();
  void boundarySpeedWrite();
  void boundaryAcceleration();
  void boundaryForceWrite();
  void boundaryTemperatureWrite();
  void boundaryFluxWrite();
  void boundaryConvectionWrite();
  void renumerateNodesAndElements();
  void historyFilesWrite();
};

#endif
