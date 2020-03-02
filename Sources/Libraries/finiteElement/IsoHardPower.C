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
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

// begin date :

/*
  Class IsoHardPower definition
*/

/*!
  \file IsoHardPower.C
  \brief fichier .C de d�finition d'un mat�riau �lasto-plastique de type Power.
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lasto-plastique de type Power sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <IsoHardPower.h>
#include <IntegrationPoint.h>
#include <Element.h>

//-----------------------------------------------------------------------------
Indice IsoHardPower::getType()
//-----------------------------------------------------------------------------
{
  return Power;
}

//-----------------------------------------------------------------------------
IsoHardPower::IsoHardPower() : IsotropicHardening()
//-----------------------------------------------------------------------------
{
  P[1] = 0.0;
  P[2] = 0.0;
  P[3] = 0.0;
  P[4] = 0.0;
}

//-----------------------------------------------------------------------------
IsoHardPower::IsoHardPower(const IsoHardPower &mat) : IsotropicHardening(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsoHardPower::~IsoHardPower()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Indice IsoHardPower::getNumberOfConstitutiveParameters()
//-----------------------------------------------------------------------------
{
  return 8;
}

//-----------------------------------------------------------------------------
const char *IsoHardPower::getConstitutiveParameterName(Indice ind)
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
  }
  return "";
}

//-----------------------------------------------------------------------------
Real &IsoHardPower::getConstitutiveParameter(Indice ind)
//-----------------------------------------------------------------------------
{
  return P[ind + 1];
}

//-----------------------------------------------------------------------------
void IsoHardPower::plot(FILE *pfile, Real epsMax)
//-----------------------------------------------------------------------------
{
  for (Real evpl = 0; evpl < epsMax; evpl += (evpl / 5.0) + 1.0 / 1000.0)
  {
    Real val = P[1];
    for (Indice i = 0; i < constitutiveLawMaxIterations; i++)
    {
      Real valfct = P[1] * pow((P[2] * val + P[3] * evpl), P[4]) - val;
      if (valfct < constitutiveLawPrecision)
        break;
      Real derivfct = P[1] * P[2] * P[4] * pow((P[2] * val + P[3] * evpl), (P[4] - 1)) - 1.0;
      val -= valfct / derivfct;
    }
    fprintf(pfile, "%lf, %lf\n", evpl, val);
  }
}

//-----------------------------------------------------------------------------
Real IsoHardPower::getIsotropicYieldStress(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evpl = element->ref->evp + Sqrt23 * shift;
  Real val;

  val = P[1];

  int i;
  for (i = 0; i < constitutiveLawMaxIterations; i++)
  {
    Real valfct = P[1] * pow((P[2] * val + P[3] * evpl), P[4]) - val;
    if (valfct < constitutiveLawPrecision)
      break;
    Real derivfct = P[1] * P[2] * P[4] * pow((P[2] * val + P[3] * evpl), (P[4] - 1)) - 1.0;
    val -= valfct / derivfct;
  }

  return val;
}

//-----------------------------------------------------------------------------
Real IsoHardPower::getIsotropicYieldHardening(Element *element, Real shift)
//-----------------------------------------------------------------------------
{
  Real evpl = element->ref->evp + Sqrt23 * shift;
  Real sigmav = getIsotropicYieldStress(element);
  Real val = P[1] * P[3] * P[4] * (pow((P[2] * sigmav + P[3] * evpl), (P[4] - 1))) / (1.0 - P[1] * P[2] * P[4] * pow((P[2] * sigmav + P[3] * evpl), (P[4] - 1)));
  // ou d'apr�s moi !!
  //  Real val = P[1]*P[3]*P[4]*(pow((P[2]*sigmav+P[3]*evpl),(P[4]-1)))/(P[1]*P[2]*P[4]*pow((P[2]*sigmav+P[3]*evpl),(P[4]-1)) - 1.0);

  return val;
}

//-----------------------------------------------------------------------------
Real IsoHardPower::getYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  fatalError("IsoHardPower::getYieldStress", "Not implemented");
  return 0;
}

//-----------------------------------------------------------------------------
Real IsoHardPower::getDerYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  fatalError("IsoHardPower::getYieldStress", "Not implemented");
  return 0;
}

//-----------------------------------------------------------------------------
void IsoHardPower::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "Power Elastic Plastic law\n";
  //  Material::print (os);
  for (Indice i = 1; i <= 4; i++)
  {
    os << "P" << i << "=" << P[i] << endl;
  }
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, IsoHardPower &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void IsoHardPower::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  for (Indice i = 1; i <= 4; i++)
  {
    fprintf(pfile, "P%ld = %lf\n", i, P[i]);
  }
}

//-----------------------------------------------------------------------------
void IsoHardPower::setParameter(Indice i, Real val)
//-----------------------------------------------------------------------------
{
  if ((i < 1) || (i > 4))
  {
    CreateException(BadValue e, "message");
    e.print();
    cerr << "Value " << i << " is out of bounds [1,4]\n";
    exit(-1);
  }
  P[i] = val;
}

//-----------------------------------------------------------------------------
String IsoHardPower::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";
  for (Indice i = 1; i <= 4; i++)
  {
    sprintf(sstr, "%s.setParameter(%ld,%12.7E);\n", name.chars(), i, P[i]);
    str += sstr;
  }

  return str;
}
