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

/*
  Class Matrix implementation
*/

#ifndef __Matrix_h__
#define __Matrix_h__

/*!
  \file Matrix.h
  \brief fichier .h de d�finition des matrices g�n�rales
  \ingroup linearAlgebra

  Ce fichier sert � la d�finition de la classe de matrices g�n�rales.

  \author &copy; Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/

#include <Vector.h>
#include <Matrices.h>
class MatrixDiag;

/*!
  \class Matrix Matrix.h
  \brief Classe de gestion et manipulation des matrices denses.
  \ingroup linearAlgebra

  Cette classe est utilis�e pour la gestion et la manipulation des matrices denses. Elle comporte toutes les m�thodes d'initialisation et de gestion m�moire. Les valeurs stock�es sont des Real. La classe \c Matrix contient de plus un certain nombre de m�thodes de calcul sur les matrices comme d�crit ci-dessous.

  \author Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/
/** @dia:pos 32,50 */
/** @dia:route Matrices;v,29.675,13.8,46,45.025,50 */
class Matrix : public Matrices
{
  friend class Vector;
  Indice sz; // taille du vecteur de donn�es
  Real *v;   // vecteur de donn�es

private:
  void allocate(const Indice li_, const Indice co_);
  void desallocate();

public:
  // constructeurs
  Matrix(const Indice li_ = 3, const Indice co_ = 3);
  Matrix(const Matrix &mat);
  Matrix(Indice l, Indice c, Real val, ...);
  ~Matrix();

  // operations sur les composantes
  void redim(const Indice li_, const Indice co_);
  inline Real operator()(Indice i, Indice j) const;
  inline Real &operator()(Indice i, Indice j);
  inline Boolean indexOK(Indice i, Indice j) const;

  // operations
  void fill(Real val);
  void Identity();
  Matrix &operator=(const Real &val);
  Matrix &operator=(const Matrix &mat);

  // operations de comparaison
  Boolean operator==(const Matrix &mat) const;
  Boolean operator!=(const Matrix &mat) const;

  // operations arithmetiques de base entre matrices
  Matrix operator+(const Matrix &mat) const;
  Matrix operator-(const Matrix &mat) const;
  void operator+=(const Matrix &mat);
  void operator-=(const Matrix &mat);
  void operator*=(const Real &lambda);
  void operator/=(const Real &lambda);
  Matrix operator*(const Real &lambda) const;
  Matrix operator*(const Matrix &mat) const;
  Matrix operator*(const MatrixDiag &mat) const;
  Matrix operator/(const Real &mat) const;
  friend Matrix operator*(const Real &val, const Matrix &mat);
  Vector operator*(const Vector &vec) const;
  Vector trans_mult(const Vector &vec) const;
  void squareMultiplyBy(const MatrixDiag &mat);
  void squareDivideBy(const MatrixDiag &mat);
  void productBy(Vector &resu) const;

  // operations arithmetiques autres
  Real Trace();
  Real moyTrace();
  Matrix Transpose();
  Vector rowSum();
  Vector colSum();
  void getSymetricPart(Matrix &va) const;
  void getAntiSymetricPart(Matrix &va) const;
  Vector getRow(Indice i);
  Vector getCol(Indice i);
  Real det() const;
  Matrix inverse();
  Matrix factLU();
  Vector forwBackLU(const Vector b);
  Matrix factLU(Vector &perm_, Vector &scratch);
  void forwBackLU(Vector &b, Vector &x, const Vector &perm_);
  Vector getSolve(const Vector &x);
  void solve(Vector &x);
  Matrix getNullSpace(Boolean relative = False, double tol = 1e-10);
  Matrix getNullSpace2(Boolean relative = False, double tol = 1e-10);
  void computeRightEigenVectors(Vector &eigenValues, Matrix &eigenVectors);
  void computeLeftEigenVectors(Vector &eigenValues, Matrix &eigenVectors);
  void computeEigenVectors(Vector &eigenValues, Matrix &leftEigenVectors, Matrix &rightEigenVectors);
  Matrix getPseudoInverse(Boolean relative = False, Real tol = 1e-10);
  void computePseudoInverse(Matrix &Kplus, Matrix &NS, Boolean relative = False, Real tol = 1e-10);
  void gatherFrom(const Matrix &M, Indice *ind, int shift);
  void scatterFrom(const Matrix &M, Indice *ind, int shift);

  // gestion des flux entree et sortie
  friend ostream &operator<<(ostream &os, const Matrix &mat);
  friend ofstream &operator<<(ofstream &os, const Matrix &mat);
  friend ifstream &operator>>(ifstream &is, Matrix &mat);
  void write(ofstream &ofs) const;
  void read(ifstream &ifs);
  void print(ostream &os) const;
  void printOut();
  inline Real sumTerms() const;

  // Matrices virtuals functions
  long Memory() const;
  void outStdout() { cout << *this << endl; }
};

//------inline functions-------------------------------------------------------

//!teste les bornes de la matrice
/*!
  Cette m�thode teste les bornes d'une matrice
  \param i indice de ligne
  \param j indice de colonne
  \return True si les indices fournis sont dans les bornes, False dans le cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Boolean
Matrix::indexOK(Indice i, Indice j) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < li) && (j >= 0) && (j < co))
    return (True);

  printf("Matrix::indexOK\nindice(s) [%ld,%ld] out of allowed range [0-%ld,0-%ld]", i, j, li - 1, co - 1);
  exit(-1);

  // fonction inutile, mais necessaire pour le compilateur
  return (False);
}

//!acc�s aux valeurs d'une matrice
/*!
  \param i indice de ligne
  \param j indice de colonne
  \return valeur de la matrice � la ligne \c i et colonne \c j
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Real
Matrix::operator()(Indice i, Indice j)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j);
#endif
  return v[Ind(i, j, li, co)];
}

//!acc�s aux valeurs d'une matrice
/*!
  \param i indice de ligne
  \param j indice de colonne
  \return valeur de la matrice � la ligne \c i et colonne \c j
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Real &Matrix::operator()(Indice i, Indice j)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j);
#endif
  return v[Ind(i, j, li, co)];
}

//!somme de tous les termes d'une matrice
/*!
  Cette m�thode calcule la somme de tous les termes d'une matrice
  \return somme de tous les termes de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Real
Matrix::sumTerms() const
//-----------------------------------------------------------------------------
{
  Real val = 0;
  for (Indice i = 0; i < co * li; i++)
    val += v[i];
  return val;
}

#endif
