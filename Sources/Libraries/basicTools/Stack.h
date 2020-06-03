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

// begin date : 24/01/2002
// revision date :

/*
  Class Stack definition
*/

/*!
  \file Stack.h
  \brief fichier .h de d�finition de la classe de pile d'objets
  \ingroup basicTools

  Ici, on d�finit la notion de pile d'objets. Ce fichier est li� � la d�finition de la classe Stack utilis�e pour g�rer des piles d'objets en m�moire. C'est une notion de pile classique avec les m�thodes pop() et push().

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

/* #include <upBasicTools.h> */
#include <Const.h>
#include <Error.h>
#include <Boolean.h>

#ifndef __Stack_h__
#define __Stack_h__

template <class T>
class Stack;

/** @dia:pos 4,44 */
template <class T>
class StackItem
{
  friend class Stack<T>;

private:
  T rep;
  StackItem *next;

  // constructors
  StackItem(T r, StackItem<T> *c) : rep(r), next(c)
  {
  }
  ~StackItem(){};
};

/*!
  \class Stack Stack.h
  \brief classe pile pour tout type d'objet
  \ingroup basicTools

  Cette classe impl�mente un m�canisme de pile pour tout type d'objet en m�moire. Dans la mesure ou cette pile stocke les objets par veleur, elle n'est pas tr�s efficace pour g�rer des objets de grosse taille. Il est alors plus efficace de g�rer des pointeus sur ces objets de grosse taille.

  Le constructeur de cette classe ne prend pas d'arguments et d�finit une nouvelle pile vide par d�faut. Cette classe inclut des m�canismes de base pour la gestion des d�passement m�moire.

  Cette pile est de type LIFO (Last In First Out).

  Example :
  Le code suivant illustre l'utilisation pour cr�er une pile d'entiers.
  \code
  Stack <int> st;
  st.push(10);
  st.push(5);
  cout << "size : "<<st.size()<<endl; // taille de la pile
  st.push(st.pop()+st.pop()); // somme des deux termes de la pile i.e. : 15
  cout << st.pop() << endl; // affichage du r�sultat : 15
  \endcode
  
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
  \date 1997-2004
*/
/** @dia:pos 4,56 */
template <class T>
class Stack
{
private:
  StackItem<T> *stackptr;
  Indice size_;

public:
  // constructors
  Stack(void);
  /*   { */
  /*     stackptr = NULL; */
  /*     size_ = 0; */
  /*   } */
  ~Stack();
  /*   { */
  /*     while (!empty ()) */
  /*       pop (); */
  /*   } */

  // members functions
  void push(T);
  T pop();
  T top();
  void dup(void);
  void swap(void);
  Boolean empty();
  Indice size(void);
  void flush();
  /*   { */
  /*     while (!empty ()) */
  /*       pop (); */
  /*   } */
};

//!constructeur par d�faut
/*!
  Cette m�thode est le constructeur par d�faut de la classe Stack. Il alloue la m�moire pour cette classe et initialise la pile avec une taille initiale nulle.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
Stack<T>::Stack(void)
//-----------------------------------------------------------------------------
{
  stackptr = NULL;
  size_ = 0;
}

//!destructeur par d�faut
/*!
  Cette m�thode est le destructeur par d�faut de la classe Stack.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
Stack<T>::~Stack()
//-----------------------------------------------------------------------------
{
  flush();
}

//!vide la pile
/*!
  Cette m�thode permet de vider le contenu de la pile. Elle conserve la pr�sence de la pile en m�moire qui peut etre utilis�e pour d'autres op�rations.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
void Stack<T>::flush()
//-----------------------------------------------------------------------------
{
  while (!empty())
    pop();
}

//!pousse une valeur dans la pile
/*!
  Cette m�thode stocke une valeur dans la pile (en haut de la pile).
  \param value valeur � ajouter � la pile
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
void Stack<T>::push(T value)
//-----------------------------------------------------------------------------
{
  stackptr = new StackItem<T>(value, stackptr);
  size_++;
}

//!r�cup�re une valeur de la pile
/*!
  Cette m�thode r�cup�re une valeur dans la pile et d�cr�mente la pile. C'est une action de r�cup�ration qui efface la valeur dans la pile.
  \return valeur du haut de la pile
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
T Stack<T>::pop()
//-----------------------------------------------------------------------------
{
  T ret;

  if (empty())
    fatalError("Stack::pop ", "Stack is empty, nothing to pop");

  ret = stackptr->rep;
  StackItem<T> *c = stackptr;
  stackptr = stackptr->next;
  delete c;
  size_--;

  return ret;
}

//!recopie une valeur de la pile
/*!
  Cette m�thode r�cup�re une valeur dans la pile sans d�cr�menter la taille de la pile. C'est une action de copie qui n'efface pas la valeur dans la pile.
  \return valeur du haut de la pile
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
T Stack<T>::top()
//-----------------------------------------------------------------------------
{
  T ret;

  if (empty())
    fatalError("Stack::top ", "Stack is empty, nothing on top");

  ret = stackptr->rep;

  return ret;
}

//!duplique la valeur du haut de la pile
/*!
  Cette m�thode duplique la valeur situ�e en haut de la pile. Elle est �quivalente � la commande push(top()).
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
void Stack<T>::dup(void)
//-----------------------------------------------------------------------------
{
  push(top());
}

//!�change les deux valeur en haut de la pile
/*!
  Cette m�thode �change les deux valeurs situ�es en haut de la pile. Si moins de deux valeurs sont actuellement dans la pile, cette m�thode g�n�re une erreur.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
void Stack<T>::swap(void)
//-----------------------------------------------------------------------------
{
  T v1 = pop();
  T v2 = pop();
  push(v1);
  push(v2);
}

//!teste si une pile est vide
/*!
  Cette m�thode test si la pile est vide, elle retourne la valeur True en cas de succ�s.
  \return True si la pile est vide, False s'il y a au moins un �lement dans la pile.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
Boolean Stack<T>::empty()
//-----------------------------------------------------------------------------
{
  return getBooLean(stackptr == NULL);
}

//!taille de la pile
/*!
  Retourne la taille de la pile, c'est � dire le nombre d'objets contenus dans la pile.
  \return nombre d'objets contenus dans la pile.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
template <class T>
Indice Stack<T>::size()
//-----------------------------------------------------------------------------
{
  return size_;
}

#endif
