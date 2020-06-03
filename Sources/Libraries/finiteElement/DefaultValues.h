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

#ifndef __DefaultValues_h__
#define __DefaultValues_h__

// time step default values
#define defaultComputeTimeStepFrequency 10
#define defaultSpectralRadius 0.8182
#define defaultSafetyFactor 0.9

// constitutive law defines
#define radialReturnPrecision 1.0E-8
#define radialReturnMaxIterations 2000
#define constitutiveLawPrecision 1.0E-8
#define constitutiveLawMaxIterations 100

// time step computation
#define maximumFrequencyMaxIterations 1000
#define maximumFrequencyConvergence 1e-4

#endif
