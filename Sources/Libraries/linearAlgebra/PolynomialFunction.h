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
#ifndef __PolynomialFunction_h__
#define __PolynomialFunction_h__
#include <upLinearAlgebra.h>
#include <Function.h>

/*!
  \file PolynomialFunction.h
  \brief fichier .h de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

/*!
  \class PolynomialFunction PolynomialFunction.h
  \brief Classe de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  \author &copy; Olivier PANTALE
  \date 1997-2004
  \since 1.0.4
*/
/** @dia:pos 154,36 */
struct PolynomialFragment
{
  int factor;
  Real value;
};

/** @dia:pos 140,20 */
/** @dia:route Function;v,140.025,12.95,16,151.45,20 */
class PolynomialFunction : public Function
{
private:
  /** @dia:route 4,2;h,161.75,36.7,166,20,162.9 */
  List<PolynomialFragment> fragment;

public:
public:
  PolynomialFunction();
  ~PolynomialFunction();
  Real getValue(Real x);
  void set(Indice i, Real val);
  Real get(Indice i);
  Real getSlope(Real x);
  Indice getNumberOfFragments();
  Real getFactor(Indice i);
  Real getCoefficient(Indice i);
  void print();
  void toGnuplot(String filename, Real xmin, Real xmax, Real steps);
  void flush();
  String convertToDynELASourceFile();
};

#endif
