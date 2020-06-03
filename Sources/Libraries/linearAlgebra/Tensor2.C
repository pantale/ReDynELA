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
  \file Tensor2.C
  \brief Definition file for the second order tensor class

  This file is the declaration file for the second order tensor class. A second order tensor is a like a matrix with the following form:
  \f[ T=\left[\begin{array}{ccc}
  T_{11} & T_{12} & T_{13}\\
  T_{21} & T_{22} & T_{23}\\
  T_{31} & T_{32} & T_{33}
  \end{array}\right] \f]
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#include <Tensor2.h>
#define precisionPolarDecompose 10.0E-10

//!Constructor of the Tensor2 class
/*!
  This method is the default constructor of a second order tensor. All components are initialized to zero by default.
*/
//-----------------------------------------------------------------------------
Tensor2::Tensor2()
//-----------------------------------------------------------------------------
{
  // initialisation
  fill(0.);
}

//!Destructor of the Tensor2 class
//-----------------------------------------------------------------------------
Tensor2::~Tensor2()
//-----------------------------------------------------------------------------
{
}

//!Send the content of a second order tensor to the output flux for display
/*!
  Example
  \code
  Tensor2 t;
  cout << t << endl;
  \endcode
  \param os Output flux
*/
//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const Tensor2 &t1)
//-----------------------------------------------------------------------------
{
  t1.print(os);
  return os;
}

//!Print the content of a second order tensor to the output flux for display
/*!
  \param os Output flux
*/
//-----------------------------------------------------------------------------
void Tensor2::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  Indice i, j;
  os << "tensor 3x3 ={{";
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      if (j != 0)
      {
        os << ",";
      }
      os << v[tensorInd2(i, j, 3)];
    }
    if (i != 2)
    {
      os << "},{";
    }
  }
  os << "}}";
}

//!Returns an identity tensor
/*!
  This method transforms the current tensor to an identity tensor.

  Example :
  \code
  Tensor2 t1;
  t1.Identity(); // Returns and identity tensor
  \endcode
  \warning This method modify it's argument
*/
//-----------------------------------------------------------------------------
void Tensor2::Identity()
//-----------------------------------------------------------------------------
{
  v[0] = 1.;
  v[1] = 0.;
  v[2] = 0.;
  v[3] = 0.;
  v[4] = 1.;
  v[5] = 0.;
  v[6] = 0.;
  v[7] = 0.;
  v[8] = 1.;
}

//!Fill a second order tensor with a scalar value
/*!
  This method is a surdefinition of the = operator for the second order tensor class.

  Example :
  \code
  Tensor2 t1;
  t1 = 1.0; // All components of the tensor are set to 1.0
  \endcode
  \param val Real value to give to all components of the second order tensor
*/
//-----------------------------------------------------------------------------
Tensor2 &Tensor2::operator=(const Real &val)
//-----------------------------------------------------------------------------
{
  fill(val);
  return *this;
}

//!Copy the content of a second order tensor into a new one
/*!
  This method is the so called = operator between two second order tensors. If the \ref MEM_funct is set, the \ref memcpy function is used for the copy.

  Example :
  \code
  Tensor2 t1, t2;
  t1 = t2; // copy of the tensor
  \endcode
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 &Tensor2::operator=(const Tensor2 &t1)
//-----------------------------------------------------------------------------
{
#ifdef MEM_funct
  memcpy(v, t1.v, 9 * sizeof(Real));
#else
  v[0] = t1.v[0];
  v[1] = t1.v[1];
  v[2] = t1.v[2];
  v[3] = t1.v[3];
  v[4] = t1.v[4];
  v[5] = t1.v[5];
  v[6] = t1.v[6];
  v[7] = t1.v[7];
  v[8] = t1.v[8];
#endif
  return *this;
}

//!Addition of 2 second order tensors
/*!
  This method defines the addition of 2 second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A + B \f]

  Example :
  \code
  Tensor2 t1,t2,t3;
  t3 = t1 + t2; // sum of 2 second order tensors
  \endcode
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator+(const Tensor2 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor2 t2;

  // calcul de la somme
  t2.v[0] = v[0] + t1.v[0];
  t2.v[1] = v[1] + t1.v[1];
  t2.v[2] = v[2] + t1.v[2];
  t2.v[3] = v[3] + t1.v[3];
  t2.v[4] = v[4] + t1.v[4];
  t2.v[5] = v[5] + t1.v[5];
  t2.v[6] = v[6] + t1.v[6];
  t2.v[7] = v[7] + t1.v[7];
  t2.v[8] = v[8] + t1.v[8];

  // renvoi de l'objet
  return t2;
}

//!Difference of 2 second order tensors
/*!
  This method defines the difference of 2 second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A - B \f]

  Example :
  \code
  Tensor2 t1,t2,t3;
  t3 = t1 - t2; // difference of 2 second order tensors
  \endcode
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator-(const Tensor2 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor2 t2;

  // calcul de la somme
  t2.v[0] = v[0] - t1.v[0];
  t2.v[1] = v[1] - t1.v[1];
  t2.v[2] = v[2] - t1.v[2];
  t2.v[3] = v[3] - t1.v[3];
  t2.v[4] = v[4] - t1.v[4];
  t2.v[5] = v[5] - t1.v[5];
  t2.v[6] = v[6] - t1.v[6];
  t2.v[7] = v[7] - t1.v[7];
  t2.v[8] = v[8] - t1.v[8];

  // renvoi de l'objet
  return t2;
}

//!Multiplication of a second order tensor by a scalar value
/*!
  This method defines the multiplication of a second order tensor by a scalar value
  The result of this operation is also a second order tensor defined by:
  \f[ T = A . \lambda \f]
  
  Example :
  \code
  Tensor2 t1,t2;
  Real l;
  t2 = t1 * l; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator*(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor2 t2;

  t2.v[0] = lambda * v[0];
  t2.v[1] = lambda * v[1];
  t2.v[2] = lambda * v[2];
  t2.v[3] = lambda * v[3];
  t2.v[4] = lambda * v[4];
  t2.v[5] = lambda * v[5];
  t2.v[6] = lambda * v[6];
  t2.v[7] = lambda * v[7];
  t2.v[8] = lambda * v[8];

  return t2;
}

//!Division of a second order tensor by a scalar value
/*!
  This method defines the division of a second order tensor by a scalar value
  The result of this operation is also a second order tensor defined by:
  \f[ T = \frac{A}{\lambda} \f]
  
  Example :
  \code
  Tensor2 t1,t2;
  Real l;
  t2 = t1 / l; // division by a scalar
  \endcode
  \warning This is not a commutative operation, be also warn not to divide by zero.
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator/(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor2 t2;

  t2.v[0] = v[0] / lambda;
  t2.v[1] = v[1] / lambda;
  t2.v[2] = v[2] / lambda;
  t2.v[3] = v[3] / lambda;
  t2.v[4] = v[4] / lambda;
  t2.v[5] = v[5] / lambda;
  t2.v[6] = v[6] / lambda;
  t2.v[7] = v[7] / lambda;
  t2.v[8] = v[8] / lambda;

  return t2;
}

//!Multiplication of a second order tensor by a scalar value
/*!
  This method defines the multiplication of a second order tensor by a scalar value
  The result of this operation is also a second order tensor defined by:
  \f[ T = \lambda . A \f]
  
  Example :
  \code
  Tensor2 t1,t2;
  Real l;
  t2 = l * t1; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 operator*(const Real &lambda, const Tensor2 &t1)
//-----------------------------------------------------------------------------
{
  Tensor2 t2;

  t2.v[0] = lambda * t1.v[0];
  t2.v[1] = lambda * t1.v[1];
  t2.v[2] = lambda * t1.v[2];
  t2.v[3] = lambda * t1.v[3];
  t2.v[4] = lambda * t1.v[4];
  t2.v[5] = lambda * t1.v[5];
  t2.v[6] = lambda * t1.v[6];
  t2.v[7] = lambda * t1.v[7];
  t2.v[8] = lambda * t1.v[8];

  return t2;
}

//!Multiplication of 2 second order tensors
/*!
  This method defines a single contracted product of two second order tensors. 
  The result of this operation is also a second order tensor defined by:
  \f[ T = A . B \f]
  Example :
  \code
  Tensor2 t1,t2,t3;
  t3 = t1 * t2; // product of two tensors
  \endcode
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator*(const Tensor2 &t1) const
//-----------------------------------------------------------------------------
{
  Tensor2 t2;

  t2.v[0] = v[0] * t1.v[0] + v[1] * t1.v[3] + v[2] * t1.v[6];
  t2.v[1] = v[0] * t1.v[1] + v[1] * t1.v[4] + v[2] * t1.v[7];
  t2.v[2] = v[0] * t1.v[2] + v[1] * t1.v[5] + v[2] * t1.v[8];
  t2.v[3] = v[3] * t1.v[0] + v[4] * t1.v[3] + v[5] * t1.v[6];
  t2.v[4] = v[3] * t1.v[1] + v[4] * t1.v[4] + v[5] * t1.v[7];
  t2.v[5] = v[3] * t1.v[2] + v[4] * t1.v[5] + v[5] * t1.v[8];
  t2.v[6] = v[6] * t1.v[0] + v[7] * t1.v[3] + v[8] * t1.v[6];
  t2.v[7] = v[6] * t1.v[1] + v[7] * t1.v[4] + v[8] * t1.v[7];
  t2.v[8] = v[6] * t1.v[2] + v[7] * t1.v[5] + v[8] * t1.v[8];

  return t2;
}

//!Multiplication of a second order tensor by a vector
/*!
  This method defines the product of a second order tensor by a vector.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{b} = T . \overrightarrow{a} \f]

  Example :
  \code
  Tensor2 t1;
  Vec3D v1,v2;
  v2 = t1 * v1; // product of the second order tensor by a vector
  \endcode
  \param v1 Vector to use for the operation
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::operator*(const Vec3D &v1) const
//-----------------------------------------------------------------------------
{
  Vec3D v2;

  v2.v[0] = v[0] * v1.v[0] + v[1] * v1.v[1] + v[2] * v1.v[2];
  v2.v[1] = v[3] * v1.v[0] + v[4] * v1.v[1] + v[5] * v1.v[2];
  v2.v[2] = v[6] * v1.v[0] + v[7] * v1.v[1] + v[8] * v1.v[2];

  return v2;
}

//!Dyadic product of two vectors
/*!
  This method defines the dyadic product of 2 vectors that gives a second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ T = \overrightarrow{a} \otimes \overrightarrow{b} \f]

  Exemple :
  \code
  Tensor2 t1;
  Vec3D v1,v2;
  t1.extProd(v1,v2); // dyadic product
  \endcode
  \param v1 First vector to use for the operation
  \param v1 Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::extProd(const Vec3D v1, const Vec3D v2)
//-----------------------------------------------------------------------------
{
  Tensor2 t;

  t.v[0] = v1.v[0] * v2.v[0];
  t.v[1] = v1.v[0] * v2.v[1];
  t.v[2] = v1.v[0] * v2.v[2];
  t.v[3] = v1.v[1] * v2.v[0];
  t.v[4] = v1.v[1] * v2.v[1];
  t.v[5] = v1.v[1] * v2.v[2];
  t.v[6] = v1.v[2] * v2.v[0];
  t.v[7] = v1.v[2] * v2.v[1];
  t.v[8] = v1.v[2] * v2.v[2];

  return t;
}

//!Multiplication of 2 second order tensors
/*!
  This method defines a single contracted product of two second order tensors. 
  The result of this operation is also a second order tensor defined by:
  \f[ T = A . B \f]
  Example :
  \code
  Tensor2 t1,t2,t3;
  t3 = t1.prod1(t2); // product of two tensors
  \endcode
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::prod1(const Tensor2 t1)
//-----------------------------------------------------------------------------
{
  Tensor2 t2;

  t2.v[0] = v[0] * t1.v[0] + v[1] * t1.v[3] + v[2] * t1.v[6];
  t2.v[1] = v[0] * t1.v[1] + v[1] * t1.v[4] + v[2] * t1.v[7];
  t2.v[2] = v[0] * t1.v[2] + v[1] * t1.v[5] + v[2] * t1.v[8];
  t2.v[3] = v[3] * t1.v[0] + v[4] * t1.v[3] + v[5] * t1.v[6];
  t2.v[4] = v[3] * t1.v[1] + v[4] * t1.v[4] + v[5] * t1.v[7];
  t2.v[5] = v[3] * t1.v[2] + v[4] * t1.v[5] + v[5] * t1.v[8];
  t2.v[6] = v[6] * t1.v[0] + v[7] * t1.v[3] + v[8] * t1.v[6];
  t2.v[7] = v[6] * t1.v[1] + v[7] * t1.v[4] + v[8] * t1.v[7];
  t2.v[8] = v[6] * t1.v[2] + v[7] * t1.v[5] + v[8] * t1.v[8];

  return t2;
}

//!Norm of a second order tensor
/*!
  This method returns the norm of a second order tensor defined by:
  \f[ \left\Vert s \right\Vert  = \sqrt {s_{ij}:s_{ij}} \f]

  Example :
  \code
  Tensor2 t1;
  Real s;
  s = t1.norme(); // norme du tenseur
  \endcode
  \return equivalent norm of a second order tensor
*/
//-----------------------------------------------------------------------------
Real Tensor2::norme(void)
//-----------------------------------------------------------------------------
{
  return sqrt(SQ(v[0]) + SQ(v[1]) + SQ(v[2]) +
              SQ(v[3]) + SQ(v[4]) + SQ(v[5]) +
              SQ(v[6]) + SQ(v[7]) + SQ(v[8]));
}

//!J2 norm of a second order tensor
/*!
  This method returns the J2 norm of a second order tensor defined by:
  \f[ n = \sqrt {\frac{3}{2}} \left\Vert s \right\Vert  = \sqrt {\frac{3}{2} s_{ij}:s_{ij}} \f]

  Example :
  \code
  Tensor2 t1;
  Real s;
  s = t1.J2(); // J2 norm of the tensor
  \endcode
*/
//-----------------------------------------------------------------------------
Real Tensor2::J2(void)
//-----------------------------------------------------------------------------
{
  return Sqrt32 * sqrt(SQ(v[0]) + SQ(v[1]) + SQ(v[2]) +
                       SQ(v[3]) + SQ(v[4]) + SQ(v[5]) +
                       SQ(v[6]) + SQ(v[7]) + SQ(v[8]));
}

//!Double contracted product of 2 second order tensors
/*!
  This method defines a double contracted product of two second order tensors. 
  The result of this operation is a scalar defined by:
  \f[ s = A : B \f]
  Example :
  \code
  Tensor2 t1,t2;
  Real s;
  s = t1.prod2(t2); // double contracted product
  \endcode
  \param t2 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Real Tensor2::prod2(const Tensor2 t2)
//-----------------------------------------------------------------------------
{
  return (v[0] * t2.v[0] + v[1] * t2.v[1] + v[2] * t2.v[2] +
          v[3] * t2.v[3] + v[4] * t2.v[4] + v[5] * t2.v[5] +
          v[6] * t2.v[6] + v[7] * t2.v[7] + v[8] * t2.v[8]);
}

//!Deviator of a second order tensor
/*!
  This method defines the deviator of a second second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ s=\sigma-\frac{1}{3}\tr[\sigma].I \f]
  \return Deviator of a second order tensor
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::deviator()
//-----------------------------------------------------------------------------
{
  Tensor2 t2;
  Real tr = moyTrace();

  t2.v[0] = v[0] - tr;
  t2.v[1] = v[1];
  t2.v[2] = v[2];
  t2.v[3] = v[3];
  t2.v[4] = v[4] - tr;
  t2.v[5] = v[5];
  t2.v[6] = v[6];
  t2.v[7] = v[7];
  t2.v[8] = v[8] - tr;

  return t2;
}

//!Transpose of a second order tensor
/*!
  This method defines the transpose of a second second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ T_2 = T_1^T \f]
  \return Transpose of a second order tensor
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::Transpose() const
//-----------------------------------------------------------------------------
{
  Tensor2 t2;

  t2.v[0] = v[0];
  t2.v[1] = v[3];
  t2.v[2] = v[6];
  t2.v[3] = v[1];
  t2.v[4] = v[4];
  t2.v[5] = v[7];
  t2.v[6] = v[2];
  t2.v[7] = v[5];
  t2.v[8] = v[8];

  return t2;
}

//!Sum of the rows of a second order tensor
/*!
  This method returns a vector by computing the sum of the components on all rows of a second second order tensor.
  The result of this operation is a vector defined by:
  \f[ v_{i}=\sum_{j=1}^{3}T_{ji} \f]
  \return Vector containing the sum of the rows of a second order tensor
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::rowSum()
//-----------------------------------------------------------------------------
{
  Vec3D v2;

  v2.v[0] = v[0] + v[1] + v[2];
  v2.v[1] = v[3] + v[4] + v[5];
  v2.v[2] = v[6] + v[7] + v[8];

  return v2;
}

//!Sum of the columns of a second order tensor
/*!
  This method returns a vector by computing the sum of the components on all columns of a second second order tensor.
  The result of this operation is a vector defined by:
  \f[ v_{i}=\sum_{j=1}^{3}T_{ij} \f]
  \return Vector containing the sum of the rows of a second order tensor
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::colSum()
//-----------------------------------------------------------------------------
{
  Vec3D v2;

  v2.v[0] = v[0] + v[3] + v[6];
  v2.v[1] = v[1] + v[4] + v[7];
  v2.v[2] = v[2] + v[5] + v[8];

  return v2;
}

//!Symmetric part of a second order tensor
/*!
  This method returns the symmetric part of a second second order tensor.
  The result of this operation is a second second order tensor defined by:
  \f[
  s = \left[\begin{array}{ccc}
 v_{11} & \frac{v{12} + v{21}}{2} & \frac{v{13} + v{31}}{2}\\
 \frac{v{12} + v{21}}{2} & v_{22} & \frac {v{23} + v{32}}{2}\\
 \frac{v{13} + v{31}}{2} & \frac {v{23} + v{32}}{2} & v_{33}\end{array}
\right]
  \f]
  \param s_v Returned symmetric part of a second order tensor
*/
//-----------------------------------------------------------------------------
void Tensor2::getSymetricPart(Tensor2 &s_v) const
//-----------------------------------------------------------------------------
{
  s_v.v[0] = v[0];
  s_v.v[4] = v[4];
  s_v.v[8] = v[8];
  s_v.v[1] = (v[1] + v[3]) / 2.;
  s_v.v[2] = (v[2] + v[6]) / 2.;
  s_v.v[5] = (v[5] + v[7]) / 2.;
  s_v.v[3] = s_v.v[1];
  s_v.v[6] = s_v.v[2];
  s_v.v[7] = s_v.v[5];
}

//!Skew-symmetric part of a second order tensor
/*!
  This method returns the skew-symmetric part of a second second order tensor.
  The result of this operation is a second second order tensor defined by:
  \f[
  s = \left[\begin{array}{ccc}
 v_{11} & \frac{v{12} - v{21}}{2} & \frac{v{13} - v{31}}{2}\\
 -\frac{v{12} -  v{21}}{2} & v_{22} & \frac {v{23} - v{32}}{2}\\
 -\frac{v{13} - v{31}}{2} & -\frac {v{23} - v{32}}{2} & v_{33}\end{array}
\right]
  \f]
  \param a_v Returned skew-symmetric part of a second order tensor
*/
//-----------------------------------------------------------------------------
void Tensor2::getAntiSymetricPart(Tensor2 &a_v) const
//-----------------------------------------------------------------------------
{
  a_v.v[0] = 0.;
  a_v.v[4] = 0.;
  a_v.v[8] = 0.;
  a_v.v[1] = (v[1] - v[3]) / 2.;
  a_v.v[2] = (v[2] - v[6]) / 2.;
  a_v.v[5] = (v[5] - v[7]) / 2.;
  a_v.v[3] = -a_v.v[1];
  a_v.v[6] = -a_v.v[2];
  a_v.v[7] = -a_v.v[5];
}

//!Extraction of a row from a second order tensor
/*!
  This method returns a vector as part of a second second order tensor.
  The result of this operation with the argument j is a vector defined by:
  \f[ v_{i} = T_{ij} \f]
  \param row row line number to extract.
  \return vecteur containing the corresponding row
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::getRow(Indice row)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(row, 0);
#endif

  Vec3D t2;

  for (Indice i = 0; i < 3; i++)
  {
    t2(i) = v[tensorInd2(row, i, 3)];
  }

  return t2;
}

//!Extraction of a column from a second order tensor
/*!
  This method returns a vector as part of a second second order tensor.
  The result of this operation with the argument j is a vector defined by:
  \f[ v_{i} = T_{ji} \f]
  \param col col line number to extract.
  \return vecteur containing the corresponding column
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::getCol(Indice col)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(0, col);
#endif

  Vec3D t2;

  for (Indice i = 0; i < 3; i++)
  {
    t2(i) = v[tensorInd2(i, col, 3)];
  }

  return t2;
}

//!Test the equality of two second order tensors
/*!
  This method tests the equality of two second order tensors.
  It returns \ref True if all components of the two second order tensors are equals, \ref False on other case.
  \return \ref True or \ref False depending on the result of the test.
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Boolean Tensor2::operator==(const Tensor2 &t1) const
//-----------------------------------------------------------------------------
{
  Indice i;

  for (i = 0; i < 9; i++)
    if (v[i] != t1.v[i])
    {
      return False;
    }
  return True;
}

//!Test the equality of two second order tensors
/*!
  This method tests the equality of two second order tensors.
  It returns \ref False if all components of the two second order tensors are equals, \ref True on other case.
  \return \ref True or \ref False depending on the result of the test.
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Boolean Tensor2::operator!=(const Tensor2 &t1) const
//-----------------------------------------------------------------------------
{
  return !(*this == t1);
}

//!Writes a second order tensor in a binary flux for storage
/*!
  This method is used to store the components of a second order tensor in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Tensor2 t;
  t.write(pfile);
  \endcode
  \param ofs Output file stream to use for writting operation
*/
//-----------------------------------------------------------------------------
void Tensor2::write(ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)v, 9 * sizeof(Real));
}

//!Reads a second order tensor in a binary flux from storage
/*!
  This method is used to read the components of a second order tensor in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Tensor2 t;
  t.read(pfile);
  \endcode
  \param ofs Input file stream to use for reading operation
*/
//-----------------------------------------------------------------------------
void Tensor2::read(ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)v, 9 * sizeof(Real));
}

//!Writes a second order tensor in a binary flux for storage
/*!
  This method is used to store the components of a second order tensor in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Tensor2 t;
  pfile << t;
  \endcode
  \param os Output file stream to use for writting operation
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const Tensor2 &t1)
//-----------------------------------------------------------------------------
{
  t1.write(os);
  return os;
}

//!Reads a second order tensor from a binary flux for storage
/*!
  This method is used to read the components of a second order tensor in a binary file.

  Example :
  \code
  ifstream pfile("fichier");
  Tensor2 t;
  pfile >> t;
  \endcode
  \param os Input file stream to use for reading operation
  \param t1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, Tensor2 &t1)
//-----------------------------------------------------------------------------
{
  t1.read(is);
  return is;
}

//!Determinant of a second order tensor
/*!
  This method returns the determinant of a second second order tensor.
  The result of this operation is a scalar value defined by:
  \f[ D = T_{11} T_{22} T_{33} + T_{21} T_{32} T_{13} + T_{31} T_{12} T_{23} - T_{31} T_{22} T_{13} - T_{11} T_{32} T_{23} - T_{21} T_{12} T_{33} \f]
  \return Scalar value containing the determinant of a second order tensor
*/
//-----------------------------------------------------------------------------
Real Tensor2::det() const
//-----------------------------------------------------------------------------
{
  return (v[0] * v[4] * v[8] + v[3] * v[7] * v[2] + v[6] * v[1] * v[5] - v[6] * v[4] * v[2] - v[0] * v[7] * v[5] - v[3] * v[1] * v[8]);
}

//!Inverse of a second order tensor
/*!
  This method returns the inverse of a second second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ D = T_{11} T_{22} T_{33} + T_{21} T_{32} T_{13} + T_{31} T_{12} T_{23} - T_{31} T_{22} T_{13} - T_{11} T_{32} T_{23} - T_{21} T_{12} T_{33} \f]
  and
  \f[ T^{-1} = \frac {1}{D} \left[\begin{array}{ccc}
  -(T{23} * T{32}) + T{22} * T{33} & T{13} * T{32} - T{12} * T{33} & -(T{13} * T{22}) + T{12} * T{23}\\
  T{23} * T{31} - T{21} * T{33}& -(T{13} * T{31}) + T{11} * T{33} & T{13} * T{21} - T{11} * T{23}\\
  -(T{22} * T{31}) + T{21} * T{32}&T{12} * T{31} - T{11} * T{32} & -(T{12} * T{21}) + T{11} * T{22}\end{array}
  \right] 
  \f]
  \return Tensor containing the inverse
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::inverse()
//-----------------------------------------------------------------------------
{
  Tensor2 inv;

  Real unSurDeter =
      1.0 / (v[0] * v[4] * v[8] + v[3] * v[7] * v[2] + v[6] * v[1] * v[5] -
             v[6] * v[4] * v[2] - v[0] * v[7] * v[5] - v[3] * v[1] * v[8]);

  inv.v[0] = (-(v[5] * v[7]) + v[4] * v[8]) * unSurDeter;
  inv.v[1] = (v[2] * v[7] - v[1] * v[8]) * unSurDeter;
  inv.v[2] = (-(v[2] * v[4]) + v[1] * v[5]) * unSurDeter;
  inv.v[3] = (v[5] * v[6] - v[3] * v[8]) * unSurDeter;
  inv.v[4] = (-(v[2] * v[6]) + v[0] * v[8]) * unSurDeter;
  inv.v[5] = (v[2] * v[3] - v[0] * v[5]) * unSurDeter;
  inv.v[6] = (-(v[4] * v[6]) + v[3] * v[7]) * unSurDeter;
  inv.v[7] = (v[1] * v[6] - v[0] * v[7]) * unSurDeter;
  inv.v[8] = (-(v[1] * v[3]) + v[0] * v[4]) * unSurDeter;

  return inv;
}

// //-----------------------------------------------------------------------------
// Tensor2
// Tensor2::ln ()
// //-----------------------------------------------------------------------------
// {
//     fatalError ( "Tensor2::ln()", "exit(-1);" );
//     Tensor2 rep;
//
//     rep.v[0] = log ( v[0] );
//     rep.v[1] = log ( v[1] );
//     rep.v[2] = log ( v[2] );
//     rep.v[3] = log ( v[3] );
//     rep.v[4] = log ( v[4] );
//     rep.v[5] = log ( v[5] );
//     rep.v[6] = log ( v[6] );
//     rep.v[7] = log ( v[7] );
//     rep.v[8] = log ( v[8] );
//
//     return rep;
// }

// //-----------------------------------------------------------------------------
// Tensor2
// Tensor2::polarDecomposeU ()
// //-----------------------------------------------------------------------------
// {
// //  fatalError("Tensor2::polarDecomposeU()","exit(-1);");
//     Indice p, q;
//     Real alpha, t, Ctheta, Stheta;
//     Tensor2 R, Rot, U, FF;
//     Indice it = 1;
//     Vec3D vec;
//
//     // calcul de FF=F(T).F
//     FF = ( *this ).Transpose () * ( *this );
//
//     // initialisation de Rot
//     Rot.Identity ();
//
//     while ( it < 30 ) {
//         // recherche du terme maxi
//         q = ( Abs ( FF.v[1] ) > Abs ( FF.v[2] ) ? 1 : 2 );
//         p = ( Abs ( FF.v[q] ) > Abs ( FF.v[5] ) ? 0 : 1 );
//         if ( p == 1 ) {
//             q = 2;
//         }
//
//         // test de convergence
//         if ( Abs ( FF.v[tensorInd2 ( p, q, 3 )] ) < precisionPolarDecompose ) {
//             vec ( 0 ) = Rot.v[0];
//             vec ( 1 ) = Rot.v[3];
//             vec ( 2 ) = Rot.v[6];
//             U = sqrt ( FF.v[0] ) * extProd ( vec, vec );
//             vec ( 0 ) = Rot.v[1];
//             vec ( 1 ) = Rot.v[4];
//             vec ( 2 ) = Rot.v[7];
//             U += sqrt ( FF.v[4] ) * extProd ( vec, vec );
//             vec ( 0 ) = Rot.v[2];
//             vec ( 1 ) = Rot.v[5];
//             vec ( 2 ) = Rot.v[8];
//             U += sqrt ( FF.v[8] ) * extProd ( vec, vec );
//             return U;
//         }
//
//         // calcul de l'angle
//         alpha =
//             ( FF.v[tensorInd2 ( q, q, 3 )] -
//               FF.v[tensorInd2 ( p, p, 3 )] ) / ( 2. * FF.v[tensorInd2 ( p, q, 3 )] );
//
//         // resolution
//         t = ( alpha != 0. ? 1. / ( alpha + sqrt ( alpha * alpha + 1. ) * ( alpha > 0. ? 1. : -1. ) ) : 1. );
//
//         // calcul des cosinus
//         Ctheta = 1. / ( sqrt ( 1. + t * t ) );
//         Stheta = t * Ctheta;
//
//         // calcul du nouveau tenseur
//         R.Identity ();
//         R ( p, q ) = Stheta;
//         R ( q, p ) = -Stheta;
//         R ( p, p ) = R ( q, q ) = Ctheta;
//
//         // appel aux multiplications de tenseurs
//         //*this=(R.Transpose())*(*this)*(R);
//
//         // produit R(T).FF.R avec FF sym�trique
//         U.v[0] =
//             R.v[0] * ( FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6] ) +
//             R.v[3] * ( FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6] ) +
//             R.v[6] * ( FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6] );
//         U.v[1] =
//             R.v[1] * ( FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6] ) +
//             R.v[4] * ( FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6] ) +
//             ( FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6] ) * R.v[7];
//         U.v[2] =
//             R.v[2] * ( FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6] ) +
//             R.v[5] * ( FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6] ) +
//             ( FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6] ) * R.v[8];
//         U.v[4] =
//             R.v[1] * ( FF.v[0] * R.v[1] + FF.v[3] * R.v[4] + FF.v[6] * R.v[7] ) +
//             R.v[4] * ( FF.v[1] * R.v[1] + FF.v[4] * R.v[4] + FF.v[7] * R.v[7] ) +
//             R.v[7] * ( FF.v[2] * R.v[1] + FF.v[5] * R.v[4] + FF.v[8] * R.v[7] );
//         U.v[5] =
//             R.v[2] * ( FF.v[0] * R.v[1] + FF.v[3] * R.v[4] + FF.v[6] * R.v[7] ) +
//             R.v[5] * ( FF.v[1] * R.v[1] + FF.v[4] * R.v[4] + FF.v[7] * R.v[7] ) +
//             ( FF.v[2] * R.v[1] + FF.v[5] * R.v[4] + FF.v[8] * R.v[7] ) * R.v[8];
//         U.v[8] =
//             R.v[2] * ( FF.v[0] * R.v[2] + FF.v[3] * R.v[5] + FF.v[6] * R.v[8] ) +
//             R.v[5] * ( FF.v[1] * R.v[2] + FF.v[4] * R.v[5] + FF.v[7] * R.v[8] ) +
//             R.v[8] * ( FF.v[2] * R.v[2] + FF.v[5] * R.v[5] + FF.v[8] * R.v[8] );
//
//         // recopie
//         FF.v[0] = U.v[0];
//         FF.v[1] = U.v[1];
//         FF.v[2] = U.v[2];
//         FF.v[3] = U.v[1];
//         FF.v[4] = U.v[4];
//         FF.v[5] = U.v[5];
//         FF.v[6] = U.v[2];
//         FF.v[7] = U.v[5];
//         FF.v[8] = U.v[8];
//
//         // cumul de R
//         Rot = Rot * R;
//         it++;
//     }
//
//     cout << ( *this ) << endl;
//     fatalError ( "Eigen Value Computation", "No convergence" );
//     return U;
// }
//
// //-----------------------------------------------------------------------------
// Tensor2
// Tensor2::polarDecomposeLnU ()
// //-----------------------------------------------------------------------------
// {
//     Indice p, q;
//     Real alpha, t, Ctheta, Stheta;
//     Tensor2 R, Rot, U, FF;
//     Indice it = 1;
//     Vec3D vec;
//
//     // calcul de FF=F(T).F
//     FF = ( *this ).Transpose () * ( *this );
//
//     // initialisation de Rot
//     Rot.Identity ();
//
//     while ( it < 30 ) {
//         // recherche du terme maxi
//         q = ( Abs ( FF.v[1] ) > Abs ( FF.v[2] ) ? 1 : 2 );
//         p = ( Abs ( FF.v[q] ) > Abs ( FF.v[5] ) ? 0 : 1 );
//         if ( p == 1 ) {
//             q = 2;
//         }
//
//         // test de convergence
//         if ( Abs ( FF.v[tensorInd2 ( p, q, 3 )] ) < precisionPolarDecompose ) {
//             vec ( 0 ) = Rot.v[0];
//             vec ( 1 ) = Rot.v[3];
//             vec ( 2 ) = Rot.v[6];
//             U = 0.5 * log ( FF.v[0] ) * extProd ( vec, vec );
//             vec ( 0 ) = Rot.v[1];
//             vec ( 1 ) = Rot.v[4];
//             vec ( 2 ) = Rot.v[7];
//             U += 0.5 * log ( FF.v[4] ) * extProd ( vec, vec );
//             vec ( 0 ) = Rot.v[2];
//             vec ( 1 ) = Rot.v[5];
//             vec ( 2 ) = Rot.v[8];
//             U += 0.5 * log ( FF.v[8] ) * extProd ( vec, vec );
//             return U;
//         }
//
//         // calcul de l'angle
//         alpha =
//             ( FF.v[tensorInd2 ( q, q, 3 )] -
//               FF.v[tensorInd2 ( p, p, 3 )] ) / ( 2. * FF.v[tensorInd2 ( p, q, 3 )] );
//
//         // resolution
//         t = ( alpha != 0. ? 1. / ( alpha + sqrt ( alpha * alpha + 1. ) * ( alpha > 0. ? 1. : -1. ) ) : 1. );
//
//         // calcul des cosinus
//         Ctheta = 1. / ( sqrt ( 1. + t * t ) );
//         Stheta = t * Ctheta;
//
//         // calcul du nouveau tenseur
//         R.Identity ();
//         R ( p, q ) = Stheta;
//         R ( q, p ) = -Stheta;
//         R ( p, p ) = R ( q, q ) = Ctheta;
//
//         // appel aux multiplications de tenseurs
//         //*this=(R.Transpose())*(*this)*(R);
//
//         // produit R(T).FF.R avec FF sym�trique
//         U.v[0] =
//             R.v[0] * ( FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6] ) +
//             R.v[3] * ( FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6] ) +
//             R.v[6] * ( FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6] );
//         U.v[1] =
//             R.v[1] * ( FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6] ) +
//             R.v[4] * ( FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6] ) +
//             ( FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6] ) * R.v[7];
//         U.v[2] =
//             R.v[2] * ( FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6] ) +
//             R.v[5] * ( FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6] ) +
//             ( FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6] ) * R.v[8];
//         U.v[4] =
//             R.v[1] * ( FF.v[0] * R.v[1] + FF.v[3] * R.v[4] + FF.v[6] * R.v[7] ) +
//             R.v[4] * ( FF.v[1] * R.v[1] + FF.v[4] * R.v[4] + FF.v[7] * R.v[7] ) +
//             R.v[7] * ( FF.v[2] * R.v[1] + FF.v[5] * R.v[4] + FF.v[8] * R.v[7] );
//         U.v[5] =
//             R.v[2] * ( FF.v[0] * R.v[1] + FF.v[3] * R.v[4] + FF.v[6] * R.v[7] ) +
//             R.v[5] * ( FF.v[1] * R.v[1] + FF.v[4] * R.v[4] + FF.v[7] * R.v[7] ) +
//             ( FF.v[2] * R.v[1] + FF.v[5] * R.v[4] + FF.v[8] * R.v[7] ) * R.v[8];
//         U.v[8] =
//             R.v[2] * ( FF.v[0] * R.v[2] + FF.v[3] * R.v[5] + FF.v[6] * R.v[8] ) +
//             R.v[5] * ( FF.v[1] * R.v[2] + FF.v[4] * R.v[5] + FF.v[7] * R.v[8] ) +
//             R.v[8] * ( FF.v[2] * R.v[2] + FF.v[5] * R.v[5] + FF.v[8] * R.v[8] );
//
//         // recopie
//         FF.v[0] = U.v[0];
//         FF.v[1] = U.v[1];
//         FF.v[2] = U.v[2];
//         FF.v[3] = U.v[1];
//         FF.v[4] = U.v[4];
//         FF.v[5] = U.v[5];
//         FF.v[6] = U.v[2];
//         FF.v[7] = U.v[5];
//         FF.v[8] = U.v[8];
//
//         // cumul de R
//         Rot = Rot * R;
//         it++;
//     }
//
//     cout << ( *this ) << endl;
//     fatalError ( "Eigen Value Computation", "No convergence" );
//     return U;
// }

//!Polar decomposition of a second order tensor with computation of the \f$ ln[U] \f$ and \f$ R \f$ tensors.
/*!
  This method computes the polar decomposition of a second order tensor with computation of the \f$ ln[U] \f$ and \f$ R \f$ tensors as the returning arguments.
  The logarithm of a symmetric tensor is givent by the following formulation:
  \f[ \ln U =\sum _{i=1}^{3}\ln \lambda _{i}(u_{i}\otimes u_{i}) \f]

  \param U Return second order tensor containing \f$ ln[U] \f$
  \param R Return second order tensor containing \f$ R \f$
*/
//-----------------------------------------------------------------------------
void Tensor2::polarDecomposeLnU(Tensor2 &U, Tensor2 &R)
//-----------------------------------------------------------------------------
{
  Indice p, q;
  Real alpha, t, Ctheta, Stheta;
  Tensor2 Rot, FF;
  Indice it = 1;
  Vec3D vec;

  // calcul de FF=F(T).F
  FF = (*this).Transpose() * (*this);

  // initialisation de Rot
  Rot.Identity();

  while (it < 30)
  {
    // recherche du terme maxi
    q = (Abs(FF.v[1]) > Abs(FF.v[2]) ? 1 : 2);
    p = (Abs(FF.v[q]) > Abs(FF.v[5]) ? 0 : 1);
    if (p == 1)
    {
      q = 2;
    }

    // test de convergence
    if (Abs(FF.v[tensorInd2(p, q, 3)]) < precisionPolarDecompose)
    {
      Tensor2 ext;
      vec = Rot.getCol(0);
      ext = extProd(vec, vec);
      U = (log(FF.v[0]) / 2.) * ext;  //Prod(vec,vec);
      R = (1. / sqrt(FF.v[0])) * ext; //Prod(vec,vec);
      vec = Rot.getCol(1);
      ext = extProd(vec, vec);
      U += (log(FF.v[4]) / 2.) * ext;  //Prod(vec,vec);
      R += (1. / sqrt(FF.v[4])) * ext; //Prod(vec,vec);
      vec = Rot.getCol(2);
      ext = extProd(vec, vec);
      U += (log(FF.v[8]) / 2.) * ext;  //Prod(vec,vec);
      R += (1. / sqrt(FF.v[8])) * ext; //Prod(vec,vec);
      R = (*this) * R;
      return;
    }

    // calcul de l'angle
    alpha = (FF.v[tensorInd2(q, q, 3)] - FF.v[tensorInd2(p, p, 3)]) / (2. * FF.v[tensorInd2(p, q, 3)]);

    // resolution
    t = (alpha != 0. ? 1. / (alpha + sqrt(alpha * alpha + 1.) * (alpha > 0. ? 1. : -1.)) : 1.);

    // calcul des cosinus
    Ctheta = 1. / (sqrt(1. + t * t));
    Stheta = t * Ctheta;

    // calcul du nouveau tenseur
    R.Identity();
    R(p, q) = Stheta;
    R(q, p) = -Stheta;
    R(p, p) = R(q, q) = Ctheta;

    // produit R(T).FF.R avec FF sym�trique
    U.v[0] =
        R.v[0] * (FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6]) +
        R.v[3] * (FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6]) +
        R.v[6] * (FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6]);
    U.v[1] =
        R.v[1] * (FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6]) +
        R.v[4] * (FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6]) +
        (FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6]) * R.v[7];
    U.v[2] =
        R.v[2] * (FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6]) +
        R.v[5] * (FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6]) +
        (FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6]) * R.v[8];
    U.v[4] =
        R.v[1] * (FF.v[0] * R.v[1] + FF.v[3] * R.v[4] + FF.v[6] * R.v[7]) +
        R.v[4] * (FF.v[1] * R.v[1] + FF.v[4] * R.v[4] + FF.v[7] * R.v[7]) +
        R.v[7] * (FF.v[2] * R.v[1] + FF.v[5] * R.v[4] + FF.v[8] * R.v[7]);
    U.v[5] =
        R.v[2] * (FF.v[0] * R.v[1] + FF.v[3] * R.v[4] + FF.v[6] * R.v[7]) +
        R.v[5] * (FF.v[1] * R.v[1] + FF.v[4] * R.v[4] + FF.v[7] * R.v[7]) +
        (FF.v[2] * R.v[1] + FF.v[5] * R.v[4] + FF.v[8] * R.v[7]) * R.v[8];
    U.v[8] =
        R.v[2] * (FF.v[0] * R.v[2] + FF.v[3] * R.v[5] + FF.v[6] * R.v[8]) +
        R.v[5] * (FF.v[1] * R.v[2] + FF.v[4] * R.v[5] + FF.v[7] * R.v[8]) +
        R.v[8] * (FF.v[2] * R.v[2] + FF.v[5] * R.v[5] + FF.v[8] * R.v[8]);

    // recopie
    FF.v[0] = U.v[0];
    FF.v[1] = U.v[1];
    FF.v[2] = U.v[2];
    FF.v[3] = U.v[1];
    FF.v[4] = U.v[4];
    FF.v[5] = U.v[5];
    FF.v[6] = U.v[2];
    FF.v[7] = U.v[5];
    FF.v[8] = U.v[8];

    // cumul de R
    Rot = Rot * R;
    it++;
  }

  cout << (*this) << endl;
  fatalError("Eigen Value Computation", "No convergence");
}

//!Polar decomposition of a second order tensor with computation of the \f$ U \f$ and \f$ R \f$ tensors.
/*!
  This method computes the polar decomposition of a second order tensor with computation of the \f$ U \f$ and \f$ R \f$ tensors as the returning arguments.

  \param U Return second order tensor containing \f$ U \f$
  \param R Return second order tensor containing \f$ R \f$
*/
//-----------------------------------------------------------------------------
void Tensor2::polarDecompose(Tensor2 &U, Tensor2 &R)
//-----------------------------------------------------------------------------
{
  //  fatalError("Tensor2::polarDecompose()","exit(-1);");
  Indice p, q;
  Real alpha, t, Ctheta, Stheta;
  Tensor2 Rot, FF;
  Indice it = 1;
  Vec3D vec;

  // calcul de FF=F(T).F
  FF = (*this).Transpose() * (*this);

  // initialisation de Rot
  Rot.Identity();

  while (it < 30)
  {
    // recherche du terme maxi
    q = (Abs(FF.v[1]) > Abs(FF.v[2]) ? 1 : 2);
    p = (Abs(FF.v[q]) > Abs(FF.v[5]) ? 0 : 1);
    if (p == 1)
    {
      q = 2;
    }

    // test de convergence
    if (Abs(FF.v[tensorInd2(p, q, 3)]) < precisionPolarDecompose)
    {
      // premier vecteur propre
      vec(0) = Rot.v[0];
      vec(1) = Rot.v[3];
      vec(2) = Rot.v[6];
      // FF.v[0] premiere valeur propre
      U = sqrt(FF.v[0]) * extProd(vec, vec);
      // deuxieme vecteur propre
      vec(0) = Rot.v[1];
      vec(1) = Rot.v[4];
      vec(2) = Rot.v[7];
      // FF.v[4] deuxi�me valeur propre
      U += sqrt(FF.v[4]) * extProd(vec, vec);
      // troisieme vecteur propre
      vec(0) = Rot.v[2];
      vec(1) = Rot.v[5];
      vec(2) = Rot.v[8];
      // FF.v[8] troisieme valeur propre
      U += sqrt(FF.v[8]) * extProd(vec, vec);
      R = (*this) * U.inverse();
      return;
    }

    // calcul de l'angle
    alpha =
        (FF.v[tensorInd2(q, q, 3)] -
         FF.v[tensorInd2(p, p, 3)]) /
        (2. * FF.v[tensorInd2(p, q, 3)]);

    // resolution
    t = (alpha != 0. ? 1. / (alpha + sqrt(alpha * alpha + 1.) * (alpha > 0. ? 1. : -1.)) : 1.);

    // calcul des cosinus
    Ctheta = 1. / (sqrt(1. + t * t));
    Stheta = t * Ctheta;

    // calcul du nouveau tenseur
    R.Identity();
    R(p, q) = Stheta;
    R(q, p) = -Stheta;
    R(p, p) = R(q, q) = Ctheta;

    // produit R(T).FF.R avec FF sym�trique
    U.v[0] =
        R.v[0] * (FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6]) +
        R.v[3] * (FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6]) +
        R.v[6] * (FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6]);
    U.v[1] =
        R.v[1] * (FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6]) +
        R.v[4] * (FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6]) +
        (FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6]) * R.v[7];
    U.v[2] =
        R.v[2] * (FF.v[0] * R.v[0] + FF.v[3] * R.v[3] + FF.v[6] * R.v[6]) +
        R.v[5] * (FF.v[1] * R.v[0] + FF.v[4] * R.v[3] + FF.v[7] * R.v[6]) +
        (FF.v[2] * R.v[0] + FF.v[5] * R.v[3] + FF.v[8] * R.v[6]) * R.v[8];
    U.v[4] =
        R.v[1] * (FF.v[0] * R.v[1] + FF.v[3] * R.v[4] + FF.v[6] * R.v[7]) +
        R.v[4] * (FF.v[1] * R.v[1] + FF.v[4] * R.v[4] + FF.v[7] * R.v[7]) +
        R.v[7] * (FF.v[2] * R.v[1] + FF.v[5] * R.v[4] + FF.v[8] * R.v[7]);
    U.v[5] =
        R.v[2] * (FF.v[0] * R.v[1] + FF.v[3] * R.v[4] + FF.v[6] * R.v[7]) +
        R.v[5] * (FF.v[1] * R.v[1] + FF.v[4] * R.v[4] + FF.v[7] * R.v[7]) +
        (FF.v[2] * R.v[1] + FF.v[5] * R.v[4] + FF.v[8] * R.v[7]) * R.v[8];
    U.v[8] =
        R.v[2] * (FF.v[0] * R.v[2] + FF.v[3] * R.v[5] + FF.v[6] * R.v[8]) +
        R.v[5] * (FF.v[1] * R.v[2] + FF.v[4] * R.v[5] + FF.v[7] * R.v[8]) +
        R.v[8] * (FF.v[2] * R.v[2] + FF.v[5] * R.v[5] + FF.v[8] * R.v[8]);

    // recopie
    FF.v[0] = U.v[0];
    FF.v[1] = U.v[1];
    FF.v[2] = U.v[2];
    FF.v[3] = U.v[1];
    FF.v[4] = U.v[4];
    FF.v[5] = U.v[5];
    FF.v[6] = U.v[2];
    FF.v[7] = U.v[5];
    FF.v[8] = U.v[8];

    // cumul de R
    Rot = Rot * R;
    it++;
  }

  cout << (*this) << endl;
  fatalError("Eigen Value Computation", "No convergence");
}
