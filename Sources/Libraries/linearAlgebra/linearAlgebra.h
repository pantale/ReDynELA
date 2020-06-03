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
  \file linearAlgebra.h
  \brief Main includes of the linearAlgebra library.

  This file refers all the classes of the linearAlgebra library.
  Including this file allows the inclusion of all the definitions defined in the linearAlgebra library.
  This inclusion slow down the compilation of the library but helps a lot for future use.

  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __linearAlgebra_h__
#define __linearAlgebra_h__

// Vectors
#include <Vector.h>
#include <Vec3D.h>

// Matrices
#include <Matrices.h>
#include <Matrix.h>
#include <MatrixDiag.h>

// Tensors
#include <Tensor2.h>
#include <Tensor3.h>
#include <Tensor4.h>

// Functions
#include <Function.h>
#include <DiscreteFunction.h>
#include <PolynomialFunction.h>
#include <SinusFunction.h>
#include <RampFunction.h>

// Miscs
#include <Plane.h>

#endif
