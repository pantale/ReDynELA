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
  \file Unix.C
  \brief Fichier .C de d�finitions associ�es � l'OS Unix
  \ingroup basicTools

  Ce fichier regroupe la d�finition de la classe Unix utilis�e pour l'environnement de travail Unix.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <Unix.h>
#include <Error.h>
#include <unistd.h>

//!ex�cution d'une commande syst�me
/*!
  Cette m�thode ex�cute une commande externe. Lancement d'un nouveau processus par l'interm�diaire de la commande syst�me \b system(). Le nouveau programme est un nouveau processus totalement ind�pendant de l'application actuelle. Le seul lien est que le programme appelant attend la fin de l'ex�cution du nouveau processus pour continuer les op�rations (saus si on utilise la param�tre magique & dans la ligne de commande... Vous connaissez Unix ?). Comme cette commande est simple, l'environnement de travail pour l'�x�cution du nouveau process est /bin/sh. Un message d'erreur est g�n�r� si un probl�me � �t� rencontr� au cours de l'ex�cution.

  Pour des situations plus complexes, il faudra faire appel � des m�thodes de lancement de programme par les "classiques" fork() et exec(). Mais l� �a conduit in�vitablement aux probl�mes de \b threads et c'est une autre histoire en fait. ;-0

  \param cmd chaine de caract�res de type \b String d�finissant la commande syst�me � lancer
  \return valeur indiquant si l'ex�cution de la commande s'est ex�cut�e sans probl�mes. Une valeur 0 est retourn�e si tout � bien fonctionn�, une autre valeur est retourn�e si un probl�me � �t� rencontr� au cours de l'ex�cution.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
int Unix::execute(const String &cmd)
//-----------------------------------------------------------------------------
{
  int status = system(cmd.chars());

  if (status != 0)
  {
    if (status == 127)
    {
      cerr << "/bin/sh could not be executed\n";
    }
    cerr << "Unix::execute()\n";
    cerr << "Last Unix command execution:\n";
    cerr << cmd.chars() << "\nfailed (return value=" << status << ")\n";
  }

  return status;
}

//!recup�re la valeur associ�e � une variable d'environnement
/*!
  Cette m�thode r�cup�re la valeur associ�e � une variable d'environnement Unix. Si cette variable n'est pas d�finie, cette m�thode renvoie la chaine de caract�res suivante "cannot get \<var\> value". La valeur retourn�e est de type String.
\param envname nom de la variable d'environnement
\return valeur associ�e � la variable d'environnement sous forme de String
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String
Unix::getCurrEnvValue(const String &envname)
//-----------------------------------------------------------------------------
{
  String s;
  char *c = getenv(envname.chars());

  // if variable is defined
  if (c != NULL)
  {
    s = c;
    s.strip();
  }

  // else if not defined
  else
    s = "cannot get " + envname + " value";

  // return the string
  return (s);
}

//!teste la pr�sence d'une variable d'environnement d�finie
/*!
  Cette m�thode teste la d�finition d'une variable d'environnement. Elle renvoie une valeur bool�enne qui renseigne sur l'�tat de d�finition de cette variable d'environnement.
\param envname nom de la variable d'environnement
\return \c True si la variable d'environnement est d�finie sur le syst�me, \c False dans le cas contraire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Boolean
Unix::existCurrEnvValue(const String &envname)
//-----------------------------------------------------------------------------
{
  char *c = getenv(envname.chars());

  // test if defined
  if (c != NULL)
    return True;

  // not defined
  return False;
}

//!renvoie le login de l'utilisateur
/*!
  Cette m�thode renvoie le login de l'utilisateur du syst�me sous forme d'une chaine de caract�res.
\return login de l'utilisateur ou "unknown user" si cette information ne peut pas �tre donn�e
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String
Unix::getCurrLogin()
//-----------------------------------------------------------------------------
{
  String s;
  char *c = getlogin();

  // test if defined
  if (c != NULL)
  {
    s = c;
    s.strip();
  }

  // not defined
  else
    s = "unknown user";

  // return the string
  return (s);
}

//!renvoie le nom de la machine
/*!
  Cette m�thode renvoie le nom de la machine sur laquelle l'application est en cours d'ex�cution.
\return nom de la machine ou "unknown host" si cette information ne peut pas �tre donn�e
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String
Unix::getCurrHostname()
//-----------------------------------------------------------------------------
{
#ifdef IBM_Cplusplus
  // gethostname was not found in any .h-file....
  char *hostname = "IBM-RS6000-unknown-hostname";
#else
  char hostname[80];
  gethostname(hostname, 80); // g++ does not know about gethostname?
#endif

  String s;
  if (strlen(hostname) != 0)
  {
    s = hostname;
    s.strip();
  }
  else
    s = "unknown host";

  return (s);
}

//!renvoie le type de la machine
/*!
  Cette m�thode renvoie le type de la machine sur laquelle l'application est en cours d'ex�cution.
\return type de la machine sur laquelle l'application est en cours d'ex�cution.
*/
//-----------------------------------------------------------------------------
String
Unix::getCurrHosttype()
//-----------------------------------------------------------------------------
{
  return (getCurrEnvValue("HOSTTYPE"));
}

//!renvoie la date et l'heure courante
/*!
  Cette m�thode renvoie la date et l'heure courante au niveau du syst�me.
\param full : cette valeur bool�enne d�finit la nature du r�sultat retourn�. Si la valeur est True le format est alors le format comple sous la forme (Fri Jan 25 15:08:24 2002) si cette valeur est False, alors le format retourn� est le format court de la forme (Jan 25, 2002). La valeur par d�faut si rien n'est pr�cis� est True.
\return date et heure actuelle ou "unknown date" si cette information ne peut pas �tre donn�e
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String
Unix::getCurrDate(Boolean full)
//-----------------------------------------------------------------------------
{
  char *thedate;

  time_t theclock;

  theclock = time(0);
  thedate = (char *)ctime(&theclock);

  String s;
  if (thedate != NULL)
  {
    // Full format: Fri Aug 20 14:23:26 1993
    s = thedate;
    s.strip();

    // Short format: Aug 20, 1993
    if (!full)
    {
      s.del(0, 4); // Kill day + space, e.g. Fri
      int i = 5;
      while (s[i] != ' ')
        ++i;
      s.del(i + 1, 8); // Kill time and year and spaces, e.g. 13:00:00 1993
      s[6] = ',';
    }
  }

  else
    s = "unknown date";

  return (s);
}

//!renvoie l'heure courante
/*!
  Cette m�thode renvoie l'heure courante au niveau du syst�me.
  \return valeur de l'heure courante sous forme d'un String (format : 14:23:26)
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String
Unix::getCurrTime()
//-----------------------------------------------------------------------------
{
  String s = getCurrDate(True);

  // Time format is 14:23:26
  s.del(0, 11);
  s = s.before(8);

  return (s);
}

//!renvoie le nom du r�pertoire courant
/*!
  Cette m�thode renvoie le nom du r�pertoire courant depuis lequel l'ex�cution � �t� lanc�e.
  \return r�pertoire courant ou "unknown pathname" si cette information ne peut �tre donn�e.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String
Unix::getCurrPathname()
//-----------------------------------------------------------------------------
{
  char pathname[2500];
  if (getcwd(pathname, 2500) == NULL)
  {
    fatalError("Unix::getCurrPathname()",
               "getCurrPathname()\nsomething went wrong when calling the system function getcwd\n");
  }

  String s;
  if (strlen(pathname) > 0)
  {
    s = pathname;
    s.strip();
  }

  else
    s = "unknown pathname";

  return (s);
}

//!renvoie le hostid de la machine
/*!
  Cette m�thode renvoie le hostid de la machine sur laquelle le programme est ex�cut�. Le hostid est une valeur enti�re g�n�ralement donn�e sous une forme hexad�cimale 0xFFFFFFFF sur 32 bits. Ce num�ro est unique par machine.
  \return hostId de la machine.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
int Unix::getCurrHostId()
//-----------------------------------------------------------------------------
{
  return gethostid();
}

//-----------------------------------------------------------------------------
int compareIndice(const void *p1, const void *p2)
//-----------------------------------------------------------------------------
{
  return *((Indice *)p1) < *((Indice *)p2);
}

//-----------------------------------------------------------------------------
Boolean ArgumentsLineParse(char *rr, int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // boucle de parsing des arguments
  for (Indice i = 1; i < argc; i++)
  {

    // test d'argument
    if (strncmp(rr, argv[i] + 1, strlen(rr)) == 0)
    {
      // cout << argv[i]+1 << " ";

      // retour
      return True;
    }
  }

  // retour
  return False;
}

//-----------------------------------------------------------------------------
Indice ArgumentsLinePosition(char *rr, int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // boucle de parsing des arguments
  for (Indice i = 1; i < argc; i++)
  {

    // test d'argument
    if (strncmp(rr, argv[i] + 1, strlen(rr)) == 0)
    {
      // cout << argv[i]+1 << " ";

      // retour
      return i;
    }
  }

  // retour
  return 0;
}

//-----------------------------------------------------------------------------
char *ArgumentsLineGet(char *rr, int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // boucle de parsing des arguments
  for (Indice i = 1; i < argc; i++)
  {

    // test d'argument
    if (strcmp(rr, argv[i] + 1) == 0)
    {
      // cout << argv[i]+1 << " ";

      // retour
      return argv[i + 1];
    }
  }

  // retour
  return NULL;
}
