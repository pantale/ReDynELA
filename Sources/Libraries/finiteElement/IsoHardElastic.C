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
  Class IsoHardElastic definition
*/

/*!
  \file IsoHardElastic.C
  \brief fichier .C de d�finition d'un mat�riau �lastique
  \ingroup femLibrary

  Ce fichier sert � la d�finition d'un mat�riau de type �lastique sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <IsoHardElastic.h>
//#include <MaterialNames.h>

//static const char* Materials_name[];

//-----------------------------------------------------------------------------
Indice
IsoHardElastic::getType()
//-----------------------------------------------------------------------------
{
  return Elastic;
}

//-----------------------------------------------------------------------------
IsoHardElastic::IsoHardElastic() : IsotropicHardening()
//-----------------------------------------------------------------------------
{
  //  Name=Materials_name[mat_Elastic];
  is_Linear = Yes;
}

//-----------------------------------------------------------------------------
IsoHardElastic::IsoHardElastic(const IsoHardElastic &mat) : IsotropicHardening(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsoHardElastic::~IsoHardElastic()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void IsoHardElastic::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  fprintf(pfile, "\n");
}

//-----------------------------------------------------------------------------
void IsoHardElastic::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "Elastic law\n";
  //  Material::print (os);
}

//-----------------------------------------------------------------------------
String IsoHardElastic::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  //  char sstr[1000];
  str = "";

  return str;
}

//-----------------------------------------------------------------------------
void IsoHardElastic::plot(FILE *pfile, Real epsMax)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
const char *IsoHardElastic::getConstitutiveParameterName(Indice ind)
//-----------------------------------------------------------------------------
{
  return "void";
}

//-----------------------------------------------------------------------------
Real &IsoHardElastic::getConstitutiveParameter(Indice ind)
//-----------------------------------------------------------------------------
{
  return dummyReal;
}
