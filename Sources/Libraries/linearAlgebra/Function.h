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
#ifndef __Function_h__
#define __Function_h__
#include <upLinearAlgebra.h>

/*!
  \file Function.h
  \brief fichier .h de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

/*!
  \class Function Function.h
  \brief Classe de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  \author &copy; Olivier PANTALE
  \date 1997-2004
  \since 1.0.4
*/
/** @dia:pos 134.35,3.95 */
class Function
{
public:
public:
  /** @dia:route 0,2;h,101.15,-224.4,265.65,-172.85,-95.25 */
  String name; //!< nom de la fonction discr�te

public:
  Function();
  virtual ~Function();
  virtual Real getValue(Real x) = 0;
  virtual Real getSlope(Real x) = 0;
  String getName()
  {
    return name;
  } //!< retourne le nom de la fonction
  void setName(String st);
  virtual void print() = 0;
  virtual String convertToDynELASourceFile() = 0;
};

#endif
