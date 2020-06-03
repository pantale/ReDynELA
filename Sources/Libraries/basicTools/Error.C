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
  \file Error.C
  \brief Definition of the errors for the DynELA FEM code

  This file defines the errors and warning that are used in the DynELA FEM code.
  The functions defined in this files serves to display messages and warnings.
  \ingroup basicTools
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#include <Error.h>

int totalWarningMessagesDisplayed = 1; //!< Total number of warnings actually displayed. This later is initialized to 1.

/*!
  \brief Displays a warning message

  This function displays a warning message on the stderr flux.
  Each call to this function will increment the internal counter \ref totalWarningMessagesDisplayed.
  When the maximal allowed value of \ref maxWarnings is reached, the execution of program is stopped.

  Example :
  \code
  warning("duplicate::operation(int i)",
          "the value of i is negative\ni=%d\n",i);
  \endcode

  \param function_name Warning message to be displayed given as const char *
*/
//-----------------------------------------------------------------------------
void warning(const char *function_name, ...)
//-----------------------------------------------------------------------------
{
  va_list ap;
  char *fmt;
  va_start(ap, function_name);

  // affichage du message de warning
  fprintf(stderr, "WARNING [%d] in function: %s\n",
          totalWarningMessagesDisplayed, function_name);

  // affichage de la ligne de commentaires
  fmt = va_arg(ap, char *);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);

  // forcer l'affichage
  cerr.flush();

  if (totalWarningMessagesDisplayed++ >= maxWarnings)
    fatalError("Maximum number of warnings reached",
               "You may modify the source code of your application "
               "or increase the total number of allowed warning messages "
               "fixed here to %d\n",
               maxWarnings);
}

/*!
  \brief Displays a warning message and ask the user if he wants to continue the execution of the program
  
  This function displays a warning message on the stderr flux and ask the user if he wants to continue the execution of the program.
  Each call to this function will increment the internal counter \ref totalWarningMessagesDisplayed.
  When the maximal allowed value of \ref maxWarnings is reached, the execution of program is stopped.

  Exemple :
  \code
  warningQuestion("duplicate::operation(int i)",
  "the value of i is negative\ni=%d\n",i);
  \endcode
  \param function_name Warning message to be displayed given as const char *
*/
//-----------------------------------------------------------------------------
void warningQuestion(char *function_name, ...)
//-----------------------------------------------------------------------------
{
  va_list ap;
  char *fmt;
  va_start(ap, function_name);

  // affichage du message fatal
  fprintf(stderr, "WARNING [%d] in function: %s\n",
          totalWarningMessagesDisplayed, function_name);

  // affichage de la ligne de commentaires
  fmt = va_arg(ap, char *);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);

  // forcer l'affichage
  cerr.flush();

  if (totalWarningMessagesDisplayed++ >= maxWarnings)
    fatalError("Maximum number of warnings reached",
               "You may modify the source code of your application "
               "or increase the total number of allowed warning messages "
               "fixed here to %d\n",
               maxWarnings);
  cout << "\ndo you want to continue the parsing of the file (y/n)\n";
  cout.flush();
  fflush(stdin);
  if (getchar() != 'y')
  {
    exit(0);
  }
}

/*!
  \brief Displays an error message and stop the execution of the program.

  This function displays an error message on the stderr flux and the execution of program is stopped.

  Exemple :
  \code
  fatalError("duplicate::operation(int i)",
  "the value of i is negative\ni=%d\n",i);
  \endcode

  \param function_name Error message to be displayed given as const char *
*/
//-----------------------------------------------------------------------------
void fatalError(const char *function_name, ...)
//-----------------------------------------------------------------------------
{
  va_list ap;
  char *fmt;
  va_start(ap, function_name);

  // affichage du message fatal
  fprintf(stderr, "\nFATAL ERROR in function: %s\n", function_name);

  // affichage de la ligne de commentaires
  fmt = va_arg(ap, char *);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);

  // fin de programme
  fprintf(stderr, "program aborted\n");
  exit(-1);
}

/*!
  \brief Displays an error message and stop the execution of the program.

  This function displays an internal error message on the stderr flux and the execution of program is stopped.

  Exemple :
  \code
  internalFatalError("duplicate::operation(int i)",
  "the value of i is negative\ni=%d\n",i);
  \endcode
  \param function_name Error message to be displayed given as const char *
*/
//-----------------------------------------------------------------------------
void internalFatalError(char *function_name, ...)
//-----------------------------------------------------------------------------
{
  va_list ap;
  char *fmt;
  va_start(ap, function_name);

  // affichage du message fatal
  fprintf(stderr, "\nINTERNAL FATAL ERROR in function: %s\n", function_name);

  // affichage de la ligne de commentaires
  fmt = va_arg(ap, char *);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);

  // contact
  fprintf(stderr, "You might contact main developper of this module\n");
  fprintf(stderr, "Or DynELA main developper\n");
  fprintf(stderr, "email this message to : pantale@enit.fr\n");

  // fin de programme
  fprintf(stderr, "\nprogram aborted\n");
  exit(-1);
}
