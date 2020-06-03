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
  \file Tensor3.h
  \brief Declaration file for the third order tensor class

  This file is the declaration file for the third order tensor class. A third order tensor has the following form:
  \f[ T = T_{ijk} \f]
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __Tensor3_h__
#define __Tensor3_h__

#include <upLinearAlgebra.h>
#include <Tensor2.h>

#include <Vec3D.h>
class Vec3D;

/*!
  \class Tensor3
  \brief Declaration of the third order tensor class

  A third order tensor has the following form:
  \f[ T = T_{ijk} \f]
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/
class Tensor3
{
    Real v[27];

private:
    inline Boolean indexOK(Indice i, Indice j, Indice k) const;

public:
    Tensor3();
    ~Tensor3();

    // operations sur les composantes
    inline Real operator()(Indice i, Indice j, Indice k) const;
    inline Real &operator()(Indice i, Indice j, Indice k);

    // operations d'affectation
    inline void fill(Real val);
    void Identity();
    Tensor3 &operator=(const Real &val);
    Tensor3 &operator=(const Tensor3 &tens);

    // operations de comparaison
    Boolean operator==(const Tensor3 &tens) const;
    Boolean operator!=(const Tensor3 &tens) const;

    // operations arithmetiques de base entre tenseurs
    inline void operator+=(const Tensor3 &tens);
    inline void operator-=(const Tensor3 &tens);
    inline void operator*=(const Real val);
    inline void operator/=(const Real val);
    Tensor3 operator+(const Tensor3 &tens) const;
    Tensor3 operator-(const Tensor3 &tens) const;
    Tensor3 operator*(const Real &lambda) const;
    Tensor3 operator/(const Real &tens) const;
    friend Tensor3 operator*(const Real &val, const Tensor3 &tens);
    Tensor2 operator*(const Vec3D &vec) const;

    // gestion des flux entree et sortie
    friend ostream &operator<<(ostream &os, const Tensor3 &tens);
    friend ofstream &operator<<(ofstream &os, const Tensor3 &tens);
    friend ifstream &operator>>(ifstream &is, Tensor3 &tens);
    void write(ofstream &ofs) const;
    void read(ifstream &ifs);
    void print(ostream &os) const;
};

//------inline functions-------------------------------------------------------

//-----------------------------------------------------------------------------
inline Boolean Tensor3::indexOK(Indice i, Indice j, Indice k) const
//-----------------------------------------------------------------------------
{
    if ((i >= 0) && (i < 3) && (j >= 0) && (j < 3) && (k >= 0) && (k < 3))
    {
        return (True);
    }

    printf("Tensor3::indexOK\nindice(s) [%ld,%ld,%ld] out of allowed range [0-2,0-2,0-2]", i, j,
           k);
    exit(-1);

    // fonction inutile, mais necessaire pour le compilateur
    return (False);
}

//!Access to the values T[i,j,k] of a third order tensor
/*!
  \param i indice 1
  \param j indice 2
  \param k indice 3
  \return Value of the third order tensor T[i,j,k]
*/
//-----------------------------------------------------------------------------
inline Real &Tensor3::operator()(Indice i, Indice j, Indice k)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
    indexOK(i, j, k);
#endif
    return v[tensorInd3(i, j, k, 3)];
}

//!Access to the values T[i,j,k] of a third order tensor (Read only method)
/*!
  \param i indice 1
  \param j indice 2
  \param k indice 3
  \return Value of the third order tensor T[i,j,k]
*/
//-----------------------------------------------------------------------------
inline Real Tensor3::operator()(Indice i, Indice j, Indice k) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
    indexOK(i, j, k);
#endif
    return v[tensorInd3(i, j, k, 3)];
}

//!Fill a third order tensor with a scalar value
/*!
  This method is a surdefinition of the = operator for the third order tensor class.

  Example :
  \code
  Tensor3 t1;
  t1 = fill(1.0); // All components of the tensor are set to 1.0
  \endcode
  \param val Real value to give to all components of the third order tensor
*/
//-----------------------------------------------------------------------------
inline void Tensor3::fill(Real val)
//-----------------------------------------------------------------------------
{
    for (Indice i = 0; i < 27; i++)
    {
        v[i] = val;
    }
}

//!Addition of 2 third order tensors
/*!
  This method defines the addition of 2 third order tensors.

  Example :
  \code
  Tensor3 t1,t2;
  t2 += t1; // sum of two third order tensors
  \endcode
  \param tens Third order tensor to add
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator+=(const Tensor3 &tens)
//-----------------------------------------------------------------------------
{
    // calcul de la somme
    for (Indice i = 0; i < 27; i++)
    {
        v[i] += tens.v[i];
    }
}

//!Difference of 2 third order tensors
/*!
  This method defines the difference of 2 third order tensors.

  Example :
  \code
  Tensor3 t1,t2;
  t2 -= t1; // difference of two third order tensors
  \endcode
  \param tens Third order tensor to substract
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator-=(const Tensor3 &tens)
//-----------------------------------------------------------------------------
{
    // calcul de la difference
    for (Indice i = 0; i < 27; i++)
    {
        v[i] -= tens.v[i];
    }
}

//!Multiplication of a third order tensor by a scalar value
/*!
  This method defines the multiplication of a third order tensor by a scalar value
  
  Example :
  \code
  Tensor3 t1;
  Real l;
  t1 *= l; // multiplication by a scalar
  \endcode
  \param val Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator*=(const Real val)
//-----------------------------------------------------------------------------
{
    for (Indice i = 0; i < 27; i++)
    {
        v[i] *= val;
    }
}

//!Division of a third order tensor by a scalar value
/*!
  This method defines the division of a third order tensor by a scalar value
  
  Example :
  \code
  Tensor3 t1;
  Real l;
  t1 /= l; // division by a scalar
  \endcode
  \param val Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator/=(const Real val)
//-----------------------------------------------------------------------------
{
    for (Indice i = 0; i < 27; i++)
    {
        v[i] /= val;
    }
}

#endif
