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
  \file Error.h
  \brief Declaration of the errors for the DynELA FEM code

  This file defines the errors and warning that are used in the DynELA FEM code.
  The functions defined in this files serves to display messages and warnings.
  \ingroup basicTools
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __Errorme_h__
#define __Errorme_h__

#include <Const.h>

void fatalError(const char *st, ...);
void internalFatalError(char *st, ...);
void warning(const char *st, ...);
void warningQuestion(char *st, ...);

extern int totalWarningMessagesDisplayed;
/*!
  \def lineERROR
  \brief Display an error message.

  This inline function display an error message including the name of the source file and the line number where the error has occured.
  This is a very usefull method for debugging.
  The execution of the program is not stopped, this is just a display of the error.
*/
#define lineERROR "source file %s line %d", __FILE__, __LINE__

/*!
  \def fatalErrorLine
  \brief Display an error message and stop the execution of the program.

  This inline function display an error message including the name of the source file and the line number where the error has occured.
  This is a very usefull method for debugging.
  The execution of the program is stopped.
*/
#define fatalErrorLine                                                \
  cerr << "source file " << __FILE__ << " line " << __LINE__ << endl; \
  fatalError

/*!
  \def internalFatalErrorLine
  \brief Display an error message and stop the execution of the program.

  This inline function display an error message including the name of the source file and the line number where the error has occured.
  This function has to be used for internal fatal errors.
  This is a very usefull method for debugging.
  The execution of the program is stopped.
*/
#define internalFatalErrorLine                                        \
  cerr << "source file " << __FILE__ << " line " << __LINE__ << endl; \
  internalFatalError

/*!
  \def warningLine
  \brief Display a warning message.

  This inline function display a warning message including the name of the source file and the line number where the error has occured.
  This is a very usefull method for debugging.
  The execution of the program is not stopped, this is just a display of the warning.
*/
#define warningLine                                                   \
  cerr << "source file " << __FILE__ << " line " << __LINE__ << endl; \
  warning

#endif
