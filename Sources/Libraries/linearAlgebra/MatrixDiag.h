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

// begin date : 25/03/1997

/*
  Class MatrixDiag definition
*/

#ifndef __MatrixDiag_h__
#define __MatrixDiag_h__

/*!
  \file MatrixDiag.h
  \brief fichier .h de d�finition des matrices disgonales
  \ingroup linearAlgebra

  Ce fichier sert � la d�finition de la classe des matrices disgonales.

  \author &copy; Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/

#include <Vector.h>
#include <Matrix.h>

class Vector;

/*!
  \class MatrixDiag MatrixDiag.h
  \brief Classe de gestion et manipulation des matrices disgonales.
  \ingroup linearAlgebra

  Cette classe est utilis�e pour la gestion et la manipulation des matrices disgonales. Elle comporte toutes les m�thodes d'initialisation et de gestion m�moire. Les valeurs stock�es sont des Real. La classe \c MatrixDiag contient de plus un certain nombre de m�thodes de calcul sur les matrices comme d�crit ci-dessous.

  \author Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/
/** @dia:pos 4,50 */
/** @dia:route Matrices;v,29.675,13.8,46,13.7,50 */
class MatrixDiag : public Matrices
{
  friend class Matrix;
  Real *v;

public:
  // allocateurs
  void allocate(const Indice li_, const Indice co_);
  void desallocate();
  void redim(const Indice li_, const Indice co_);

public:
  // constructeurs
  MatrixDiag(const Indice li = 3, const Indice co = 3);
  MatrixDiag(const MatrixDiag &mat);
  ~MatrixDiag();

  // operations sur les composantes
  inline Real &operator()(Indice i);
  inline Real operator()(Indice i) const;
  inline Boolean indexOK(Indice i) const;

  // operations d'affectation
  void fill(Real val);
  void Identity();
  MatrixDiag &operator=(const Real &val);
  MatrixDiag &operator=(const MatrixDiag &mat);

  // operations de comparaison
  Boolean operator==(const MatrixDiag &mat) const;
  Boolean operator!=(const MatrixDiag &mat) const;

  // operations arithmetiques de base entre matrices
  MatrixDiag operator+(const MatrixDiag &mat) const;
  MatrixDiag operator-(const MatrixDiag &mat) const;
  void operator+=(const MatrixDiag &mat);
  void operator-=(const MatrixDiag &mat);
  MatrixDiag operator*(const Real &lambda) const;
  MatrixDiag operator*(const MatrixDiag &mat) const;
  MatrixDiag operator/(const Real &mat) const;
  friend MatrixDiag operator*(const Real &val, const MatrixDiag &mat);
  Vector operator*(const Vector &vec) const;
  void productBy(Vector &vec) const;
  void divideBy(Vector &vec) const;
  MatrixDiag getInverse() const;

  // operations arithmetiques autres
  Real Trace();
  MatrixDiag Transpose();
  Vector Solve(Vector &vec);

  // gestion des flux entree et sortie
  friend ostream &operator<<(ostream &os, const MatrixDiag &mat);
  friend ofstream &operator<<(ofstream &os, const MatrixDiag &mat);
  friend ifstream &operator>>(ifstream &is, MatrixDiag &mat);
  void write(ofstream &ofs) const;
  void read(ifstream &ifs);
  void print(ostream &os) const;

  // MatrixDiagrix virtuals functions
  long Memory() const;
};

//------inline functions-------------------------------------------------------

//!teste les bornes de la matrice
/*!
  Cette m�thode teste les bornes d'une matrice
  \param i indice de ligne
  \return True si l'indice fourni est dans les bornes, False dans le cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Boolean
MatrixDiag::indexOK(Indice i) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < li))
    return (True);

  printf("MatrixDiag::indexOK\nindice(s) [%ld] out of allowed range [0-%ld]", i, li - 1);
  exit(-1);
  //fatalError ("MatrixDiag::indexOK","indice(s) [%ld] out of allowed range [0-%ld]", i, li - 1);

  // fonction inutile, mais necessaire pour le compilateur
  return (False);
}

//!acc�s aux valeurs d'une matrice
/*!
  \param i indice de ligne
  \return valeur de la matrice � la ligne et colonne \c i
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Real &
MatrixDiag::operator()(Indice i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif

  return v[i];
}

//!acc�s aux valeurs d'une matrice
/*!
  \param i indice de ligne
  \return valeur de la matrice � la ligne et colonne \c i
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Real
MatrixDiag::operator()(Indice i)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif

  return v[i];
}

#endif
