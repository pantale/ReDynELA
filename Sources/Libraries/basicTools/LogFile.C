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
// revision date : 17/07/1997

/*!
  \file LogFile.C
  \brief Fichier .C de d�finitions d'un interface pour les fichiers logs
  \ingroup basicTools

  Ce fichier regroupe la d�finition de la classe LogFile utilis�e pour l'�criture et la gestion de fichiers de log.

  \author Olivier PANTALE
  \since DynELA 1.0.0
  \date 2002
*/

#include <LogFile.h>
#include <Unix.h>
#include <Error.h>

//!Constructeur de la classe LogFile
/*!
  Le constructeur ouvre le fichier � cr�er et ajoute automatiquement un ent�te standard en haut de celui-ci (voir m�thode headerWrite()).
  \param str nom du fichier log � cr�er
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
LogFile::LogFile(String str, int verb)
//-----------------------------------------------------------------------------
{
  if (str == "")
  {
    fatalError("LogFile::LogFile", "Must specify a log filename in the constructor");
  }

  // put the name
  name = str;

  // open the stream
  stream.open(name.chars());

  if (!stream)
  {
    fatalError("LogFile::LogFile", "Cannot open stream for file %s", name.chars());
  }

  // �criture de l'ent�te
  headerWrite();

  verbosity = verb;
  level = 1;
}

//!Destructeur de la classe LogFile
/*!
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
LogFile::~LogFile()
//-----------------------------------------------------------------------------
{
  stream.close();
}

//!�criture dans le fichier log
/*!
  Cette m�thode est utilis�e pour �crire une chaine de caract�res sur le fichier log. En plus de l'�criture, on ajoute une commande de vidage du buffer d'�criture afin que le fichier soit mis � jour en continu.
  \param s chaine de caract�res de type String
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &st, const String &s)
//-----------------------------------------------------------------------------
{
  // out the string
  st.out(s);

  // continue the buffer
  return st;
}

//!�criture dans le fichier log
/*!
  Cette m�thode est utilis�e pour �crire une valeur num�rique enti�re le fichier log. En plus de l'�criture, on ajoute une commande de vidage du buffer d'�criture afin que le fichier soit mis � jour en continu.
  \param val valeur num�rique de type Indice
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &st, const Indice &val)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(val);

  // out the string
  st.out(str);

  // continue the buffer
  return st;
}

//!�criture dans le fichier log
/*!
  Cette m�thode est utilis�e pour �crire une valeur num�rique r�elle le fichier log. En plus de l'�criture, on ajoute une commande de vidage du buffer d'�criture afin que le fichier soit mis � jour en continu.
  \param val valeur num�rique de type Real
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &st, const Real &val)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(val);

  // out the string
  st.out(str);

  // continue the buffer
  return st;
}

//!�criture dans le fichier log
/*!
  Cette m�thode est utilis�e pour �crire une chaine de caract�res sur le fichier log. En plus de l'�criture, on ajoute une commande de vidage du buffer d'�criture afin que le fichier soit mis � jour en continu.
  \param str chaine de caract�res � �crire sur le fichier log
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void LogFile::out(String str)
//-----------------------------------------------------------------------------
{
  if (level <= verbosity)
  {

    // write the string
    stream << str.chars();

    // force writing information
    stream.flush();
  }
}

//!�criture d'un ent�te au fichier
/*!
  Cette m�thode �crit un ent�te � un fichier log. Cet ent�te comporte divers renseignements comme indiqu� ci-dessous sur un exemple:
  DynELA v.1. 0.0-0
  
  Tue Dec 17 08:59:24 2002
  
  Job created by pantale
  
  Job executed on a i386-linux system
  
  Name of the host pcpantale.enit.fr
  
  Host ID 868306178

  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void LogFile::headerWrite()
//-----------------------------------------------------------------------------
{
  Unix os;

  // write the header of the file
  stream << "DynELA v. " << VERSION << "." << RELEASE << "." << UNDER_RELEASE << "-" << ALPHA_VERSION << endl;
  stream << os.getCurrDate() << endl;
  stream << "Job created by user " << os.getCurrEnvValue("LOGNAME") << endl;
  stream << "Job executed on a " << os.getCurrHosttype() << " system\n";
  stream << "Name of the host " << os.getCurrHostname() << endl;
  stream << "Host ID " << os.getCurrHostId() << endl;
  stream << endl
         << endl;

  separatorWrite();
}

//!ecriture d'un s�parateur dans un fichier log
/*!
  Cette m�thode �crit un s�parateur de champ dans un fichier log. Cette m�thode est utile pour ins�rer un ensemble de caract�res servant de s�paration dans le fichier afin de visualiser les diff�rentes parties de texte.
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void LogFile::separatorWrite()
//-----------------------------------------------------------------------------
{
  stream << "\n//-----------------------------------------------------------------------------\n\n";
}

//!r�glage de niveau de sortie pour les prochains messages
/*!
  Cette m�thode permet de sp�cifier que les prochains messages achemin�s seront de niveau donn� en argument. Tous les masseges de niveau plus �lev� que celui-ci seront ignor�s.
  \param lev nouveau niveau
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void LogFile::setLevel(int lev)
//-----------------------------------------------------------------------------
{
  level = lev;
}

//!r�glage de niveau de sortie pour les prochains messages
/*!
  Cette m�thode permet de sp�cifier que les prochains messages achemin�s seront de niveau plus �l�v� d'une unit�. Tous les masseges de niveau plus �lev� que celui-ci seront ignor�s.
  \param lev nouveau niveau
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void LogFile::upLevel()
//-----------------------------------------------------------------------------
{
  level++;
}

//!r�glage de niveau de sortie pour les prochains messages
/*!
  Cette m�thode permet de sp�cifier que les prochains messages achemin�s seront de niveau plus bas d'une unit�. Tous les masseges de niveau plus �lev� que celui-ci seront ignor�s.
  \param lev nouveau niveau
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void LogFile::downLevel()
//-----------------------------------------------------------------------------
{
  level--;
  if (level <= 1)
    level = 1;
}
