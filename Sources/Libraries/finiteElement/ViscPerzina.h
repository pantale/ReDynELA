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
	Class ViscPerzina definition
*/

#ifndef __ViscPerzina_h__
#define __ViscPerzina_h__

#include <upFemSolver.h>
#include <Material.h>
#include <Viscosity.h>

/*!
  \file ViscPerzina.h
  \brief fichier .h de d�finition d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#define lawName_ViscPerzina "Elasto-Plastic"
#define Name_ViscPerzina "ViscPerzina"

class Element;

/*!
  \class ViscPerzina
  \brief Classe de d�finition et de manipulation d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Cette classe sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA. La loi de comportement �lastoplastique permet de traduire une loi d'�coulement de la forme donn�e ci-dessous:
  \f[ \sigma=A+B\overline{\varepsilon ^{p}}^{n} \f]

  \see Material
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/
/** @dia:pos 3.8,22.8 */
/** @dia:route Viscosity;v,14.05,16.2,18.25,12.45,22.8 */
class ViscPerzina : public Viscosity
{
private:
  Real K;
  Real m;
  Real n;

public:
  // constructeurs
  ViscPerzina();
  ViscPerzina(const ViscPerzina &mat);
  ~ViscPerzina();

  // fonctions membres
  //  void setParameter(Indice i, Real val);
  String convertToDynELASourceFile(String name);

  // fonction de calcul
  Real getViscousStress(Element *, Real shift = 0.0);

  const char *lawName()
  {
    return (lawName_ViscPerzina);
  }

  const char *Name()
  {
    return (Name_ViscPerzina);
  }

  Indice getType();
  Indice getNumberOfConstitutiveParameters();
  const char *getConstitutiveParameterName(Indice ind);
  Real &getConstitutiveParameter(Indice ind);

  // fonctions entree sortie
  void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, ViscPerzina &mat);
  void toFile(FILE *pfile);
  void plot(FILE *pfile, Real epsMax = 1);
};

#endif
