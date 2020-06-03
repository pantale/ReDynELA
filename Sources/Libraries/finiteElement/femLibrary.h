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

/*!
  \file femLibrary.h
  \brief Main includes of the femLibrary library.

  This file refers all the classes of the femLibrary library.
  Including this file allows the inclusion of all the definitions defined in the femLibrary library.
  This inclusion slow down the compilation of the library but helps a lot for future use.

  \ingroup femLibrary
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __femLibrary_h__
#define __femLibrary_h__

/*
 * All includes of the current directory
 */

#include <upFemSolver.h>
#include <Domain.h>
#include <Node.h>
#include <Elements.h>
#include <Structure.h>
#include <Materials.h>
#include <io_Structure.h>
#include <io_Data.h>
#include <Select.h>
#include <ExportAbaqus.h>
#include <ExportDynELA.h>
#include <ExplicitSolver.h>
#include <ExplicitSolverCH.h>
#include <ContactLaw.h>
#include <Coulomb.h>
#include <HistoryFile.h>
#include <Interface.h>
#include <Boundary.h>
#include <Side.h>
#include <SideFace.h>
#include <NodalField.h>
#include <IntegrationPoint.h>
#include <Mesher.h>

#endif
