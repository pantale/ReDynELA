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

// begin date : 24/06/1998
// revision date :

#include <iostream>
#include <String.h>
#include <Boolean.h>

/*!
  \file String.C
  \brief fichier . h de d�finition des chaines de caract�res
  \ingroup basicTools

  Ce fichier regroupe la d�finition des chaines de caract�res pour DynELA. Les chaines de caract�res font partie d'une nouvelle classe nomm�e String qui permet la pluspart des manipulations sur les caract�res. L'ensemble des m�thodes de cette classe sont document�es.

  Depuis la version v. 1.0.0, cette classe h�rite de la classe \b string de Standard Template Library.

  \author &copy; Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/

//!constructeur par d�faut de la classe String
/*!
  Allocation m�moire pour une chaine de caract�res vide. Cette m�thode est utilis�e pour cr�er l'allocation m�moire pour une chaine de caract�res vide.

  Exemple:
  \code
  String s; // allocation m�moire vide (taille ind�finie)
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String::String()
//-----------------------------------------------------------------------------
{
}

//!constructeur de la classe String � partir d'un caract�re unique
/*!
  Ce constructeur alloue l'espace m�moire pour une chaine de caract�res � partir de la donn�e d'un seul caract�re. Cette m�thode peut sembler inutile car utiliser un String pour un seul caract�re est bien b�te, mais elle permet de faire l'addition de petits bouts de chaine de caract�res.

  Exemple:
  \code
  String s = 'k'; // s est une chaine d'un caract�re 'k'
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
// //-----------------------------------------------------------------------------
// String::String (const char c) : string (1, c)
// //-----------------------------------------------------------------------------
// {
// }

//!constructeur � partir d'un type char*
/*!
  Ce constructeur permet d'allouer l'espace m�moire pour un String et d'initialiser cette chaine de caract�res � partir d'une suite de caract�res standerd de type char*.
  
  Exemple:
  \code
  String y = "a string";
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String::String(const char *s) // : string (s)
//-----------------------------------------------------------------------------
{
  assign(s);
}

//-----------------------------------------------------------------------------
String::String(const string &st) // : string (st)
//-----------------------------------------------------------------------------
{
  assign(st);
}

//-----------------------------------------------------------------------------
String::String(const String &st) // : string (st)
//-----------------------------------------------------------------------------
{
  assign(st);
}
//!destructeur associ� � la classe String
/*!
  Ceci est le destructeur de la classe String.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String::~String()
//-----------------------------------------------------------------------------
{
}

//!constructeur � partir d'un type string
/*!
  Ce constructeur permet d'allouer l'espace m�moire pour un String et d'initialiser cette chaine de caract�res � partir d'une suite de caract�res standerd de type char*.
  
  Exemple:
  \code
  String y = "a string";
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::operator=(const char *st)
//-----------------------------------------------------------------------------
{
  assign(st);
  return (*this);
}

// String::operator const char *() const
// {
//   return c_str();
// }
// //-----------------------------------------------------------------------------
// String & String::operator = (const char* st)
// //-----------------------------------------------------------------------------
// {
// (*this)=st;

//  return (*this);
// }

//!conversion d'une valeur num�rique enti�re en un String
/*!
  Cette m�thode permet de convertir une valeur num�rique de type int en une chaine de caract�res.

  Exemple:
  \code
  String toto;
  toto.convert(2);
  cout << toto << endl;
  \endcode
  \param val valeur num�rique � convertir en un String
  \return La chaine de caract�res de retour.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::convert(int val, short leading)
//-----------------------------------------------------------------------------
{
  char tmpStr[100];
  sprintf(tmpStr, "%d", val);
  assign(tmpStr);
  if (length() < leading)
  {
    String leadString = std::string(leading - length(), '0');
    *this = leadString + *this;
  }
  return *this;
}

//!conversion d'une valeur num�rique enti�re en un String
/*!
  Cette m�thode permet de convertir une valeur num�rique de type int en une chaine de caract�res.

  Exemple:
  \code
  String toto;
  toto.convert(2);
  cout << toto << endl;
  \endcode
  \param val valeur num�rique � convertir en un String
  \return La chaine de caract�res de retour.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::convert(Indice val)
//-----------------------------------------------------------------------------
{
  char tmpStr[100];
  sprintf(tmpStr, "%ld", val);
  assign(tmpStr);
  return *this;
}

//!conversion d'une valeur r�elle en un String
/*!
  Cette m�thode permet de convertir une valeur num�rique r�elle en une chaine de caract�res en sp�cifiant le type de conversion par une chaine de caract�res de contr�le.

  Exemple:
  \code
  String toto;
  toto.convert(2.0,"%10.3E");
  cout << toto << endl;
  \endcode
  \param val valeur r�elle � convertir en un String
  \param st format de sortie de la chaine de caract�res. par d�faut, si on ne pr�cise rien, on utilise un format de type "%ld"
  \return chaine de caract�res de retour
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::convert(Real val, const char *st)
//-----------------------------------------------------------------------------
{
  char tmpStr[100];
  sprintf(tmpStr, st, val);
  assign(tmpStr);
  return *this;
}

//!remplacement d'un caract�re dans une chaine
/*!
  Cette m�thode recherche et remplace un caract�re donn� par un autre dans une chaine de caract�res.
  \param from caract�re � remplacer dans la chaine
  \param to caract�re de remplacement dans la chaine
  \return nouvelle chaine de caract�res avec la substitution
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::replace(const char from, const char to)
//-----------------------------------------------------------------------------
{
  string _to(1, to);
  const char *p = data();
  int l = length();

  // boucle de recherche
  for (int i = 0; i < l; i++)
    if (p[i] == from)
      string::replace(i, 1, _to);

  // retour
  return (*this);
}

//!remplacement d'une sous chaine de caract�res dans une chaine
/*!
  Cette m�thode recherche et remplace une sous-chaine de caract�res donn�e par une autre dans une chaine de caract�res.
  \param from sous-chaine de caract�res � remplacer dans la chaine
  \param to sous-chaine de caract�res de remplacement dans la chaine
  \return nouvelle chaine de caract�res avec la substitution
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::replace(const String &from, const String &to)
//-----------------------------------------------------------------------------
{
  size_type i;
  size_type pos = length();

  // boucle de recherche
  while ((i = rfind(from, pos)) != npos)
  {
    string::replace(i, from.length(), to);
    pos = i;
  }

  // retour
  return (*this);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res. La position de la sous-chaine est donn�e par la valeur de l'indice de d�part et la longueur de la sous-chaine. La chaine d'origine est inchang�e.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.subString(4,5); // b="haine"
  \endcode
  \param startpos position de d�part dans la chaine de caract�res
  \param length longueur de la chaine � extraire
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::subString(int startpos, int length) const
//-----------------------------------------------------------------------------
{
  String ret;
  ret.assign(*this, startpos, length);
  return ret;
}

//!recherche d'un caract�re dans une chaine
/*!
  Cette m�thode cherche un caract�re dans une chaine de caract�res � partir d'une position donn�e dans la chaine.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  printf("%d\n",a.search('�',1); // renvoie 19 ('�' � la 19 eme position)
  printf("%d\n",a.search('�',20); // renvoie -1 (non trouv�)
  \endcode
  \param startpos position de d�part de recherche dans la chaine
  \param c caract�re � rechercher
  \return position du caract�re dans la chaine, ou valeur -1 si ce caract�re n'a pu �tre trouv�.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
int String::search(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int pos = find(c, startpos);
  if (pos == (int)npos)
    return -1;
  return pos;
}

//!recherche d'une sous chaine de caract�res dans une chaine
/*!
  Cette m�thode cherche une sous chaine de caract�res dans une chaine de caract�res � partir d'une position donn�e dans la chaine.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  printf("%d\n",a.search("de",20); // renvoie 10 (position dans la chaine)
  printf("%d\n",a.search("del",20); // renvoie -1 (non trouv�)
  \endcode
  \param startpos position de d�part de recherche dans la chaine
  \param substring sous chaine � rechercher � rechercher
  \return position de la sous-chaine dans la chaine, ou valeur -1 si cette sous-chaine n'a pu �tre trouv�.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
int String::search(const String &substring, int startpos) const
//-----------------------------------------------------------------------------
{
  int pos = find(substring, startpos);
  if (pos == (int)npos)
    return -1;
  return pos;
}

//!suppression d'une partie d'une chaine de caract�res
/*!
  Cette m�thode supprime une partie d'une chaine de caract�res � partir d'une position donn�e et sur une longueur donn�e. La chaine donn�es est modifi�e

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  a.del(4,5); // a="ma c de caract�res"
  \endcode
  \param startpos position de d�part de la partie � supprimer
  \param length longueur de la partie � supprimer
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::del(int startpos, int length)
//-----------------------------------------------------------------------------
{
  string::replace(startpos, length, "");
}

//!suppression d'une partie d'une chaine de caract�res
/*!
  Cette m�thode supprime une partie d'une chaine de caract�res sp�cifi�e dans une autre chaine � partir d'une position de d�part donn�e. La chaine donn�es est modifi�e

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  a.del("chaine",0); // a="ma  de caract�res"
  \endcode
  \param y sous-chaine � supprimer
  \param startpos position de d�part pour la recherche dans la chaine
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::del(const String &y, int startpos)
//-----------------------------------------------------------------------------
{
  string::replace(find(y, startpos), y.length(), "");
}

//!suppression d'un caract�re dans une chaine
/*!
  Cette m�thode supprime un caract�re sp�cifi� dans une autre chaine � partir d'une position de d�part donn�e. La chaine donn�es est modifi�e

Exemple:
  \code
  String a = "ma chaine de caract�res";
  a.del('c',0); // a="ma haine de caract�res"
  \endcode
  \param y sous-chaine � supprimer
  \param startpos position de d�part pour la recherche dans la chaine
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::del(char c, int startpos)
//-----------------------------------------------------------------------------
{
  string::replace(find(c, startpos), 1, "");
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res. La position de la sous-chaine est donn�e par la valeur de l'indice de d�part et la longueur de la sous-chaine.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.at(4,5); // b="haine"
  \endcode
  \param first position de d�part dans la chaine de caract�res
  \param len longueur de la chaine � extraire
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::at(int first, int len) const
//-----------------------------------------------------------------------------
{
  return subString(first, len);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res. La position de la sous-chaine est donn�e par la valeur de l'indice de d�part et la longueur de la sous-chaine.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a(4,5); // b="haine"
  \endcode
  \param first position de d�part dans la chaine de caract�res
  \param len longueur de la chaine � extraire
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::operator()(int first, int len) const
//-----------------------------------------------------------------------------
{
  return subString(first, len);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res avant une position donn�e.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.before(4); // b="ma c"
  \endcode
  \param pos position de fin dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::before(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(0, pos);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res avant une position donn�e (caract�re compris).

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.through(4); // b="ma ch"
  \endcode
  \param pos position de fin dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::through(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(0, pos + 1);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res apr�s une position donn�e.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.after(4); // b="haine de caract�res"
  \endcode
  \param pos position de d�part dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::after(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(pos + 1, length() - (pos + 1));
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res apr�s une position donn�e (caract�re compris).

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.from(4); // b="chaine de caract�res"
  \endcode
  \param pos position de d�part dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::from(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(pos, length() - pos);
}
//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res. La position de la sous-chaine est donn�e par la premi�re occurence d'une chaine donn�e en param�tres � partir d'une position donn�e.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.at("haine",2); // b="haine"
  \endcode
  \param y sous-chaine de caract�res � rechercher
  \param startfirst position de d�part dans la chaine de caract�res
  \param len longueur de la chaine � extraire
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::at(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(y, startpos);
  if (first == -1)
    return "";
  return subString(first, y.length());
}

//!extraction d'un caract�re d'une chaine de caract�res
/*!
  Cette m�thode extrait un caract�re dans une chaine de caract�res. La position du caract�re est donn�e par la premi�re occurence dde ce caract�re donn� en param�tre � partir d'une position donn�e. 

C'est un peu idiot comme truc ca !!!

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.at('h',2); // b="h"
  \endcode
  \param y sous-chaine de caract�res � rechercher
  \param startfirst position de d�part dans la chaine de caract�res
  \param len longueur de la chaine � extraire
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::at(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(startpos, c);
  if (first == -1)
    return "";
  return subString(first, 1);
}
//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res avant une position donn�e par la premi�re occurence d'une chaine donn�e en param�tre de cette m�thode.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.before("chaine",2); // b="ma "
  \endcode
  \param y sous chaine � rechercher
  \param startpos position de d�but de recherche dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::before(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(y, startpos);
  if (last == -1)
    return "";
  return subString(0, last);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res avant une position donn�e par la premi�re occurence d'un caract�re donn� en param�tre de cette m�thode.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.before('c',2); // b="ma "
  \endcode
  \param c caract�re � rechercher
  \param startpos position de d�but de recherche dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::before(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(startpos, c);
  return subString(0, last);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res avant une position donn�e par la premi�re occurence d'une chaine donn�e en param�tre de cette m�thode. La sous chaine recherch�e est incluse.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.through("chaine",2); // b="ma chaine"
  \endcode
  \param y sous chaine � rechercher
  \param startpos position de d�but de recherche dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::through(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(y, startpos);
  if (last == -1)
    return "";
  if (last >= 0)
    last += y.length();
  return subString(0, last);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res avant une position donn�e par la premi�re occurence d'un caract�re donn� en param�tre de cette m�thode. Le caract�re recherch� est inclus.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.through('c',2); // b="ma c"
  \endcode
  \param c caract�re � rechercher
  \param startpos position de d�but de recherche dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::through(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(startpos, c);
  if (last == -1)
    return "";
  if (last >= 0)
    last += 1;
  return subString(0, last);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res apr�s une position donn�e par la premi�re occurence d'une chaine donn�e en param�tre de cette m�thode.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.after("chaine",2); // b=" de caract�res"
  \endcode
  \param y sous chaine � rechercher
  \param startpos position de d�but de recherche dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::after(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(y, startpos);
  if (first == -1)
    return "";
  if (first >= 0)
    first += y.length();
  return subString(first, length() - first);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res apr�s une position donn�e par la derni�re occurence d'une chaine donn�e en param�tre de cette m�thode.

  Exemple:
  \code
  String a = "ma chaine de caract�res de truc";
  String b= a.after("de"); // b=" truc"
  \endcode
  \param y sous chaine � rechercher
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::afterLast(const String &y) const
//-----------------------------------------------------------------------------
{
  int pos = rfind(y);
  if (pos == (int)npos)
    return "";
  pos += y.length();
  return at(pos, length());
}

//-----------------------------------------------------------------------------
String
String::beforeLast(const String &y) const
//-----------------------------------------------------------------------------
{
  int pos = rfind(y);
  if (pos == (int)npos)
    return "";
  pos += y.length();
  return at(0, pos - 1);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res apr�s une position donn�e par la premi�re occurence d'un caract�re donn� en param�tre de cette m�thode.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.after('c',2); // b="haine de caract�res"
  \endcode
  \param c caract�re � rechercher
  \param startpos position de d�but de recherche dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::after(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(startpos, c);
  if (first >= 0)
    first += 1;
  if (first == -1)
    return "";
  return subString(first, length() - first);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res apr�s une position donn�e par la premi�re occurence d'une chaine donn�e en param�tre de cette m�thode. La sous-chaine est incluse.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.from("chaine",1); // b="chaine de caract�res"
  \endcode
  \param y sous chaine � rechercher
  \param startpos position de d�but de recherche dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::from(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(y, startpos);
  if (first == -1)
    return "";
  return subString(first, length() - first);
}

//!extraction d'une sous-chaine de caract�res d'une chaine de caract�res
/*!
  Cette m�thode extrait une sous-chaine de caract�res dans une chaine de caract�res apr�s une position donn�e par la premi�re occurence d'un caract�re donn� en param�tre de cette m�thode.

  Exemple:
  \code
  String a = "ma chaine de caract�res";
  String b= a.from('c',2); // b="chaine de caract�res"
  \endcode
  \param c caract�re � rechercher
  \param startpos position de d�but de recherche dans la chaine de caract�res
  \return sous chaine de caract�res extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::from(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(startpos, c);
  if (first == -1)
    return "";
  return subString(first, length() - first);
}

//!remplit une chaine de caract�re � partir d'un fichier
/*!
  Cette m�thode list une chaine de caract�res sur un fichier et retourne un String contenant cette chaine de caract�res. Les caract�res sont lus dans le fichier jusqu'au caract�re de fin de ligne '\n'. Si la fin de fichier est atteinte, cette m�thode renvoie 0, sinon elle renvoie 1.Cette m�thode se trouve donc bien nomm�e.

  Exemple:
  \code
  FILE* pfile;
  String s;
  pfile=fopen("fich","r");
  while (s.scanFileLine(pfile)) cout << s << "\n";
  fclose(pfile);
  \endcode
  \param pfile fichier � lire
  \return booleen sp�cifiant si il reste des caract�res � lire dans le fichier.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean
String::scanFileLine(FILE *pfile)
//-----------------------------------------------------------------------------
{
  int c;
  *this = "";

  while ((c = getc(pfile)) != '\n')
  {
    if (c == EOF)
      return 0;
    *this += c;
  }

  return 1;
}

//!suppression des blancs inutiles
/*!
  Cette m�thode supprime les blancs en d�but et fin d'une chaine de caract�res comme indiqu� ci dessous:

  "  hello " -> "hello"
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::strip()
//-----------------------------------------------------------------------------
{
  size_type start;
  size_type stop;
  if ((start = find_first_not_of(" ")) == npos)
    return;
  if ((stop = find_last_not_of(" ")) == npos)
    return;

  *this = subString(start, stop - start + 1);
}

//!teste la pr�sence d'une chaine dans une autre et renvoie la position
/*!
  Cette m�thode recherche une chaine de caract�res dans une autre en tenant compte ou non de la diff�rence majuscules/minuscules. Cette m�thode � �t� d�velopp�e pour bibView.
  \param substring chaine de caract�res � rechercher
  \param cas bool�en permettant de sp�cifier si on veut du sensitive search (True) ou non (False)
  \return position de la sous-chaine dans la chaine ou -1 en cas d'�chec de la recherche
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
int String::containsWithCaseSub(const String &substring, Boolean cas) const
//-----------------------------------------------------------------------------
{
  int startpos = 0;
  const char *s = chars();
  const char *t = substring.chars();
  int sl = length();
  int tl = substring.length();

  if (sl > 0 && tl > 0)
  {
    if (startpos >= 0)
    {
      const char *lasts = &(s[sl - tl]);
      const char *lastt = &(t[tl]);
      const char *p = &(s[startpos]);

      while (p <= lasts)
      {
        const char *x = p++;
        const char *y = t;
        if (cas)
        {
          while (*x++ == *y++)
            if (y >= lastt)
              return --p - s;
        }
        else
        {
          while (strncasecmp(x++, y++, 1) == 0)
            if (y >= lastt)
              return --p - s;
        }
      }
    }
    else
    {
      const char *firsts = &(s[tl - 1]);
      const char *lastt = &(t[tl - 1]);
      const char *p = &(s[sl + startpos + 1]);

      while (--p >= firsts)
      {
        const char *x = p;
        const char *y = lastt;
        if (cas)
        {
          while (*x-- == *y--)
            if (y < t)
              return ++x - s;
        }
        else
        {
          while (strncasecmp(x--, y--, 1) == 0)
            if (y < t)
              return ++x - s;
        }
      }
    }
  }
  return -1;
}

//!teste la pr�sence d'une chaine dans une autre
/*!
  Cette m�thode recherche une chaine de caract�res dans une autre en tenant compte ou non de la diff�rence majuscules/minuscules. Cette m�thode � �t� d�velopp�e pour bibView.
  \param substring chaine de caract�res � rechercher
  \param cas bool�en permettant de sp�cifier si on veut du sensitive search (True) ou non (False)
  \return booleen: True si la sous-chaine existe ou False sinon.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean
String::containsWithCase(const String &substring, Boolean cas) const
//-----------------------------------------------------------------------------
{
  return (containsWithCaseSub(substring, cas) >= 0);
}

//!convertit une chaine de caract�res en un type char*
/*!
  Cette m�thode convertit une chaine de caract�res en un type char*.

  Exemple:
  \code
  String x;
  x="chain to convert";
  printf("%s",x.chars());
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
const char *
String::chars() const
//-----------------------------------------------------------------------------
{
  return c_str();
}

//!teste la pr�sence d'un caract�re dans une chaine
/*!
  Cette m�thode teste si une chaine de caract�res contient un caract�re donn�.
  \param c caract�re � rechercher
  \return True si le caract�re existe, False dans le cas contraire
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean
String::contains(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  if (find(c, startpos) != npos)
    return True;
  return False;
}

//!recherche d'un caract�re dans une chaine
/*!
  Cette m�thode cherche une sous-chaine de caract�res dans une chaine existante et retourne True, ou la valeur False si cette chaine n'existe pas.
  \param startpos position de d�part pour la recherche
  \param substring sous chaine � rechercher
  \return True ou False selon le r�sultat de la recherche.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean
String::contains(const String &substring, int startpos) const
//-----------------------------------------------------------------------------
{
  if (find(substring, startpos) != npos)
    return True;
  return False;
}

//!�crit une chaine de caract�res sur le flux de sortie
/*!
  Cette m�thode �crit une chaine de caract�res sur le flux de donn�es ostream. Si la chaine de caract�res est vide, "empty" est �crit sur la sortie.

  \param s chaine de caract�res �crite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::print(const String &s) const
//-----------------------------------------------------------------------------
{
  cout << *this << endl;
}

//!teste si une chaine de caract�res est vide
/*!
  Cette m�thode teste si une chaine de caract�res est vide.
  \return True si la chaine de caract�res est vide False dans le cas contraire
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean
String::empty() const
//-----------------------------------------------------------------------------
{
  return ((*this) == "");
}

//!teste si une chaine de caract�res est non vide
/*!
  Cette m�thode teste si une chaine de caract�res est non vide.
  \return True si la chaine de caract�res est non vide False dans le cas contraire
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean
String::ok() const
//-----------------------------------------------------------------------------
{
  return !(empty());
}

//!renvoie le premier caract�re d'un String
/*!
  Cette m�thode retourne le premier caract�re d'un string
  \return premier caract�re du String
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
char String::firstchar() const
//-----------------------------------------------------------------------------
{
  return string::at(0);
}

//!renvoie le dernier caract�re d'un String
/*!
  Cette m�thode retourne le dernier caract�re d'un string
  \return dernier caract�re du String
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
char String::lastchar() const
//-----------------------------------------------------------------------------
{
  return string::at(length() - 1);
}

//!conversion des caract�res en minuscules
/*!
  Cette m�thode convertit les caract�res d'une chaine en minuscules
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::lowerCase()
//-----------------------------------------------------------------------------
{
  int n = length();
  for (int i = 0; i < n; i++)
  {
    if ((string::at(i) >= 'A') && (string::at(i) <= 'Z'))
    {
      string::at(i) += 'a' - 'A';
    }
  }
}

//!conversion des caract�res en majuscules
/*!
  Cette m�thode convertit les caract�res d'une chaine en majuscules
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::upperCase()
//-----------------------------------------------------------------------------
{
  int n = length();
  for (int i = 0; i < n; i++)
  {
    if ((string::at(i) >= 'a') && (string::at(i) <= 'z'))
    {
      string::at(i) += 'A' - 'a';
    }
  }
}

//!convertit un String en un r�el
/*!
  Cette m�thode extrait d'un String une valeur r�elle � une colonne donn�e.

  Exemple:
  \code
  String s="5.0 8.4 12.7e-6";
  double a,b,c;
  a=s.getRealAtPos(0); // a=5.0
  b=s.getRealAtPos(1); // b=8.4
  c=s.getRealAtPos(2); // c=12.7e-6
  \endcode
  \param pos sp�cifie que l'on souhaite extraire la i�me valeur sur la ligne
  \param sep sp�cifie le s�parateur de donn�es sur la ligne que l'on veut utiliser (par d�faut un espace si on ne pr�cise rien)
  \return valeur r�elle de retour convertie
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
Real String::getRealAtPos(int pos, String sep)
//-----------------------------------------------------------------------------
{
  double val;
  String tempo;
  tempo = *this;

  for (int i = 0; i < pos - 1; i++)
  {
    tempo.strip();
    tempo = tempo.after(sep);
  }
  sscanf(tempo.chars(), "%lf", &val);
  return val;
}

//!extrait une valeur r�elle d'un string
/*!
  Cette m�thode extrait d'un String une valeur r�elle et r�duit cette chaine de caract�res. Si elle ne contient plus de valeurs � extraire, cette m�thode renvoie False au moment de l'extraction de la derni�re valeur de la ligne.

  Exemple:
  \code
  String s="5.0 8.4 12.7e-6";
  double a,b,c;
  Boolean r;
  r=s.popReal(a); // a=5.0, r=True
  r=s.popReal(b); // b=8.4, r=True
  r=s.popReal(c); // c=12.7e-6, r=False
  \endcode
  \param pos sp�cifie que l'on souhaite extraire la i�me valeur sur la ligne
  \param sep sp�cifie le s�parateur de donn�es sur la ligne que l'on veut utiliser (par d�faut un espace si on ne pr�cise rien)
  \return valeur r�elle de retour convertie
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
Boolean
String::popReal(Real &ret, String sep)
//-----------------------------------------------------------------------------
{
  strip();

  if (*this != "")
  {
    // la conversion du premier en un reel
    ret = getReal();

    // la recherche du suivant
    if (contains(sep))
    {
      *this = after(sep);
      strip();
      return True;
    }
  }

  return False;
}

//!extrait une valeur enti�re d'un string
/*!
  Cette m�thode extrait d'un String une valeur enti�re et r�duit cette chaine de caract�res. Si elle ne contient plus de valeurs � extraire, cette m�thode renvoie False au moment de l'extraction de la derni�re valeur de la ligne.

  Exemple:
  \code
  String s="5 8 12";
  int a,b,c;
  Boolean r;
  r=s.popInt(a); // a=5, r=True
  r=s.popInt(b); // b=8, r=True
  r=s.popInt(c); // c=12, r=False
  \endcode
  \param pos sp�cifie que l'on souhaite extraire la i�me valeur sur la ligne
  \param sep sp�cifie le s�parateur de donn�es sur la ligne que l'on veut utiliser (par d�faut un espace si on ne pr�cise rien)
  \return valeur r�elle de retour convertie
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
Boolean
String::popInt(Indice &val, String sep)
//-----------------------------------------------------------------------------
{
  Real rval = 0;
  Boolean ret;
  ret = popReal(rval, sep);
  val = (Indice)rval;
  return ret;
}

//!extrait un mot d'une chaine de caract�res
/*!
  Cette m�thode extrait un mot d'une chaine de caract�res. Cette m�thode retourne la valeur True s'il reste des mosts dans la chaine, False dans le cas contraire.
  \param ret mot extrait de la chaine de caract�res
  \param sep sp�cifie le s�parateur de donn�es sur la ligne que l'on veut utiliser (par d�faut un espace si on ne pr�cise rien)
  \return True si la chaine contient d'autres mots, False dans le cas contraire
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean
String::popString(String &ret, String sep)
//-----------------------------------------------------------------------------
{
  strip();

  if (*this != "")
  {

    // la recherche du suivant
    if (contains(sep))
    {
      ret = before(sep);
      *this = after(sep);
      strip();
      return True;
    }
    ret = *this;
    return True;
  }
  return False;
}

//!conversion d'un String en un entier
//-----------------------------------------------------------------------------
Indice String::getInt() const
//-----------------------------------------------------------------------------
{
  return atoi(c_str());
}

//!conversion d'un String en un r�el
//-----------------------------------------------------------------------------
Real String::getReal() const
//-----------------------------------------------------------------------------
{
  return atof(c_str());
}

//!ajout d'une extension si elle n'est pas pr�sente
//-----------------------------------------------------------------------------
void String::addExtension(String ext)
//-----------------------------------------------------------------------------
{
  Boolean present = False;

  // test if len is sufficient
  if (length() > ext.length())
  {
    cout << (*this)(length() - ext.length(), length()) << endl;
    if ((*this)(length() - ext.length(), length()) == ext)
    {
      present = True;
    }
  }

  // is not present, so add it !!
  if (!present)
    *this = *this + ext;
}

//-----------------------------------------------------------------------------
void String::fromFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  Indice l;
  fread((void *)(&l), sizeof(Indice), 1, pfile);
  char stt[l + 5];
  fread(stt, sizeof(char), l, pfile);
  stt[l] = '\0';
  (*this) = stt;
}

//-----------------------------------------------------------------------------
void String::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  Indice l = length();
  fwrite((void *)(&l), sizeof(Indice), 1, pfile);
  const char *p = data();
  fwrite((void *)(p), sizeof(char), l, pfile);
}
