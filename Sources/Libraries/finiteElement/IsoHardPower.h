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

// begin date : 14/03/1997

/*
	Class IsoHardPower definition
*/

#ifndef __IsoHardPower_h__
#define __IsoHardPower_h__

#include <upFemSolver.h>
#include <IsotropicHardening.h>

/*!
  \file IsoHardPower.h
  \brief fichier .h de d�finition d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#define lawName_IsoHardPower "Elasto-Plastic"
#define Name_IsoHardPower "IsoHardPower"

class Element;

/*!
  \class IsoHardPower
  \brief Classe de d�finition et de manipulation d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Cette classe sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA. La loi de comportement �lastoplastique permet de traduire une loi d'�coulement de la forme donn�e ci-dessous:
  \f[ \sigma=A+B\overline{\varepsilon ^{p}}^{n} \f]

  \see Material
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/
/** @dia:pos 2.8,55.8 */
/** @dia:route IsotropicHardening;v,50.8,25.4,51.05,14.6,55.8 */
class IsoHardPower : public IsotropicHardening
{
private:
  Real P[5]; // 4 + 1 pour indice commencant � 1

public:
  // constructeurs
  IsoHardPower();
  IsoHardPower(const IsoHardPower &mat);
  ~IsoHardPower();

  // fonctions membres
  void setParameter(Indice i, Real val);
  String convertToDynELASourceFile(String name);

  // fonction de calcul
  Real getYieldStress(IntegrationPoint *point);    // calcule la contrainte equivalente
  Real getDerYieldStress(IntegrationPoint *point); // calcule la contrainte equivalente
  Real getIsotropicYieldStress(Element *element, Real shift = 0.0);
  Real getIsotropicYieldHardening(Element *element, Real shift = 0.0);
  Boolean isYield()
  {
    return Yes;
  }

  const char *lawName()
  {
    return (lawName_IsoHardPower);
  }

  const char *Name()
  {
    return (Name_IsoHardPower);
  }

  Indice getType();
  Indice getNumberOfConstitutiveParameters();
  const char *getConstitutiveParameterName(Indice ind);
  Real &getConstitutiveParameter(Indice ind);

  // fonctions entree sortie
  void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, IsoHardPower &mat);
  void toFile(FILE *pfile);
  void plot(FILE *pfile, Real epsMax = 1);
};

#endif
