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
	Class ObjectList definition
*/

/*!
  \file ObjectList.h
  \brief fichier .h de d�finition des objets g�n�riques
  \ingroup basicTools

  Ce fichier sert � la d�finition des objets g�n�riques de DynELA. Ces objsets sont destin�s � servir de classe de base pour tous les objets de DynELA que l'on d�sire manipuler dynamiquement � travers l'interface de commande en Lex et Yacc.

  Ces classes sont en cours de d�veloppement et ne sont pas encore finalis�es.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/

#ifndef __ObjectList_h__
#define __ObjectList_h__

#include <String.h>
#include <List.h>

class ObjectsList;

/*!
  \class Object ObjectList.h
  \brief Classe de d�finition des objets g�n�riques 
  \ingroup basicTools

  Cette classe est une classe de base pour tous les objets de l'ensemble DynELA. En d�clarant qu'un objet donn� d�rive de cette classe de base, l'utilisateur � la possibilit� de g�rer les objets de mani�re dynamique en m�moire et d'associer un nom � chaque objet et � chaque type d'objet. Les objets peuvent alors etre manipul�s comme des variables au travers du langage orient�-objets de l'interpr�teur de commandes DynELA.

  Voir ci-dessous pour un exemple d'utilisation:
  \code
  // d�claration d'une classe Truc d�rivant de Object
  class Truc : public Object
  {
  ...
  }

  // cr�ation d'une liste
  List <Object*> objectList;

  // cr�ation d'un objet et affectation d'un nom
  Truc *tr = new Truc;
  tr.setObjectName("myTrucObject_1");

  // ajout de l'objet � la liste
  objectList << tr;

  \endcode
  \author Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/
/** @dia:pos 30,6 */
class Object
{
  friend class ObjectsList;

private:
  /** @dia:route 9,12;h,43.8,7.9,48,11.9,54 */
  String objectName; //!< Nom de l'objet
  int objectLevel;   //!< Niveau de l'objet

public:
  void *object; //!< Stockage de l'objet

public:
  // constructor
  Object(int level = 0, String newName = "");
  Object(const Object &X);
  virtual ~Object();

  // virtual member functions
  virtual String getObjectClass();
  virtual int getObjectType() = 0;

  // member functions
  String getObjectName();
  void setObjectName(String st);
  void setObjectLevel(int _level);
  int getObjectLevel();
};

// m�thode externe is herited from
// d�finition par le g�n�rateur de langage
// en fonction des fichiers .class
extern Boolean isHeritedFrom(Indice, Indice);

/*!
  \class ObjectsList ObjectList.h
  \brief Class de d�finition des liste d'objets
  \ingroup basicTools

  \author Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/
/** @dia:pos 4,4 */
class ObjectsList
{
public:
  /** @dia:route 12,4;h,30,9.7,26,4.7,22 */
  List<Object *> list; //!< Liste des objets en m�moire

private:
  Indice getIndiceObject(String name); //!< M�thode priv�e � usage interne
  Indice currentObjectType;
  Indice currentObject;
  Boolean derivate;

public:
  // constructor
  ObjectsList();
  ~ObjectsList();

  // member functions
  Boolean existObject(String name);
  Boolean addObject(Object *ob);
  Boolean delObject(Object *ob);
  Boolean delObject(String ob);
  Object *getObject(String name);
  Object *getObject(Indice i);
  String getObjectName(Object *ob);
  void deleteUpLevel(int level);
  Indice getNumberOfObjectOfType(Indice type);
  Object *getObjectOfType(Indice type, Indice num);
  Object *getFirstObject(Indice type);
  Object *getNextObject();
  Object *getFirstObjectDerivatedFrom(Indice type);
};

#endif
