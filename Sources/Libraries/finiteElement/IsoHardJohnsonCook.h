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
	Class IsoHardJohnsonCook definition
*/

#ifndef __IsoHardJohnsonCook_h__
#define __IsoHardJohnsonCook_h__

#include <upFemSolver.h>
#include <Material.h>
#include <IsotropicHardening.h>

/*!
  \file IsoHardJohnsonCook.h
  \brief fichier .h de d�finition d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#define lawName_IsoHardJohnsonCook "Johnson-Cook"
#define Name_IsoHardJohnsonCook "IsoHardJohnsonCook"

class Element;

/*!
  \class IsoHardJohnsonCook
  \brief Johnson-Cook hardening law.
  
  The general formulation of the Johnson-Cook constitutive law have the following form:
  \f[ \sigma^{y}=\left(A+B\overline{\varepsilon^{p}}^{n}\right)\left[1+C\ln\left(\frac{\stackrel{\bullet}{\overline{\varepsilon^{p}}}}{\stackrel{\bullet}{\overline{\varepsilon_{0}}}}\right)\right]\left[1-\left(\frac{T-T_{0}}{T_{m}-T_{0}}\right)^{m}\right] \f]

  In this approach, a modified version is used to compute the yield stress of the material at the current point:
  \f[ \sigma^{y}=\left(A+B\overline{\varepsilon^{p}}^{n}\right)\sigma_{\varepsilon^{p}}^{y}(\stackrel{\bullet}{\overline{\varepsilon^{p}}})\sigma_{T}^{y}(T) \f]

  where the dependence on deformation rate \f$ \sigma_{\varepsilon^{p}}^{y}(\stackrel{\bullet}{\overline{\varepsilon^{p}}}) \f$
  is defined by the following: 
  \f[ \left\{ \begin{array}{lll}
  \sigma_{\varepsilon^{p}}^{y}(\stackrel{\bullet}{\overline{\varepsilon^{p}}})=1+C\ln\left(\frac{\stackrel{\bullet}{\overline{\varepsilon^{p}}}}{\stackrel{\bullet}{\overline{\varepsilon_{0}}}}\right) & \mbox{if} & \stackrel{\bullet}{\overline{\varepsilon^{p}}}\geq\stackrel{\bullet}{\overline{\varepsilon_{0}}}\\
  \sigma_{\varepsilon^{p}}^{y}(\stackrel{\bullet}{\overline{\varepsilon^{p}}})=1 & \mbox{if} & \stackrel{\bullet}{\overline{\varepsilon^{p}}}<\stackrel{\bullet}{\overline{\varepsilon_{0}}}
  \end{array}\right.\f]
  and the dependence on temperature \f$ \sigma_{T}^{y}(T)\f$ is defined by the following:
  \f[ \left\{ \begin{array}{lll}
  \sigma_{T}^{y}(T)=1-\left(\frac{T-T_{0}}{T_{m}-T_{0}}\right)^{m} & \mbox{if} & T_{0}\leq T\leq T_{m}\\
  \sigma_{T}^{y}(T)=1 & \mbox{if} & T<T_{0}\\
  \sigma_{T}^{y}(T)=0 & \mbox{if} & T\geq T_{m}
  \end{array}\right.\f]
 
  \ingroup femLibrary
  \see Material
  \author &copy; Olivier PANTALE
  \since DynELA 3.0.0
  \date 1997-2016
*/
class IsoHardJohnsonCook : public IsotropicHardening
{
protected:
  Real A, B, C, n, m, epsp0, T0, Tm;

public:
  // constructeurs
  IsoHardJohnsonCook(/*Indice No_ = 1*/);
  IsoHardJohnsonCook(const IsoHardJohnsonCook &mat);
  ~IsoHardJohnsonCook();

  // m�thodes de renseignement
  void defineLaw(Real par_A, Real par_B, Real par_C, Real par_n, Real par_m, Real par_epsp0, Real par_Tm, Real par_T0);
  String convertToDynELASourceFile(String name);

  // fonction de calcul
  Real getYieldStress(IntegrationPoint *point);    // calcule la contrainte equivalente
  Real getDerYieldStress(IntegrationPoint *point); // calcule la contrainte equivalente
  Real getIsotropicYieldStress(Element *element, Real shift = 0.0);
  Real getIsotropicYieldHardening(Element *element, Real shift = 0.0);
  Boolean isYield() { return Yes; }
  const char *lawName() { return (lawName_IsoHardJohnsonCook); }
  const char *Name() { return (Name_IsoHardJohnsonCook); }
  Indice getType();
  Indice getNumberOfConstitutiveParameters();
  const char *getConstitutiveParameterName(Indice ind);
  Real &getConstitutiveParameter(Indice ind);

  // fonctions entree sortie
  void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, IsoHardJohnsonCook &mat);
  void toFile(FILE *pfile);
  void plot(FILE *pfile, Real epsMax = 1);
};

#endif
