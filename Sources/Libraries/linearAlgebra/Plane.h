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

// begin date :
// revision date :

/*
  Class Plane definition
*/

#ifndef __Plane_h__
#define __Plane_h__
#include <Vec3D.h>
#include <Vector.h>

/*!
  \file Plane.h
  \brief fichier .h de d�finition et de manipulation de plans 3D
  \ingroup linearAlgebra

  Ce fichier d�finit les m�thodes permettant de g�rer des plans dans une espace cart�sien � 3 dimensions. Ces m�thodes sont tr�ss simple, et cette classe n'est pas tres compl�te.

  \author Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004  
*/

/*!
  \class Plane Plane.h
  \brief Classe de d�finition et de manipulation de plans 3D
  \ingroup linearAlgebra

  Cette classe d�finit les m�thodes permettant de g�rer des plans dans une espace cart�sien � 3 dimensions. Ces m�thodes sont tr�ss simple, et cette classe n'est pas tres compl�te.

  \author Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004  
*/
/** @dia:pos 176.4,-149.95 */
class Plane
{
public:
  /** @dia:route 4,2;h,195.45,-149.25,199.161,-193.55,195.85 */
  Vector composante;

public:
  // constructeurs
  Plane();
  Plane(const Plane &X);
  ~Plane();

  // fonctions membres
  void set(Vec3D p1, Vec3D p2, Vec3D p3);
  double *equation()
  {
    return &composante(0);
  } //!<renvoie le plan d�fini sous la forme d'une �quation ax+by+cz+d=0
  void inverse();
  void translate(Vec3D x);
  Boolean isOnPositiveSide(Vec3D);
  Real distance(Vec3D);
  Vec3D intersection(const Vec3D &p1, const Vec3D &p2);
  Vec3D normal();
  Vec3D projection(const Vec3D &p1);
};

#endif
