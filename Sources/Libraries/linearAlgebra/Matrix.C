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

#include <Matrix.h>
#include <MatrixDiag.h>

/*!
  \file Matrix.C
  \brief fichier .C de d�finition des matrices g�n�rales
  \ingroup linearAlgebra

  Ce fichier sert � la d�finition de la classe de matrices g�n�rales.

  \author &copy; Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/

// typedef double doublereal;
// typedef long int integer;
// extern "C" int dgemv_(char *trans, integer *m, integer *n, doublereal *
// 	alpha, doublereal *a, integer *lda, doublereal *x, integer *incx,
// 	   doublereal *beta, doublereal *y, integer *incy);
// extern "C" int dgemm_(char *transa, char *transb, integer *m, integer *
// 	n, integer *k, doublereal *alpha, doublereal *a, integer *lda,
// 	doublereal *b, integer *ldb, doublereal *beta, doublereal *c__,
// 	integer *ldc);

// extern "C" int dgetrf_(integer *m, integer *n, doublereal *a, integer *
// 	lda, integer *ipiv, integer *info);

// extern "C" int dgetri_(integer *n, doublereal *a, integer *lda, integer
// 	*ipiv, doublereal *work, integer *lwork, integer *info);
// extern "C" int dgesv_(integer *n, integer *nrhs, doublereal *a, integer
// 	*lda, integer *ipiv, doublereal *b, integer *ldb, integer *info);
// extern "C" int dgesvd_(char *jobu, char *jobvt, integer *m, integer *n,
// 	doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *
// 	ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork,
// 	integer *info);
// extern "C" int dgesdd_(char *jobz, integer *m, integer *n, doublereal *
// 	a, integer *lda, doublereal *s, doublereal *u, integer *ldu,
// 	doublereal *vt, integer *ldvt, doublereal *work, integer *lwork,
// 	integer *iwork, integer *info);

// inclusion de Lapack
#ifndef __CLAPACK_H
extern "C"
{
#include <clapack.h>
}
#endif

//!constructeur de la classe Matrix
/*!
  Cette m�thode construit une matrice de lignes/colonnes elements. Par d�faut, le contenu de la matrice est mis � z�ro
  \param lignes nombre de lignes
  \param colonnes nombre de colonnes 
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix::Matrix(const Indice lignes, const Indice colonnes)
//-----------------------------------------------------------------------------
{
  // allocation memoire;
  allocate(lignes, colonnes);

  // remplissage initial
  fill(0.);
}

//!constructeur par recopie d'une matrice
/*!
  Ce constructeur recopie les valeurs contenues dans une matrice. Selon la valeur du flag \ref MEM_funct, la copie est faite soit terme � terme (flag non d�fini) ou par appel � memcopy() (flag d�fini).
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix::Matrix(const Matrix &mat)
//-----------------------------------------------------------------------------
{
  allocate(mat.li, mat.co);

#ifdef MEM_funct
  memcpy(v, mat.v, sz * sizeof(Real));
#else
  for (Indice i = 0; i < sz; i++)
    v[i] = mat.v[i];
#endif
}

//-----------------------------------------------------------------------------
Matrix::Matrix(Indice l, Indice c, Real val0, ...)
//-----------------------------------------------------------------------------
{
  // allocation memoire;
  allocate(l, c);
  //  Indice s = l*c;

  // A place to store the list of arguments
  va_list arguments;

  // Store arguments values
  va_start(arguments, val0);

  // traitement de la premi�re valeur
  v[0] = val0;

  for (Indice i = 0; i < l; i++)
    for (Indice j = 0; j < c; j++)
    {
      if (!((i == 0) && (j == 0)))
        v[Ind(i, j, li, co)] = va_arg(arguments, Real);
    }
}

//!destructeur de la classe Matrix
/*!
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix::~Matrix()
//-----------------------------------------------------------------------------
{
  // desallocation de memoire
  desallocate();
}

//-----------------------------------------------------------------------------
void Matrix::allocate(const Indice lignes, const Indice colonnes)
//-----------------------------------------------------------------------------
{
  li = lignes;
  co = colonnes;
  sz = li * co;

  // allocation dynamique de memoire
  v = new Real[sz];

#ifdef VERIF_alloc
  if (!v)
  {
    internalFatalErrorLine("Matrix::allocate()",
                           "new Real[%d] for a [%d,%d] Matrixrix allocation Error\n"
                           "Seems to have an overflow memory error\n"
                           "Check your memory size, and memory consumption first\n",
                           sz, li, co);
  }
#endif
}

//-----------------------------------------------------------------------------
void Matrix::desallocate()
//-----------------------------------------------------------------------------
{
  // destruction du tableau de pointeurs de vecteurs
  // en meme temps, les vecteurs sont detruits
  if (sz >= 0)
    delete[] v;

#ifdef VERIF_assert
  else
    assert(v == NULL);
#endif

  li = 0;
  co = 0;
  sz = 0;
}

//!redimensionne la matrice
/*!
  Cette m�thode est utilis�e pour sp�cifier une nouvelle dimension de matrice de celle donn�e lors de l'initialisation par le constructeur
  \param lignes nombre de lignes
  \param colonnes nombre de colonnes 
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::redim(const Indice lignes, const Indice colonnes)
//-----------------------------------------------------------------------------
{
  if ((lignes == li) && (colonnes == co))
  {
    // initialisation
    fill(0.);
    return;
  }
  desallocate();
  allocate(lignes, colonnes);

  // initialisation
  fill(0.);
}

//!affichage du contenu d'une matrice
/*!
  Cette m�thode est une surd�finition de << pour les flux de sortie, son utilisation est donn�e comme suit
  
  Exemple
  \code
  Matrix t;
  cout << t << endl;
  \endcode
  \param os flux de sortie
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const Matrix &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//!affichage du contenu d'une matrice
/*!
  Cette m�thode permet d'afficher le contenu d'une matrice sur la sortie ostream
  \param os flux de sortie
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  Indice i, j;
  os << "matrix " << li << "x" << co << " ={{";
  for (i = 0; i < li; i++)
  {
    for (j = 0; j < co; j++)
    {
      if (j != 0)
        os << ",";
      os << v[Ind(i, j, li, co)];
    }
    if (i != li - 1)
      os << "},{";
  }
  os << "}}";
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement le remplissage des valeurs d'une matrice par un scalaire
  
  Exemple :
  \code
  Matrix t1;
  t1=fill(1.); // affecte 1 � toutes les composantes de la matrice
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::fill(Real val)
//-----------------------------------------------------------------------------
{
  Indice i;

  for (i = 0; i < sz; i++)
    v[i] = val;
}

//!renvoie une matrice identit� � partir d'une matrice quelconque
/*!
  Cette m�thode prend en argument une matrice quelconque et renvoie une matrice identit�
  
  Exemple :
  \code
  Matrix t1;
  t1.Identity(); // renvoie identit�
  \endcode
  \warning Cette m�thode modifie son argument
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::Identity()
//-----------------------------------------------------------------------------
{
  Indice i, j;

#ifdef VERIF_math
  if (li != co)
    fatalError("Matrix::Identity",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               li, co);
#endif

  for (i = 0; i < li; i++)
    for (j = 0; j < li; j++)
      v[Ind(i, j, li, co)] = (Real)(i == j);
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement le remplissage des valeurs d'une matrice par un scalaire
  
  Exemple :
  \code
  Matrix t1;
  t1=1.; // affecte 1 � toutes les composantes de la matrice
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix &Matrix::operator=(const Real &val)
//-----------------------------------------------------------------------------
{
  fill(val);
  return *this;
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement l'affectation sous la forme donn�e en exemple
  
  Exemple :
  \code
  Matrix t1,t2;
  t1=t2; // egalit� de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix &Matrix::operator=(const Matrix &mat)
//-----------------------------------------------------------------------------
{
  // redimension de la matrice resultat
  redim(mat.li, mat.co);

  // recopie des valeurs
#ifdef MEM_funct
  memcpy(v, mat.v, sz * sizeof(Real));
#else
  for (Indice i = 0; i < sz; i++)
    v[i] = mat.v[i];
#endif
  return *this;
}

//!addition de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration d'addition des matrices et d'�crire simplement la somme de deux matrices sous la forme donn�e en exemple
  
  Exemple :
  \code
  Matrix t1,t2,t3;
  t3=t1+t2; // somme de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix
Matrix::operator+(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
  Indice i;

  // test si egalite de dimensions
#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
    fatalError("Matrix::operator +",
               "matrix sizes incompatible\n"
               "You're about to add a [%d,%d] and a [%d,%d] matrix",
               li, co,
               mat.li, mat.co);
#endif

  // creation d'une nouvelle matrice
  Matrix resu(mat.li, mat.co);

  // calcul de la somme
  for (i = 0; i < sz; i++)
    resu.v[i] = v[i] + mat.v[i];

  // renvoi de l'objet
  return resu;
}

//!soustraction de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration de soustraction des matrices et d'�crire simplement la soustraction de deux matrices sous la forme donn�e en exemple
  
  Exemple :
  \code
  Matrix t1,t2,t3;
  t3=t1-t2; // soustraction de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix
Matrix::operator-(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
  Indice i;

#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
    fatalError("Matrix::operator -",
               "matrix sizes incompatible\n"
               "You're about to substract a [%d,%d] and a [%d,%d] matrix",
               li, co, mat.li, mat.co);
#endif

  // creation d'une nouvelle matrice
  Matrix resu(li, co);

  // calcul de la somme
  for (i = 0; i < sz; i++)
    resu.v[i] = v[i] - mat.v[i];

  // renvoi de l'objet
  return resu;
}

//!addition de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration d'addition des matrices et d'�crire simplement la somme de deux matrices sous la forme donn�e en exemple
  
  Exemple :
  \code
  Matrix t1,t2;
  t2+=t1; // somme de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::operator+=(const Matrix &mat)
//-----------------------------------------------------------------------------
{
  Indice i;

  // test si egalite de dimensions
#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
    fatalError("Matrix::operator +=",
               "matrix sizes incompatible\n"
               "You're about to add a [%d,%d] and a [%d,%d] matrix",
               li, co, mat.li, mat.co);
#endif

  // calcul de la somme
  for (i = 0; i < sz; i++)
    v[i] += mat.v[i];
}

//!soustraction de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration de soustraction des matrices et d'�crire simplement la soustraction de deux matrices sous la forme donn�e en exemple
  
  Exemple :
  \code
  Matrix t1,t2;
  t2-=t1; // soustraction de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::operator-=(const Matrix &mat)
//-----------------------------------------------------------------------------
{
  Indice i;

#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
    fatalError("Matrix::operator -=",
               "matrix sizes incompatible\n"
               "You're about to substract a [%d,%d] and a [%d,%d] matrix",
               li, co, mat.li, mat.co);
#endif

  // calcul de la somme
  for (i = 0; i < sz; i++)
    v[i] -= mat.v[i];
}

//!multiplication d'une matrice par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des matrices et d'�crire simplement la multiplication d'une matrice par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Matrix t1,t2;
  Real l;
  t2=t1*l; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix
    Matrix::operator*(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Indice i;
  Matrix resu(li, co);

  for (i = 0; i < sz; i++)
    resu.v[i] = lambda * v[i];
  return resu;
}

//!multiplication d'une matrice par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des matrices et d'�crire simplement la multiplication d'une matrice par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Matrix t1;
  Real l;
  t1*=l; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::operator*=(const Real &lambda)
//-----------------------------------------------------------------------------
{
  Indice i;

  for (i = 0; i < sz; i++)
    v[i] *= lambda;
}

//!division d'une matrice par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de division des matrices et d'�crire simplement la division d'une matrice par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Matrix t1;
  Real l;
  t1/=l; // division par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::operator/=(const Real &lambda)
//-----------------------------------------------------------------------------
{
  Indice i;

  for (i = 0; i < sz; i++)
    v[i] /= lambda;
}

//!division d'une matrice par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de division des matrices et d'�crire simplement la division d'une matrice par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Matrix t1,t2;
  Real l;
  t2=t1/l; // division par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix
Matrix::operator/(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Indice i;
  Matrix resu(li, co);

  for (i = 0; i < sz; i++)
    resu.v[i] = v[i] / lambda;
  return resu;
}

//!multiplication d'une matrice par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des matrices et d'�crire simplement la multiplication d'une matrice par un scalaire sous la forme donn�e en exemple. Elle est identique � la forme pr�c�dente (et commutative).
  
  Exemple :
  \code
  Matrix t1,t2;
  Real l;
  t2=l*t1; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix
operator*(const Real &lambda, const Matrix &mat)
//-----------------------------------------------------------------------------
{
  Indice i;
  Matrix resu(mat.li, mat.co);

  for (i = 0; i < mat.sz; i++)
    resu.v[i] = lambda * mat.v[i];
  return resu;
}

//!multiplication de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des matrices et d'�crire simplement la multiplication de deux matrice sous la forme donn�e en exemple. Cette op�ration correspond � la notion de produit contract� de deux matrices.
  
  Exemple :
  \code
  Matrix t1,t2,t3;
  t3=t1*t2; // produit contract�
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix Matrix::operator*(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (co != mat.li)
    cout << *this << endl;
  cout << mat << endl;
  fatalError("Matrix::operator *",
             "matrix sizes incompatible\n"
             "You're about to multiply a [%d,%d] and a [%d,%d] matrix",
             li, co, mat.li, mat.co);
#endif

  Matrix resu(li, mat.co);

  integer m, n, k;
  m = li;
  n = mat.co;
  k = co;
  integer lda = li;
  integer ldb = mat.li;
  integer ldc = li;
  double beta = 0.0;
  double alpha = 1.0;

  dgemm_("N", "N", &m, &n, &k, &alpha, v, &lda, mat.v, &ldb, &beta, resu.v, &ldc);

  return resu;
  /*
  Indice i, j, k;
  Real tmp;

  Matrix resu (li, mat.co);

  for (i = 0; i < li; i++)
    {
      for (k = 0; k < mat.co; k++)
	{
	  tmp = 0.;
	  for (j = 0; j < co; j++)
	    tmp += v[Ind (i, j, li, co)] * mat.v[Ind (j, k, co, mat.co)];
	  resu.v[Ind (i, k, li, mat.co)] = tmp;
	}
    }

  return resu;*/
}

//!multiplication de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des matrices et d'�crire simplement la multiplication de deux matrice sous la forme donn�e en exemple. Cette op�ration correspond � la notion de produit contract� de deux matrices.
  
  Exemple :
  \code
  Matrix t1,t2,t3;
  t3=t1*t2; // produit contract�
  \endcode
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
Matrix
    Matrix::operator*(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (co != mat.rows())
    fatalError("Matrix::operator *",
               "matrix sizes incompatible\n"
               "You're about to multiply a [%d,%d] and a [%d,%d] matrix",
               li, co, mat.rows(), mat.columns());
#endif

  Indice i, k;

  Matrix resu(li, mat.columns());

  for (i = 0; i < li; i++)
  {
    for (k = 0; k < mat.columns(); k++)
    {
      resu.v[Ind(i, k, li, mat.columns())] = v[Ind(i, k, li, co)] * mat(k);
    }
  }

  return resu;
}

//!multiplication d'une matrices par une matrice diagonale
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication d'une matrice par une matrice diagonale et de stocker le r�sultat dans la matrice sur laquelle on applique la m�thode. Les deux matrices doivent �tre de m�me dimension pour que cette m�thode soit utilisable.
  Exemple :
  \code
  Matrix t1;
  MatrixDiag t2;
  t1.squareMultiplyBy(t2); // produit 
  \endcode
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void Matrix::squareMultiplyBy(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (co != mat.rows())
    fatalError("Matrix::squareMultiplyBy (const MatrixDiag & mat)",
               "matrix sizes incompatible\n"
               "You're about to multiply a [%d,%d] and a [%d,%d] matrix",
               li, co, mat.rows(), mat.columns());
  if (co != li)
    fatalError("Matrix::squareMultiplyBy (const MatrixDiag & mat)",
               "matrix sizes incompatible\n"
               "Matrix must be a square one to do this operation");
#endif

  Indice k = 0;
  Indice i, j;
  for (i = 0; i < li; i++)
  {
    Real m = mat.v[i];
    for (j = 0; j < li; j++)
    {
      v[k++] *= m;
    }
  }
}

//!multiplication d'une matrices par l'inverse d'une matrice diagonale
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication d'une matrice par l'inverse d'une matrice diagonale et de stocker le r�sultat dans la matrice sur laquelle on applique la m�thode. Les deux matrices doivent �tre de m�me dimension pour que cette m�thode soit utilisable.
  Exemple :
  \code
  Matrix t1;
  MatrixDiag t2;
  t1.squareDivideBy(t2); // produit 
  \endcode
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void Matrix::squareDivideBy(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (co != mat.rows())
    fatalError("Matrix::squareMultiplyBy (const MatrixDiag & mat)",
               "matrix sizes incompatible\n"
               "You're about to multiply a [%d,%d] and a [%d,%d] matrix",
               li, co, mat.rows(), mat.columns());
  if (co != li)
    fatalError("Matrix::squareMultiplyBy (const MatrixDiag & mat)",
               "matrix sizes incompatible\n"
               "Matrix must be a square one to do this operation");
#endif

  Indice k = li * co - 1;
  Indice i, j;
  for (i = li - 1; i >= 0; i--)
  {
    Real m = 1.0 / mat.v[i];
    for (j = li - 1; j >= 0; j--)
    {
      v[k--] *= m;
    }
  }
}

//!multiplication d'une matrice par un vecteur
/*!
  Cette m�thode calcule le produit d'une matrice A de taille N-x-M par un vecteur x de taille N. ceci g�n�re un vecteur y de taille M. Cette m�thode utilise une routine Blas 2 et calcule le r�sultat de:
  \f[ y=A^{T}.x \f]
  Cette m�thode retourne un vecteur
  Exemple :
  \code
  Matrix t1;
  Vector v1,v2;
  v2=t1.trans_mult(v1); // produit
  \endcode
  \param vec vecteur du second membre
  \return vecteur r�sultant de l'op�ration de multiplication
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Matrix::trans_mult(const Vector &vec) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (li != vec.size())
    fatalError("Matrix::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               li, co, vec.size());
#endif

  Vector resu(co);

  integer m, n;
  m = li;
  n = co;
  integer lda = m;
  integer incx = 1;
  integer incy = 1;
  double beta = 0;
  double alpha = 1.0;

  dgemv_("T", &m, &n, &alpha, v, &lda, vec.v, &incx, &beta, resu.v, &incy);

  return resu;
}

//!multiplication d'une matrice par un vecteur
/*!
  Cette m�thode calcule le produit d'une matrice A de taille N-x-M par un vecteur x de taille M. ceci g�n�re un vecteur y de taille N. Cette m�thode utilise une routine Blas 2 et calcule le r�sultat de:
  \f[ y=A^{T}.x \f]
  Cette m�thode retourne un vecteur
  Exemple :
  \code
  Matrix t1;
  Vector v1,v2;
  v2=t1*v1; // produit
  \endcode
  \param vec vecteur du second membre
  \return vecteur r�sultant de l'op�ration de multiplication
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
    Matrix::operator*(const Vector &vec) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (co != vec.size())
    fatalError("Matrix::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               li, co, vec.size());
#endif

  Vector resu(li);

  integer m, n;
  m = li;
  n = co;
  integer lda = m;
  integer incx = 1;
  integer incy = 1;
  double beta = 0;
  double alpha = 1.0;

  dgemv_("N", &m, &n, &alpha, v, &lda, vec.v, &incx, &beta, resu.v, &incy);

  return resu;
}

//-----------------------------------------------------------------------------
void Matrix::productBy(Vector &resu) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (co != resu.size())
    fatalError("Matrix::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               li, co, resu.size());
#endif

  Vector vec = resu;

  integer m, n;
  m = li;
  n = co;
  integer lda = m;
  integer incx = 1;
  integer incy = 1;
  double beta = 0;
  double alpha = 1.0;

  dgemv_("N", &m, &n, &alpha, v, &lda, vec.v, &incx, &beta, resu.v, &incy);
}

//!calcule la trace d'une matrice
/*!
  Cette m�thode calcule la trace d'une matrice carr�e
  \return valeur de la trace de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Real Matrix::Trace()
//-----------------------------------------------------------------------------
{
  Indice i;

#ifdef VERIF_math
  if (li != co)
    fatalError("Matrix::Trace()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               li, co);
#endif

  Real trace = 0.;
  for (i = 0; i < li; i++)
    trace += v[Ind(i, i, li, co)];
  return trace;
}

//!calcule la moyenne de la trace d'une matrice
/*!
  Cette m�thode calcule la moyenne de la trace d'une matrice carr�e
  \return valeur de la moyenne de la trace de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Real Matrix::moyTrace()
//-----------------------------------------------------------------------------
{
  Indice i;

#ifdef VERIF_math
  if (li != co)
    fatalError("Matrix::moyTrace()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               li, co);
#endif

  Real trace = 0.;
  for (i = 0; i < li; i++)
    trace += v[Ind(i, i, li, co)];
  return (trace / (Real)li);
}

//!transpos�e d'une matrice
/*!
  Cette m�thode renvoie la transpos�e d'une matrice
  \return transpos�e de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix
Matrix::Transpose()
//-----------------------------------------------------------------------------
{
  Matrix resu(co, li);
  Indice i, j;

  for (i = 0; i < li; i++)
    for (j = 0; j < co; j++)
      resu.v[Ind(j, i, co, li)] = v[Ind(i, j, li, co)];
  return resu;
}

//!sommation des lignes d'une matrice
/*!
  Cette m�thode calcule la somme des termes sur les lignes d'une matrice et renvoie un vecteur correspondant
  \return vecteur contenant les sommes sur les lignes
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Matrix::rowSum()
//-----------------------------------------------------------------------------
{
  Vector resu(li);

  resu = 0.;

  for (Indice i = 0; i < li; i++)
    for (Indice j = 0; j < co; j++)
      resu(i) += v[Ind(i, j, li, co)];

  return resu;
}

//!sommation des colonnes d'une matrice
/*!
  Cette m�thode calcule la somme des termes sur les colonnes d'une matrice et renvoie un vecteur correspondant
  \return vecteur contenant les sommes sur les lignes
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Matrix::colSum()
//-----------------------------------------------------------------------------
{
  Vector resu(co);

  resu = 0.;

  for (Indice j = 0; j < co; j++)
    for (Indice i = 0; i < li; i++)
      resu(i) += v[Ind(i, j, li, co)];

  return resu;
}

//!partie sym�trique d'une matrice
/*!
  Cette m�thode extraie la partie sym�trique d'une matrice et la renvoie
  \warning Cette m�thode n'est disponible que pour les matrices de taille 2 et 3.
  \return partie sym�trique de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::getSymetricPart(Matrix &s_v) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (li != co)
    fatalError("Matrix::getSymetricPart()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               li, co);
  if (s_v.li != s_v.co)
    fatalError("Matrix::getSymetricPart()",
               "The resulting matrix is not a square matrix, it's a [%d,%d] matrix",
               s_v.li, s_v.co);
  if (li != s_v.li)
    fatalError("Matrix::getSymetricPart()",
               "The dimensions of the two matrix are'nt corresponding\n"
               "they are respectively a [%d,%d] and a [%d,%d] matrix",
               li, co, s_v.li, s_v.co);
#endif

  // partie symetrique de v
  if (co == 2)
  {
    s_v.v[0] = v[0];
    s_v.v[3] = v[3];
    s_v.v[2] = s_v.v[1] = (v[1] + v[2]) / 2.;
    return;
  }

  if (co == 3)
  {
    s_v.v[0] = v[0];
    s_v.v[4] = v[4];
    s_v.v[8] = v[8];
    s_v.v[3] = s_v.v[1] = (v[1] + v[3]) / 2.;
    s_v.v[2] = s_v.v[6] = (v[2] + v[6]) / 2.;
    s_v.v[5] = s_v.v[7] = (v[5] + v[7]) / 2.;
    return;
  }

  fatalError("Matrix::getSymetricPart()",
             "Function not implemented for a dimension other than 2 or 3\n");
}

//!partie anti-sym�trique d'une matrice
/*!
  Cette m�thode extraie la partie anti-sym�trique d'une matrice et la renvoie
  \warning Cette m�thode n'est disponible que pour les matrices de taille 2 et 3.
  \return partie anti-sym�trique de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::getAntiSymetricPart(Matrix &a_v) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (li != co)
    fatalError("Matrix::getAntiSymetricPart()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               li, co);
  if (a_v.li != a_v.co)
    fatalError("Matrix::getAntiSymetricPart()",
               "The resulting matrix is not a square matrix, it's a [%d,%d] matrix",
               a_v.li, a_v.co);
  if (li != a_v.li)
    fatalError("Matrix::getAntiSymetricPart()",
               "The dimensions of the two matrix are'nt corresponding\n"
               "they are respectively a [%d,%d] and a [%d,%d] matrix",
               li, co, a_v.li, a_v.co);
#endif

  // partie antisymetrique de v
  if (co == 2)
  {
    a_v.v[0] = 0.;
    a_v.v[3] = 0.;
    a_v.v[2] = (v[2] - v[1]) / 2.;
    a_v.v[1] = -a_v.v[2];
    return;
  }

  if (co == 3)
  {
    a_v.v[0] = 0.;
    a_v.v[4] = 0.;
    a_v.v[8] = 0.;
    a_v.v[3] = (v[3] - v[1]) / 2.;
    a_v.v[1] = -a_v.v[3];
    a_v.v[6] = (v[6] - v[2]) / 2.;
    a_v.v[7] = (v[7] - v[5]) / 2.;
    a_v.v[2] = -a_v.v[6];
    a_v.v[5] = -a_v.v[7];
    return;
  }

  fatalError("Matrix::getAntiSymetricPart()",
             "Function not implemented for a dimension other than 2 or 3\n");
}

//!extraction d'une ligne d'une matrice
/*!
  Cette m�thode extraie une ligne d'une matrice et la renvoie
  \param row num�ro de la ligne de la matrice (origine 0).
  \return vecteur contenant la ligne \c row de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Matrix::getRow(Indice row)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(row, 0);
#endif

  Vector resu(co);

  for (Indice j = 0; j < co; j++)
    resu(j) = v[Ind(row, j, li, co)];

  return resu;
}

//!extraction d'une colonne d'une matrice
/*!
  Cette m�thode extraie une colonne d'une matrice et la renvoie
  \param col num�ro de la colonne de la matrice (origine 0).
  \return vecteur contenant la colonne \c col de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Matrix::getCol(Indice col)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(0, col);
#endif

  Vector resu(li);

  for (Indice i = 0; i < li; i++)
    resu(i) = v[Ind(i, col, li, co)];

  return resu;
}

//!egalit� de deux matrices
/*!
  Cette m�thode teste l'�galit� de deux matrices
  \return True si les deux matrices sont identiques, False dans la cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Boolean
Matrix::operator==(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
  Indice i;

#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
    fatalError("Matrix::operator ==",
               "The dimensions of the two matrix are'nt corresponding\n"
               "they are respectively a [%d,%d] and a [%d,%d] matrix",
               li, co, mat.li, mat.co);
#endif

  for (i = 0; i < sz; i++)
    if (v[i] != mat.v[i])
      return False;
  return True;
}

//!in�galit� de deux matrices
/*!
  Cette m�thode teste l'in�galit� de deux matrices
  \return True si les deux matrices sont differentes, False dans la cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Boolean
Matrix::operator!=(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
  return !(*this == mat);
}

//!sortie sur flux ofstream
/*!
  Cette m�thode permet d'�crire une matrice dans un fichier (notament) binaire
  
  Exemple :
  \code
  ofstream pfile("fichier");
  Matrix t;
  t.write(pfile);
  t.close();
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::write(ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)&li, sizeof(Indice));
  ofs.write((char *)&co, sizeof(Indice));
  ofs.write((char *)v, sz * sizeof(Real));
}

//!lecture sur flux ifstream
/*!
  Cette m�thode permet de lire une matrice depuis un fichier (notament) binaire
  
  Exemple :
  \code
  ifstream pfile("fichier");
  Matrix t;
  t.read(pfile);
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::read(ifstream &ifs)
//-----------------------------------------------------------------------------
{
  Indice lignes, colonnes;

  ifs.read((char *)&lignes, sizeof(Indice));
  ifs.read((char *)&colonnes, sizeof(Indice));
  redim(lignes, colonnes);
  ifs.read((char *)v, sz * sizeof(Real));
}

//!sortie sur flux ofstream
/*!
  Cette m�thode permet d'�crire une matrice dans un fichier (notament) binaire
  
  Exemple :
  \code
  ofstream pfile("fichier");
  Matrix t;
  pfile << t;
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const Matrix &mat)
//-----------------------------------------------------------------------------
{
  mat.write(os);
  return os;
}

//!lecture sur flux ifstream
/*!
  Cette m�thode permet de lire une matrice depuis un fichier (notament) binaire
  
  Exemple :
  \code
  ifstream pfile("fichier");
  Matrix t;
  pfile >> t;
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, Matrix &mat)
//-----------------------------------------------------------------------------
{
  mat.read(is);
  return is;
}

//-----------------------------------------------------------------------------
long Matrix::Memory() const
//-----------------------------------------------------------------------------
{
  return (sz * sizeof(Real));
}

//!d�terminant d'une matrice carr�e
/*!
  Cette m�thode calcule le d�terminant d'une matrice carr�e en utilisant la routine DGETRF de la librairie Lapack. Cette routine calcule la d�composition LU d'une matrice g�n�rale A de taille M-x-N en utilisant des pivots partiels avec �change de ligne. La factorisation a la forme  A = P * L * U avec P matrice de permutations, L la partie triangulaire inf�rieure avec �l�ments unitaires sur la diagonale et U la partie triangulaire sup�rieure. On a alors 
  \f[
  det A = tr [U]
  \f]
  \return valeur du d�terminant
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Real Matrix::det() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (li != co)
    fatalError("Matrix::det",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               li, co);
#endif

  integer n, m, info, lda;
  double Det = 1;

  n = li;
  m = li;
  lda = li;
  integer *ipiv = new integer[li];

  // init a matrix
  double *A = new double[sz];
#ifdef MEM_funct
  memcpy(A, v, sz * sizeof(Real));
#else
  for (Indice i = 0; i < sz; i++)
    A[i] = v[i];
#endif

  // DGETRF computes an LU factorization of a general M-by-N matrix A
  // using partial pivoting with row interchanges.

  // The factorization has the form
  // A = P * L * U
  // where P is a permutation matrix, L is lower triangular with unit
  // diagonal elements (lower trapezoidal if m > n), and U is upper
  // triangular (upper trapezoidal if m < n).

  dgetrf_(&m, &n, A, &lda, ipiv, &info);
  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::det",
                             "parameter %d of function dgetrf_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::det",
                 "pivot (%d) is exactly zero\n"
                 "The factorization has been completed, but this factor is exactly singular, and division by zero will occur if it is used to solve a system of equations",
                 info);
    }
  }

  for (int i = 0; i < li; i++)
    Det *= A[Ind(i, i, li, co)];

  delete[] ipiv;
  delete[] A;

  return Det;
}

/*
 Real 	De=0.;
  
#ifdef VERIF_math	
  if (li!=co) 
    fatalError("Matrix::det",
	       "Your matrix is not a square matrix, it's a [%d,%d] matrix",li,co);
#endif
  
 //  cas determinant 4x4 
  if (li==4) {
    De=v[3]*v[co+2]*v[2*co+1]*v[3*co]-v[2]*v[co+3]*v[2*co+1]*v[3*co]-
      v[3]*v[co+1]*v[2*co+2]*v[3*co]+v[1]*v[co+3]*v[2*co+2]*v[3*co];
    De+=v[2]*v[co+1]*v[2*co+3]*v[3*co]-v[1]*v[co+2]*v[2*co+3]*v[3*co]-
      v[3]*v[co+2]*v[2*co]*v[3*co+1]+v[2]*v[co+3]*v[2*co]*v[3*co+1];
    De+=v[3]*v[co]*v[2*co+2]*v[3*co+1]-v[0]*v[co+3]*v[2*co+2]*v[3*co+1]-
      v[2]*v[co]*v[2*co+3]*v[3*co+1]+v[0]*v[co+2]*v[2*co+3]*v[3*co+1];
    De+=v[3]*v[co+1]*v[2*co]*v[3*co+2]-v[1]*v[co+3]*v[2*co]*v[3*co+2]-
      v[3]*v[co]*v[2*co+1]*v[3*co+2]+v[0]*v[co+3]*v[2*co+1]*v[3*co+2];
    De+=v[1]*v[co]*v[2*co+3]*v[3*co+2]-v[0]*v[co+1]*v[2*co+3]*v[3*co+2]-
      v[2]*v[co+1]*v[2*co]*v[3*co+3]+v[1]*v[co+2]*v[2*co]*v[3*co+3];
    De+=v[2]*v[co]*v[2*co+1]*v[3*co+3]-v[0]*v[co+2]*v[2*co+1]*v[3*co+3]-
      v[1]*v[co]*v[2*co+2]*v[3*co+3]+v[0]*v[co+1]*v[2*co+2]*v[3*co+3];
    return De;
  }
  
 //  cas determinant 3x3 
  if (li==3) {
    De=v[0]*v[co+1]*v[2*co+2]+v[co]*v[2*co+1]*v[2]+v[2*co]*v[1]*v[co+2]
      -v[2*co]*v[co+1]*v[2]-v[0]*v[2*co+1]*v[co+2]-v[co]*v[1]*v[2*co+2];
    return De;
  }
	
  // cas determinant 2x2 
  if (li==2) {
    De=v[0]*v[co+1]-v[1]*v[co];
    return De;
  }
	
//   cas ou on ne sait pas faire directement 
  Indice 	k,i,l,j;
  Matrix 	Dj(li-1,co-1);
  
  for (j=0;j<li;j++) {
    
    // Determinant de Dj 
    
    for (i=1;i<li;i++) {
      l=0; 
      for (k=0;k<li;k++) {
	if (k!=j) Dj.v[Ind(i-1,l++,li-1,co-1)]=v[Ind(i,k,li,co)];
      }
    }
    De+=Dj.det()*(((j % 2)==0) ? 1. : -1.)*v[Ind(0,j,li,co)];
  }
  
  return De;
}*/

//!resoud un syst�me lin�aire en utilisant une m�thode de Gauss
/*!
  Diagonalise une matrice pour r�soudre un syst�me lin�aire. Pour r�soudre un syst�me, on fait ensuite appel � la m�thode forBackLY()
  
  La syntaxe de r�solution d'un probl�me lin�aire est alors
  \code
  Matrix A;
  Vector X,Y;
  X=(A.factLU()).forwBackLU(Y); // r�solution de A.X=Y
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix
Matrix::factLU()
//-----------------------------------------------------------------------------
{
  Indice i, j, k;
  Real sum, h;
  Matrix A;
  A.redim(li, co);
  A = *this;

  if (li != co)
    fatalError("Matrix::factLU()", "called whith a non square matrix");

  Indice n = li;

  for (j = 1; j <= n; j++)
  {
    for (i = 1; i < j; i++)
    {
      sum = A(i - 1, j - 1);
      for (k = 1; k < i; k++)
        sum -= A(i - 1, k - 1) * A(k - 1, j - 1);
      A(i - 1, j - 1) = sum;
    }

    for (i = j; i <= n; i++)
    {
      sum = A(i - 1, j - 1);
      for (k = 1; k < j; k++)
        sum -= A(i - 1, k - 1) * A(k - 1, j - 1);
      A(i - 1, j - 1) = sum;
    }

    if (j != n)
    {

      if (A(j - 1, j - 1) == 0)
      {
        fatalError("Matrix(Type)::factLU (no pivoting)",
                   "Division by zero - try LU with pivoting");
      }
      else if (Abs(A(j - 1, j - 1)) < 1.0e-12)
      {
        warning("Matrix(Type)::factLU (no pivoting)",
                "Dividing by %g (abs) in Gaussian elimination, row=%d",
                Abs(A(j - 1, j - 1)), j);
      }

      h = 1.0 / (A(j - 1, j - 1));
      for (i = j + 1; i <= n; i++)
        A(i - 1, j - 1) *= h;
    }
  }

  //   *this=cp;
  return A;
}

//!resoud une syst�me lin�aire en utilisant une m�thode de Gauss
/*!
  Pour utiliser cette m�thode, l'objet doit etre une matrice diagonale obtenue � partir de la m�thode factLU
  \param b vecteur des constantes
  \return Vectorteur de solutions
  
  La syntaxe de r�solution d'un probl�me lin�aire est alors
  \code
  Matrix A;
  Vector X,Y;
  X=(A.factLU()).forwBackLU(Y); // r�solution de A.X=Y
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Matrix::forwBackLU(const Vector b)
//-----------------------------------------------------------------------------
{

  // Triangular solves for unpivoted LU factorization
  Indice i, j;
  Indice ii = 0;
  Real sum;
  Indice n = li;

  if (b.size() != li)
  {
    fatalError("Matrix(Type)::forwBackLU",
               "b's size (=%d) != A's size (=%d)", b.size(), li);
  }

  Vector x;
  x.redim(li);

  for (i = 1; i <= n; i++)
  {
    sum = b(i - 1);
    if (ii)
      for (j = ii; j <= i - 1; j++)
        sum -= v[Ind(i - 1, j - 1, li, co)] * x(j - 1);
    else if (sum != 0.0)
      ii = i;

    x(i - 1) = sum;
  }

  for (i = n; i >= 1; i--)
  {
    sum = x(i - 1);
    for (j = i + 1; j <= n; j++)
      sum -= v[Ind(i - 1, j - 1, li, co)] * x(j - 1);

    x(i - 1) = sum / (v[Ind(i - 1, i - 1, li, co)]);
  }

  return x;
}

/*
//-----------------------------------------------------------------------------
Matrix Matrix::inverse()
//-----------------------------------------------------------------------------
{
  Matrix SMatrix(li-1,co-1);
  Matrix inv(li,co);
  Real deter;
  Indice i,j,k,KS,LS,l;	

  // calcul du determinant de la matrice 
  deter=det();

 // if (Abs(deter)<1e-15) fatalError("Matrix::inverse","Matrix determinant is too less (%10.6E)",deter);

  // Calcul de la matrice des cofacteurs
  for (i=0;i<li;i++) {
    for (j=0;j<li;j++) {
      KS=0;
      for (k=0;k<li;k++) {
	if (k!=i) {
	  LS=0; 
	  for (l=0;l<li;l++) { 
	    if (l!=j) SMatrix.v[Ind(KS,LS++,li-1,co-1)]=v[Ind(k,l,li,co)];
	  }
	  KS++;
	}
      }
      inv.v[Ind(j,i,li,co)]=SMatrix.det()*((((i+j) % 2)==0) ? 1. : -1.)/deter;
    }	
  }
  return inv;
}
*/

//!inverse d'une matrice
/*!
  Cette m�thode calcule l'inverse d'une matrice et le renvoie. Elle utilise les fonctions DGETRF et DGETRI de la librairie Lapack. La matrice initiale est pr�serv�e par cette m�thode.
  \return valeur de l'inverse d'une matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix
Matrix::inverse()
//-----------------------------------------------------------------------------
{
  Matrix inv(li, co);
  integer n, m, info, lda, /*nrhs,*/ lwork;
  double *work;
  integer *ipiv;
  double worksize;
  n = li;
  m = li;
  lda = li;
  //  nrhs = 1;
  ipiv = new integer[li];

  // copie de la matrice
  inv = *this;

  // DGETRF computes an LU factorization of a general M-by-N matrix A
  // using partial pivoting with row interchanges.

  // The factorization has the form
  // A = P * L * U
  // where P is a permutation matrix, L is lower triangular with unit
  // diagonal elements (lower trapezoidal if m > n), and U is upper
  // triangular (upper trapezoidal if m < n).

  dgetrf_(&m, &n, inv.v, &lda, ipiv, &info);
  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::inverse",
                             "parameter (%d) of function dgetrf_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::inverse",
                 "pivot (%d) is exactly zero\n"
                 "The factorization has been completed, but this factor is exactly singular, and division by zero will occur if it is used to solve a system of equations",
                 info);
    }
  }

  // DGETRI computes the inverse of a matrix using the LU factorization
  // computed by DGETRF.

  // This method inverts U and then computes inv(A) by solving the system
  // inv(A)*L = inv(U) for inv(A).

  // computes the work length vector
  lwork = -1;
  dgetri_(&n, inv.v, &lda, ipiv, &worksize, &lwork, &info);
  lwork = (integer)worksize;

  // allocate memory for the work buffer
  work = new double[lwork];

  // computes the inverse of the matrix inv
  dgetri_(&n, inv.v, &lda, ipiv, work, &lwork, &info);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::inverse",
                             "parameter %d of function dgetri_ has illegal value",
                             -info);
    }
  }

  // delete tempory memory allocations
  delete[] work;
  delete[] ipiv;

  // and return the result
  return inv;
}

//!resoud une syst�me lin�aire en utilisant une m�thode de Gauss
/*!
  Diagonalise une matrice pour r�soudre un syst�me lin�aire. Pour r�soudre un syst�me, on fait ensuite appel � la m�thode forBackLY()
  \param perm_ vecteur des permutations
  \param scratch vecteur temporaire
  
  La syntaxe de r�solution d'un probl�me lin�aire est alors
  \code
  Vector b;
  Vector perm;
  Vector v1;
  
  b.redim(li);
  (factLU(perm,v1)).forwBackLU(x,b,perm);
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Matrix
Matrix::factLU(Vector &perm_, Vector &scratch)
//-----------------------------------------------------------------------------
{
  Matrix A;
  A.redim(li, co);
  A = *this;

  const Real TINY = 1.0e-20;

  Indice i, imax, j, k;
  Real big, dum, sum, temp;
  imax = 0;

  if (li != co)
    fatalError("Matrix::factLU()", "called whith a non square matrix");

  // redim des vecteurs
  perm_.redim(li);
  scratch.redim(li);

  Indice nperm = 1; // indicates pivoting and odd/even number of row changes

  const Indice n = li;

  for (i = 1; i <= n; i++)
  {
    big = 0.0;
    for (j = 1; j <= n; j++)
      if ((temp = Abs(A(i - 1, j - 1))) > big)
        big = temp;
    if (big == 0.0)
    {
      fatalError("Matrix::factLU (with pivoting)",
                 "Division by zero - singular matrix?");
    }
    scratch(i - 1) = 1.0 / big;
  }
  for (j = 1; j <= n; j++)
  {
    for (i = 1; i < j; i++)
    {
      sum = A(i - 1, j - 1);
      for (k = 1; k < i; k++)
        sum -= A(i - 1, k - 1) * A(k - 1, j - 1);
      A(i - 1, j - 1) = sum;
    }
    big = 0.0;
    for (i = j; i <= n; i++)
    {
      sum = A(i - 1, j - 1);
      for (k = 1; k < j; k++)
        sum -= A(i - 1, k - 1) * A(k - 1, j - 1);
      A(i - 1, j - 1) = sum;
      if ((dum = scratch(i - 1) * Abs(sum)) >= big)
      {
        big = dum;
        imax = i;
      }
    }
    if (j != imax)
    {
      for (k = 1; k <= n; k++)
      {
        dum = A(imax - 1, k - 1);
        A(imax - 1, k - 1) = A(j - 1, k - 1);
        A(j - 1, k - 1) = dum;
      }
      nperm = -nperm;
      scratch(imax - 1) = scratch(j - 1);
    }
    perm_(j - 1) = imax;
    if (A(j - 1, j - 1) == 0.0)
      A(j - 1, j - 1) = TINY;

    if (j != n)
    {

      if (Abs(A(j - 1, j - 1)) < 1.0e-12)
        warning("Matrix(Type)::factLU (w/ pivoting)",
                "Dividing by %g (abs) in Gaussian elimination, row=%d",
                Abs(A(j - 1, j - 1)), j);

      dum = 1.0 / (A(j - 1, j - 1));
      for (i = j + 1; i <= n; i++)
        A(i - 1, j - 1) *= dum;
    }
  }
  return A;
}

//!resoud une syst�me lin�aire en utilisant une m�thode de Gauss
/*!
  Pour utiliser cette m�thode, l'objet doit etre une matrice diagonale obtenue � partir de la m�thode factLU
  \param b vecteur des solutions
  \param x vecteur du second membre
  \param perm_ vecteur contenant les permutations
  \return Vectorteur de solutions
  
  La syntaxe de r�solution d'un probl�me lin�aire est alors
  \code
  Vector b;
  Vector perm;
  Vector v1;
  
  b.redim(li);
  (factLU(perm,v1)).forwBackLU(x,b,perm);
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Matrix::forwBackLU(Vector &b, Vector &x, const Vector &perm_)
//-----------------------------------------------------------------------------
{
  // Triangular solves for pivoted LU factorization
  Indice i, ip, j;
  Indice ii = 0;
  Real sum;

  Matrix A;
  A.redim(li, co);
  A = *this;

  const Indice n = li;

  if (b.size() != n)
  {
    fatalError("Matrix::forwBackLU", "b's size (=%d) != A's size (=%d)",
               b.size(), n);
  }

  x.redim(n);

  if (&x != &b)
    x = b;

  for (i = 1; i <= n; i++)
  {
    ip = (Indice)perm_(i - 1);
    sum = x(ip - 1);
    x(ip - 1) = x(i - 1);
    if (ii)
      for (j = ii; j <= i - 1; j++)
        sum -= A(i - 1, j - 1) * x(j - 1);
    else if (sum != 0.0)
      ii = i;

    x(i - 1) = sum;
  }

  for (i = n; i >= 1; i--)
  {
    sum = x(i - 1);
    for (j = i + 1; j <= n; j++)
      sum -= A(i - 1, j - 1) * x(j - 1);
    x(i - 1) = sum / (A(i - 1, i - 1));
  }
}

//!r�solution d'un syst�me lin�aire
/*!
  Cette m�thode calcule la solution du syst�me lin�aire  \f[ A x = b \f] avec A matrice N-x-N et x et y vecteurs de taille N. Cette m�thode utilise les routines DGETRF et DGETRS de la librairie Lapack.

  Cette m�thode utilise des copies de vecteurs et matrices donn�s en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel.
  \param x vecteur du second membre
  \return vecteur solution du syst�me lin�aire
  \author Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
Vector
Matrix::getSolve(const Vector &x)
//-----------------------------------------------------------------------------
{
  Vector b(li);
  integer n, /*m,*/ info, lda, ldb, nrhs;
  integer *ipiv;

  n = li;
  //  m = li;
  lda = li;
  ldb = li;
  ipiv = new integer[li];
  nrhs = 1;
  b = x;

  // init a matrix
  double *A = new double[sz];
#ifdef MEM_funct
  memcpy(A, v, sz * sizeof(Real));
#else
  for (Indice i = 0; i < sz; i++)
    A[i] = v[i];
#endif

  // DGETRF computes an LU factorization of a general M-by-N matrix A
  // using partial pivoting with row interchanges.

  // The factorization has the form
  // A = P * L * U
  // where P is a permutation matrix, L is lower triangular with unit
  // diagonal elements (lower trapezoidal if m > n), and U is upper
  // triangular (upper trapezoidal if m < n).

  dgesv_(&n, &nrhs, A, &lda, ipiv, b.v, &ldb, &info);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::solveLinearPivoting",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::solveLinearPivoting",
                 "pivot (%d) is exactly zero\n"
                 "The factorization has been completed, but this factor is exactly singular, and division by zero will occur if it is used to solve a system of equations",
                 info);
    }
  }

  delete[] ipiv;
  delete[] A;

  return b;
}

//!r�solution d'un syst�me lin�aire
/*!
  Cette m�thode calcule la solution du syst�me lin�aire  \f[ A x = b \f] avec A matrice N-x-N et x et y vecteurs de taille N. Cette m�thode utilise les routines DGETRF et DGETRS de la librairie Lapack.

  Cette m�thode utilise directement les vecteurs et matrices donn�s en argument et modifie les valeurs contenues dans ceux-ci au cours de l'appel. Cette methode est un peu plus rapide que la m�thode getSolve(const Vector& x) pour les larges matrices mais poss�de un avantage concernant les besoins m�moire.
  \param x vecteur du second membre
  \return vecteur solution du syst�me lin�aire
  \author Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
void Matrix::solve(Vector &b)
//-----------------------------------------------------------------------------
{
  integer n, /*m,*/ info, lda, ldb, nrhs;
  integer *ipiv;

  n = li;
  //  m = li;
  lda = li;
  ldb = li;
  ipiv = new integer[li];
  nrhs = 1;

  // DGETRF computes an LU factorization of a general M-by-N matrix A
  // using partial pivoting with row interchanges.

  // The factorization has the form
  // A = P * L * U
  // where P is a permutation matrix, L is lower triangular with unit
  // diagonal elements (lower trapezoidal if m > n), and U is upper
  // triangular (upper trapezoidal if m < n).

  //  dgetrf_ (&m, &n, v, &lda, ipiv, &info);
  dgesv_(&n, &nrhs, v, &lda, ipiv, b.v, &ldb, &info);
  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::solveLinearPivoting",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::solveLinearPivoting",
                 "pivot (%d) is exactly zero\n"
                 "The factorization has been completed, but this factor is exactly singular, and division by zero will occur if it is used to solve a system of equations",
                 info);
    }
  }

  // DGETRS solves a system of linear equations
  // A * X = B  or  A' * X = B
  // with a general N-by-N matrix A using the LU factorization computed
  // by DGETRF.

  //   dgetrs_ ("N", &n, &nrhs, v, &lda, ipiv, b.v, &ldb, &info);
  //   if (info != 0)
  //     {
  //       if (info < 0)
  // 	{
  // 	  internalFatalErrorLine ("Matrix::solveLinearPivoting",
  // 				  "parameter %d of function dgetrf_ has illegal value",
  // 				  -info);
  // 	}
  //     }

  delete[] ipiv;
}

//!�valuation du Null Space d'une matrice
/*!
  Cette m�thode calcule le Null Space d'une matrice \f$ \textbf{A} \f$. Le null-space de  \f$ \textbf{A}=N.S.(\textbf{A})\subset\mathcal{W} \f$  est l'ensemble des �l�ments de \f$ \mathcal{V} \f$ tels que \f$ \textbf{A}(\overrightarrow{v})=\overrightarrow{0} \f$. Cette m�thode utilise la routine DGESVD de la librairie Lapack.

  Cette m�thode utilise des copies de vecteurs et matrices donn�s en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel.
  \param relative Param�tre bool�en indiquant si la tol�rance de recherche est relative ou non. Dans le cas d'une tol�rance relative, la tol�rance de recherche de la plus patite valeur preopres est recalcul�e par rapport � la plus grande valeur propre.
  \param tol tol�rance de recherche du Null-Space par d�faut 1e-10.
  \return matrice contenant les vecteurs du Null-Space en colonnes (taille N x nNs) avce nNs nombre de "modes rigides"
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
Matrix
Matrix::getNullSpace(Boolean relative, double tol)
//-----------------------------------------------------------------------------
{
  Vector S(li);
  integer n, m, info, lda;
  integer ldu, ldvt, lwork;
  double *work;
  double worksize;
  double *U = NULL;
  double *VT = NULL;

  n = li;
  m = li;
  lda = li;
  //  ldb = li;
  ldu = li;
  ldvt = li;

  // init a matrix
  Matrix A = *this;

  //  DGESVD computes the singular value decomposition (SVD) of a real
  //  M-by-N matrix A, optionally computing the left and/or right singular
  //  vectors. The SVD is written

  //       A = U * SIGMA * transpose(V)

  //  where SIGMA is an M-by-N matrix which is zero except for its
  //  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  //  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
  //  are the singular values of A; they are real and non-negative, and
  //  are returned in descending order.  The first min(m,n) columns of
  //  U and V are the left and right singular vectors of A.

  //  Note that the routine returns V**T, not V.

  // computes the work length vector
  lwork = -1;
  dgesvd_("N", "O", &m, &n, &A(0, 0), &lda, &S(0), U, &ldu, VT, &ldvt, &worksize, &lwork, &info);
  lwork = (integer)worksize;

  // allocate memory for the work buffer
  work = new double[lwork];

  // compute eigenvalues and eigenvectors
  dgesvd_("N", "O", &m, &n, &A(0, 0), &lda, &S(0), U, &ldu, VT, &ldvt, work, &lwork, &info);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getNullSpace",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getNullSpace",
                 "Eigenvalues decomposition incorrect",
                 info);
    }
  }

  // on recalcule la tol�rance relative en la multipliant par la plus grande valeur propre
  if (relative)
    tol *= S(0);

  // on compte le nombre de null colonnes
  Indice ic = 0;
  for (Indice i = n; i > 0;)
    if (S(--i) <= tol)
      ic++;

  // dimension de la matrice de sortie
  Matrix ret(n, ic);

  // remplissage du Null-Space
  ic = 0;
  for (Indice i = 0; i < n; i++)
  {
    if (S(i) < tol)
    {
      // copy the last rows
      for (Indice j = 0; j < n; j++)
        ret(j, ic) = A(i, j);
      ic++;
    }
  }

  delete[] work;

  return ret;
}

//-----------------------------------------------------------------------------
Matrix
Matrix::getNullSpace2(Boolean relative, double tol)
//-----------------------------------------------------------------------------
{
  Vector S(li);
  integer n, m, info, lda;
  integer ldu, ldvt, lwork;
  integer *iwork;
  double *work;
  double worksize;
  Matrix U;
  Matrix VT;

  n = li;
  m = li;
  lda = li;
  //  ldb = li;
  ldu = li;
  ldvt = li;

  // init a matrix
  Matrix A = *this;

  U.redim(ldu, m);
  VT.redim(ldvt, n);

  //  DGESVD computes the singular value decomposition (SVD) of a real
  //  M-by-N matrix A, optionally computing the left and/or right singular
  //  vectors. The SVD is written

  //       A = U * SIGMA * transpose(V)

  //  where SIGMA is an M-by-N matrix which is zero except for its
  //  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  //  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
  //  are the singular values of A; they are real and non-negative, and
  //  are returned in descending order.  The first min(m,n) columns of
  //  U and V are the left and right singular vectors of A.

  //  Note that the routine returns V**T, not V.

  // computes the work length vector
  iwork = new integer[8 * m];

  lwork = -1;
  dgesdd_("A", &m, &n, &A(0, 0), &lda, &S(0), &U(0, 0), &ldu, &VT(0, 0), &ldvt, &worksize, &lwork, iwork, &info);
  lwork = (integer)worksize;

  // allocate memory for the work buffer
  work = new double[lwork];

  // compute eigenvalues and eigenvectors
  dgesdd_("A", &m, &n, &A(0, 0), &lda, &S(0), &U(0, 0), &ldu, &VT(0, 0), &ldvt, work, &lwork, iwork, &info);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getNullSpace",
                             "parameter %d of function dgesdd_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getNullSpace",
                 "Eigenvalues decomposition incorrect",
                 info);
    }
  }

  // on recalcule la tol�rance relative en la multipliant par la plus grande valeur propre
  if (relative)
    tol *= S(0);

  // on compte le nombre de null colonnes
  Indice ic = 0;
  for (Indice i = n; i > 0;)
    if (S(--i) <= tol)
      ic++;

  // dimension de la matrice de sortie
  Matrix ret(n, ic);

  // remplissage du Null-Space
  ic = 0;
  for (Indice i = 0; i < n; i++)
  {
    if (S(i) < tol)
    {
      // copy the last rows
      for (Indice j = 0; j < n; j++)
        ret(j, ic) = VT(i, j);
      ic++;
    }
  }

  delete[] work;
  delete[] iwork;

  return ret;
}

//!�valuation valeurs propres et des vecteurs propres � droite
/*!
  Cette m�thode calcule les valeurs propres et les vecteurs propres � droite d'une matrice \f$ \textbf{A} \f$. Cette m�thode utilise la routine DGESVD de la librairie Lapack.

  Cette m�thode utilise des copies de vecteurs et matrices donn�s en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel. De plus, les matrices et vecteurs de retour donn�s en argument n'on pas besoin d'�tre dimensionn�s � un taille correcte avant appel (ceci est fait en interne dans la routine).
  \warning la matrice eigenVectors contient les vecteurs propres de la matrice sous une forme TRANSPOSEE.
  \param eigenValues vecteur contenant en retour les valeurs propres de la matrice
  \param eigenVectors Matrice contenant en retour les vecteurs propres de la matrice (TRANSPOSES)
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void Matrix::computeRightEigenVectors(Vector &eigenValues, Matrix &eigenVectors)
//-----------------------------------------------------------------------------
{
  integer n, m, info, lda;
  integer ldu, ldvt, lwork;
  double *work;
  double worksize;
  double *U = NULL;

  n = li;
  m = li;
  lda = li;
  //  ldb = li;
  ldu = li;
  ldvt = li;

  // init a matrix
  Matrix A = *this;

  // redim the matrices and vectors
  eigenVectors.redim(n, n);
  eigenValues.redim(n);

  //  DGESVD computes the singular value decomposition (SVD) of a real
  //  M-by-N matrix A, optionally computing the left and/or right singular
  //  vectors. The SVD is written

  //       A = U * SIGMA * transpose(V)

  //  where SIGMA is an M-by-N matrix which is zero except for its
  //  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  //  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
  //  are the singular values of A; they are real and non-negative, and
  //  are returned in descending order.  The first min(m,n) columns of
  //  U and V are the left and right singular vectors of A.

  //  Note that the routine returns V**T, not V.

  // computes the work length vector
  lwork = -1;
  dgesvd_("N", "A", &m, &n, &A(0, 0), &lda, &eigenValues(0), U, &ldu, &eigenVectors(0, 0), &ldvt, &worksize, &lwork, &info);
  lwork = (integer)worksize;

  // allocate memory for the work buffer
  work = new double[lwork];

  // compute eigenvalues and eigenvectors
  dgesvd_("N", "A", &m, &n, &A(0, 0), &lda, &eigenValues(0), U, &ldu, &eigenVectors(0, 0), &ldvt, work, &lwork, &info);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getNullSpace",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getNullSpace",
                 "Eigenvalues decomposition incorrect",
                 info);
    }
  }

  delete[] work;
}

//!�valuation valeurs propres et des vecteurs propres � gauche
/*!
  Cette m�thode calcule les valeurs propres et les vecteurs propres � gauche d'une matrice \f$ \textbf{A} \f$. Cette m�thode utilise la routine DGESVD de la librairie Lapack.

  Cette m�thode utilise des copies de vecteurs et matrices donn�s en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel. De plus, les matrices et vecteurs de retour donn�s en argument n'on pas besoin d'�tre dimensionn�s � un taille correcte avant appel (ceci est fait en interne dans la routine).
  \param eigenValues vecteur contenant en retour les valeurs propres de la matrice
  \param eigenVectors Matrice contenant en retour les vecteurs propres de la matrice
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void Matrix::computeLeftEigenVectors(Vector &eigenValues, Matrix &eigenVectors)
//-----------------------------------------------------------------------------
{
  integer n, m, info, lda;
  integer ldu, ldvt, lwork;
  double *work;
  double worksize;
  double *U = NULL;

  n = li;
  m = li;
  lda = li;
  //ldb = li;
  ldu = li;
  ldvt = li;

  // init a matrix
  Matrix A = *this;

  // redim the matrices and vectors
  eigenVectors.redim(n, n);
  eigenValues.redim(n);

  //  DGESVD computes the singular value decomposition (SVD) of a real
  //  M-by-N matrix A, optionally computing the left and/or right singular
  //  vectors. The SVD is written

  //       A = U * SIGMA * transpose(V)

  //  where SIGMA is an M-by-N matrix which is zero except for its
  //  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  //  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
  //  are the singular values of A; they are real and non-negative, and
  //  are returned in descending order.  The first min(m,n) columns of
  //  U and V are the left and right singular vectors of A.

  //  Note that the routine returns V**T, not V.

  // computes the work length vector
  lwork = -1;
  dgesvd_("A", "N", &m, &n, &A(0, 0), &lda, &eigenValues(0), &eigenVectors(0, 0), &ldu, U, &ldvt, &worksize, &lwork, &info);
  lwork = (integer)worksize;

  // allocate memory for the work buffer
  work = new double[lwork];

  // compute eigenvalues and eigenvectors
  dgesvd_("A", "N", &m, &n, &A(0, 0), &lda, &eigenValues(0), &eigenVectors(0, 0), &ldu, U, &ldvt, work, &lwork, &info);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getNullSpace",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getNullSpace",
                 "Eigenvalues decomposition incorrect",
                 info);
    }
  }

  delete[] work;
}

//!�valuation valeurs propres et des vecteurs propres � droite et � gauche
/*!
  Cette m�thode calcule les valeurs propres et les vecteurs propres � droite et � gauche d'une matrice \f$ \textbf{A} \f$. Cette m�thode utilise la routine DGESVD de la librairie Lapack.

  Cette m�thode utilise des copies de vecteurs et matrices donn�s en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel. De plus, les matrices et vecteurs de retour donn�s en argument n'on pas besoin d'�tre dimensionn�s � un taille correcte avant appel (ceci est fait en interne dans la routine).
  \warning la matrice rightEigenVectors contient les vecteurs propres de la matrice sous une forme TRANSPOSEE.
  \param eigenValues vecteur contenant en retour les valeurs propres de la matrice
  \param leftEigenVectors Matrice contenant en retour les vecteurs propres � gauche de la matrice (TRANSPOSES)
  \param rightEigenVectors Matrice contenant en retour les vecteurs propres � droite de la matrice
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void Matrix::computeEigenVectors(Vector &eigenValues, Matrix &leftEigenVectors, Matrix &rightEigenVectors)
//-----------------------------------------------------------------------------
{
  integer n, m, info, lda;
  integer ldu, ldvt, lwork;
  double *work;
  double worksize;
  //  double* U;

  n = li;
  m = li;
  lda = li;
  // ldb = li;
  ldu = li;
  ldvt = li;

  // init a matrix
  Matrix A = *this;

  // redim the matrices and vectors
  leftEigenVectors.redim(n, n);
  rightEigenVectors.redim(n, n);
  eigenValues.redim(n);

  //  DGESVD computes the singular value decomposition (SVD) of a real
  //  M-by-N matrix A, optionally computing the left and/or right singular
  //  vectors. The SVD is written

  //       A = U * SIGMA * transpose(V)

  //  where SIGMA is an M-by-N matrix which is zero except for its
  //  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  //  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
  //  are the singular values of A; they are real and non-negative, and
  //  are returned in descending order.  The first min(m,n) columns of
  //  U and V are the left and right singular vectors of A.

  //  Note that the routine returns V**T, not V.

  // computes the work length vector
  lwork = -1;
  dgesvd_("A", "A", &m, &n, &A(0, 0), &lda, &eigenValues(0), &leftEigenVectors(0, 0), &ldu, &rightEigenVectors(0, 0), &ldvt, &worksize, &lwork, &info);
  lwork = (integer)worksize;

  // allocate memory for the work buffer
  work = new double[lwork];

  // compute eigenvalues and eigenvectors
  dgesvd_("A", "A", &m, &n, &A(0, 0), &lda, &eigenValues(0), &leftEigenVectors(0, 0), &ldu, &rightEigenVectors(0, 0), &ldvt, work, &lwork, &info);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getNullSpace",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getNullSpace",
                 "Eigenvalues decomposition incorrect",
                 info);
    }
  }

  delete[] work;
}

//!�valuation de la pseudo inverse d'une matrice
/*!
  Cette m�thode calcule la pseudoInverse d'une matrice \f$ \textbf{A} \f$. Cette m�thode utilise la routine DGESVD de la librairie Lapack.

  La pseudo inverse d'une matrice, �galement appel�e inverse de Moore-Penrose est g�n�ralement not�e par \f$ A^{+} \f$ 

  Cette m�thode utilise des copies de vecteurs et matrices donn�s en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel. De plus, les matrices et vecteurs de retour donn�s en argument n'on pas besoin d'�tre dimensionn�s � un taille correcte avant appel (ceci est fait en interne dans la routine).
  \warning Pour plus d'efficacit�, cette m�thode est (Me semble-t-il !!!) restreinte au calcul des matrices carr�es sym�triques uniquement. Dans le cas d'une matrice non sym�trique, il conviendra de v�rifier le comportement.
  \param relative Param�tre bool�en indiquant si la tol�rance de recherche est relative ou non. Dans le cas d'une tol�rance relative, la tol�rance de recherche de la plus patite valeur preopres est recalcul�e par rapport � la plus grande valeur propre.
  \param tol tol�rance de recherche du Null-Space par d�faut 1e-10.
  \return pseudo inverse de la matrice donn�e en argument
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
Matrix
Matrix::getPseudoInverse(Boolean relative, Real tol)
//-----------------------------------------------------------------------------
{
  Vector eigenValues;
  Matrix leftEigenVectors;
  Matrix rightEigenVectors;
  Matrix ret(li, co);
  Indice i;

  // compute all eigenvalues end eigenvectors
  computeEigenVectors(eigenValues, leftEigenVectors, rightEigenVectors);

  // on recalcule la tol�rance relative en la multipliant par la plus grande valeur propre
  if (relative)
    tol *= eigenValues(0);

  // calcul des inverses des valeurs propres (si dans la tol�rance)
  for (i = 0; i < eigenValues.size(); i++)
  {
    ret(i, i) = (eigenValues(i) >= tol ? 1. / eigenValues(i) : 0.);
  }

  // compute the pseudo inverse
  ret = (leftEigenVectors * ret * rightEigenVectors);

  // retour
  return ret;
}

//!�valuation de la pseudo inverse d'une matrice et du Null-Space
/*!
  Cette m�thode calcule la pseudoInverse et le Null-Space d'une matrice \f$ \textbf{A} \f$. Cette m�thode utilise la routine DGESVD de la librairie Lapack.

  La pseudo inverse d'une matrice, �galement appel�e inverse de Moore-Penrose est g�n�ralement not�e par \f$ A^{+} \f$.

  Cette m�thode utilise des copies de vecteurs et matrices donn�s en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel. De plus, les matrices et vecteurs de retour donn�s en argument n'on pas besoin d'�tre dimensionn�s � un taille correcte avant appel (ceci est fait en interne dans la routine).
  \warning Pour plus d'efficacit�, cette m�thode est (Me semble-t-il !!!) restreinte au calcul des matrices carr�es sym�triques uniquement. Dans le cas d'une matrice non sym�trique, il conviendra de v�rifier le comportement.
  \param Kplus Matrice de retour pour la pseudo-inverse de la matrice donn�e en argument
  \param NS Matrice de retour pour le Null-Space de la matrice donn�e en argument
  \param relative Param�tre bool�en indiquant si la tol�rance de recherche est relative ou non. Dans le cas d'une tol�rance relative, la tol�rance de recherche de la plus patite valeur preopres est recalcul�e par rapport � la plus grande valeur propre.
  \param tol tol�rance de recherche du Null-Space par d�faut 1e-10.
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void Matrix::computePseudoInverse(Matrix &Kplus, Matrix &NS, Boolean relative, Real tol)
//-----------------------------------------------------------------------------
{
  Vector eigenValues;
  Matrix leftEigenVectors;
  Matrix rightEigenVectors;
  Indice nFloats;
  Indice i;

  // compute all eigenvalues end eigenvectors
  computeEigenVectors(eigenValues, leftEigenVectors, rightEigenVectors);

  // on recalcule la tol�rance relative en la multipliant par la plus grande valeur propre
  if (relative)
    tol *= eigenValues(0);

  // dimension de Kplus
  Kplus.redim(co, li);

  // init de nFloats
  nFloats = 0;

  // calcul des inverses des valeurs propres (si dans la tol�rance)
  for (i = 0; i < eigenValues.size(); i++)
  {
    if (eigenValues(i) > tol)
    {
      Kplus(i, i) = 1. / eigenValues(i);
    }
    else
    {
      Kplus(i, i) = 0.;
      nFloats++;
    }
  }

  // compute the pseudo inverse
  Kplus = (leftEigenVectors * Kplus * rightEigenVectors);

  // dimension de la matrice de Null-Space
  NS.redim(li, nFloats);

  // remplissage du Null-Space
  Indice ic = 0;
  for (Indice i = 0; i < eigenValues.size(); i++)
  {
    if (eigenValues(i) <= tol)
    {
      // copy the last rows
      for (Indice j = 0; j < li; j++)
        NS(j, ic) = rightEigenVectors(i, j);
      ic++;
    }
  }
}

//!Assemblage d'une matrice dans une autre
/*!
  Cette m�thode effectue la phase d'assemblage d'une matrice dans une autre conform�ment � ce qui se fait classiquement en Elements Finis.
  
  \param M Matrice �lementaire � assembler dans la grande matrice
  \param int tableau d'Indice correspondant au nombre de NOEUDS de l'�l�ment et contenant les Id des noeuds que l'on utilise
  \param numberOfDim nombre de dimensions du probl�me
  \code
  Matrix Ke;
  Indice numberOfDim=...; // nombre de dimensions du probl�me
  Indice *ind=new Indice[loop_I]; // allocation m�moire pour ind
  for (Indice I=0; I < loop_I; I++) ind[I]=pel->nodes (I)->Id; // affectation des indices
  K.gatherFrom(Ke, ind, numberOfDim); 
  delete [] ind;
  \endcode
  \author Olivier PANTALE
  \version 1.1.0
*/
//-----------------------------------------------------------------------------
void Matrix::gatherFrom(const Matrix &M, Indice *ind0, int numberOfDim)
//-----------------------------------------------------------------------------
{
  //   Indice mr=M.rows();
  //   Indice mc=M.columns();
  Indice mr = M.li;
  Indice mc = M.co;
  Indice loop_I = mc / numberOfDim;
  Indice loop_J = mr / numberOfDim;
  Indice *ind = new Indice[loop_I];

  // preparation
  Indice *pind = ind;
  Indice *pind0 = ind0;
  for (Indice I = loop_I; I > 0; I--)
    *pind++ = *pind0++ * numberOfDim;

  // boucle principale
  for (Indice I = loop_I - 1; I >= 0; I--)
  {
    for (Indice J = loop_J - 1; J >= 0; J--)
    {
      switch (numberOfDim)
      {
      case 1:
      {
        Real *gl = &v[Ind(ind[I], ind[J], li, co)];
        Real *lo = &M.v[Ind(I * numberOfDim, J * numberOfDim, mr, mc)];
        *gl += *lo;
      }
      break;
      case 2:
      {
        Real *gl = &v[Ind(ind[I], ind[J], li, co)];
        Real *lo = &M.v[Ind(I * numberOfDim, J * numberOfDim, mr, mc)];
        *gl++ += *lo++;
        *gl += *lo;
        gl += (li);
        lo += (mr);
        *gl-- += *lo--;
        *gl += *lo;
      }
      break;
      case 3:
      {
        Real *gl = &v[Ind(ind[I], ind[J], li, co)];
        Real *lo = &M.v[Ind(I * numberOfDim, J * numberOfDim, mr, mc)];
        *gl++ += *lo++;
        *gl++ += *lo++;
        *gl += *lo;
        gl += (li);
        lo += (mr);
        *gl-- += *lo--;
        *gl-- += *lo--;
        *gl += *lo;
        gl += (li);
        lo += (mr);
        *gl++ += *lo++;
        *gl++ += *lo++;
        *gl += *lo;
      }
      break;

        // cas g�n�ral
      default:
      {
        // boucle sur les ddl
        Indice i = I * numberOfDim;
        Indice j = J * numberOfDim;
        Indice globI = ind[I];
        Indice globJ = ind[J];
        for (Indice Ii = 0; Ii < numberOfDim; Ii++)
          for (Indice Jj = 0; Jj < numberOfDim; Jj++)
            (*this)(globI + Ii, globJ + Jj) += M(i + Ii, j + Jj);
      }
      }
    }
  }
  delete[] ind;
}

//-----------------------------------------------------------------------------
void Matrix::scatterFrom(const Matrix &M, Indice *ind0, int numberOfDim)
//-----------------------------------------------------------------------------
{
  //   Indice mr=M.rows();
  //   Indice mc=M.columns();
  Indice mr = li;
  Indice mc = co;
  Indice loop_I = mc / numberOfDim;
  Indice loop_J = mr / numberOfDim;
  Indice *ind = new Indice[loop_I];

  // preparation
  Indice *pind = ind;
  Indice *pind0 = ind0;
  for (Indice I = loop_I; I > 0; I--)
    *pind++ = *pind0++ * numberOfDim;

  // boucle principale
  for (Indice I = loop_I - 1; I >= 0; I--)
  {
    for (Indice J = loop_J - 1; J >= 0; J--)
    {
      switch (numberOfDim)
      {
      case 1:
      {
        Real *lo = &M.v[Ind(ind[I], ind[J], li, co)];
        Real *gl = &v[Ind(I * numberOfDim, J * numberOfDim, mr, mc)];
        *gl += *lo;
      }
      break;
      case 2:
      {
        Real *lo = &M.v[Ind(ind[I], ind[J], li, co)];
        Real *gl = &v[Ind(I * numberOfDim, J * numberOfDim, mr, mc)];
        *gl++ += *lo++;
        *gl += *lo;
        gl += (li);
        lo += (mr);
        *gl-- += *lo--;
        *gl += *lo;
      }
      break;
      case 3:
      {
        Real *lo = &M.v[Ind(ind[I], ind[J], li, co)];
        Real *gl = &v[Ind(I * numberOfDim, J * numberOfDim, mr, mc)];
        *gl++ += *lo++;
        *gl++ += *lo++;
        *gl += *lo;
        gl += (li);
        lo += (mr);
        *gl-- += *lo--;
        *gl-- += *lo--;
        *gl += *lo;
        gl += (li);
        lo += (mr);
        *gl++ += *lo++;
        *gl++ += *lo++;
        *gl += *lo;
      }
      break;

        // cas g�n�ral
      default:
      {
        // boucle sur les ddl
        Indice i = I * numberOfDim;
        Indice j = J * numberOfDim;
        Indice globI = ind[I];
        Indice globJ = ind[J];
        for (Indice Ii = 0; Ii < numberOfDim; Ii++)
          for (Indice Jj = 0; Jj < numberOfDim; Jj++)
            (*this)(i + Ii, j + Jj) += M(globI + Ii, globJ + Jj);
      }
      }
    }
  }
  delete[] ind;
}

//!affichage � l'�cran suivant format pr�d�fini
/*!
  Cette m�thode affiche le contenu de la matrice � l'�cran selon un format pr�d�fini. La s�lection du format d'affichage est faite en utilisant la m�thode \ref setOutType(). Le type de sortie est d�finit par les variable enum�r�es dans \ref OutMatrix.
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void Matrix::printOut()
//-----------------------------------------------------------------------------
{
  switch (outType)
  {
  case outMatrixTypeNormal:
  {
    cout << "matrix " << li << "x" << co << " ={{";
    for (Indice i = 0; i < li; i++)
    {
      for (Indice j = 0; j < co; j++)
      {
        if (j != 0)
          cout << ",";
        cout << (*this)(i, j);
      }
      if (i != li - 1)
        cout << "},{";
    }
    cout << "}}\n";
  }
  break;
  case outMatrixTypeMupad:
  {
    cout << "matrix([[";
    for (Indice i = 0; i < li; i++)
    {
      for (Indice j = 0; j < co; j++)
      {
        if (j != 0)
          cout << ",";
        cout << v[Ind(i, j, li, co)];
      }
      if (i != li - 1)
        cout << "],[";
    }
    cout << "]])\n";
  }
  break;
  default:
    fatalError("Matrices::printOut()", "unknown type %d\n", outType);
  }
}
