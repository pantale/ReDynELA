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
  Class IsoHardElastoplastic definition
*/

/*!
  \file IsoHardElastoplastic.C
  \brief fichier .C de d�finition d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <IsoHardElastoplastic.h>
#include <IntegrationPoint.h>
#include <Element.h>

//-----------------------------------------------------------------------------
Indice
IsoHardElastoplastic::getType()
//-----------------------------------------------------------------------------
{
  return ElastoPlastic;
}

//-----------------------------------------------------------------------------
IsoHardElastoplastic::IsoHardElastoplastic() : IsotropicHardening()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsoHardElastoplastic::IsoHardElastoplastic(const IsoHardElastoplastic &mat) : IsotropicHardening(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsoHardElastoplastic::~IsoHardElastoplastic()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Real &IsoHardElastoplastic::Yield_Stress()
//-----------------------------------------------------------------------------
{
  return A;
}

//-----------------------------------------------------------------------------
Real &IsoHardElastoplastic::Hard_Param()
//-----------------------------------------------------------------------------
{
  return B;
}

//-----------------------------------------------------------------------------
Real &IsoHardElastoplastic::Hard_Exponent()
//-----------------------------------------------------------------------------
{
  //  is_Linear=No;
  return n;
}

/*
 * //-----------------------------------------------------------------------------
Real & IsoHardElastoplastic::Fail_Strain ()
//-----------------------------------------------------------------------------
{
  return EpsM;
}

//-----------------------------------------------------------------------------
Real & IsoHardElastoplastic::Max_Stress ()
//-----------------------------------------------------------------------------
{
  return SigM;
}
*/

//-----------------------------------------------------------------------------
Indice
IsoHardElastoplastic::getNumberOfConstitutiveParameters()
//-----------------------------------------------------------------------------
{
  return 3;
}

//-----------------------------------------------------------------------------
const char *
IsoHardElastoplastic::getConstitutiveParameterName(Indice ind)
//-----------------------------------------------------------------------------
{
  String retour;
  switch (ind)
  {
  case 0:
    return "A";
    break;
  case 1:
    return "B";
    break;
  case 2:
    return "n";
    break;
  default:
    fatalError("IsoHardElastoplastic::geConstitutiveParameterName",
               "called with ind = %d", ind);
  }
  return "";
}

//-----------------------------------------------------------------------------
Real &IsoHardElastoplastic::getConstitutiveParameter(Indice ind)
//-----------------------------------------------------------------------------
{
  switch (ind)
  {
  case 0:
    return A;
    break;
  case 1:
    return B;
    break;
  case 2:
    return n;
    break;
  default:
    fatalError("IsoHardElastoplastic::geConstitutiveParameterName",
               "called with ind = %d", ind);
  }

  // pour tromper le compilo, mais ca sert a rien car il ne passe jamais ici
  return A;
}

//-----------------------------------------------------------------------------
void IsoHardElastoplastic::plot(FILE *pfile, Real epsMax)
//-----------------------------------------------------------------------------
{
  for (Real evpl = 0; evpl < epsMax; evpl += (evpl / 5.0) + 1.0 / 1000.0)
  {
    Real val = (A + B * pow(evpl, n));
    fprintf(pfile, "%lf, %lf\n", evpl, val);
  }
}

//-----------------------------------------------------------------------------
Real IsoHardElastoplastic::getIsotropicYieldStress(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evp = element->ref->evp + Sqrt23 * shift;
  return (A + B * pow(evp, n));
}

//-----------------------------------------------------------------------------
Real IsoHardElastoplastic::getIsotropicYieldHardening(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evp = element->ref->evp + Sqrt23 * shift;
  return (n * B * pow(evp, (n - 1.)));
}

//-----------------------------------------------------------------------------
Real IsoHardElastoplastic::getYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  return (A + B * pow(point->evp, n));
}

//-----------------------------------------------------------------------------
Real IsoHardElastoplastic::getDerYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  return (n * B * pow(point->evp, (n - 1.)));
}

//-----------------------------------------------------------------------------
void IsoHardElastoplastic::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "Elastic Plastic law\n";
  //  Material::print (os);
  os << "A=" << A << endl;
  os << "B=" << B << endl;
  os << "n=" << n << endl;
  //  os << "EpsM=" << EpsM << endl;
  //  os << "SigM=" << SigM << endl;
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, IsoHardElastoplastic &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void IsoHardElastoplastic::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  fprintf(pfile, "     YIELD      HARDENING    HARDENING\n");
  fprintf(pfile, "    STRESS      PARAMETER    EXPONENT\n");
  fprintf(pfile, "  %8.5E  %8.5E  %8.5E\n", A, B, n);
  fprintf(pfile, "\n");
}

//-----------------------------------------------------------------------------
void IsoHardElastoplastic::setYieldStress(Real val)
//-----------------------------------------------------------------------------
{
  A = val;
}

//-----------------------------------------------------------------------------
void IsoHardElastoplastic::setHardParameter(Real val)
//-----------------------------------------------------------------------------
{
  B = val;
}

//-----------------------------------------------------------------------------
void IsoHardElastoplastic::setHardExponent(Real val)
//-----------------------------------------------------------------------------
{
  n = val;
}

//-----------------------------------------------------------------------------
String IsoHardElastoplastic::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";

  sprintf(sstr, "%s.setYieldStress(%12.7E);\n", name.chars(), A);
  str += sstr;
  sprintf(sstr, "%s.setHardParameter(%12.7E);\n", name.chars(), B);
  str += sstr;
  sprintf(sstr, "%s.setHardExponent(%12.7E);\n", name.chars(), n);
  str += sstr;

  return str;
}
