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

// begin date : 19/03/1997

/*
  Class ElementAx definition
*/

#ifndef __ElementAx_h__
#define __ElementAx_h__

#include <Element2D.h>
class Element2D;

/**
*/
/** @dia:pos 84.8003,185.8 */
/** @dia:route Element2D;v,103.95,174.8,180.851,96.9503,185.8 */
class ElementAx : public Element2D
{

public:
  //Constructor
  ElementAx(Indice No_ = 1);
  //copy constructor
  ElementAx(const ElementAx &X);
  //Destructor
  ~ElementAx();

  void getdU_atIntPoint(Tensor2 &du, int t);
  void getF_atIntPoint(Tensor2 &du, int t);
  void getdV_atIntPoint(Tensor2 &dv, int t);
  Boolean checkLevel2();
  Indice numberOfUnderIntegrationPoints() = 0;
  Real getLength() = 0;
  Real getVolume() = 0;
  // Boolean getUnderIntegrPointCoords (Indice, Vec3D & coords, Real& weight) = 0;
  void getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const = 0;
  void getDerShapeFunctionAtPoint(Matrix &N, const Vec3D &point) = 0;
  //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;
  void computeGlob2Loc() = 0;
  void glob2Loc(const Vec3D &point, Vec3D &local) = 0;
};

#endif
