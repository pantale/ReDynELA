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
	Class IsoHardElastoplastic definition
*/

#ifndef __IsoHardElastoplastic_h__
#define __IsoHardElastoplastic_h__

#include <upFemSolver.h>
#include <Material.h>
#include <IsotropicHardening.h>

/*!
  \file IsoHardElastoplastic.h
  \brief fichier .h de d�finition d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#define lawName_IsoHardElastoplastic "Elasto-Plastic"
#define Name_IsoHardElastoplastic "IsoHardElastoplastic"

class Element;

/*!
  \class IsoHardElastoplastic
  \brief Classe de d�finition et de manipulation d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Cette classe sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA. La loi de comportement �lastoplastique permet de traduire une loi d'�coulement de la forme donn�e ci-dessous:
  \f[ \sigma=A+B\overline{\varepsilon ^{p}}^{n} \f]

  \see Material
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/
/** @dia:pos 30.8,55.8 */
/** @dia:route IsotropicHardening;v,50.8,25.4,51.05,42.6,55.8 */
class IsoHardElastoplastic : public IsotropicHardening
{
protected:
  Real A, B, n;
  //  Real EpsM, SigM;

public:
  // constructeurs
  IsoHardElastoplastic(/*Indice No_ = 1*/);
  IsoHardElastoplastic(const IsoHardElastoplastic &mat);
  ~IsoHardElastoplastic();

  // fonctions membres
  Real &Yield_Stress();
  Real &Hard_Param();
  Real &Hard_Exponent();
  Real &Fail_Strain();
  Real &Max_Stress();

  // m�thodes de renseignement
  void setYieldStress(Real val);
  void setHardParameter(Real val);
  void setHardExponent(Real val);
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
    return (lawName_IsoHardElastoplastic);
  }

  const char *Name()
  {
    return (Name_IsoHardElastoplastic);
  }

  Indice getType();
  Indice getNumberOfConstitutiveParameters();
  const char *getConstitutiveParameterName(Indice ind);
  Real &getConstitutiveParameter(Indice ind);

  // fonctions entree sortie
  /*virtual */ void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, IsoHardElastoplastic &mat);
  void toFile(FILE *pfile);
  void plot(FILE *pfile, Real epsMax = 1);
};

#endif
