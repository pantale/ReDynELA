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
  Class Material definition
*/

#ifndef __Material_h__
#define __Material_h__

#include <upFemSolver.h>

class ElementSet;

/*!
  \file Material.h
  \brief fichier .h de d�finition de la classe g�nerique des mat�riaux
  \ingroup femLibrary

  Ce fichier sert � la d�finition la classe g�nerique des mat�riaux.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#define lawName_Material "generic material"
#define Name_Material "Material"

class IntegrationPoint;
class Element;
class IsotropicHardening;
class Viscosity;

/*!
  \brief Classe virtuelle de gestion des mat�riaux
  \ingroup femLibrary

  Classe virtuelle de gestion des mat�riaux pour DynELA. Cette classe regroupe les param�tres de base communs � tous les types de mat�riaux sur DynELA, comme le module de Young, le coefficient de Poisson, ou la densit� d'un mat�riau. Pour pouvoir utiliser un mat�riau sur DynELA, on utilise alors une des classes d�riv�es de celle-ci, ce qui permet alors de d�finir un comportement �lastique, �lasto-plastique, ou tout autre type de comportement.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
/** @dia:pos 83.065,0.2794 */
class Material
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
  enum
  {
    planeStrain,
    planeStress,
    axisymetric,
    threedimensional
  };

public:
  /** @dia:route 0,2;h,-74.2647,-70.4794,78.3147,-172.85,-95.25 */
  String name;
  /** @dia:route 16,3;h,183.8,110.1,179.75,226.05,1.75,0.9794,83.065 */
  List<ElementSet *> elementsSet; //!< Liste d'�lements qui r�f�rencent ce mat�riau
  /** @dia:route 0,2;h,-74.2647,-70.4794,78.3147,-168.35,165.05 */
  Vec3D color; //!< Couleur du mat�riau. Vecteur � 3 composantes sp�cifiant la valeur des canaux rouge, vert et bleu respectivement dans l'intervalle [0-1].

protected:
  /** @dia:route 0,2;h,-74.2647,-70.4794,78.3147,-184.4,76.15 */
  Tensor4 C;  //!< Tenseur de comportement �lastique du 4�me ordre (loi de Hooke)
  Real E, Nu; /* elasticity */
  Real ro;    /* density */
  Real Cp;
  Real alpha;        /* dilatation */
  Real T0;           /* reference temperature */
  Real l;            /* thermal conductivity */
  Boolean is_Linear; /* boolean value */
  /** @dia:route 9,18;h,62.6,7.9,70.7503,6.1794,83.065 */
  IsotropicHardening *isotropicHardening;
  /** @dia:route 10,2;h,83.065,2.9794,71.7503,4,24.1 */
  Viscosity *viscosity;

public:
  /* constructeurs */
  Material(/*Indice No_ = 1*/);
  Material(const Material &X);
  virtual ~Material();

  /* access aux donnees */
  inline Real &heatCoeff();
  inline Real &dilat();
  inline Real &initTemp();
  inline Real &conductivity();
  inline Real &young();
  inline Real &nu();
  inline Real &density();
  inline Real mu();

  // acces aux donn�es pour l'interpr�teur
  void setHeatCoefficient(Real val);
  void setDilatation(Real val);
  void setInitTemperature(Real val);
  void setConductivity(Real val);
  void setYoung(Real val);
  void setPoisson(Real val);
  void setDensity(Real val);
  void setColor(Vec3D color);
  void setColor(Real red, Real green, Real blue);
  void setHardening(IsotropicHardening *hard);

  // acc�s aux �crouissages
  IsotropicHardening *getIsotropicHardening();

  /* fonctions virtuelles */
  Real getYieldStress(IntegrationPoint *pfield);
  Real getDerYieldStress(IntegrationPoint *pfield);
  Real getIsotropicYieldStress(Element *, Real shift = 0.0);
  Real getIsotropicYieldHardening(Element *, Real shift = 0.0);
  Boolean isYield();

  Boolean isLinear()
  {
    return is_Linear;
  }
  void compute_C();
  void checkValues();
  Matrix getHookeMatrix(short type);
  virtual String convertToDynELASourceFile(String name);

  /* fonctions entree sortie */
  virtual void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, Material &mat);
  void toFile(FILE *pfile);
};

//-----------------------------------------------------------------------------
inline Real &
Material::young()
//-----------------------------------------------------------------------------
{
  return E;
}

//-----------------------------------------------------------------------------
inline Real
Material::mu()
//-----------------------------------------------------------------------------
{
  return E / (2. * (1. + Nu));
}

//-----------------------------------------------------------------------------
inline Real &
Material::nu()
//-----------------------------------------------------------------------------
{
  return Nu;
}

//-----------------------------------------------------------------------------
inline Real &
Material::density()
//-----------------------------------------------------------------------------
{
  return ro;
}

//-----------------------------------------------------------------------------
inline Real &
Material::heatCoeff()
//-----------------------------------------------------------------------------
{
  return Cp;
}

//-----------------------------------------------------------------------------
inline Real &
Material::dilat()
//-----------------------------------------------------------------------------
{
  return alpha;
}

//-----------------------------------------------------------------------------
inline Real &
Material::initTemp()
//-----------------------------------------------------------------------------
{
  return T0;
}

//-----------------------------------------------------------------------------
inline Real &
Material::conductivity()
//-----------------------------------------------------------------------------
{
  return l;
}

#endif
