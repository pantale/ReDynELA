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
  Class IsoHardElastic definition
*/

#ifndef __IsoHardElastic_h__
#define __IsoHardElastic_h__

#include <Material.h>
#include <Element.h>
#include <IsotropicHardening.h>

/*!
  \file IsoHardElastic.h
  \brief fichier .h de d�finition d'un mat�riau �lastique
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lastique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#define lawName_IsoHardElastic "Elastic"
#define Name_IsoHardElastic "IsoHardElastic"

/*!
  \class IsoHardElastic
  \brief Classe de d�finition et de manipulation d'un mat�riau �lastique
  \ingroup femLibrary

  Cette classe sert � la d�finition d'un mat�riau de type �lastique sur DynELA.

  \see Material
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/
/** @dia:pos 53,30 */
/** @dia:route IsotropicHardening;v,50.8,25.4,27.05,64.8,30 */
class IsoHardElastic : public IsotropicHardening
{

  Real dummyReal;

public:
  /* constructeurs */
  IsoHardElastic(/*Indice No_ = 1*/);
  IsoHardElastic(const IsoHardElastic &X);
  ~IsoHardElastic();

  Boolean isYield()
  {
    return No;
  }
  //  Boolean isLinear() {return Yes;}
  Real getIsotropicYieldStress(Element *element, Real shift = 0.0)
  {
    cout << element << shift << endl;
    cerr << "error to call getDerYieldStress cause not yield\n";
    return 0.;
  }
  Real getIsotropicYieldHardening(Element *element, Real shift = 0.0)
  {
    cout << element << shift << endl;
    cerr << "error to call getDerYieldStress cause not yield\n";
    return 0.;
  }
  Real getDerYieldStress(IntegrationPoint *pfield)
  {
    cout << pfield << endl;
    cerr << "error to call getDerYieldStress cause not yield\n";
    return 0.;
  }
  Real getYieldStress(IntegrationPoint *pfield)
  {
    cout << pfield << endl;
    cerr << "error to call getYieldStress cause not yield\n";
    return 0.;
  }

  /* fonctions membres */
  Indice getNumberOfConstitutiveParameters()
  {
    return 0;
  }
  const char *getConstitutiveParameterName(Indice ind);
  Real &getConstitutiveParameter(Indice ind);

  const char *lawName()
  {
    return (lawName_IsoHardElastic);
  }
  const char *Name()
  {
    return (Name_IsoHardElastic);
  }
  void print(ostream &os) const;
  Indice getType();
  void toFile(FILE *pfile);
  String convertToDynELASourceFile(String name);
  void plot(FILE *pfile, Real epsMax = 1);
};

#endif
