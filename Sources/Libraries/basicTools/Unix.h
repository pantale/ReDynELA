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

// begin date : 16/07/1997
// revision date : 25/09/2002

/*!
  \file Unix.h
  \brief Fichier .h de d�finitions associ�es � l'OS Unix
  \ingroup basicTools

  Ce fichier regroupe la d�finition de la classe Unix utilis�e pour l'environnement de travail Unix.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#ifndef __Unix_h__
#define __Unix_h__

#include <Const.h>
#include <Boolean.h>
#include <String.h>
/* #include <unistd.h> */
/* #include <stdlib.h> */
/* #include <time.h> */
/* #include <iostream> */

/*!
  \brief fonctions de basse associ�es � l'O.S Unix.
  \ingroup basicTools
  \since DynELA 0.9.5
  \date 1997-2004
  \author &copy; Olivier PANTALE
  
  Cette classe contient un ensemble de m�thodes d�di�es � l'environnement de travail Unix. Ces m�thodes sont utilis�es pour r�cup�rer des variables d'environnement syst�me depuis un programme, la date courante, l'heure et certaines d�finitions concernant les r�pertoires et chemins d'acc�s aux programmes, le tout dans un environnement Unix. Cette classe contient �galement des m�thodes utilis�es pour lancer des applications externes.

\warning Cette classe est li�e � Unix. Pour un portage de DynELA sur Windows, il faudra faire des coupes franches ici... Mais de toutes fa�on, �a ne m'int�resse pas, mais si quelqu'un veut le faire, il peut ;-)
*/

/** @dia:pos 4,26 */
class Unix
{

public:
  Unix()
  {
  }
  ~Unix()
  {
  }
  int execute(const String &cmd);
  String getCurrEnvValue(const String &envname);
  Boolean existCurrEnvValue(const String &envname);
  String getCurrLogin();
  String getCurrHostname();
  String getCurrHosttype();
  String getCurrDate(Boolean full = True);
  String getCurrTime();
  int getCurrHostId();
  String getCurrPathname();
};

int compareIndice(const void *p1, const void *p2);

Boolean ArgumentsLineParse(char *rr, int argc, char **argv);
Indice ArgumentsLinePosition(char *rr, int argc, char **argv);
char *ArgumentsLineGet(char *rr, int argc, char **argv);

#endif
