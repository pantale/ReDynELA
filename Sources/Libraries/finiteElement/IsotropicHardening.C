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
  Class IsotropicHardening definition
*/

/*!
  \file IsotropicHardening.h
  \brief fichier .h de d�finition de la classe g�nerique des mat�riaux
  \ingroup femLibrary

  Ce fichier sert � la d�finition la classe g�nerique des mat�riaux.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#include <IsotropicHardening.h>
//#include <IsotropicHardeningNames.h>

//-----------------------------------------------------------------------------
IsotropicHardening::IsotropicHardening()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsotropicHardening::IsotropicHardening(const IsotropicHardening &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
IsotropicHardening::~IsotropicHardening()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void IsotropicHardening::plotToFile(String name, Real epsMax)
//-----------------------------------------------------------------------------
{
  FILE *pfile;

  pfile = fopen(name.chars(), "w");
  if (pfile == NULL)
  {
    CreateException(FileNotOpen exception, "toto");
    exception.print();
    cerr << "Unable to open file " << name << "\n";
    exit(-1);
  }

  plot(pfile, epsMax);

  fclose(pfile);
}

//-----------------------------------------------------------------------------
String IsotropicHardening::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  //   String str;
  //   char sstr[1000];
  //   str="";

  // //   sprintf(sstr,"%s.setHeatCoefficient(%12.7E);\n",name.chars(),heatCoeff());
  // //   str+=sstr;
  // //   sprintf(sstr,"%s.setDilatation(%12.7E);\n",name.chars(),dilat());
  // //   str+=sstr;
  // //   sprintf(sstr,"%s.setInitTemperature(%12.7E);\n",name.chars(),initTemp());
  // //   str+=sstr;
  // //   sprintf(sstr,"%s.setConductivity(%12.7E);\n",name.chars(),conductivity());
  // //   str+=sstr;
  // //   sprintf(sstr,"%s.setYoung(%12.7E);\n",name.chars(),young());
  // //   str+=sstr;
  // //   sprintf(sstr,"%s.setPoisson(%12.7E);\n",name.chars(),nu());
  // //   str+=sstr;
  // //   sprintf(sstr,"%s.setDensity(%12.7E);\n",name.chars(),density());
  // //   str+=sstr;
  // //   sprintf(sstr,"%s.setColor(%12.7E, %12.7E, %12.7E);\n",name.chars(),color(0),color(1),color(2));
  // //   str+=sstr;

  return "";
}
