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
  Class IsotropicHardening definition
*/

#ifndef __IsotropicHardening_h__
#define __IsotropicHardening_h__

#include <upFemSolver.h>

class ElementSet;

/*!
  \file IsotropicHardening.h
  \brief fichier .h de d�finition de la classe g�nerique des mat�riaux
  \ingroup femLibrary

  Ce fichier sert � la d�finition la classe g�nerique des mat�riaux.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#define lawName_IsotropicHardening "generic material"
#define Name_IsotropicHardening "IsotropicHardening"

class IntegrationPoint;
class Element;

/*!
  \brief Classe virtuelle de gestion des mat�riaux
  \ingroup femLibrary

  Classe virtuelle de gestion des mat�riaux pour DynELA. Cette classe regroupe les param�tres de base communs � tous les types de mat�riaux sur DynELA, comme le module de Young, le coefficient de Poisson, ou la densit� d'un mat�riau. Pour pouvoir utiliser un mat�riau sur DynELA, on utilise alors une des classes d�riv�es de celle-ci, ce qui permet alors de d�finir un comportement �lastique, �lasto-plastique, ou tout autre type de comportement.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
/** @dia:pos 39,6 */
class IsotropicHardening
{
public:
  enum
  {
    Virtual = 0,
    Elastic,
    ElastoPlastic,
    ElastoPlasticTabular,
    Metafor,
    Power,
    JohnsonCook
  };

public:
  /** @dia:route 0,2;h,-118.265,-65.4794,122.315,-172.85,-95.25 */
  String name;

protected:
  Boolean is_Linear;

public:
  IsotropicHardening();
  IsotropicHardening(const IsotropicHardening &X);
  virtual ~IsotropicHardening();

  void plotToFile(String name, Real epsMax = 5);

  /* fonctions virtuelles */
  virtual Indice getType() = 0;
  virtual const char *lawName() { return lawName_IsotropicHardening; }
  virtual const char *Name() { return Name_IsotropicHardening; }
  virtual Indice getNumberOfConstitutiveParameters() = 0;
  virtual const char *getConstitutiveParameterName(Indice ind) = 0;
  virtual Real &getConstitutiveParameter(Indice ind) = 0;
  virtual Real getYieldStress(IntegrationPoint *pfield) = 0;
  virtual Real getDerYieldStress(IntegrationPoint *pfield) = 0;
  virtual Real getIsotropicYieldStress(Element *, Real shift = 0.0) = 0;
  virtual Real getIsotropicYieldHardening(Element *, Real shift = 0.0) = 0;
  virtual Boolean isYield() = 0;
  virtual void plot(FILE *pfile, Real epsMax = 10) = 0;
  Boolean isLinear()
  {
    return is_Linear;
  }
  virtual void print(ostream &os) const = 0;
  virtual void toFile(FILE *pfile) = 0;

  /*   void compute_C (); */
  /*   void checkValues (); */
  /*   Matrix getHookeMatrix(short type); */
  virtual String convertToDynELASourceFile(String name);

  /* fonctions entree sortie */
  /*   friend ostream & operator << (ostream & os, IsotropicHardening & mat); */
  /*   virtual void toFile (FILE * pfile) = 0; */
};

#endif
