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
	Class Function implementation
*/
/*!
  \file Function.C
  \brief fichier .C de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

#include <Function.h>

//!constructeur par d�faut de la classe Function
/*!
  Ce constructeur alloue la m�moire pour la cr�ation d'une fonction discr�te. Par d�faut, la nouvelle fonction est d�finie avec le flag \ref Sort actif, c'est � dire que les points de la fonction sont automatiquement tri�s sur l'axe des abscisses.
  \author &copy; Olivier PANTALE
  \since 0.9.5
*/
//-----------------------------------------------------------------------------
Function::Function()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Function::~Function()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Function::setName(String st)
//-----------------------------------------------------------------------------
{
  name = st;
}
