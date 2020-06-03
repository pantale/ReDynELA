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
  Class IsoHardMetafor definition
*/

/*!
  \file IsoHardMetafor.C
  \brief fichier .C de d�finition d'un mat�riau �lasto-plastique de type Metafor.
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique de type Metafor sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <IsoHardMetafor.h>
#include <IntegrationPoint.h>
#include <Element.h>

//-----------------------------------------------------------------------------
Indice IsoHardMetafor::getType()
//-----------------------------------------------------------------------------
{
  return Metafor;
}

//-----------------------------------------------------------------------------
IsoHardMetafor::IsoHardMetafor() : IsotropicHardening()
//-----------------------------------------------------------------------------
{
  P[1] = 0.0;
  P[2] = 0.0;
  P[3] = 0.0;
  P[4] = 0.0;
  P[5] = 0.0;
  P[6] = 0.0;
  P[7] = 0.0;
  P[8] = 0.0;
}

//-----------------------------------------------------------------------------
IsoHardMetafor::IsoHardMetafor(const IsoHardMetafor &mat) : IsotropicHardening(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsoHardMetafor::~IsoHardMetafor()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Indice IsoHardMetafor::getNumberOfConstitutiveParameters()
//-----------------------------------------------------------------------------
{
  return 8;
}

//-----------------------------------------------------------------------------
const char *IsoHardMetafor::getConstitutiveParameterName(Indice ind)
//-----------------------------------------------------------------------------
{
  switch (ind)
  {
  case 1:
    return "P1";
    break;
  case 2:
    return "P2";
    break;
  case 3:
    return "P3";
    break;
  case 4:
    return "P4";
    break;
  case 5:
    return "P5";
    break;
  case 6:
    return "P6";
    break;
  case 7:
    return "P7";
    break;
  case 8:
    return "P8";
    break;
  }
  return "";
}

//-----------------------------------------------------------------------------
Real &IsoHardMetafor::getConstitutiveParameter(Indice ind)
//-----------------------------------------------------------------------------
{
  return P[ind + 1];
}

//-----------------------------------------------------------------------------
void IsoHardMetafor::plot(FILE *pfile, Real epsMax)
//-----------------------------------------------------------------------------
{
  for (Real evpl = 0; evpl < epsMax; evpl += (evpl / 5.0) + 1.0 / 1000.0)
  {
    Real val = (P[2] - P[1]) * (1 - exp(-P[3] * (evpl))) + P[4] * pow(evpl, P[5]) + P[1] * pow((1 + P[6] * evpl), P[7]) + P[8] * evpl;
    fprintf(pfile, "%lf, %lf\n", evpl, val);
  }
}

//-----------------------------------------------------------------------------
Real IsoHardMetafor::getIsotropicYieldStress(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evpl = element->ref->evp + Sqrt23 * shift;
  Real ret;
  ret = (P[2] - P[1]) * (1 - exp(-P[3] * (evpl))) + P[4] * pow(evpl, P[5]) + P[1] * pow((1 + P[6] * evpl), P[7]) + P[8] * evpl;
  return ret;
}

//-----------------------------------------------------------------------------
Real IsoHardMetafor::getIsotropicYieldHardening(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evpl = element->ref->evp + Sqrt23 * shift;
  Real ret;
  if (evpl > 0.)
    ret = ((P[2] - P[1]) * P[3] * exp(-P[3] * (evpl)) + P[4] * P[5] * pow(evpl, (P[5] - 1.0)) + P[1] * P[6] * P[7] * pow((1.0 + P[6] * evpl), (P[7] - 1.0)) + P[8]);
  else
    ret = ((P[2] - P[1]) * P[3] * exp(-P[3] * (evpl)) + P[1] * P[6] * P[7] * pow((1.0 + P[6] * evpl), (P[7] - 1.0)) + P[8]);

  return ret;
}

//-----------------------------------------------------------------------------
Real IsoHardMetafor::getYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  fatalError("IsoHardMetafor::getYieldStress", "Not implemented");
  return 0;
}

//-----------------------------------------------------------------------------
Real IsoHardMetafor::getDerYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  fatalError("IsoHardMetafor::getYieldStress", "Not implemented");
  return 0;
}

//-----------------------------------------------------------------------------
void IsoHardMetafor::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "Metafor Elastic Plastic law\n";
  //  Material::print (os);
  for (Indice i = 1; i <= 8; i++)
  {
    os << "P" << i << "=" << P[i] << endl;
  }
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, IsoHardMetafor &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void IsoHardMetafor::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  for (Indice i = 1; i <= 8; i++)
  {
    fprintf(pfile, "P%ld = %lf\n", i, P[i]);
  }
}

//-----------------------------------------------------------------------------
void IsoHardMetafor::setParameter(Indice i, Real val)
//-----------------------------------------------------------------------------
{
  if ((i < 1) || (i > 8))
  {
    CreateException(BadValue e, "message");
    e.print();
    cerr << "Value " << i << " is out of bounds [1,8]\n";
    exit(-1);
  }
  P[i] = val;
}

//-----------------------------------------------------------------------------
String IsoHardMetafor::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";
  for (Indice i = 1; i <= 8; i++)
  {
    sprintf(sstr, "%s.setParameter(%ld,%12.7E);\n", name.chars(), i, P[i]);
    str += sstr;
  }

  return str;
}
