/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2006                                                *
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

/*!
  \file Vec3D.h
  \brief Declaration file for the 3D vector class

  This file is the declaration file for the 3D vector class. A 3D vector class is a vector with the following form:
  \f[ \overrightarrow{v}=\left[\begin{array}{c}
  v_{1}\\
  v_{2}\\
  v_{3}
  \end{array}\right] \f]
  This type of data structure is usefull for storing three-dimensional coordinates (for example nodal coordinates, vectors forces,...).

  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __Vec3D_h__
#define __Vec3D_h__

#include <upLinearAlgebra.h>

/*!
  \class Vec3D
  \brief Declaration of the 3D vector class

  A 3D vector class is a vector with the following form:
  \f[ \overrightarrow{v}=\left[\begin{array}{c}
  v_{1}\\
  v_{2}\\
  v_{3}
  \end{array}\right] \f]
  This type of data structure is usefull for storing three-dimensional coordinates (for example nodal coordinates, vectors forces,...).

  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/
class Vec3D
{
  // gives direct access to private data for class Tensor2
  friend class Tensor2;

private:
  Real v[3]; // staticaly allocated vector storage
  inline Boolean indexOK(Indice) const;

public:
  // constructors and destructor
  Vec3D();
  Vec3D(Real x, Real y, Real z);
  //     Vec3D ( const Vec3D & );
  ~Vec3D();

  // components operations
  inline Real &operator()(Indice);
  inline Real operator()(Indice) const;
  inline Indice size() const;
  //     inline const Real *addr () const;

  // operations d'affectation
  inline void fill(Real);
  inline Vec3D &operator=(const Vec3D &);
  inline Vec3D &operator=(Real);
  inline Vec3D &operator=(const Real *);

  // operations de comparaison
  Boolean operator==(const Vec3D &) const;
  Boolean operator!=(const Vec3D &) const;

  // operations arithmetiques de base
  inline void operator+=(const Vec3D &);
  inline void operator-=(const Vec3D &);
  inline void operator*=(const Real);
  inline void operator/=(const Real);
  Vec3D operator+(const Vec3D &) const;
  Vec3D operator-(const Vec3D &) const;
  Vec3D operator*(const Real) const;
  Vec3D operator/(const Real) const;
  friend Vec3D operator*(const Real, const Vec3D &);
  friend Vec3D operator*(const Vec3D &, const Real);

  // operations arithmetiques autres
  inline Real distance(const Vec3D &) const;
  inline Real distance2(const Vec3D &) const;
  Real dot(const Vec3D &) const;
  Vec3D vectorial(const Vec3D &) const;
  inline Real norm();
  inline Real norm2();
  inline void normalize();

  // gestion des flux entree et sortie
  friend ostream &operator<<(ostream &, const Vec3D &);
  friend ofstream &operator<<(ofstream &, const Vec3D &);
  friend ifstream &operator>>(ifstream &, Vec3D &);
  void write(ofstream &) const;
  Vec3D &read(ifstream &);
  void print(ostream &) const;
};

//------inline functions-------------------------------------------------------

//-----------------------------------------------------------------------------
inline Boolean Vec3D::indexOK(Indice i) const
//-----------------------------------------------------------------------------
{
  // test de la condition
  if ((i >= 0) && (i < 3))
  {
    return (True);
  }

  // ici, on a detecte une erreur dans les indices
  printf("Vec3D::indexOK\nindice %ld out of allowed range [0-3]", i);
  exit(-1);

  // idiot, car on ne passera jamais ici a cause du fatalError
  // mais c'est pour eviter une remarque du compilateur
  return (False);
}

//!Access to the values v[i] of a 3D vector
/*!
  \param i indice inside of the vector
  \return Value of the 3D vector v[i]
*/
//-----------------------------------------------------------------------------
inline Real &Vec3D::operator()(Indice i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif
  return v[i];
}

//!Access to the values v[i] of a 3D vector (Read only method)
/*!
  \param i indice inside of the vector
  \return Value of the 3D vector v[i]
*/
//-----------------------------------------------------------------------------
inline Real Vec3D::operator()(Indice i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif
  return v[i];
}

//!Fill a 3D vector with a scalar value
/*!
  This method is a surdefinition of the = operator for the 3D vector class.

  Example :
  \code
  Vec3D v1;
  v1 = fill(1.0); // All components of the vector are set to 1.0
  \endcode
  \param val Real value to give to all components of the 3D vector
*/
//-----------------------------------------------------------------------------
inline void Vec3D::fill(Real val)
//-----------------------------------------------------------------------------
{
  v[0] = v[1] = v[2] = val;
}

//!Size of the 3D vector
/*!
  This method returns the size of the 3D vector (of course here it's always 3).
  \return Size of the 3D vector
*/
//-----------------------------------------------------------------------------
inline Indice Vec3D::size() const
//-----------------------------------------------------------------------------
{
  return 3;
}

// //!renvoie un pointeur sur le tableau de stockage interne
// /*
//   Cette m�thode renvoie un pointeur sur le tableau de stockage des donn�es internes
//   \author Olivier PANTALE
//   \version 0.9.4
// */
// //-----------------------------------------------------------------------------
// inline const double *Vec3D::addr () const
// //-----------------------------------------------------------------------------
// {
//     return v;
// }

//!Copy the content of a 3D vector into a new one
/*!
  This method is the so called = operator between two 3D vectors. If the \ref MEM_funct is set, the \ref memcpy function is used for the copy.

  Example :
  \code
  Vec3D v1, v2;
  v1 = v2; // copy of 3D vector
  \endcode
  \param vec Second 3D vector to use for the operation
*/
//-----------------------------------------------------------------------------
inline Vec3D &Vec3D::operator=(const Vec3D &vec)
//-----------------------------------------------------------------------------
{
#ifdef MEM_funct
  memcpy(v, vec.v, 3 * sizeof(Real));
#else
  v[0] = vec.v[0];
  v[1] = vec.v[1];
  v[2] = vec.v[2];
#endif
  return *this;
}

//!Fill a 3D vector with a scalar value
/*!
  This method is a surdefinition of the = operator for the 3D vector class.

  Example :
  \code
  Vec3D v1;
  v1 = fill(1.0); // All components of the vector are set to 1.0
  \endcode
  \param val Real value to give to all components of the 3D vector
*/
//-----------------------------------------------------------------------------
inline Vec3D &Vec3D::operator=(Real val)
//-----------------------------------------------------------------------------
{
  // recopie des valeurs
  v[0] = v[1] = v[2] = val;

  // retour du vecteur
  return *this;
}

//!Fill a 3D vector with a table of values
/*!
  This method is a surdefinition of the equality operator used to fill a vector with a table of values.

  Exemple :
  \code
  Vec3D v1;
  Real v2[3];
  v1 = v2; // copy the values to the vector
  \endcode
*/
//-----------------------------------------------------------------------------
inline Vec3D &Vec3D::operator=(const Real *vals)
//-----------------------------------------------------------------------------
{
#ifdef MEM_funct
  memcpy(v, vals, 3 * sizeof(Real));
#else
  v[0] = vals[0];
  v[1] = vals[1];
  v[2] = vals[2];
#endif
  return *this;
}

//!Addition of 2 3D vectors
/*!
  This method defines the addition of 2 3D vectors.

  Example :
  \code
  Vec3D t1,t2;
  t2 += t1; // sum of two 3D vectors
  \endcode
  \param vect Second vector to add
*/
//-----------------------------------------------------------------------------
inline void Vec3D::operator+=(const Vec3D &vect)
//-----------------------------------------------------------------------------
{
  v[0] += vect.v[0];
  v[1] += vect.v[1];
  v[2] += vect.v[2];
}

//!Difference of 2 3D vectors
/*!
  This method defines the difference of 2 3D vectors.

  Example :
  \code
  Vec3D t1,t2;
  t2 -= t1; // sum of two 3D vectors
  \endcode
  \param vect Second vector to add
*/
//-----------------------------------------------------------------------------
inline void Vec3D::operator-=(const Vec3D &vect)
//-----------------------------------------------------------------------------
{
  v[0] -= vect.v[0];
  v[1] -= vect.v[1];
  v[2] -= vect.v[2];
}

//!Multiplication of a 3D vector by a scalar value
/*!
  This method defines the multiplication of a 3D vector by a scalar value
  
  Example :
  \code
  Vec3D t1;
  Real l;
  t1 *= l; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
inline void Vec3D::operator*=(const Real lambda)
//-----------------------------------------------------------------------------
{
  v[0] *= lambda;
  v[1] *= lambda;
  v[2] *= lambda;
}

//!Division of a 3D vector by a scalar value
/*!
  This method defines the division of a 3D vector by a scalar value
  
  Example :
  \code
  Tensor2 t1;
  Real l;
  t1 /= l; // division by a scalar
  \endcode
  \param lambda Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
inline void Vec3D::operator/=(const Real lambda)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (lambda == 0.)
  {
    fatalError("Vec3D:: operator /=", "divide by zero");
  }
#endif
  v[0] /= lambda;
  v[1] /= lambda;
  v[2] /= lambda;
}

//!Returns the equivalent norm of a 3D vector
/*!
  This method returns equivalent norm of a 3D vector defined by:
  \f[ \left\Vert \overrightarrow{v} \right\Vert  = \sqrt {v_{1}^2 + v_{2}^2 + v_{3}^2} \f]
  \return equivalent norm of a 3D vector
*/
//-----------------------------------------------------------------------------
inline Real Vec3D::norm()
//-----------------------------------------------------------------------------
{
  return sqrt(SQ(v[0]) + SQ(v[1]) + SQ(v[2]));
}

//!Returns the square of the norm of a 3D vector
/*!
  This method returns square of the norm of a 3D vector defined by:
  \f[ \left\Vert \overrightarrow{v} \right\Vert  = v_{1}^2 + v_{2}^2 + v_{3}^2 \f]
  \return square of the norm of a 3D vector
*/
//-----------------------------------------------------------------------------
inline Real Vec3D::norm2()
//-----------------------------------------------------------------------------
{
  return (SQ(v[0]) + SQ(v[1]) + SQ(v[2]));
}

//!Normalization of a 3D vector
/*!
  This method returns an colinear vector with a unary norm.
  \return colinear vector with a norm equal to 1.
*/
//-----------------------------------------------------------------------------
inline void Vec3D::normalize()
//-----------------------------------------------------------------------------
{
  // calcul de la norme
  Real norme = sqrt(SQ(v[0]) + SQ(v[1]) + SQ(v[2]));

  // recalcul des composantes
  v[0] /= norme;
  v[1] /= norme;
  v[2] /= norme;
}

//!Distance between two points
/*!
  This method computes the distance between two points using an Euclidian norm.
  \param w Second vector to use
  \f[ d = \left\Vert \overrightarrow{v2} - \overrightarrow{v1} \right\Vert \f]
  \return Distance between both points
*/
//-----------------------------------------------------------------------------
inline Real Vec3D::distance(const Vec3D &w) const
//-----------------------------------------------------------------------------
{
  Vec3D x = w - *this;
  return x.norm();
}

//!Square value of the distance between two points
/*!
  This method computes the distance between two points using an Euclidian norm and returns the square value of this distance.
  \param w Second vector to use
  \f[ d = {\left\Vert \overrightarrow{v2} - \overrightarrow{v1} \right\Vert}^2 \f]
  \return Sqare value of the distance between two points
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Real Vec3D::distance2(const Vec3D &w) const
//-----------------------------------------------------------------------------
{
  Vec3D x = w - *this;
  return x.norm2();
}

#endif
