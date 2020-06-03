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
	Class Object implementation
*/

/*!
  \file ObjectList.C
  \brief fichier .C de d�finition des objets g�n�riques
  \ingroup basicTools

  Ce fichier sert � la d�finition des objets g�n�riques de DynELA. Ces objsets sont destin�s � servir de classe de base pour tous les objets de DynELA que l'on d�sire manipuler dynamiquement � travers l'interface de commande en Lex et Yacc.

  Ces classes sont en cours de d�veloppement et ne sont pas encore finalis�es.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/

#include <ObjectList.h>
#include <Const.h>
#include <Boolean.h>
#include <Error.h>

//!Constructeur par d�faut de la classe Object
/*!
  Cette m�thode est le constructeur par d�faut de la classe Object.

  \param newName nom de l'objet
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Object::Object(int level, String newName)
//-----------------------------------------------------------------------------
{
  objectLevel = level;
  objectName = newName;
}

//!Constructeur par recopie de la classe Object
/*!
  Cette m�thode est le constructeur par recopie de la classe Object.
  
  \param X object � recopier
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Object::Object(const Object &X)
//-----------------------------------------------------------------------------
{
  // if no name, nothing to do
  if (X.objectName == "")
    return;

  // if a name, have to copy it

  // memory allocation
  //  objectName=new String;

  // copy the name
  objectName = X.objectName;
}

//!Destructeur de la classe Object
/*!
  Cette m�thode est le destructeur de la classe Object.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Object::~Object()
//-----------------------------------------------------------------------------
{
  // test if the object name exists
  //   if (objectName!=NULL) {

  //     // delete the object name
  //     delete objectName;
  //   }

  // affect NULL to object name
  //  objectName=NULL;
}

//!Renvoie le nom de l'objet
/*!
  Cette m�thode renvoie le nom de l'objet.

  \return nom de l'objet
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
String
Object::getObjectName()
//-----------------------------------------------------------------------------
{
  return objectName;
}

//!Renvoie la classe de l'objet
/*!
  Cette m�thode renvoie la classe de l'objet.

  \return nom de l'objet
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
String
Object::getObjectClass()
//-----------------------------------------------------------------------------
{
  return "Unknown class";
}

//!Affecte le nom de l'objet
/*!
  Cette m�thode affecte le nom de l'objet.

  \param st nom de l'objet
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
void Object::setObjectName(String st)
//-----------------------------------------------------------------------------
{
  // copy the name
  objectName = st;
}

//-----------------------------------------------------------------------------
void Object::setObjectLevel(int level)
//-----------------------------------------------------------------------------
{
  objectLevel = level;
}

//-----------------------------------------------------------------------------
int Object::getObjectLevel()
//-----------------------------------------------------------------------------
{
  return objectLevel;
}

//!Constructeur par d�faut de la classe ObjectList
/*!
  Cette m�thode est le constructeur par d�faut de la classe ObjectList.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
ObjectsList::ObjectsList()
//-----------------------------------------------------------------------------
{
}

//!Destructeur de la classe ObjectsList
/*!
  Cette m�thode est le destructeur de la classe ObjectsList.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
ObjectsList::~ObjectsList()
//-----------------------------------------------------------------------------
{
}

//!Teste si un objet existe
/*!
  Cette m�thode teste l'existence d'un objet dans la liste des objets en m�moire et renvoie True si celui-ci existe.

  \param name nom de l'objet � rechercher
  \return True si l'objet existe, False dans le cas contraire
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Boolean
ObjectsList::existObject(String name)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < list.size(); i++)
  {
    if (list(i)->objectName == name)
      return True;
  }
  return False;
}

//!Indice dans la liste des objets
/*!
  Cette m�thode renvoie l'indice dans la liste des objets en m�moire, ou la valeur -1 si l'objet n'existe pas dans la liste.

  \param name nom de l'objet � rechercher
  \return indice dans la liste ou la valeur -1 si l'objet n'existe pas
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Indice
ObjectsList::getIndiceObject(String name)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < list.size(); i++)
  {
    if (list(i)->objectName == name)
      return i;
  }
  return -1;
}

//!Renvoie un objet de la liste
/*!
  Cette m�thode renvoie un pointeur sur un objet dans la liste des objets en m�moire, ou la valeur NULL si l'objet n'existe pas dans la liste.

  \param name nom de l'objet � rechercher
  \return pointeur sur l'objet ou NULL si celui-ci n'existe pas
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Object *
ObjectsList::getObject(String name)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < list.size(); i++)
  {
    if (list(i)->objectName == name)
      return list(i);
  }
  return NULL;
}

//!Renvoie le nom d'un objet de la liste
/*!
  Cette m�thode renvoie le nom d'un objet dans la liste des objets en m�moire, ou la valeur "" si l'objet n'existe pas dans la liste.

  \param ob pointeur de l'objet � rechercher
  \return nom de l'objet ou "" si celui-ci n'existe pas
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
String ObjectsList::getObjectName(Object *ob)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < list.size(); i++)
  {
    if (list(i)->object == ob)
      return list(i)->objectName;
  }
  return "";
}

//!Renvoie un objet de la liste
/*!
  Cette m�thode renvoie un pointeur sur le i�me objet dans la liste des objets en m�moire

  \param name nom de l'objet � rechercher
  \return pointeur sur l'objet.
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Object *
ObjectsList::getObject(Indice i)
//-----------------------------------------------------------------------------
{
  return list(i);
}

//!Ajoute un objet � la liste
/*!
  Cette m�thode ajoute un objet � la liste des objets en m�moire.

  \param ob pointeur sur l'objet � ajouter
  \return True si l'op�ration est correcte
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Boolean
ObjectsList::addObject(Object *ob)
//-----------------------------------------------------------------------------
{
  // if no name, no list adding
  if (ob->objectName == "")
  {
    return False;
  }

  // scan if name already exists
  if (existObject(ob->objectName))
  {
    fatalError("Object list management", "Object %s already exists\n",
               ob->objectName.chars());
  }

  // add to list
  list << ob;

  // return True because happy
  return True;
}

//!Supprime un objet de la liste
/*!
  Cette m�thode supprime un objet de la liste des objets en m�moire.

  \param ob pointeur sur l'objet � supprimer
  \return True si l'op�ration est correcte
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Boolean
ObjectsList::delObject(Object *ob)
//-----------------------------------------------------------------------------
{
  // if no name, no list adding
  if (ob->objectName == "")
  {
    return False;
  }

  // scan if name already exists
  if (existObject(ob->objectName))
  {
    list.del(getIndiceObject(ob->objectName));
    return True;
  }

  // return False because not happy
  return False;
}

//!Supprime un objet de la liste
/*!
  Cette m�thode supprime un objet de la liste des objets en m�moire.

  \param name nom de l'objet � supprimer
  \return True si l'op�ration est correcte
  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
Boolean
ObjectsList::delObject(String name)
//-----------------------------------------------------------------------------
{
  // if no name, no list adding
  Indice i;
  if ((i = getIndiceObject(name)) == -1)
  {
    return False;
  }

  list.del(i);
  return True;
}

//-----------------------------------------------------------------------------
void ObjectsList::deleteUpLevel(int level)
//-----------------------------------------------------------------------------
{
  Indice i;
  for (i = 0; i < list.size(); i++)
  {
    if (list(i)->getObjectLevel() > level)
    {
      list.del(i);
      i--;
    }
  }
}

//-----------------------------------------------------------------------------
Indice ObjectsList::getNumberOfObjectOfType(Indice type)
//-----------------------------------------------------------------------------
{
  Indice ob, nb;
  nb = 0;

  for (ob = 0; ob < list.size(); ob++)
  {
    if (getObject(ob)->getObjectType() == type)
      nb++;
  }

  return nb;
}

//-----------------------------------------------------------------------------
Object *ObjectsList::getObjectOfType(Indice type, Indice num)
//-----------------------------------------------------------------------------
{
  Indice ob, nb;
  nb = 0;

  for (ob = 0; ob < list.size(); ob++)
  {
    if (getObject(ob)->getObjectType() == type)
    {
      if (nb == num)
        return getObject(ob);
      nb++;
    }
  }

  return NULL;
}

//-----------------------------------------------------------------------------
Object *ObjectsList::getFirstObject(Indice type)
//-----------------------------------------------------------------------------
{
  currentObjectType = type;
  currentObject = 0;
  derivate = False;
  Indice ob;

  for (ob = 0; ob < list.size(); ob++)
  {
    if (getObject(ob)->getObjectType() == type)
    {
      currentObject = ob;
      return getObject(ob);
    }
  }

  return NULL;
}

//-----------------------------------------------------------------------------
Object *ObjectsList::getFirstObjectDerivatedFrom(Indice type)
//-----------------------------------------------------------------------------
{
  currentObjectType = type;
  currentObject = 0;
  derivate = True;
  Indice ob;

  for (ob = 0; ob < list.size(); ob++)
  {
    if (isHeritedFrom(getObject(ob)->getObjectType(), type))
    {
      currentObject = ob;
      return getObject(ob);
    }
  }

  return NULL;
}

//-----------------------------------------------------------------------------
Object *ObjectsList::getNextObject()
//-----------------------------------------------------------------------------
{
  Indice ob;

  if (derivate)
  {
    for (ob = currentObject + 1; ob < list.size(); ob++)
    {
      if (isHeritedFrom(getObject(ob)->getObjectType(), currentObjectType))
      {
        currentObject = ob;
        return getObject(ob);
      }
    }
  }

  else
  {
    for (ob = currentObject + 1; ob < list.size(); ob++)
    {
      if (getObject(ob)->getObjectType() == currentObjectType)
      {
        currentObject = ob;
        return getObject(ob);
      }
    }
  }

  return NULL;
}
