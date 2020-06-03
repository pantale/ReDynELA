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
  Class ViscPerzina definition
*/

/*!
  \file ViscPerzina.C
  \brief fichier .C de d�finition d'un mat�riau �lasto-plastique de type Metafor.
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique de type Metafor sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <ViscPerzina.h>
#include <IntegrationPoint.h>
#include <Element.h>

//-----------------------------------------------------------------------------
Indice ViscPerzina::getType()
//-----------------------------------------------------------------------------
{
  return Perzina;
}

//-----------------------------------------------------------------------------
ViscPerzina::ViscPerzina() : Viscosity()
//-----------------------------------------------------------------------------
{
  K = 0;
  m = 0;
  n = 0;
}

//-----------------------------------------------------------------------------
ViscPerzina::ViscPerzina(const ViscPerzina &mat) : Viscosity(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ViscPerzina::~ViscPerzina()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Indice ViscPerzina::getNumberOfConstitutiveParameters()
//-----------------------------------------------------------------------------
{
  return 3;
}

//-----------------------------------------------------------------------------
const char *ViscPerzina::getConstitutiveParameterName(Indice ind)
//-----------------------------------------------------------------------------
{
  switch (ind)
  {
  case 1:
    return "K";
    break;
  case 2:
    return "m";
    break;
  case 3:
    return "n";
    break;
  }
  return "";
}

//-----------------------------------------------------------------------------
Real &ViscPerzina::getConstitutiveParameter(Indice ind)
//-----------------------------------------------------------------------------
{
  switch (ind)
  {
  case 1:
    return K;
    break;
  case 2:
    return m;
    break;
  case 3:
    return n;
    break;
  }
  return K;
}

//-----------------------------------------------------------------------------
Real ViscPerzina::getViscousStress(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evp = element->ref->evp + Sqrt23 * shift;

  return K * pow(element->ref->evp, m) * pow(evp, n);
}

//-----------------------------------------------------------------------------
void ViscPerzina::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "Perzina viscosity law\n";
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, ViscPerzina &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void ViscPerzina::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  for (Indice i = 1; i <= 8; i++)
  {
    //      fprintf (pfile, "P%ld = %lf\n",i,P[i]);
  }
}

//-----------------------------------------------------------------------------
String ViscPerzina::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";
  if (K != 0)
  {
    sprintf(sstr, "%s.setK(%12.7E);\n", name.chars(), K);
    str += sstr;
  }
  if (m != 0)
  {
    sprintf(sstr, "%s.setm(%12.7E);\n", name.chars(), m);
    str += sstr;
  }
  if (n != 0)
  {
    sprintf(sstr, "%s.setn(%12.7E);\n", name.chars(), n);
    str += sstr;
  }

  return str;
}
