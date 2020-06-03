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
  \file Vec3D.C
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

#include <Vec3D.h>

//!Constructor of the Vec3D class
/*!
  This method is the default constructor of the Vec3D class. All components are initialized to zero by default.
*/
//-----------------------------------------------------------------------------
Vec3D::Vec3D()
//-----------------------------------------------------------------------------
{
  // remplissage de 0
  v[0] = 0.;
  v[1] = 0.;
  v[2] = 0.;
}

//!Constructor of the Vec3D class with initialization
/*!
  This method is a  constructor of the Vec3D class. All components are initialized with given values.
  \param x first value of the vector
  \param y second value of the vector
  \param z third value of the vector
*/
//-----------------------------------------------------------------------------
Vec3D::Vec3D(Real x, Real y, Real z)
//-----------------------------------------------------------------------------
{
  v[0] = x;
  v[1] = y;
  v[2] = z;
}

//!Destructor of the Vec3D class
//-----------------------------------------------------------------------------
Vec3D::~Vec3D()
//-----------------------------------------------------------------------------
{
}

//!Addition of 2 vectors
/*!
  This method defines the addition of 2 vectors.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{v} = \overrightarrow{a} + \overrightarrow{b} \f]

  Example :
  \code
  Vec3D t1, t2, t3;
  t3 = t1 + t2; // sum of the vectors
  \endcode
  \param vect Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
Vec3D Vec3D::operator+(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
  Vec3D resu;

  resu.v[0] = v[0] + vect.v[0];
  resu.v[1] = v[1] + vect.v[1];
  resu.v[2] = v[2] + vect.v[2];

  return resu;
}

//!Difference of 2 vectors
/*!
  This method defines the difference of 2 vectors.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{v} = \overrightarrow{a} - \overrightarrow{b} \f]

  Example :
  \code
  Vec3D t1, t2, t3;
  t3 = t1 - t2; // difference of the vectors
  \endcode
  \param vect Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
Vec3D Vec3D::operator-(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
  Vec3D resu;

  resu.v[0] = v[0] - vect.v[0];
  resu.v[1] = v[1] - vect.v[1];
  resu.v[2] = v[2] - vect.v[2];

  return resu;
}

//!Multiplication of a vector by a scalar value
/*!
  This method defines the multiplication of a vector by a scalar value
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{v} = \lambda . \overrightarrow{a} \f]
  
  Example :
  \code
  Vec3D t1, t2;
  Real l;
  t2 = l * t1; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Vec3D operator*(const Real lambda, const Vec3D &vect)
//-----------------------------------------------------------------------------
{
  Vec3D resu;

  resu.v[0] = lambda * vect.v[0];
  resu.v[1] = lambda * vect.v[1];
  resu.v[2] = lambda * vect.v[2];

  return resu;
}

//!Multiplication of a vector by a scalar value
/*!
  This method defines the multiplication of a vector by a scalar value
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{v} = \overrightarrow{a} . \lambda \f]
  
  Example :
  \code
  Vec3D t1, t2;
  Real l;
  t2=t1*l; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Vec3D Vec3D::operator*(const Real lambda) const
//-----------------------------------------------------------------------------
{
  Vec3D resu;

  resu.v[0] = lambda * v[0];
  resu.v[1] = lambda * v[1];
  resu.v[2] = lambda * v[2];

  return resu;
}

//!Division of a vector by a scalar value
/*!
  This method defines the division of a vector by a scalar value
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{v} = \overrightarrow{a} / \lambda \f]
  
  Example :
  \code
  Vec3D t1, t2;
  Real l;
  t1 = t2 / l; // division by a scalar
  \endcode
  \param lambda Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
Vec3D Vec3D::operator/(const Real lambda) const
//-----------------------------------------------------------------------------
{
  Vec3D resu;

#ifdef VERIF_math
  if (lambda == 0.)
  {
    fatalError("Vec3D:: operator /", "divide by zero");
  }
#endif

  resu.v[0] = v[0] / lambda;
  resu.v[1] = v[1] / lambda;
  resu.v[2] = v[2] / lambda;

  return resu;
}

//!Test the equality of two vectors
/*!
  This method tests the equality of two vectors.
  It returns \ref True if all components of the two vectors are equals, \ref False on other case.
  \return \ref True or \ref False depending on the result of the test.
  \param t1 Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
Boolean Vec3D::operator==(const Vec3D &vec) const
//-----------------------------------------------------------------------------
{
  if (v[0] != vec.v[0])
  {
    return False;
  }
  if (v[1] != vec.v[1])
  {
    return False;
  }
  if (v[2] != vec.v[2])
  {
    return False;
  }

  return True;
}

//!Test the inequality of two vectors
/*!
  This method tests the inequality of two vectors.
  It returns \ref True if all components of the two vectors are differents, \ref False on other case.
  \return \ref True or \ref False depending on the result of the test.
  \param t1 Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
Boolean Vec3D::operator!=(const Vec3D &vec) const
//-----------------------------------------------------------------------------
{
  return !(*this == vec);
}

//!Scalar product of two vectors
/*!
  This method computes the scalar product of two vectors defined by:
  \f[ s = \overrightarrow{a} . \overrightarrow{b} \f]
  \param w second vector to use for the operation
  \return Result of the scalar product of the two vectors
*/
//-----------------------------------------------------------------------------
Real Vec3D::dot(const Vec3D &w) const
//-----------------------------------------------------------------------------
{
  return (v[0] * w.v[0] + v[1] * w.v[1] + v[2] * w.v[2]);
}

//!Vectorial product of two vectors
/*!
  This method computes the vectorial product of two vectors defined by:
  \f[ \overrightarrow{v} = \overrightarrow{a} \land \overrightarrow{b} \f]
  \param w second vector to use for the operation
  \return Vector result of the vectorial product of the two vectors
*/
//-----------------------------------------------------------------------------
Vec3D Vec3D::vectorial(const Vec3D &w) const
//-----------------------------------------------------------------------------
{
  Vec3D resu;

  resu.v[0] = v[1] * w.v[2] - v[2] * w.v[1];
  resu.v[1] = v[2] * w.v[0] - v[0] * w.v[2];
  resu.v[2] = v[0] * w.v[1] - v[1] * w.v[0];

  return resu;
}

//!Writes a vector in a binary flux for storage
/*!
  This method is used to store the components of a vector in a binary file.

  Example :
  \code
  ofstream pfile("fichier");
  Vec3D t;
  t.write(pfile);
  t.close();
  \endcode
  \param ofs Output file stream to use for writting operation
*/
//-----------------------------------------------------------------------------
void Vec3D::write(ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)v, 3 * sizeof(Real));
}

//!Reads a vector from a binary flux for storage
/*!
  This method is used to read the components of a vector in a binary file.

  Example :
  \code
  ifstream pfile("fichier");
  Vec3D t;
  t.read(pfile);
  \endcode
  \param ifs Input file stream to use for reading operation
*/
//-----------------------------------------------------------------------------
Vec3D &Vec3D::read(ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)v, 3 * sizeof(Real));
  return *this;
}

/*!
  This method is used to store the components of a vector in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Vec3D t;
  pfile << t;
  \endcode
  \param os Output file stream to use for writting operation
  \param vect Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const Vec3D &vect)
//-----------------------------------------------------------------------------
{
  vect.write(os);
  return os;
}

//!Reads a vector from a binary flux for storage
/*!
  This method is used to read the components of a vector in a binary file.

  Example :
  \code
  ifstream pfile("fichier");
  Vec3D t;
  pfile >> t;
  \endcode
  \param os Input file stream to use for reading operation
  \param vect Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, Vec3D &vect)
//-----------------------------------------------------------------------------
{
  vect.read(is);
  return is;
}

//!Display the content of a Vec3D
/*!
  This method is a surdefintiion of the << operator

  Example
  \code
  Vec3D t;
  cout << t << endl;
  \endcode
  \param os Output flux
*/
//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const Vec3D &vector)
//-----------------------------------------------------------------------------
{
  vector.print(os);
  return os;
}

//!Display the content of a Vec3D
/*!
  This method is a surdefintiion of the << operator

  Example
  \code
  Vec3D t;
  t.print(os);
  \endcode
  \param os Output flux
*/
//-----------------------------------------------------------------------------
void Vec3D::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  Indice i;

  os << "Vec3D coords={";
  os << v[0];
  for (i = 1; i < 3; i++)
  {
    os << "," << v[i];
  }
  os << "}";
}
