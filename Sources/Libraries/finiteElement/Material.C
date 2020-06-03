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

/*!
  \file Material.h
  \brief fichier .h de d�finition de la classe g�nerique des mat�riaux
  \ingroup femLibrary

  Ce fichier sert � la d�finition la classe g�nerique des mat�riaux.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#include <Material.h>
#include <IsotropicHardening.h>

//-----------------------------------------------------------------------------
Material::Material()
//-----------------------------------------------------------------------------
{
  isotropicHardening = NULL;
  viscosity = NULL;
  E = 0.0;
  Nu = 0.0;
  ro = 0.0;
  Cp = 0.0;
  alpha = 0.0;
  T0 = 0.0;
  l = 0.0;
}

//-----------------------------------------------------------------------------
Material::Material(const Material &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Material::~Material()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Material::checkValues()
//-----------------------------------------------------------------------------
{
  // check poisson value
  if ((Nu < 0.) || (Nu > 0.5))
    fatalError("Materials integrity check",
               "material %s have a poisson value of %lf\n", name.chars(),
               Nu);

  // check Young modulus
  if (E <= 0.)
    fatalError("Materials integrity check",
               "material %s have a young value of %lf\n", name.chars(), E);

  // check density
  if (ro <= 0.)
    fatalError("Materials integrity check",
               "material %s have a density value of %lf\n", name.chars(),
               ro);
}

//-----------------------------------------------------------------------------
void Material::compute_C()
//-----------------------------------------------------------------------------
{
  Indice i, j, k, l;
  Real mu, lambda;

  // definition de mu
  lambda = Nu * E / ((1. - 2. * Nu) * (1. + Nu));
  mu = E / (2. * (1. + Nu));

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
        for (l = 0; l < 3; l++)
        {
          C(i, j, k, l) = lambda * (kronecker(i, j) * kronecker(k, l)) + mu * (kronecker(i, k) * kronecker(j, l) + kronecker(i, l) * kronecker(j, k));
        }
}

//-----------------------------------------------------------------------------
void Material::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "material: " << name << endl;
  os << "E= " << E << endl;
  os << "nu=" << Nu << endl;
  os << "ro=" << ro << endl;
  os << "Cp=" << Cp << endl;
  os << "T0=" << T0 << endl;

  // complement d'�criture de la partie �crouissage isotrope
  if (isotropicHardening != NULL)
    isotropicHardening->print(os);
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, Material &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void Material::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  // fprintf(pfile,"MATERIAL %4d\n",);
  fprintf(pfile, "MATERIAL %s\n", name.chars());
  fprintf(pfile, "    YOUNG'S      POISSON'S    DENSITY\n");
  fprintf(pfile, "    MODULUS        RATIO\n");
  fprintf(pfile, "  %8.5E  %8.5E  %8.5E\n\n", E, Nu, ro);
  fprintf(pfile, "    INITIAL       HEAT      DILATATION     THERMAL\n");
  fprintf(pfile, "  TEMPERATURE   CAPACITY       COEFF     CONDUCTIVITY\n");
  fprintf(pfile, "  %8.5E  %8.5E  %8.5E  %8.5E\n\n", T0, Cp, alpha, l);

  // complement d'�criture de la partie �crouissage isotrope
  if (isotropicHardening != NULL)
    isotropicHardening->toFile(pfile);
}

//-----------------------------------------------------------------------------
void Material::setHeatCoefficient(Real val)
//-----------------------------------------------------------------------------
{
  Cp = val;
}

//-----------------------------------------------------------------------------
void Material::setDilatation(Real val)
//-----------------------------------------------------------------------------
{
  alpha = val;
}

//-----------------------------------------------------------------------------
void Material::setInitTemperature(Real val)
//-----------------------------------------------------------------------------
{
  T0 = val;
}

//-----------------------------------------------------------------------------
void Material::setConductivity(Real val)
//-----------------------------------------------------------------------------
{
  l = val;
}

//-----------------------------------------------------------------------------
void Material::setYoung(Real val)
//-----------------------------------------------------------------------------
{
  E = val;
}

//-----------------------------------------------------------------------------
void Material::setPoisson(Real val)
//-----------------------------------------------------------------------------
{
  Nu = val;
}

//-----------------------------------------------------------------------------
void Material::setDensity(Real val)
//-----------------------------------------------------------------------------
{
  ro = val;
}

//-----------------------------------------------------------------------------
void Material::setColor(Vec3D col)
//-----------------------------------------------------------------------------
{
  color = col;
}

//-----------------------------------------------------------------------------
void Material::setColor(Real red, Real green, Real blue)
//-----------------------------------------------------------------------------
{
  color(0) = red;
  color(1) = green;
  color(2) = blue;
}

/*//-----------------------------------------------------------------------------
void Material::plotToFile(String name, Real epsMax)
//-----------------------------------------------------------------------------
{
  FILE* pfile;

  pfile=fopen(name.chars(),"w");
  if (pfile==NULL)
    {
      CreateException(FileNotOpen exception,"toto");
      exception.print();
      cerr << "Unable to open file "<<name<<"\n";
      exit(-1);
    }  

  plot(pfile,epsMax);

  fclose(pfile);
}*/

//-----------------------------------------------------------------------------
String Material::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";

  sprintf(sstr, "%s.setHeatCoefficient(%12.7E);\n", name.chars(), heatCoeff());
  str += sstr;
  sprintf(sstr, "%s.setDilatation(%12.7E);\n", name.chars(), dilat());
  str += sstr;
  sprintf(sstr, "%s.setInitTemperature(%12.7E);\n", name.chars(), initTemp());
  str += sstr;
  sprintf(sstr, "%s.setConductivity(%12.7E);\n", name.chars(), conductivity());
  str += sstr;
  sprintf(sstr, "%s.setYoung(%12.7E);\n", name.chars(), young());
  str += sstr;
  sprintf(sstr, "%s.setPoisson(%12.7E);\n", name.chars(), nu());
  str += sstr;
  sprintf(sstr, "%s.setDensity(%12.7E);\n", name.chars(), density());
  str += sstr;
  sprintf(sstr, "%s.setColor(%12.7E, %12.7E, %12.7E);\n", name.chars(), color(0), color(1), color(2));
  str += sstr;

  // attach the isotropic hardening part to the material if exist
  if (isotropicHardening != NULL)
  {
    sprintf(sstr, "%s.setHardening(%s);\n", name.chars(), isotropicHardening->name.chars());
    str += sstr;
  }

  return str;
}

//-----------------------------------------------------------------------------
Matrix Material::getHookeMatrix(short type)
//-----------------------------------------------------------------------------
{
  Matrix H;

  switch (type)
  {
  case planeStrain:
    H.redim(3, 3);
    H(0, 0) = 1;
    H(1, 1) = 1;
    H(1, 0) = Nu / (1.0 - Nu);
    H(0, 1) = Nu / (1.0 - Nu);
    H(2, 2) = (1.0 - 2.0 * Nu) / (2.0 * (1 - Nu));
    H *= (E * (1.0 - Nu) / ((1.0 + Nu) * (1. - 2.0 * Nu)));
    break;

  case planeStress:
    H.redim(3, 3);
    H(0, 0) = 1;
    H(1, 1) = 1;
    H(1, 0) = Nu;
    H(0, 1) = Nu;
    H(2, 2) = (1.0 - Nu) / 2.0;
    H *= (E / (1.0 - SQ(Nu)));
    break;

  case axisymetric:
    H.redim(4, 4);
    H(0, 0) = 1;
    H(1, 1) = 1;
    H(3, 3) = 1;
    H(1, 0) = Nu / (1. - Nu);
    H(0, 1) = Nu / (1. - Nu);
    H(3, 0) = Nu / (1. - Nu);
    H(3, 1) = Nu / (1. - Nu);
    H(0, 3) = Nu / (1. - Nu);
    H(1, 3) = Nu / (1. - Nu);
    H(2, 2) = (1.0 - 2.0 * Nu) / (2. * (1.0 - Nu));
    H *= (E * (1.0 - Nu) / ((1.0 + Nu) * (1.0 - 2.0 * Nu)));
    break;

  case threedimensional:
    H.redim(6, 6);
    H = 0.0;
    H(0, 0) = 1;
    H(1, 1) = 1;
    H(2, 2) = 1;
    H(3, 3) = (1.0 - 2.0 * Nu) / (2. * (1.0 - Nu));
    H(4, 4) = (1.0 - 2.0 * Nu) / (2. * (1.0 - Nu));
    H(5, 5) = (1.0 - 2.0 * Nu) / (2. * (1.0 - Nu));
    H(0, 1) = H(1, 0) = Nu / (1. - Nu);
    H(0, 2) = H(2, 0) = Nu / (1. - Nu);
    H(1, 2) = H(2, 1) = Nu / (1. - Nu);
    H *= (E * (1.0 - Nu) / ((1.0 + Nu) * (1.0 - 2.0 * Nu)));
    break;

  default:
    fatalError("Material::getHookeMatrix", "Matrix not computed for this case");
  }
  return H;
}

IsotropicHardening *Material::getIsotropicHardening()
{
  return isotropicHardening;
}

Real Material::getYieldStress(IntegrationPoint *pfield)
{
  if (isotropicHardening != NULL)
    return (isotropicHardening->getYieldStress(pfield));
  return 0;
}

Real Material::getDerYieldStress(IntegrationPoint *pfield)
{
  if (isotropicHardening != NULL)
    return (isotropicHardening->getDerYieldStress(pfield));
  return 0;
}

Real Material::getIsotropicYieldStress(Element *el, Real shift)
{
  if (isotropicHardening != NULL)
    return (isotropicHardening->getIsotropicYieldStress(el, shift));
  return 0;
}

Real Material::getIsotropicYieldHardening(Element *el, Real shift)
{
  if (isotropicHardening != NULL)
    return (isotropicHardening->getIsotropicYieldHardening(el, shift));
  return 0;
}

Boolean Material::isYield()
{
  if (isotropicHardening != NULL)
    return (isotropicHardening->isYield());
  return False;
}

void Material::setHardening(IsotropicHardening *hard)
{
  isotropicHardening = hard;
}
