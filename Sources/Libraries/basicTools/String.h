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

// begin date : 04/03/1997
// revision date : 18/07/1997

#ifndef __String_h__
#define __String_h__

// include the STL string
#include <string>
#include <Const.h>

/*!
  \file String.h
  \brief fichier . h de d�finition des chaines de caract�res
  \ingroup basicTools

  Ce fichier regroupe la d�finition des chaines de caract�res pour DynELA. Les chaines de caract�res font partie d'une nouvelle classe nomm�e String qui permet la pluspart des manipulations sur les caract�res. L'ensemble des m�thodes de cette classe sont document�es.

  Depuis la version v. 1.0.0, cette classe h�rite de la classe \b string de Standard Template Library.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/

/*!
  \class String String.h
  \brief classe de d�finition des chaines de caract�res
  \ingroup basicTools

  Cette classe sert � l'impl�mentation des chaines de caract�res sur DynELA. Elle s'inspire des exemples donn�s par B. Stroutrup dans son livre sur le C++. Des fonctions additionnelles on �t� rajout�es par rapport � ce qui est donn� dans cet ouvrage.

  Depuis la version v. 1.0.0, cette classe h�rite de la classe \b string de Standard Template Library.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/
/** @dia:pos 54,8 */
class String : public string
{

public:
  String();
  String(const char *s);
  String(const string &st);
  String(const String &st);
  ~String();

  String &operator=(const char *st);
  String &convert(int, short leading = 0);
  String &convert(Indice i);
  String &convert(Real r, const char *st = "%lf");
  String &replace(const char from, const char to);
  String &replace(const String &from, const String &to);
  String subString(int startpos, int length) const;
  int search(char c, int startpos = 0) const;
  int search(const String &substring, int startpos = 0) const;
  void del(int startpos, int length);
  void del(const String &y, int startpos = 0);
  void del(char c, int startpos = 0);
  String at(int first, int len) const;
  String operator()(int first, int len) const;
  String before(int pos) const;
  String through(int pos) const;
  String after(int pos) const;
  String from(int pos) const;
  String at(const String &y, int startpos = 0) const;
  String at(char c, int startpos = 0) const;
  String before(const String &y, int startpos = 0) const;
  String before(char c, int startpos = 0) const;
  String through(const String &y, int startpos = 0) const;
  String through(char c, int startpos = 0) const;
  String after(const String &y, int startpos = 0) const;
  String after(char c, int startpos = 0) const;
  String afterLast(const String &y) const;
  String beforeLast(const String &y) const;
  String from(const String &y, int startpos = 0) const;
  String from(char c, int startpos = 0) const;
  Boolean scanFileLine(FILE *pfile);
  void strip();
  Boolean containsWithCase(const String &substring, Boolean cas) const;
  int containsWithCaseSub(const String &substring, Boolean cas) const;
  const char *chars() const;
  Boolean contains(char c, int startpos = 0) const;
  Boolean contains(const String &y, int startpos = 0) const;
  void print(const String &s) const;
  Boolean empty() const;
  Boolean ok() const;
  char firstchar() const;
  char lastchar() const;
  void lowerCase();
  void upperCase();
  Real getRealAtPos(int i, String sep = " ");
  Boolean popReal(Real &val, String sep = " ");
  Boolean popInt(Indice &val, String sep = " ");
  Boolean popString(String &val, String sep = " ");
  Indice getInt() const;
  Real getReal() const;
  void addExtension(String ext);
  void fromFile(FILE *pfile);
  void toFile(FILE *pfile);
};

#endif
