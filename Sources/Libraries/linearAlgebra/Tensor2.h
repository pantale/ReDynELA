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
  \file Tensor2.h
  \brief Declaration file for the second order tensor class

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

#ifndef __Tensor2_h__
#define __Tensor2_h__

#include <upLinearAlgebra.h>
#include <Vec3D.h>
#include <math.h>

class Vec3D;

/*!
  \class Tensor2
  \brief Declaration of the second order tensor class

  The second order tensor is a like a matrix with the following form:
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
class Tensor2
{
  Real v[9];

private:
  inline Boolean indexOK(Indice i, Indice j) const;

public:
  Tensor2();
  ~Tensor2();

  inline Real operator()(Indice i, Indice j) const;
  inline Real &operator()(Indice i, Indice j);

  inline void fill(const Real val);
  void Identity();
  Tensor2 &operator=(const Real &val);
  Tensor2 &operator=(const Tensor2 &tens);

  Boolean operator==(const Tensor2 &tens) const;
  Boolean operator!=(const Tensor2 &tens) const;

  // operations arithmetiques de base entre tenseurs
  inline void operator+=(const Tensor2 &tens);
  inline void operator-=(const Tensor2 &tens);
  inline void operator*=(const Real val);
  inline void operator/=(const Real val);
  Tensor2 operator+(const Tensor2 &tens) const;
  Tensor2 operator-(const Tensor2 &tens) const;
  Tensor2 operator*(const Tensor2 &tens) const;
  Tensor2 operator*(const Real &lambda) const;
  Tensor2 operator/(const Real &tens) const;
  friend Tensor2 operator*(const Real &val, const Tensor2 &tens);
  Vec3D operator*(const Vec3D &vec) const;

  // operations particulieres sur les tenseurs
  Tensor2 extProd(const Vec3D v1, const Vec3D v2);
  Tensor2 prod1(const Tensor2 t2);
  Real prod2(const Tensor2 t2);
  Tensor2 deviator();
  Real norme();
  Real J2();
  //     Tensor2 polarDecomposeU ();
  void polarDecompose(Tensor2 &U, Tensor2 &R);
  //     Tensor2 polarDecomposeLnU ();
  void polarDecomposeLnU(Tensor2 &U, Tensor2 &R);
  //     Tensor2 ln ();

  // operations arithmetiques autres
  inline Real Trace();
  inline Real moyTrace();
  Tensor2 Transpose() const;
  Vec3D colSum();
  Vec3D rowSum();
  void getSymetricPart(Tensor2 &va) const;
  void getAntiSymetricPart(Tensor2 &va) const;
  Vec3D getRow(Indice i);
  Vec3D getCol(Indice i);
  Real det() const;
  Tensor2 inverse();
  inline Real misesEquivalent();
  inline Real equivalent();

  // gestion des flux entree et sortie
  friend ostream &operator<<(ostream &os, const Tensor2 &tens);
  friend ofstream &operator<<(ofstream &os, const Tensor2 &tens);
  friend ifstream &operator>>(ifstream &is, Tensor2 &tens);
  void write(ofstream &ofs) const;
  void read(ifstream &ifs);
  void print(ostream &os) const;
};

//------inline functions-------------------------------------------------------

//-----------------------------------------------------------------------------
inline Boolean Tensor2::indexOK(Indice i, Indice j) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < 3) && (j >= 0) && (j < 3))
  {
    return (True);
  }

  printf("Tensor2::indexOK\nindice(s) [%ld,%ld] out of allowed range [0-2,0-2]", i, j);
  exit(-1);

  // fonction inutile, mais necessaire pour le compilateur
  return (False);
}

//!Access to the values T[i,j] of a second order tensor
/*!
  \param i indice for row
  \param j indice for column
  \return Value of the second order tensor T[i,j]
*/
//-----------------------------------------------------------------------------
inline Real &Tensor2::operator()(Indice i, Indice j)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j);
#endif
  return v[tensorInd2(i, j, 3)];
}

//!Access to the values T[i,j] of a second order tensor (Read only method)
/*!
  \param i indice for row
  \param j indice for column
  \return Value of the second order tensor T[i,j]
*/
//-----------------------------------------------------------------------------
inline Real Tensor2::operator()(Indice i, Indice j) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j);
#endif
  return v[tensorInd2(i, j, 3)];
}

//!Fill a second order tensor with a scalar value
/*!
  This method is a surdefinition of the = operator for the second order tensor class.

  Example :
  \code
  Tensor2 t1;
  t1 = fill(1.0); // All components of the tensor are set to 1.0
  \endcode
  \param val Real value to give to all components of the second order tensor
*/
//-----------------------------------------------------------------------------
inline void Tensor2::fill(const Real val)
//-----------------------------------------------------------------------------
{
  v[0] = val;
  v[1] = val;
  v[2] = val;
  v[3] = val;
  v[4] = val;
  v[5] = val;
  v[6] = val;
  v[7] = val;
  v[8] = val;
}

//!Addition of 2 second order tensors
/*!
  This method defines the addition of 2 second order tensors.

  Example :
  \code
  Tensor2 t1,t2;
  t2 += t1; // sum of two second order tensors
  \endcode
  \param tens Second order tensor to add
*/
//-----------------------------------------------------------------------------
inline void Tensor2::operator+=(const Tensor2 &tens)
//-----------------------------------------------------------------------------
{
  // calcul de la somme
  v[0] += tens.v[0];
  v[1] += tens.v[1];
  v[2] += tens.v[2];
  v[3] += tens.v[3];
  v[4] += tens.v[4];
  v[5] += tens.v[5];
  v[6] += tens.v[6];
  v[7] += tens.v[7];
  v[8] += tens.v[8];
}

//!Difference of 2 second order tensors
/*!
  This method defines the difference of 2 second order tensors.

  Example :
  \code
  Tensor2 t1,t2;
  t2 -= t1; // difference of two second order tensors
  \endcode
  \param tens Second order tensor to substract
*/
//-----------------------------------------------------------------------------
inline void Tensor2::operator-=(const Tensor2 &tens)
//-----------------------------------------------------------------------------
{
  // calcul de la difference
  v[0] -= tens.v[0];
  v[1] -= tens.v[1];
  v[2] -= tens.v[2];
  v[3] -= tens.v[3];
  v[4] -= tens.v[4];
  v[5] -= tens.v[5];
  v[6] -= tens.v[6];
  v[7] -= tens.v[7];
  v[8] -= tens.v[8];
}

//!Multiplication of a second order tensor by a scalar value
/*!
  This method defines the multiplication of a second order tensor by a scalar value
  
  Example :
  \code
  Tensor2 t1;
  Real l;
  t1 *= l; // multiplication by a scalar
  \endcode
  \param val Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
inline void Tensor2::operator*=(const Real val)
//-----------------------------------------------------------------------------
{
  v[0] *= val;
  v[1] *= val;
  v[2] *= val;
  v[3] *= val;
  v[4] *= val;
  v[5] *= val;
  v[6] *= val;
  v[7] *= val;
  v[8] *= val;
}

//!Division of a second order tensor by a scalar value
/*!
  This method defines the division of a second order tensor by a scalar value
  
  Example :
  \code
  Tensor2 t1;
  Real l;
  t1 /= l; // division by a scalar
  \endcode
  \param val Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
inline void Tensor2::operator/=(const Real val)
//-----------------------------------------------------------------------------
{
  v[0] /= val;
  v[1] /= val;
  v[2] /= val;
  v[3] /= val;
  v[4] /= val;
  v[5] /= val;
  v[6] /= val;
  v[7] /= val;
  v[8] /= val;
}

//!Returns the trace of a second order tensor
/*!
  This method returns the trace of a second order tensor, i.e. the sum of all the terms of the diagonal:
  \f[ t = tr[T] = T_{11}+T_{22}+T_{33} \f]
  \return Trace of a second order tensor
*/
//-----------------------------------------------------------------------------
inline Real Tensor2::Trace()
//-----------------------------------------------------------------------------
{
  return (v[0] + v[4] + v[8]);
}

//!Returns the average value of the trace of a second order tensor
/*!
  This method returns average value of the trace of a second order tensor, i.e. the sum of all the terms of the diagonal divided by 3:
    \f[ t = \frac{1}{3} tr[T] =  \frac{1}{3} \left( T_{11}+T_{22}+T_{33} \right) \f]
  \return Trace of a second order tensor divided by 3
*/
//-----------------------------------------------------------------------------
inline Real Tensor2::moyTrace()
//-----------------------------------------------------------------------------
{
  return ((v[0] + v[4] + v[8]) / 3.0);
}

//!Returns the von-Mises equivalent stress of a second order tensor
/*!
  This method returns the von-Mises equivalent stress of a second order tensor defined by:
  \f[ \overline{\sigma} = \frac {1}{\sqrt{2}}\sqrt{(s_{11}-s_{22})^2+(s_{22}-s_{33})^2+(s_{33}-s_{11})^2+6(s_{12}^2+s_{23}^2+s_{31}^2)}\f]
  \return von-Mises equivalent stress of a second order tensor
*/
//-----------------------------------------------------------------------------
inline Real Tensor2::misesEquivalent()
//-----------------------------------------------------------------------------
{
  return (VonMises(v[0], v[4], v[8], v[1], v[2], v[5]));
}

//!Returns the equivalent norm of a second order tensor
/*!
  This method returns equivalent norm of a second order tensor defined by:
  \f[ \left\Vert s \right\Vert  = \sqrt {\frac{2}{3} s_{ij}:s_{ij}} \f]
  \return equivalent norm of a second order tensor
*/
//-----------------------------------------------------------------------------
inline Real Tensor2::equivalent()
//-----------------------------------------------------------------------------
{
  return (sqrt(2. *
               (v[0] * v[0] +
                v[1] * v[1] +
                v[2] * v[2] +
                v[3] * v[3] +
                v[4] * v[4] +
                v[5] * v[5] +
                v[6] * v[6] +
                v[7] * v[7] +
                v[8] * v[8]) /
               3.));
}

#endif
