/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2004                                                *
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
// revision date :

/*
	Class PolynomialFunction implementation
*/
/*!
  \file PolynomialFunction.C
  \brief fichier .C de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

#include <PolynomialFunction.h>

//!constructeur par d�faut de la classe PolynomialFunction
/*!

  \author &copy; Olivier PANTALE
  \since 1.0.4
*/
//-----------------------------------------------------------------------------
PolynomialFunction::PolynomialFunction()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
PolynomialFunction::~PolynomialFunction()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Real PolynomialFunction::getValue(Real x)
//-----------------------------------------------------------------------------
{
  Indice i;
  Real val;
  val = 0;
  for (i = 0; i < fragment.size(); i++)
  {
    val += fragment(i).value * pow(x, fragment(i).factor);
  }
  return val;
}

//-----------------------------------------------------------------------------
void PolynomialFunction::set(Indice fac, Real val)
//-----------------------------------------------------------------------------
{
  Indice i;
  for (i = 0; i < fragment.size(); i++)
  {
    if (fragment(i).factor == fac)
    {
      fragment(i).value = val;
    }
  }

  PolynomialFragment pol;
  pol.factor = fac;
  pol.value = val;
  fragment << pol;
}

//-----------------------------------------------------------------------------
Real PolynomialFunction::get(Indice fac)
//-----------------------------------------------------------------------------
{
  Indice i;
  for (i = 0; i < fragment.size(); i++)
  {
    if (fragment(i).factor == fac)
    {
      return fragment(i).value;
    }
  }
  return 0.0;
}

//-----------------------------------------------------------------------------
Real PolynomialFunction::getSlope(Real x)
//-----------------------------------------------------------------------------
{
  Indice i;
  Real val;
  val = 0;
  for (i = 0; i < fragment.size(); i++)
  {
    if (fragment(i).factor != 0)
      val += fragment(i).value * fragment(i).factor * pow(x, fragment(i).factor - 1);
  }
  return val;
}

//-----------------------------------------------------------------------------
Indice PolynomialFunction::getNumberOfFragments()
//-----------------------------------------------------------------------------
{
  return fragment.size();
}

//-----------------------------------------------------------------------------
Real PolynomialFunction::getFactor(Indice i)
//-----------------------------------------------------------------------------
{
  return fragment(i).factor;
}

//-----------------------------------------------------------------------------
Real PolynomialFunction::getCoefficient(Indice i)
//-----------------------------------------------------------------------------
{
  return fragment(i).value;
}

//-----------------------------------------------------------------------------
void PolynomialFunction::print()
//-----------------------------------------------------------------------------
{
  Indice i;
  printf("Polynomial Fonction : %s\n", name.chars());
  printf("f(x) = ");
  for (i = 0; i < fragment.size(); i++)
  {
    if (fragment(i).value < 0)
      printf(" - ");
    else if (i != 0)
      printf(" + ");
    printf("%lf*x^(%d)", Abs(fragment(i).value), fragment(i).factor);
  }
  printf("\n");
}

//-----------------------------------------------------------------------------
void PolynomialFunction::flush()
//-----------------------------------------------------------------------------
{
  fragment.flush();
}

//-----------------------------------------------------------------------------
void PolynomialFunction::toGnuplot(String filename, Real xmin, Real xmax, Real steps)
//-----------------------------------------------------------------------------
{
  FILE *pfile;

  pfile = fopen(filename.chars(), "w");
  if (pfile == NULL)
    fatalError("open Gnuplot file", "failed to open file for write");
  fprintf(pfile, "#DynELA_plot :%s\n", name.chars());
  fprintf(pfile, "#plotted :%s\n", name.chars());

  for (Real x = xmin; x <= xmax; x += steps)
    fprintf(pfile, "%10.6E %10.6E\n", x, getValue(x));
  fclose(pfile);
}

//-----------------------------------------------------------------------------
String PolynomialFunction::convertToDynELASourceFile()
//-----------------------------------------------------------------------------
{
  String str = "";
  char sstr[1000];

  sprintf(sstr, "PolynomialFunction %s;\n", name.chars());
  str += sstr;

  for (Indice i = 0; i < getNumberOfFragments(); i++)
  {
    sprintf(sstr, "%s.set(%12.7E, %12.7E);\n", name.chars(), getFactor(i), getCoefficient(i));
    str += sstr;
  }
  return str;
}
