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
// revision date : 19/03/1999

/*
  Class Element2D definition
*/

#ifndef __Element2D_h__
#define __Element2D_h__

#include <Element.h>
class Element;

/**Plane element base class definition.
This class implements a base class for plane elements (i.e 2D elements). This class serves as a base class for all 2D elements in DynELA.
see Domain, Node, IntegrationPoint
author Olivier PANTALE
version DynELA v. 0.9.0
*/
/** @dia:pos 91.8003,160.8 */
/** @dia:route Element;v,136.265,148.079,150.65,103.95,160.8 */
class Element2D : public Element
{

public:
  /**
     name Constructors and destructor
  */
  //{
  /**.Default constructor*/
  Element2D(Indice No_ = 1);

  /**.Copy constructor*/
  Element2D(const Element2D &X);

  /**.Destructor*/
  ~Element2D();
  //}

  void getRadius_atIntPoint(Real &R);
  void getU_atIntPoint(Vec3D &u, int t);
  void getV_atIntPoint(Vec3D &v, int t);
  Indice numberOfUnderIntegrationPoints() = 0;
  void computeInternalMatrices(Boolean reference = False);
  void computeElasticStiffnessMatrix(Matrix &K, Boolean under = True);
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
