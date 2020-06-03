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
	Class Function definition
*/
#ifndef __RampFunction_h__
#define __RampFunction_h__
#include <upLinearAlgebra.h>
#include <Function.h>

/*!
  \file RampFunction.h
  \brief fichier .h de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

/*!
  \class RampFunction RampFunction.h
  \brief Classe de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  \author &copy; Olivier PANTALE
  \date 1997-2004
  \since 1.0.4
*/

/** @dia:pos 114,20 */
/** @dia:route Function;v,140.025,12.95,16,123,20 */
class RampFunction : public Function
{
private:
  short type;
  Real min, max;
  Real arg[10];

public:
  enum
  {
    none,
    linear,
    sinus,
    constant,
    erf,
    soft
  };

public:
  RampFunction();
  ~RampFunction();
  Real getValue(Real x);
  Real getSlope(Real x);
  void set(String st, Real min, Real max);
  short getType(Indice i);
  void print();
  void toGnuplot(String filename, Indice steps = 250);
  void flush();
  String convertToDynELASourceFile();
};

#endif
