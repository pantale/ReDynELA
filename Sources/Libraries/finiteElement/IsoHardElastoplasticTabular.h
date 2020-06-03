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
	Class IsoHardElastoplasticTabular definition
*/

#ifndef __IsoHardElastoplasticTabular_h__
#define __IsoHardElastoplasticTabular_h__

#include <upFemSolver.h>
#include <Material.h>
#include <IsotropicHardening.h>
//class NodeMotion;

/*!
  \file IsoHardElastoplasticTabular.h
  \brief fichier .h de d�finition d'un mat�riau �lasto-plastique tabul�
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique tabul� sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#define lawName_IsoHardElastoplasticTabular "Elasto-Plastic Tabular"
#define Name_IsoHardElastoplasticTabular "IsoHardElastoplasticTabular"

/* #include <Material.h> */
class Element;

/*!
  \class IsoHardElastoplasticTabular
  \brief Classe de d�finition et de manipulation d'un mat�riau �lasto-plastique tabul�
  \ingroup femLibrary

  Cette classe sert � la d�finition d'un mat�riau de type �lasto-plastique tabul� sur DynELA. La loi de comportement �lastoplastique tabul�e permet de traduire toutes les formes de loi d'�coulement de la forme donn�e ci-dessous:
  \f[ \sigma_{s}=f(\overline{\varepsilon^{p}}) \f]

  \see Material
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/
/** @dia:pos 57.8003,55.8 */
/** @dia:route IsotropicHardening;v,50.8,25.4,51.05,69.9503,55.8 */
class IsoHardElastoplasticTabular : public IsotropicHardening
{
protected:
  /** @dia:route 0,2;h,90.8,-186.35,276,-16.4794,-74.9647 */
  DiscreteFunction *tabul;
  Real valeurBidon;

public:
  // constructeurs
  IsoHardElastoplasticTabular(/*Indice No_ = 1*/);
  IsoHardElastoplasticTabular(const IsoHardElastoplasticTabular &mat);
  ~IsoHardElastoplasticTabular();

  // fonction de calcul
  Real getYieldStress(IntegrationPoint *point);    // calcule la contrainte equivalente
  Real getDerYieldStress(IntegrationPoint *point); // calcule la contrainte equivalente
  Real getIsotropicYieldStress(Element *element, Real shift = 0.0);
  Real getIsotropicYieldHardening(Element *element, Real shift = 0.0);
  Boolean isYield()
  {
    return Yes;
  }
  void setFunction(DiscreteFunction *fu);
  DiscreteFunction *getFunction()
  {
    return (tabul);
  }
  String getFunctionName()
  {
    return tabul->getName();
  }

  const char *lawName()
  {
    return (lawName_IsoHardElastoplasticTabular);
  }
  const char *Name()
  {
    return (Name_IsoHardElastoplasticTabular);
  }
  String convertToDynELASourceFile(String name);
  Indice getType();
  Indice getNumberOfConstitutiveParameters();
  const char *getConstitutiveParameterName(Indice ind);
  Real &getConstitutiveParameter(Indice ind);

  // fonctions entree sortie
  void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, IsoHardElastoplasticTabular &mat);
  void toFile(FILE *pfile);
  void plot(FILE *pfile, Real epsMax = 1);
};

#endif
