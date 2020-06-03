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
  Class ElQua4n2D definition
*/

#ifndef __ElQua4n2D_h__
#define __ElQua4n2D_h__

#define Name_ElQua4n2D "ElQua4n2D"

class ElementPlane;
#include <ElementPlane.h>

/** Classe g�n�rique �l�ments.
Classe g�n�rique de d�finition et de manipulation des �l�ments du maillage. Cette classe sert de classe de base pour les classes d'�l�ments. Elle comporte les fonctions g�n�riques de manipulation de ces �lements.
see Domain, Node, IntegrationPoint
author Olivier PANTALE
version DynELA v. 0.9.0
*/
/** @dia:pos 24.8,206.8 */
/** @dia:route ElementPlane;v,49.95,199,201.85,36.95,206.8 */
class ElQua4n2D : public ElementPlane
{

public:
  /** @dia:route 3,20;h,24.8,207.5,20.75,146.5,41.8 */
  static const ElementData elementData;

public:
  /* constructeurs */
  ElQua4n2D(Indice No_ = 1);
  ElQua4n2D(const ElQua4n2D &el);
  ~ElQua4n2D();

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
/*>ElQua4n2D:*/
#endif
