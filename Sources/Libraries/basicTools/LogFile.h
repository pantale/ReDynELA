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
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

// begin date : 16/07/1997
// revision date : 25/09/2002

/*!
  \file LogFile.h
  \brief Fichier .h de d�finitions d'un interface pour les fichiers logs
  \ingroup basicTools

  Ce fichier regroupe la d�finition de la classe LogFile utilis�e pour l'�criture et la gestion de fichiers de log.

  \author Olivier PANTALE
  \since DynELA 1.0.0
  \date 2002
*/

#ifndef __LogFile_h__
#define __LogFile_h__

#include <Const.h>
#include <Boolean.h>
#include <String.h>
/* #include <upBasicTools.h> */

/*!
  \brief d�finitions d'un interface pour les fichiers logs
  \ingroup basicTools
  \since DynELA 1.0.0
  \date 2002
  \author Olivier PANTALE
  
  Cette classe contient un ensemble de m�thodes d�di�es � la gestion des fichiers de log. Elle contient en plus une notion de niveau de logfile.
*/
/** @dia:pos 62,60 */
class LogFile
{

  /** @dia:route 3,90;h,62,60.7,50,43.1,54 */
  String name;
  ofstream stream;
  int verbosity;
  int level;

public:
  LogFile(String str, int verb = 1);
  ~LogFile();
  friend LogFile &operator<<(LogFile &out, const String &s);
  friend LogFile &operator<<(LogFile &out, const Indice &val);
  friend LogFile &operator<<(LogFile &out, const Real &val);
  void out(String str);
  void headerWrite();
  void separatorWrite();
  void setLevel(int lev);
  void upLevel();
  void downLevel();
  void setVerbosity(int verb) { verbosity = verb; }
};

#endif
