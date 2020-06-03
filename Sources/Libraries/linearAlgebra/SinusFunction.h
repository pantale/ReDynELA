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
#ifndef __SinusFunction_h__
#define __SinusFunction_h__
#include <upLinearAlgebra.h>
#include <Function.h>

/*!
  \file SinusFunction.h
  \brief fichier .h de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

/*!
  \class SinusFunction SinusFunction.h
  \brief Classe de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  \author &copy; Olivier PANTALE
  \date 1997-2004
  \since 1.0.4
*/

/** @dia:pos 186,40 */
struct SinusFragment
{
  Real pulse;
  Real factor;
  Real constant;
  char type;
};

/** @dia:pos 170,20 */
/** @dia:route Function;v,140.025,12.95,16,181.45,20 */
class SinusFunction : public Function
{
private:
  /** @dia:route 4,2;h,191.85,40.7,196,20,192.9 */
  List<SinusFragment> fragment;
  Real constant;

public:
  enum
  {
    none,
    sinus,
    cosinus
  };

public:
  SinusFunction();
  ~SinusFunction();
  Real getValue(Real x);
  Indice getNumberOfFragments();
  void setSin(Real coef, Real pulse, Real constant = 0);
  void setCos(Real coef, Real pulse, Real constant = 0);
  void setConstant(Real constant);
  short getType(Indice i);
  Real getPulse(Indice i);
  Real getCoefficient(Indice i);
  Real getConstant(Indice i);
  Real getConstant();
  Real getSlope(Real x);
  void print();
  void toGnuplot(String filename, Real xmin, Real xmax, Real steps);
  void flush();
  String convertToDynELASourceFile();
};

#endif
