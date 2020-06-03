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

// begin date : 14/03/1997

/*
	Class IsoHardMetafor definition
*/

#ifndef __IsoHardMetafor_h__
#define __IsoHardMetafor_h__

#include <upFemSolver.h>
#include <Material.h>
#include <IsotropicHardening.h>

/*!
  \file IsoHardMetafor.h
  \brief fichier .h de d�finition d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#define lawName_IsoHardMetafor "Elasto-Plastic"
#define Name_IsoHardMetafor "IsoHardMetafor"

class Element;

/*!
  \class IsoHardMetafor
  \brief Classe de d�finition et de manipulation d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Cette classe sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA. La loi de comportement �lastoplastique permet de traduire une loi d'�coulement de la forme donn�e ci-dessous:
  \f[ \sigma=A+B\overline{\varepsilon ^{p}}^{n} \f]

  \see Material
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/
/** @dia:pos 24,30 */
/** @dia:route IsotropicHardening;v,50.8,25.4,27.05,35.8,30 */
class IsoHardMetafor : public IsotropicHardening
{
private:
  Real P[9]; // 8 + 1 pour indice commencant � 1

public:
  // constructeurs
  IsoHardMetafor();
  IsoHardMetafor(const IsoHardMetafor &mat);
  ~IsoHardMetafor();

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
    return (lawName_IsoHardMetafor);
  }

  const char *Name()
  {
    return (Name_IsoHardMetafor);
  }

  Indice getType();
  Indice getNumberOfConstitutiveParameters();
  const char *getConstitutiveParameterName(Indice ind);
  Real &getConstitutiveParameter(Indice ind);

  // fonctions entree sortie
  void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, IsoHardMetafor &mat);
  void toFile(FILE *pfile);
  void plot(FILE *pfile, Real epsMax = 1);
};

#endif
