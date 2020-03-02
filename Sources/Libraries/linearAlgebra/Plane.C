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
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

// begin date : 
// revision date : 

/*
	Class Plane implementation
*/

/*!
  \file Plane.C
  \brief fichier .C de définition et de manipulation de plans 3D
  \ingroup linearAlgebra

  Ce fichier définit les méthodes permettant de gérer des plans dans une espace cartésien à 3 dimensions. Ces méthodes sont trèss simple, et cette classe n'est pas tres complète.

  \author Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004  
*/


#include <Plane.h>

//!constructeur par défaut de la classe Plane
/*!
  Ce constructeur alloue la mémoire pour un plan
  \author &copy; Olivier PANTALE
  \since 0.9.5
*/
//-----------------------------------------------------------------------------
Plane::Plane ()
//-----------------------------------------------------------------------------
{
  composante.redim (4);
}

//!constructeur par recopie
//-----------------------------------------------------------------------------
Plane::Plane (const Plane & X)
//-----------------------------------------------------------------------------
{
}

//!destructeur
//-----------------------------------------------------------------------------
Plane::~Plane ()
//-----------------------------------------------------------------------------
{
}

//!définit un plan
/*!
  Cette méthode permet de définir un plan passant par trois points de l'espace. Le plan ainsi construit est tel que la normale unitaire extérieure est définie par le produit vectoriel des vecteurs

  \f[
  \left\{ \begin{array}{l}
  \overrightarrow{v_{1}}=\overrightarrow{p_{2}}-\overrightarrow{p_{1}}\\
  \overrightarrow{v_{2}}=\overrightarrow{p_{3}}-\overrightarrow{p_{1}}
  \end{array}\right. 
  \f]

  et la normale extérieure est définie par

  \f[
  \overrightarrow{n}=\overrightarrow{v_{1}}\overrightarrow{\wedge v_{1}}
  \f]
  \param p1 le premier point
  \param p2 le deuxième point
  \param p3 le troisième point
  \author &copy; Olivier PANTALE
  \since 0.9.5
*/
//-----------------------------------------------------------------------------
void
Plane::set (Vec3D p1, Vec3D p2, Vec3D p3)
//-----------------------------------------------------------------------------
{
  Vec3D v1, v2, v;

  /* calcul des deux vecteurs */
  v1 = p2 - p1;
  v2 = p3 - p1;

  /* calcul du produit vectoriel pour le vecteur normal du plan */
  v = v1.vectorial (v2);

  /* normalisation du vecteur normal */
  v.normalize ();

  /* equation du plan */
  composante (0) = v (0);
  composante (1) = v (1);
  composante (2) = v (2);
  composante (3) = -(v.dot (p1));
}

//!inverse la normale du plan
/*!
  Cette méthode inverse le sens de la normale définissant le plan.
  \author &copy; Olivier PANTALE
  \since 0.9.5
*/
//-----------------------------------------------------------------------------
void
Plane::inverse ()
//-----------------------------------------------------------------------------
{
  composante (0) = -composante (0);
  composante (1) = -composante (1);
  composante (2) = -composante (2);
  composante (3) = -composante (3);
}

//!translate le plan suivant un vecteur
/*!
  Cette méthode translate le plan suivant un vecteur défini en argument
  \param vect vecteur de translation
  \author &copy; Olivier PANTALE
  \since 0.9.5
*/
//-----------------------------------------------------------------------------
void
Plane::translate (Vec3D vect)
//-----------------------------------------------------------------------------
{
  composante (3) =
    -(composante (0) * vect (0) + composante (1) * vect (1) +
      composante (2) * vect (2));
}

//-----------------------------------------------------------------------------
Real
Plane::distance(Vec3D point)
//-----------------------------------------------------------------------------
{
  return (composante(0)*point(0)+composante(1)*point(1)+composante(2)*point(2)+composante(3));
}

//-----------------------------------------------------------------------------
Vec3D Plane::projection(const Vec3D &p1)
//-----------------------------------------------------------------------------
{
  Vec3D proj;
  proj=p1 - distance(p1) * normal();

  return proj;
}

//-----------------------------------------------------------------------------
Boolean
Plane::isOnPositiveSide(Vec3D point)
//-----------------------------------------------------------------------------
{
  return (distance(point) >=0);
}

//-----------------------------------------------------------------------------
Vec3D Plane::intersection(const Vec3D &p1,const Vec3D &p2)
//-----------------------------------------------------------------------------
{
  Vec3D intersect;
  Real s1,s2;
  s1=Abs(distance(p1));
  s2=Abs(distance(p2));
  intersect = p1 + (s1/(s1+s2))*(p2-p1);

  return intersect;
}

//-----------------------------------------------------------------------------
Vec3D Plane::normal()
//-----------------------------------------------------------------------------
{
  Vec3D normal;
  normal(0)=composante(0);
  normal(1)=composante(1);
  normal(2)=composante(2);

  return normal;
}

