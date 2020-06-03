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
	Class SinusFunction implementation
*/
/*!
  \file SinusFunction.C
  \brief fichier .C de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

#include <SinusFunction.h>

//!constructeur par d�faut de la classe SinusFunction
/*!

  \author &copy; Olivier PANTALE
  \since 1.0.4
*/
//-----------------------------------------------------------------------------
SinusFunction::SinusFunction()
//-----------------------------------------------------------------------------
{
  constant = 0.0;
}

//-----------------------------------------------------------------------------
SinusFunction::~SinusFunction()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Real SinusFunction::getValue(Real x)
//-----------------------------------------------------------------------------
{
  Real value = constant;

  for (Indice i = 0; i < fragment.size(); i++)
  {
    if (fragment(i).type == sinus)
      value += fragment(i).factor * sin(fragment(i).pulse * x + fragment(i).constant);
    if (fragment(i).type == cosinus)
      value += fragment(i).factor * cos(fragment(i).pulse * x + fragment(i).constant);
  }

  return value;
}

//-----------------------------------------------------------------------------
void SinusFunction::setSin(Real coef, Real pulse, Real constant)
//-----------------------------------------------------------------------------
{
  SinusFragment fr;
  fr.type = sinus;
  fr.factor = coef;
  fr.pulse = pulse;
  fr.constant = constant;
  fragment << fr;
}

//-----------------------------------------------------------------------------
void SinusFunction::setCos(Real coef, Real pulse, Real constant)
//-----------------------------------------------------------------------------
{
  SinusFragment fr;
  fr.type = cosinus;
  fr.factor = coef;
  fr.pulse = pulse;
  fr.constant = constant;
  fragment << fr;
}

//-----------------------------------------------------------------------------
void SinusFunction::setConstant(Real _constant)
//-----------------------------------------------------------------------------
{
  constant = _constant;
}

//-----------------------------------------------------------------------------
Real SinusFunction::getSlope(Real x)
//-----------------------------------------------------------------------------
{
  Real value = 0;

  for (Indice i = 0; i < fragment.size(); i++)
  {
    if (fragment(i).type == sinus)
      value += fragment(i).pulse * fragment(i).factor * cos(fragment(i).pulse * x + fragment(i).constant);
    if (fragment(i).type == cosinus)
      value -= fragment(i).pulse * fragment(i).factor * sin(fragment(i).pulse * x + fragment(i).constant);
  }

  return value;
}

//-----------------------------------------------------------------------------
void SinusFunction::print()
//-----------------------------------------------------------------------------
{
  printf("Sinus Fonction : %s\nf(x) = ", name.chars());
  if (constant != 0)
    printf("%lf ", constant);
  for (Indice i = 0; i < fragment.size(); i++)
  {
    if ((fragment(i).factor >= 0) && ((i != 0) || (constant != 0)))
      printf("+ ");
    if (fragment(i).factor < 0)
      printf("- ");
    if (fragment(i).type == sinus)
    {
      printf("%lf sin (%lf x", Abs(fragment(i).factor), fragment(i).pulse);
      if (fragment(i).constant != 0)
        printf(" %c %lf", (fragment(i).constant >= 0 ? '+' : '-'), Abs(fragment(i).constant));
      printf(") ");
    }
    if (fragment(i).type == cosinus)
    {
      printf("%lf cos (%lf x", Abs(fragment(i).factor), fragment(i).pulse);
      if (fragment(i).constant != 0)
        printf(" %c %lf", (fragment(i).constant >= 0 ? '+' : '-'), Abs(fragment(i).constant));
      printf(") ");
    }
  }

  printf("\n");
}

//-----------------------------------------------------------------------------
void SinusFunction::toGnuplot(String filename, Real xmin, Real xmax, Real steps)
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
short SinusFunction::getType(Indice i)
//-----------------------------------------------------------------------------
{
  return fragment(i).type;
}

//-----------------------------------------------------------------------------
Real SinusFunction::getPulse(Indice i)
//-----------------------------------------------------------------------------
{
  return fragment(i).pulse;
}

//-----------------------------------------------------------------------------
Real SinusFunction::getCoefficient(Indice i)
//-----------------------------------------------------------------------------
{
  return fragment(i).factor;
}

//-----------------------------------------------------------------------------
Real SinusFunction::getConstant(Indice i)
//-----------------------------------------------------------------------------
{
  return fragment(i).constant;
}

//-----------------------------------------------------------------------------
Real SinusFunction::getConstant()
//-----------------------------------------------------------------------------
{
  return constant;
}

//-----------------------------------------------------------------------------
Indice SinusFunction::getNumberOfFragments()
//-----------------------------------------------------------------------------
{
  return fragment.size();
}

//-----------------------------------------------------------------------------
String SinusFunction::convertToDynELASourceFile()
//-----------------------------------------------------------------------------
{
  String str = "";
  char sstr[1000];

  sprintf(sstr, "SinusFunction %s;\n", name.chars());
  str += sstr;

  if (constant != 0.0)
    sprintf(sstr, "%s.setConstant(%12.7E);\n", name.chars(), constant);
  str += sstr;

  for (Indice i = 0; i < getNumberOfFragments(); i++)
  {
    if (getType(i) == SinusFunction::cosinus)
      sprintf(sstr, "%s.setCos(%12.7E, %12.7E, %12.7E);\n", name.chars(), getCoefficient(i), getPulse(i), getConstant(i));
    if (getType(i) == SinusFunction::sinus)
      sprintf(sstr, "%s.setSin(%12.7E, %12.7E, %12.7E);\n", name.chars(), getCoefficient(i), getPulse(i), getConstant(i));
    str += sstr;
  }

  return str;
}
