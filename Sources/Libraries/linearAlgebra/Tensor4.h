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
  Class Tensor4 definition
*/

#ifndef __Tensor4_h__
#define __Tensor4_h__

/*!
  \file Tensor4.h
  \brief fichier .h de d�finition et de manipulation tenseurs d'ordre 4
  \ingroup linearAlgebra

  Ce fichier d�finit les m�thodes permettant de g�rer les tenseurs d'ordre 4.

  \author Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004  
*/

#include <upLinearAlgebra.h>
#include <Tensor2.h>
#include <Tensor3.h>

#include <Vec3D.h>
class Vec3D;

/*!
  \class Tensor4 Tensor4.h
  \brief Classe de gestion et manipulation des tenseurs du second ordre.
  \ingroup linearAlgebra
  \author Olivier PANTALE
  \version 0.9.5
  \date 1997-2004

Cette classe est utilis�e pour la gestion et la manipulation des tenseurs du quatri�me ordre. Elle comporte toutes les m�thodes d'initialisation et de gestion m�moire. Les valeurs stock�es sont des Real. La classe \c Tensor4 contient de plus un certain nombre de m�thodes de calcul sur les tenseurs comme d�crit ci-dessous.
*/
/** @dia:pos 64,78 */
class Tensor4
{
  Real v[81];

public:
  // constructeurs
  Tensor4();
  ~Tensor4();

  // operations sur les composantes
  inline Real operator()(Indice i, Indice j, Indice k, Indice l) const;
  inline Real &operator()(Indice i, Indice j, Indice k, Indice l);
  inline Boolean indexOK(Indice i, Indice j, Indice k, Indice l) const;

  // operations d'affectation
  inline void fill(Real val);
  void Identity();
  Tensor4 &operator=(const Real &val);
  Tensor4 &operator=(const Tensor4 &tens);

  // operations de comparaison
  Boolean operator==(const Tensor4 &tens) const;
  Boolean operator!=(const Tensor4 &tens) const;

  // operations arithmetiques de base entre tenseurs
  inline void operator+=(const Tensor4 &tens);
  inline void operator-=(const Tensor4 &tens);
  inline void operator*=(const Real val);
  inline void operator/=(const Real val);
  Tensor4 operator+(const Tensor4 &tens) const;
  Tensor4 operator-(const Tensor4 &tens) const;
  Tensor4 operator*(const Real &lambda) const;
  Tensor4 operator/(const Real &tens) const;
  friend Tensor4 operator*(const Real &val, const Tensor4 &tens);

  // multiplications particulieres
  Tensor3 operator*(const Vec3D &vec) const;
  Tensor2 operator*(const Tensor2 &t) const;

  // gestion des flux entree et sortie
  friend ostream &operator<<(ostream &os, const Tensor4 &tens);
  friend ofstream &operator<<(ofstream &os, const Tensor4 &tens);
  friend ifstream &operator>>(ifstream &is, Tensor4 &tens);
  void write(ofstream &ofs) const;
  void read(ifstream &ifs);
  void print(ostream &os) const;
};

//------inline functions-------------------------------------------------------

//!teste les bornes du tenseur
/*!
  Cette m�thode teste les bornes d'un tenseur
  \param i indice 1
  \param j indice 2
  \param k indice 3
  \param l indice 4
  \return True si les indices fournis sont dans les bornes, False dans le cas contraire
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline Boolean
Tensor4::indexOK(Indice i, Indice j, Indice k, Indice l) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < 3) && (j >= 0) && (j < 3) && (k >= 0) && (k < 3) && (l >= 0) && (l < 3))
    return (True);

  printf("Tensor4::indexOK\nindice(s) [%ld,%ld,%ld,%ld] out of allowed range [0-2,0-2,0-2,0-2]",
         i, j, k, l);
  exit(-1);

  // fonction inutile, mais necessaire pour le compilateur
  return (False);
}

//!acc�s aux valeurs d'un tenseur
/*!
  \param i indice 1
  \param j indice 2
  \param k indice 3
  \param l indice 4
  \return valeur du tenseur Tijkl
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline Real &
Tensor4::operator()(Indice i, Indice j, Indice k, Indice l)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j, k, l);
#endif
  return v[tensorInd4(i, j, k, l, 3)];
}

//!acc�s aux valeurs d'un tenseur
/*!
  \param i indice 1
  \param j indice 2
  \param k indice 3
  \param l indice 4
  \return valeur du tenseur Tijkl
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline Real
Tensor4::operator()(Indice i, Indice j, Indice k, Indice l)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j, k, l);
#endif
  return v[tensorInd4(i, j, k, l, 3)];
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement le remplissage des valeurs d'un tenseur par un scalaire

  Exemple :
  \code
  Tensor4 t1;
  t1=fill(1.); // affecte 1 � toutes les composantes du tenseur
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void Tensor4::fill(Real val)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < 81; i++)
    v[i] = val;
}

//!addition de deux tenseurs du deuxi�me ordre
/*!
  Cette methode permet de surd�finir l'op�ration d'addition des tenseurs et d'�crire simplement la somme de deux tenseurs sous la forme donn�e en exemple

  Exemple :
  \code
  Tensor4 t1,t2;
  t2+=t1; // somme de deux tenseurs
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void
Tensor4::operator+=(const Tensor4 &tens)
//-----------------------------------------------------------------------------
{
  // calcul de la somme
  for (Indice i = 0; i < 81; i++)
    v[i] += tens.v[i];
}

//!soustraction de deux tenseurs du deuxi�me ordre
/*!
  Cette methode permet de surd�finir l'op�ration de soustraction des tenseurs et d'�crire simplement la soustraction de deux tenseurs sous la forme donn�e en exemple

  Exemple :
  \code
  Tensor4 t1,t2;
  t2-=t1; // soustraction de deux tenseurs
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void
Tensor4::operator-=(const Tensor4 &tens)
//-----------------------------------------------------------------------------
{
  // calcul de la difference
  for (Indice i = 0; i < 81; i++)
    v[i] -= tens.v[i];
}

//!multiplication d'un tenseur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des tenseurs et d'�crire simplement la multiplication d'un tenseur par un scalaire sous la forme donn�e en exemple

  Exemple :
  \code
  Tensor4 t1;
  Real l;
  t1*=l; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void
Tensor4::operator*=(const Real val)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < 81; i++)
    v[i] *= val;
}

//!division d'un tenseur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de division des tenseurs et d'�crire simplement la division d'un tenseur par un scalaire sous la forme donn�e en exemple

  Exemple :
  \code
  Tensor4 t1;
  Real l;
  t1/=l; // division par un scalaire
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void
Tensor4::operator/=(const Real val)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < 81; i++)
    v[i] /= val;
}

#endif
