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

// begin date :

/*
  Class IsoHardElastoplasticTabular definition
*/

/*!
  \file IsoHardElastoplasticTabular.C
  \brief fichier .C de d�finition d'un mat�riau �lasto-plastique tabul�
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique tabul� sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <IsoHardElastoplasticTabular.h>
#include <Element.h>
#include <IntegrationPoint.h>
//#include <MaterialNames.h>

//-----------------------------------------------------------------------------
Indice
IsoHardElastoplasticTabular::getType()
//-----------------------------------------------------------------------------
{
  return ElastoPlasticTabular;
}

//-----------------------------------------------------------------------------
IsoHardElastoplasticTabular::IsoHardElastoplasticTabular(/*Indice No_*/) : IsotropicHardening(/*No_*/)
//-----------------------------------------------------------------------------
{
  is_Linear = No;
  valeurBidon = 0;
}

//-----------------------------------------------------------------------------
IsoHardElastoplasticTabular::IsoHardElastoplasticTabular(const IsoHardElastoplasticTabular &mat) : IsotropicHardening(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsoHardElastoplasticTabular::~IsoHardElastoplasticTabular()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Indice
IsoHardElastoplasticTabular::getNumberOfConstitutiveParameters()
//-----------------------------------------------------------------------------
{
  return 1;
}

//-----------------------------------------------------------------------------
const char *
IsoHardElastoplasticTabular::getConstitutiveParameterName(Indice ind)
//-----------------------------------------------------------------------------
{
  return "Fonction";
}

//-----------------------------------------------------------------------------
Real &IsoHardElastoplasticTabular::getConstitutiveParameter(Indice ind)
//-----------------------------------------------------------------------------
{
  return valeurBidon;
}

//-----------------------------------------------------------------------------
void IsoHardElastoplasticTabular::plot(FILE *pfile, Real epsMax)
//-----------------------------------------------------------------------------
{
  for (Real evpl = 0; evpl < epsMax; evpl += (evpl / 5.0) + 1.0 / 1000.0)
  {
    Real val = tabul->getValue(evpl);
    fprintf(pfile, "%lf, %lf\n", evpl, val);
  }
}

//-----------------------------------------------------------------------------
Real IsoHardElastoplasticTabular::getIsotropicYieldStress(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evp = element->ref->evp + Sqrt23 * shift;
  return tabul->getValue(evp);
}

//-----------------------------------------------------------------------------
Real IsoHardElastoplasticTabular::getIsotropicYieldHardening(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evp = element->ref->evp + Sqrt23 * shift;
  return tabul->getSlope(evp);
}

//-----------------------------------------------------------------------------
Real IsoHardElastoplasticTabular::getYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  return tabul->getValue(point->evp);
}

//-----------------------------------------------------------------------------
Real IsoHardElastoplasticTabular::getDerYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  return tabul->getSlope(point->evp);
}

//-----------------------------------------------------------------------------
void IsoHardElastoplasticTabular::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "Elastic Plastic Tabular law\n";
  //  Material::print (os);
  os << "Function=" << tabul->name << endl;
  //   os << "A="<<A<<endl;
  //   os << "B="<<B<<endl;
  //   os << "n="<<n<<endl;
  //   os << "EpsM="<<EpsM<<endl;
  //   os << "SigM="<<SigM<<endl;
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, IsoHardElastoplasticTabular &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void IsoHardElastoplasticTabular::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  fprintf(pfile, "FUNCTION = %s\n", tabul->name.chars());
  //   fprintf(pfile,"    STRESS      PARAMETER    EXPONENT\n");
  //   fprintf(pfile,"  %8.5E  %8.5E  %8.5E\n",A,B,n);
  //   fprintf(pfile,"\n");
}

//-----------------------------------------------------------------------------
void IsoHardElastoplasticTabular::setFunction(DiscreteFunction *fu)
//-----------------------------------------------------------------------------
{
  tabul = fu;
}

//-----------------------------------------------------------------------------
String IsoHardElastoplasticTabular::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";

  sprintf(sstr, "%s.setFunction(%s);\n", name.chars(), tabul->name.chars());
  str += sstr;

  return str;
}
