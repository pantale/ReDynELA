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
	Class RampFunction implementation
*/
/*!
  \file RampFunction.C
  \brief fichier .C de d�finition des fonctions discr�tes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

#include <RampFunction.h>

//!constructeur par d�faut de la classe RampFunction
/*!

  \author &copy; Olivier PANTALE
  \since 1.0.4
*/
//-----------------------------------------------------------------------------
RampFunction::RampFunction()
//-----------------------------------------------------------------------------
{
  type = none;
}

//-----------------------------------------------------------------------------
RampFunction::~RampFunction()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void RampFunction::set(String st, Real _min, Real _max)
//-----------------------------------------------------------------------------
{
  min = _min;
  max = _max;
  Real mm = max - min;
  if (st == "linear")
  {
    type = linear;
    // equation y = a0 x + a1
    arg[0] = -2.0 / mm;
    arg[1] = 2.0 / mm;
    return;
  }
  if (st == "sinus")
  {
    type = sinus;
    // equation y = a0*(1-sin(a1 x));
    arg[1] = PI / 2.0;
    arg[0] = 1.0 / (1.0 - 2.0 / PI) / mm;
    return;
  }
  if (st == "constant")
  {
    type = constant;
    // equation y = a0;
    arg[0] = 1.0 / mm;
    return;
  }
  if (st == "erf")
  {
    type = erf;
    // equation y = exp(-(a0*x)^2) / a1;
    arg[0] = 5;
    arg[1] = sqrt(PI) * mm / 10;
    return;
  }
  if (st == "soft")
  {
    type = soft;
    // equation y = cos(PI * x)+1;
    arg[0] = 1 / mm;
    return;
  }

  fatalError("RampFunction::set", "%s type is unknown for ramp functions", st.chars());
}

//-----------------------------------------------------------------------------
Real RampFunction::getValue(Real x)
//-----------------------------------------------------------------------------
{
  Real value = 0;

  if (type == none)
    return 0;

  if ((x < min) || (x > max))
    return 0;

  // map function in range [0-1]
  Real point = (x - min) / (max - min);

  switch (type)
  {
  case linear:
  {
    return arg[0] * point + arg[1];
  }
  break;
  case sinus:
  {
    return arg[0] * (1.0 - sin(arg[1] * point));
  }
  break;
  case constant:
  {
    return arg[0];
  }
  break;
  case erf:
  {
    return exp(-SQ(arg[0] * point)) / arg[1];
  }
  break;
  case soft:
  {
    return arg[0] * (PI * sin(PI * point) / 2.0);
  }
  break;
  default:
    internalFatalError("RampFunction::getValue", "Unknown function %d\n", type);
  }

  return value;
}

//-----------------------------------------------------------------------------
Real RampFunction::getSlope(Real x)
//-----------------------------------------------------------------------------
{
  Real value = 0;

  if (type == none)
    return 0;

  if ((x < min) || (x > max))
    return 0;

  // map function in range [0-1]
  Real point = (x - min) / (max - min);

  switch (type)
  {
  case linear:
    return arg[0];
    break;
  case sinus:
    return -arg[0] * arg[1] * cos(arg[0] * point);
    break;
  case erf:
    return -(2 * point * SQ(arg[0]) * exp(-SQ(point * arg[0]))) / arg[1];
    break;
  case constant:
    return 0;
    break;
  case soft:
  {
    return arg[0] * SQ(PI) * cos(PI * point) / 2.0;
  }
  break;
  default:
    internalFatalError("RampFunction::getValue", "Unknown function %d\n", type);
  }

  return value;
}

//-----------------------------------------------------------------------------
void RampFunction::print()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void RampFunction::toGnuplot(String filename, Indice steps)
//-----------------------------------------------------------------------------
{
  FILE *pfile;

  pfile = fopen(filename.chars(), "w");
  if (pfile == NULL)
    fatalError("open Gnuplot file", "failed to open file for write");
  fprintf(pfile, "#DynELA_plot :%s\n", name.chars());
  fprintf(pfile, "#plotted :%s\n", name.chars());

  Indice st = 0;
  Real x, dx;
  Real xmin, xmax;
  xmin = min - (max - min) / 2;
  xmax = max + (max - min) / 2;
  x = xmin;
  dx = (xmax - xmin) / steps;
  for (st = 0; st <= steps; st++)
  {
    fprintf(pfile, "%10.6E %10.6E\n", x, getValue(x));
    x += dx;
  }
  fclose(pfile);
}

//-----------------------------------------------------------------------------
short RampFunction::getType(Indice i)
//-----------------------------------------------------------------------------
{
  return type;
}

//-----------------------------------------------------------------------------
String RampFunction::convertToDynELASourceFile()
//-----------------------------------------------------------------------------
{
  String str = "";
  char sstr[1000];

  sprintf(sstr, "RampFunction %s;\n", name.chars());
  str += sstr;

  switch (type)
  {
  case linear:
    sprintf(sstr, "%s.set(\"linear\", %12.7E, %12.7E);\n", name.chars(), min, max);
    break;
  case sinus:
    sprintf(sstr, "%s.set(\"sinus\", %12.7E, %12.7E);\n", name.chars(), min, max);
    break;
  case constant:
    sprintf(sstr, "%s.set(\"constant\", %12.7E, %12.7E);\n", name.chars(), min, max);
    break;
  case erf:
    sprintf(sstr, "%s.set(\"erf\", %12.7E, %12.7E);\n", name.chars(), min, max);
    break;
  case soft:
    sprintf(sstr, "%s.set(\"soft\", %12.7E, %12.7E);\n", name.chars(), min, max);
    break;
  default:
    fatalError("RampFunction::outDyn", " case not implemented\n");
  }
  str += sstr;

  return str;
}
