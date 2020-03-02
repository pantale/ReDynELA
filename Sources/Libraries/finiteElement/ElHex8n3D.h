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

// begin date : 13/03/1997

/*
  Class ElHex8n3D definition
*/

//#ifndef __Element_h__

#ifndef __ElHex8n3D_h__
#define __ElHex8n3D_h__

#define Name_ElHex8n3D "ElHex8n3D"

#include <Element3D.h>
class Element3D;

/**
*/
/** @dia:pos 115.8,189.8 */
/** @dia:route Element3D;v,147.95,178,184.851,127.95,189.8 */
class ElHex8n3D : public Element3D
{

public:
  /** @dia:route 3,14;h,115.8,190.5,113.75,222.85,17.75,144.1,41.8 */
  static const ElementData elementData;

public:
  // constructeurs
  ElHex8n3D(Indice No_ = 1);
  ElHex8n3D(const ElHex8n3D &el);
  ~ElHex8n3D();

  Indice numberOfUnderIntegrationPoints()
  {
    return 1;
  }
  //  void computeInternalMatrices (Boolean reference=False);

  // fonctions de calcul
  Real getLength(); // calcule et renvoie la longueur caracteristique
  Real getVolume();
  // Boolean getUnderIntegrPointCoords (Indice, Vec3D & coords, Real& weight) ;
  void getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &N, const Vec3D &point);
  void computeGlob2Loc();
  void glob2Loc(const Vec3D &point, Vec3D &local);
  //void getIntgtoNodes (Vector & N, const Vec3D & point) const;

  // fonctions entree sortie
  void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, ElHex8n3D &element);
};

#endif
