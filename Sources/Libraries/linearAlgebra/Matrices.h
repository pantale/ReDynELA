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

/*
  Class Matrices definition
*/

#ifndef __Matrices_h__
#define __Matrices_h__
/*!
  \file Matrices.h
  \brief fichier .h de d�finition des matrices g�n�rales
  \ingroup linearAlgebra

  Ce fichier set � la d�finition de la classe Matrice qui sert de classe de base pour toutes les matrices de DynELA. Cette classe de base sert � stocker les informations communes � tous les types de matrices tels que la taille de la matrice (nombre de lignes et de colonnes)...

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#include <upLinearAlgebra.h>

/*
  \enum OutMatrix
  \brief enum�ration de format d'affichage de matrices.
  Cette �num�ration permet de d�finir le format d'affichage des matrices. On a ainsi le choix entre outMatrixTypeNormal qui correspond � un affichage conforme � Mathematica et outMatrixTypeMupad qui correspond � un affichage au format Mupad.
  \author &copy; Olivier PANTALE
  \since 1.0.0
*/
enum OutMatrix
{
  outMatrixTypeNormal = 0, //!< format Mathematica
  outMatrixTypeMupad       //!< format Mupad
};

/*!
  \class Matrices Matrices.h
  \brief classe de base pour les matrices
  \ingroup linearAlgebra

  Cette classe sert � la d�finition d�finition g�n�rale des matrices, elle sert de classe de base pour toutes les matrices de DynELA. Cette classe de base sert � stocker les informations communes � tous les types de matrices tels que la taille de la matrice (nombre de lignes et de colonnes)...

  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
/** @dia:pos 24,4 */
class Matrices
{
protected:
  Indice li;    //!< nombre de lignes de la matrice
  Indice co;    //!< nombre de colonnes de la matrice
  char outType; //!< d�finit le type de format d'affichage

public:
  Matrices();
  Matrices(const Matrices &mat);
  virtual ~Matrices();

  Indice rows() const;
  Indice columns() const;
  void setOutType(char outT);
  virtual long Memory() const = 0; //!< calcule la taille requise pour le stockage de la matrice en m�moire
};

//------inline functions-------------------------------------------------------

//!constructeur par d�faut
/*!
  Cette m�thode est le constructeur par d�faut de la classe Matrices. Ce constructeur ne fait rien d'autre que cr�er l'objet.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline Matrices::Matrices()
//-----------------------------------------------------------------------------
{
  outType = outMatrixTypeNormal;
}

//!constructeur par recopie
/*!
  Cette m�thode est le constructeur par recopie de la classe Matrices. Ce constructeur ne fait rien d'autre que cr�er l'objet.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline Matrices::Matrices(const Matrices &mat)
//-----------------------------------------------------------------------------
{
  outType = mat.outType;
}

//!destructeur
/*!
  Cette m�thode est le destructeur de la classe Matrices.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline Matrices::~Matrices()
//-----------------------------------------------------------------------------
{
}

//!nombre de lignes de la matrice
/*!
  Cette m�thode calcule et retourne le nombre de lignes de la matrice.
  \nombre de lignes de la matrice
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline Indice
Matrices::rows() const
//-----------------------------------------------------------------------------
{
  return li;
}

//!nombre de colonnes de la matrice
/*!
  Cette m�thode calcule et retourne le nombre de colonnes de la matrice.
  \nombre de colonnes de la matrice
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline Indice
Matrices::columns() const
//-----------------------------------------------------------------------------
{
  return co;
}

//!selection du type d'affichage
/*!
  Cette m�thode permet de s�lectionner le type d'affichage demand�. Le type de sortie est d�fini par les variables enum�r�es dans \ref OutMatrix.
  \author Olivier PANTALE
  \since 1.0.0
*/
//-----------------------------------------------------------------------------
inline void
Matrices::setOutType(char outT)
//-----------------------------------------------------------------------------
{
  outType = outT;
}

#endif
