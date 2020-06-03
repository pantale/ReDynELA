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
  Class IsoHardJohnsonCook definition
*/

/*!
  \file IsoHardJohnsonCook.C
  \brief fichier .C de d�finition d'un mat�riau �lasto-plastique
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <IsoHardJohnsonCook.h>
#include <IntegrationPoint.h>
#include <Element.h>

//-----------------------------------------------------------------------------
Indice
IsoHardJohnsonCook::getType()
//-----------------------------------------------------------------------------
{
  return JohnsonCook;
}

//-----------------------------------------------------------------------------
IsoHardJohnsonCook::IsoHardJohnsonCook() : IsotropicHardening()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsoHardJohnsonCook::IsoHardJohnsonCook(const IsoHardJohnsonCook &mat) : IsotropicHardening(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsoHardJohnsonCook::~IsoHardJohnsonCook()
//-----------------------------------------------------------------------------
{
}

// //-----------------------------------------------------------------------------
// Real & IsoHardJohnsonCook::Yield_Stress ()
// //-----------------------------------------------------------------------------
// {
//   return A;
// }
//
// //-----------------------------------------------------------------------------
// Real & IsoHardJohnsonCook::Hard_Param ()
// //-----------------------------------------------------------------------------
// {
//   return B;
// }
//
// //-----------------------------------------------------------------------------
// Real & IsoHardJohnsonCook::Hard_Exponent ()
// //-----------------------------------------------------------------------------
// {
// //  is_Linear=No;
//   return n;
// }

//-----------------------------------------------------------------------------
Indice
IsoHardJohnsonCook::getNumberOfConstitutiveParameters()
//-----------------------------------------------------------------------------
{
  return 8;
}

//-----------------------------------------------------------------------------
const char *
IsoHardJohnsonCook::getConstitutiveParameterName(Indice ind)
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
    return "C";
    break;
  case 3:
    return "n";
    break;
  case 4:
    return "m";
    break;
  case 5:
    return "epsp0";
    break;
  case 6:
    return "Tm";
    break;
  case 7:
    return "T0";
    break;
  default:
    fatalError("IsoHardJohnsonCook::geConstitutiveParameterName",
               "called with ind = %d", ind);
  }
  return "";
}

//-----------------------------------------------------------------------------
Real &IsoHardJohnsonCook::getConstitutiveParameter(Indice ind)
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
    return C;
    break;
  case 3:
    return n;
    break;
  case 4:
    return m;
    break;
  case 5:
    return epsp0;
    break;
  case 6:
    return Tm;
    break;
  case 7:
    return T0;
    break;
  default:
    fatalError("IsoHardJohnsonCook::geConstitutiveParameterName",
               "called with ind = %d", ind);
  }

  // pour tromper le compilo, mais ca sert a rien car il ne passe jamais ici
  return A;
}

//-----------------------------------------------------------------------------
void IsoHardJohnsonCook::plot(FILE *pfile, Real epsMax)
//-----------------------------------------------------------------------------
{
  for (Real evpl = 0; evpl < epsMax; evpl += (evpl / 5.0) + 1.0 / 1000.0)
  {
    Real val = (A + B * pow(evpl, n));
    fprintf(pfile, "%lf, %lf\n", evpl, val);
  }
}

//-----------------------------------------------------------------------------
Real IsoHardJohnsonCook::getIsotropicYieldStress(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evp = element->ref->evp + Sqrt23 * shift;
  return (A + B * pow(evp, n));
}

//-----------------------------------------------------------------------------
Real IsoHardJohnsonCook::getIsotropicYieldHardening(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evp = element->ref->evp + Sqrt23 * shift;
  return (n * B * pow(evp, (n - 1.0)));
}

//-----------------------------------------------------------------------------
Real IsoHardJohnsonCook::getYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  return (A + B * pow(point->evp, n));
}

//-----------------------------------------------------------------------------
Real IsoHardJohnsonCook::getDerYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  return (n * B * pow(point->evp, (n - 1.0)));
}

//-----------------------------------------------------------------------------
void IsoHardJohnsonCook::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "Johnson-Cook law\n";
  os << "A=" << A << endl;
  os << "B=" << B << endl;
  os << "C=" << C << endl;
  os << "n=" << n << endl;
  os << "m=" << m << endl;
  os << "epsp0=" << epsp0 << endl;
  os << "Tm=" << Tm << endl;
  os << "T0=" << T0 << endl;
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, IsoHardJohnsonCook &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void IsoHardJohnsonCook::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  fprintf(pfile, "     YIELD      HARDENING    HARDENING\n");
  fprintf(pfile, "    STRESS      PARAMETER    EXPONENT\n");
  fprintf(pfile, "  %8.5E  %8.5E  %8.5E\n", A, B, n);
  fprintf(pfile, "\n");
}

//-----------------------------------------------------------------------------
void IsoHardJohnsonCook::defineLaw(Real par_A, Real par_B, Real par_C, Real par_n, Real par_m, Real par_epsp0, Real par_Tm, Real par_T0)
//-----------------------------------------------------------------------------
{
  A = par_A;
  B = par_B;
  C = par_C;
  n = par_n;
  m = par_m;
  epsp0 = par_epsp0;
  Tm = par_Tm;
  T0 = par_T0;
}

// //-----------------------------------------------------------------------------
// void IsoHardJohnsonCook::setHardParameter(Real val)
// //-----------------------------------------------------------------------------
// {
//   B=val;
// }
//
// //-----------------------------------------------------------------------------
// void IsoHardJohnsonCook::setHardExponent(Real val)
// //-----------------------------------------------------------------------------
// {
//   n=val;
// }

//-----------------------------------------------------------------------------
String IsoHardJohnsonCook::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";

  sprintf(sstr, "%s.defineLaw(%12.7E, %12.7E, %12.7E, %12.7E, %12.7E, %12.7E, %12.7E, %12.7E);\n", name.chars(), A, B, C, n, m, epsp0, Tm, T0);
  str += sstr;
  //   sprintf(sstr,"%s.setHardParameter(%12.7E);\n",name.chars(),B);
  //   str+=sstr;
  //   sprintf(sstr,"%s.setHardExponent(%12.7E);\n",name.chars(),n);
  //   str+=sstr;

  return str;
}
