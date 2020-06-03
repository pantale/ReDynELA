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

// begin date : 13/03/1997

/*
  Class ElTri3n2D definition
*/

#ifndef __ElTri3n2D_h__
#define __ElTri3n2D_h__

#define Name_ElTri3n2D "ElTri3n2D"

class ElementPlane;
#include <ElementPlane.h>

/** Classe g�n�rique �l�ments.
Classe g�n�rique de d�finition et de manipulation des �l�ments du maillage. Cette classe sert de classe de base pour les classes d'�l�ments. Elle comporte les fonctions g�n�riques de manipulation de ces �lements.
see Domain, Node, IntegrationPoint
author Olivier PANTALE
version DynELA v. 0.9.0
*/
/** @dia:pos 55.8,206.8 */
/** @dia:route ElementPlane;v,49.95,199,201.85,67.95,206.8 */
class ElTri3n2D : public ElementPlane
{

public:
  /** @dia:route 3,18;h,55.8,207.5,52.75,220.85,19.75,145.7,41.8 */
  static const ElementData elementData;

public:
  /* constructeurs */
  ElTri3n2D(Indice No_ = 1);
  ElTri3n2D(const ElTri3n2D &el);
  ~ElTri3n2D();
  Indice numberOfUnderIntegrationPoints()
  {
    return 1;
  }
  //  void computeInternalMatrices (Boolean reference=False);

  Real getLength(); /* calcule et renvoie la longueur caracteristique */
  Real getVolume();
  // Boolean getUnderIntegrPointCoords (Indice, Vec3D & coords, Real& weight) ;
  void getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &N, const Vec3D &point);
  void computeGlob2Loc();
  void glob2Loc(const Vec3D &point, Vec3D &local);
  //void getIntgtoNodes (Vector & N, const Vec3D & point) const;
};

#endif
