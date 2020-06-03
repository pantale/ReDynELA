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

// begin date : 05/03/1997
// revision date : 02/09/1997

/*
  Class Vector definition
*/

#ifndef __Vector_h__
#define __Vector_h__

/*!
  \file Vector.h
  \brief fichier .h de d�finition des vecteurs
  \ingroup linearAlgebra

  Ce fichier sert � la d�finition de la classe de vecteurs.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#include <upLinearAlgebra.h>
/*
  \enum OutVector
  \brief enum�ration de format d'affichage de vecteurs.
  Cette �num�ration permet de d�finir le format d'affichage des vecteurs. On a ainsi le choix entre outVectorTypeNormal qui correspond � un affichage conforme � Mathematica et outVectorTypeMupad qui correspond � un affichage au format Mupad.
  \author &copy; Olivier PANTALE
  \since 1.0.0
*/
enum OutVector
{
  outVectorTypeNormal = 0, //!< format Mathematica
  outVectorTypeMupad       //!< format Mupad
};

/*!
  \brief Classe de vecteurs
  \ingroup linearAlgebra
  \class Vector Vector.h

  Classe vecteur de base, allocations, gestion de donn�es, op�rations arithm�tiques.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
/** @dia:pos 88,6 */
class Vector
{
  friend class Matrix;
  friend class MatrixSparse;
  friend class MatrixDiag;

  // nombre de composantes dans le vecteur
  Indice nb;
  // vecteur de stockage
  Real *v;
  char outType;

public:
  Vector();
  Vector(Indice);
  Vector(const Vector &);
  Vector(Indice, Real, ...);
  ~Vector();

  void redim(const Indice i);
  inline Real &operator()(Indice);
  inline Real operator()(Indice) const;
  inline Boolean indexOK(Indice) const;
  inline Indice size() const;
  void fill(Real);
  Vector &operator=(Real);
  Vector &operator=(const Vector &);
  Vector &operator=(const Real *);
  void swapWith(Vector &);

  Boolean operator==(const Vector &) const;
  Boolean operator!=(const Vector &) const;
  void operator+=(const Vector &);
  void operator-=(const Vector &);
  void operator*=(const Real);
  void operator/=(const Real);
  Vector operator+(const Vector &) const;
  Vector operator-(const Vector &) const;
  Vector operator*(const Real &)const;
  Vector operator/(const Real &) const;
  friend Vector operator*(const Real, const Vector &);

  Real distance(const Vector &) const;
  Real distance2(const Vector &) const;
  Real dot(const Vector &) const;
  Vector vectorial(const Vector &) const;
  Real norm();
  Real norm2();
  void normalize();
  Real maxValue();
  Real minValue();
  Real maxAbsoluteValue();
  Real minAbsoluteValue();
  void gatherFrom(const Vector &V, Indice *ind, int shift);
  void scatterFrom(const Vector &V, Indice *ind, int shift);

  friend ostream &operator<<(ostream &, const Vector &);
  friend ofstream &operator<<(ofstream &, const Vector &);
  friend ifstream &operator>>(ifstream &, Vector &);
  void write(ofstream &) const;
  Vector &read(ifstream &);
  void print(ostream &) const;
  void printOut();
  void setOutType(char outT);
};

//------inline functions-------------------------------------------------------

//!teste les bornes du vecteur
/*!
  Cette m�thode teste les bornes d'un vecteur
  \param i indice du vecteur
  \return True si les indices fournis sont dans les bornes, False dans le cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Boolean
Vector::indexOK(Indice i) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < nb))
    return (True);

  // ici, on a detecte une erreur dans les indices
  printf("Vector::indexOK\nindice %ld out of allowed range [0-%ld]", i, nb - 1);
  exit(-1);

  // idiot, car on ne passera jamais ici a cause du fatalError
  // mais c'est pour eviter une remarque du compilateur
  return (False);
}

//!acc�s aux valeurs d'un vecteur
/*!
  \param i indice du vecteur
  \return valeur de l'�l�ment \c i du vecteur
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Real &
Vector::operator()(Indice i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif
  return v[i];
}

//!acc�s aux valeurs d'un vecteur
/*!
  \param i indice du vecteur
  \return valeur de l'�l�ment \c i du vecteur
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Real
Vector::operator()(Indice i)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif
  return v[i];
}

//!taille du vecteur
/*!
  Cette m�thode retourna la taille du vecteur, c'est � dire son rang.
  \return taille du vecteur
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Indice Vector::size() const
//-----------------------------------------------------------------------------
{
  return nb;
}

//!selection du type d'affichage
/*!
  Cette m�thode permet de s�lectionner le type d'affichage demand�. Le type de sortie est d�fini par les variables enum�r�es dans \ref OutVector.
  \author Olivier PANTALE
  \since 1.0.0
*/
//-----------------------------------------------------------------------------
inline void
Vector::setOutType(char outT)
//-----------------------------------------------------------------------------
{
  outType = outT;
}

#endif
