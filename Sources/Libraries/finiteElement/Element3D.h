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

// begin date : 19/03/1997

/*
  Class Element3D definition
*/

#ifndef __Element3D_h__
#define __Element3D_h__

#include <Element.h>
class Element;

/*<Element3D:*/
/**
*/
/** @dia:pos 135.8,160.8 */
/** @dia:route Element;v,136.265,148.079,150.65,147.95,160.8 */
class Element3D : public Element
{

public:
  // constructeurs
  Element3D(Indice No_ = 1);
  Element3D(const Element3D &X);
  ~Element3D();
  Indice numberOfUnderIntegrationPoints() = 0;
  Boolean checkLevel2();
  Real getLength() = 0;
  Real getVolume() = 0;
  // Boolean getUnderIntegrPointCoords (Indice, Vec3D & coords, Real& weight) = 0;
  void getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const = 0;
  void getDerShapeFunctionAtPoint(Matrix &N, const Vec3D &point) = 0;
  //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;
  void computeGlob2Loc() = 0;
  void glob2Loc(const Vec3D &point, Vec3D &local) = 0;

  void computeInternalMatrices(Boolean reference = False);
  void computeElasticStiffnessMatrix(Matrix &K, Boolean under = True);
  void getU_atIntPoint(Vec3D &u, int t);
  void getRadius_atIntPoint(Real &R)
  {
  }
  void getV_atIntPoint(Vec3D &v, int t);
  void getdU_atIntPoint(Tensor2 &du, int t);
  void getF_atIntPoint(Tensor2 &du, int t);
  void getdV_atIntPoint(Tensor2 &dv, int t);
};

/*>Element3D:*/

#endif
