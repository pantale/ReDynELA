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
  \file Tensor3.C
  \brief Definition file for the third order tensor class

  This file is the declaration file for the third order tensor class. A third order tensor has the following form:
  \f[ T = T_{ijk} \f]
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#include <Tensor3.h>

//!Constructor of the Tensor3 class
/*!
  This method is the default constructor of a third order tensor. All components are initialized to zero by default.
*/
//-----------------------------------------------------------------------------
Tensor3::Tensor3()
//-----------------------------------------------------------------------------
{
  // initialisation
  fill(0.);
}

//!Destructor of the Tensor3 class
//-----------------------------------------------------------------------------
Tensor3::~Tensor3()
//-----------------------------------------------------------------------------
{
}

//!Send the content of a third order tensor to the output flux for display
/*!
  Example
  \code
  Tensor3 t;
  cout << t << endl;
  \endcode
  \param os Output flux
*/
//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  t1.print(os);
  return os;
}

//!Print the content of a third order tensor to the output flux for display
/*!
  \param os Output flux
*/
//-----------------------------------------------------------------------------
void Tensor3::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  Indice i, j, k;
  os << "tensor 3x3x3\n";
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      for (k = 0; k < 3; k++)
      {
        os << "T[" << i << "," << j << "," << k << "]=" << v[tensorInd3(i, j, k, 3)];
      }
    }
  }
}

//!Returns an identity tensor
/*!
  This method transforms the current tensor to an identity tensor.

  Example :
  \code
  Tensor3 t1;
  t1.Identity(); // Returns and identity tensor
  \endcode
  \warning This method modify it's argument
*/
//-----------------------------------------------------------------------------
void Tensor3::Identity()
//-----------------------------------------------------------------------------
{
  fill(0.);
  v[tensorInd3(0, 1, 2, 3)] = 1.;
  v[tensorInd3(1, 2, 0, 3)] = 1.;
  v[tensorInd3(2, 0, 1, 3)] = 1.;
  v[tensorInd3(2, 1, 0, 3)] = -1.;
  v[tensorInd3(1, 0, 2, 3)] = -1.;
  v[tensorInd3(0, 2, 1, 3)] = -1.;
}

//!Fill a third order tensor with a scalar value
/*!
  This method is a surdefinition of the = operator for the third order tensor class.

  Example :
  \code
  Tensor3 t1;
  t1 = 1.0; // All components of the tensor are set to 1.0
  \endcode
  \param val Real value to give to all components of the third order tensor
*/
//-----------------------------------------------------------------------------
Tensor3 &Tensor3::operator=(const Real &val)
//-----------------------------------------------------------------------------
{
  fill(val);
  return *this;
}

//!Copy the content of a third order tensor into a new one
/*!
  This method is the so called = operator between two third order tensors. If the \ref MEM_funct is set, the \ref memcpy function is used for the copy.

  Example :
  \code
  Tensor3 t1, t2;
  t1 = t2; // copy of the tensor
  \endcode
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor3 &Tensor3::operator=(const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
#ifdef MEM_funct
  memcpy(v, t1.v, 27 * sizeof(Real));
#else
  for (Indice i = 0; i < 27; i++)
  {
    v[i] = t1.v[i];
  }
#endif
  return *this;
}

//!Addition of 2 third order tensors
/*!
  This method defines the addition of 2 third order tensors.
  The result of this operation is also a third order tensor defined by:
  \f[ T = A + B \f]

  Example :
  \code
  Tensor3 t1,t2,t3;
  t3 = t1 + t2; // sum of 2 third order tensors
  \endcode
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator+(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor3 t2;

  // calcul de la somme
  for (Indice i = 0; i < 27; i++)
  {
    t2.v[i] = v[i] + t1.v[i];
  }

  // renvoi de l'objet
  return t2;
}

//!Difference of 2 third order tensors
/*!
  This method defines the difference of 2 third order tensors.
  The result of this operation is also a third order tensor defined by:
  \f[ T = A - B \f]

  Example :
  \code
  Tensor3 t1,t2,t3;
  t3 = t1 - t2; // difference of 2 third order tensors
  \endcode
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator-(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor3 t2;

  // calcul de la somme
  for (Indice i = 0; i < 27; i++)
  {
    t2.v[i] = v[i] - t1.v[i];
  }

  // renvoi de l'objet
  return t2;
}

//!Multiplication of a third order tensor by a scalar value
/*!
  This method defines the multiplication of a third order tensor by a scalar value
  The result of this operation is also a third order tensor defined by:
  \f[ T = A . \lambda \f]
  
  Example :
  \code
  Tensor3 t1,t2;
  Real l;
  t2 = t1 * l; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator*(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor3 t2;

  for (Indice i = 0; i < 27; i++)
  {
    t2.v[i] = lambda * v[i];
  }

  return t2;
}

//!Division of a third order tensor by a scalar value
/*!
  This method defines the division of a third order tensor by a scalar value
  The result of this operation is also a third order tensor defined by:
  \f[ T = \frac{A}{\lambda} \f]
  
  Example :
  \code
  Tensor3 t1,t2;
  Real l;
  t2 = t1 / l; // division by a scalar
  \endcode
  \warning This is not a commutative operation, be also warn not to divide by zero.
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator/(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor3 t2;

  for (Indice i = 0; i < 27; i++)
  {
    t2.v[i] = v[i] / lambda;
  }

  return t2;
}

//!Multiplication of a third order tensor by a scalar value
/*!
  This method defines the multiplication of a third order tensor by a scalar value
  The result of this operation is also a third order tensor defined by:
  \f[ T = \lambda . A \f]
  
  Example :
  \code
  Tensor3 t1,t2;
  Real l;
  t2 = l * t1; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor3 operator*(const Real &lambda, const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  Tensor3 t2;

  for (Indice i = 0; i < 27; i++)
  {
    t2.v[i] = lambda * t1.v[i];
  }

  return t2;
}

//!Multiplication of a third order tensor by a vector
/*!
  This method defines the product of a third order tensor by a vector.
  The result of this operation is also a vector defined by:
  \f[ B = T . \overrightarrow{a} \f]

  Example :
  \code
  Tensor3 t1;
  Vec3D v1;
  Tensor 2 t2;
  t2 = t1 * v1; // product of the third order tensor by a vector
  \endcode
  \param v1 Vector to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor3::operator*(const Vec3D &v1) const
//-----------------------------------------------------------------------------
{
  Tensor2 t2;

  for (Indice i = 0; i < 3; i++)
    for (Indice j = 0; j < 3; j++)
      for (Indice k = 0; k < 3; k++)
      {
        t2(i, j) += v[tensorInd3(i, j, k, 3)] * v1(k);
      }

  return t2;
}

//!Test the equality of two third order tensors
/*!
  This method tests the equality of two third order tensors.
  It returns \ref True if all components of the two third order tensors are equals, \ref False on other case.
  \return \ref True or \ref False depending on the result of the test.
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Boolean Tensor3::operator==(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  Indice i;

  for (i = 0; i < 27; i++)
    if (v[i] != t1.v[i])
    {
      return False;
    }
  return True;
}

//!Test the equality of two third order tensors
/*!
  This method tests the equality of two third order tensors.
  It returns \ref False if all components of the two third order tensors are equals, \ref True on other case.
  \return \ref True or \ref False depending on the result of the test.
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Boolean Tensor3::operator!=(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  return !(*this == t1);
}

//!Writes a third order tensor in a binary flux for storage
/*!
  This method is used to store the components of a third order tensor in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Tensor3 t;
  t.write(pfile);
  \endcode
  \param ofs Output file stream to use for writting operation
*/
//-----------------------------------------------------------------------------
void Tensor3::write(ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)v, 27 * sizeof(Real));
}

//!Reads a third order tensor in a binary flux from storage
/*!
  This method is used to read the components of a third order tensor in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Tensor3 t;
  t.read(pfile);
  \endcode
  \param ofs Input file stream to use for reading operation
*/
//-----------------------------------------------------------------------------
void Tensor3::read(ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)v, 27 * sizeof(Real));
}

//!Writes a third order tensor in a binary flux for storage
/*!
  This method is used to store the components of a third order tensor in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Tensor3 t;
  pfile << t;
  \endcode
  \param os Output file stream to use for writting operation
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  t1.write(os);
  return os;
}

//!Reads a third order tensor from a binary flux for storage
/*!
  This method is used to read the components of a third order tensor in a binary file.

  Example :
  \code
  ifstream pfile("fichier");
  Tensor3 t;
  pfile >> t;
  \endcode
  \param os Input file stream to use for reading operation
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  t1.read(is);
  return is;
}
