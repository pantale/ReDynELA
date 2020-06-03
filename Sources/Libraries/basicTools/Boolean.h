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
  \file Boolean.h
  \brief Definition of the boolean values for the DynELA FEM code

  This file defines the boolean values used in the DynELA FEM code. Usually in C and C++ a variable of type \b int  is used to define a boolean type.
  It's better to define a distinct type \b Boolean in order to forbid it to be implicitely converted to an \b int type.
  In order to avoid conflicts with other libraries (and the bas C library), this boolean variable is written \b Boolean.
  Conversion into an \b int is automatic (and implicit) or with the help of the \e operator \b int
  Conversion from an \b int (i.e., the return value of a C standard function \b strcmp) into a Boolean have to be made through the explicit function \b getBoolean().

  \ingroup basicTools
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __BooLean_h__
#define __BooLean_h__

/*!
  \typedef BooLean
  \brief Enumerate of default boolean values

  This defines 5 groups of 2 boolean variables that are alternatively \b false and \b true.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
*/
typedef enum
{
  Off = 0,     //!< defines the value for the boolean variable \b Off
  On = 1,      //!< defines the value for the boolean variable \b On
  False = 0,   //!< defines the value for the boolean variable \b False
  True = 1,    //!< defines the value for the boolean variable \b True
  No = 0,      //!< defines the value for the boolean variable \b No
  Yes = 1,     //!< defines the value for the boolean variable \b Yes
  Fail = 0,    //!< defines the value for the boolean variable \b Fail
  Success = 1, //!< defines the value for the boolean variable \b Success
  Bad = 0,     //!< defines the value for the boolean variable \b Bad
  Ok = 1       //!< defines the value for the boolean variable \b Ok
} BooLean;

//!Explicit conversion of an int to a Boolean
/*!
  This function converts an interger value of type \b int into a \b Boolean.
  \param param \b int value to be converted into a \b Boolean
  \return \b Boolean resulting from the conversion. Returned value is \b False if the parameter \b param is zero, \b True in all the other cases
*/
inline Boolean getBooLean(int param)
{
  return (param == 0 ? False : True);
}

//!Negation operator for type Boolean
/*!
  This function is an opposite operator for the boolean type.
  \param b Boolean value valeur that we want to negate
  \return \c True if the given parameter is \c False. \c False if the given parameter is \c True.
*/
inline Boolean notBooLean(Boolean b)
{
  return (b ? False : True);
}

//!Explicit negative conversion of an int to a Boolean
/*!
  This function converts an interger value of type \b int into a \b Boolean, but returns the opposite.
  \param param \b int value to be converted into a \b Boolean
  \return Boolean resulting from the conversion. Returned value is \b True if the parameter \b param is zero, \b False in all the other cases
*/
inline Boolean notBooLean(int param)
{
  return (getBooLean(!param));
}

#endif
