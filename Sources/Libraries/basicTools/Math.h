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
  \file Math.h
  \brief Definition of the basic mathematica operators and constants.

  This file defines all the matematical constants, tools and basic operators.
  \ingroup basicTools
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __Math_h__
#define __Math_h__

/*
 * predefined constants
 */
// if PI is not defined on this system the we define it
#ifndef PI
#define PI 3.1415926535897932384626433832795 //!< Numerical value of the constant \f$ \pi \f$ if note already defined on the system
#endif
#define deuxPI 6.283185307179586476925286766559 //!< Numerical value of the constant \f$ 2\pi \f$

// usefull constants to be defined
#define Sqrt2 1.4142135623730950488016887242097      //!< Numerical value of the constant \f$ \sqrt{2} \f$
#define Sqrt3 1.7320508075688772935274463415059      //!< Numerical value of the constant \f$ \sqrt{3} \f$
#define invSqrt2 0.7071067811865475244008443621048   //!< Numerical value of the constant \f$ \frac{1}{/\sqrt{2}} \f$
#define invSqrt3 0.5773502691896257645091487805019   //!< Numerical value of the constant \f$ \frac{1}{/\sqrt{3}} \f$
#define Sqrt23 0.8164965809277260327324280249019     //!< Numerical value of the constant \f$ \frac{2}{3} \f$
#define Sqrt32 1.2247448713915890490986420373529     //!< Numerical value of the constant \f$ \frac{3}{2} \f$
#define DegToRad 0.017453292519943295769236907684886 //!< Numerical value of the constant used to convert degree to radians \f$ \frac{\pi}{180} \f$
#define RadToDeg 57.295779513082320876798154814105   //!< Numerical value of the constant used to convert radians to degree \f$ \frac{180}{\pi} \f$

#define Sqrt3m1 0.7320508075688772935274463415059  //!< Numerical value of the constant \f$ \sqrt{3} - 1\f$
#define Sqrt3m2 -0.2679491924311227064725536584941 //!< Numerical value of the constant \f$ \sqrt{3} - 2\f$
#define Sqrt3m3 -1.2679491924311227064725536584941 //!< Numerical value of the constant \f$ \sqrt{3} - 3\f$
#define Sqrt3p1 2.7320508075688772935274463415059  //!< Numerical value of the constant \f$ \sqrt{3} + 1\f$
#define Sqrt3p2 3.7320508075688772935274463415059  //!< Numerical value of the constant \f$ \sqrt{3} + 2\f$
#define Sqrt3p3 4.7320508075688772935274463415059  //!< Numerical value of the constant \f$ \sqrt{3} + 3\f$

/*
 * functions definies
 */
// Maximum ou minimum de 2 valeurs
#define Max(i, j) ((i) > (j) ? (i) : (j)) //!< Returns the maximum of 2 values
#define Min(i, j) ((i) < (j) ? (i) : (j)) //!< Returns the minimum of 2 values

// Maximum ou minimum de 3 valeurs
#define Max3(i, j, k) ((i) > (j) ? ((i) > (k) ? (i) : (k)) : ((j) > (k) ? (j) : (k))) //!< Returns the maximum of 3 values.
#define Min3(i, j, k) ((i) < (j) ? ((i) < (k) ? (i) : (k)) : ((j) < (k) ? (j) : (k))) //!< Returns the minimum of 3 values.

// Valeur absolue d'une valeur
#define Abs(i) ((i) < 0 ? -(i) : (i)) //!< Returns the absolute value of the given argument.

// Signe d'une valeur
#define Sign(i) ((i) >= 0. ? 1. : -1.) //!< Returns the sign of a given value (+1) if i>0, (-1) if i<0.

// Parenth�ses de MacAuley
#define MacAuley(x) ((x) <= 0 ? 0. : (x)) //!< Retruns the MacAuley operator defined by: \f[ \left\langle x\right\rangle =\frac{1}{2}\left[x+\left|x\right|\right]\f]

// Norme d'un vecteur de 3 composantes
#define Norme3(x1, x2, x3) (sqrt(SQ(x1) + SQ(x2) + SQ(x3))) //!< Returns the norm of a 3D vector defined by: \f[ \left\Vert x\right\Vert=\sqrt{x_1^2+x_2^2+x_3^2} \f]

// Surface d'un triangle end fonction des longueurs de cotes
#define Surf_Tri(a, b, c) ((sqrt(((a) + (b) + (c)) * ((a) - (b) + (c)) * ((a) + (b) - (c)) * (-(a) + (b) + (c)))) / 4.) //!< Returns the surface of a triangle function of the 3 edges lengths from the Heron formula: \f[ \frac{1}{4} \sqrt{-(a-b-c) (a+b-c) (a-b+c) (a+b+c)}\f]

// Surface d'un triangle defini par 3 points 2D
#define Surf_Tri_3pt(xa, ya, xb, yb, xc, yc) (ABSdbl((xb) * (ya) - (xc) * (ya) - (xa) * (yb) + (xc) * (yb) + (xa) * (yc) - (xb) * (yc)) / 2.) //!< Returns the surface of a triangle function of the coordinates of the 3 corners from the Heron formula

// Contrainte equivalente de Von Mises
#define VonMises(s1, s2, s3, s4, s5, s6) \
  (sqrt((SQ((s1) - (s2)) + SQ((s1) - (s3)) + SQ((s2) - (s3)) + 6.0 * (SQ(s4) + SQ(s5) + SQ(s6))) / 2.0)) //!< Returns the equivalent von-Mises stress with (s1,s2,s3) the normal stresses and (s4,s5,s6) the shear stresses : \f[ s = \frac {1}{\sqrt{2}}\sqrt{(s_{11}-s_{22})^2+(s_{22}-s_{33})^2+(s_{33}-s_{11})^2+6(s_{12}^2+s_{23}^2+s_{31}^2)}\f]

// Interpolation lineaire entre deux points
#define LINEARdbl(x1, x2, dx) ((dx) * (x2) + (1.0 - (dx)) * (x1)) //!< Returns the interpolated value between 2 points from the following equation: \f[ x=\lambda x_{1}+(1-\lambda)x_{2} \f]

// Carre d'une valeur
#define SQ(a) ((a) * (a)) //!< Returns the square value of the given argument from the following \f[ x^2 =x*x \f]

// fonction qui cherche si on est entre 2 valeurs
#define betweenValues(a, b, c) ((a) < (b) ? False : ((a) > (c) ? False : True)) //!< Returns \ref True if the given parameter a is within the range [b,c], False if not

// fonction qui cherche si on est entre 2 valeurs
#define betweenValuesExclude(a, b, c) ((a) <= (b) ? False : ((a) >= (c) ? False : True)) //!< Returns \ref True if the given parameter a is within the range ]b,c[, False if not

// fonction qui recherche une valeur dans un tenseur
#define tensorInd2(i, j, m) ((i) * (m) + (j)) //!< Returns the value of T(i,j) in a second order tensor of dimension m

#define tensorInd3(i, j, k, m) ((i) * (m * m) + (j) * (m) + k) //!< Returns the value of T(i,j,k) in a third order tensor of dimension m

#define tensorInd4(i, j, k, l, m) ((i) * (m * m * m) + (j) * (m * m) + k * (m) + l) //!< Returns the value of T(i,j,k,l) in a fourth order tensor of dimension m

#define kronecker(i, j) ((i) == (j) ? 1. : 0.) //!< Returns the Kronecker: (1) if i=j, (0) in all other cases

#endif
