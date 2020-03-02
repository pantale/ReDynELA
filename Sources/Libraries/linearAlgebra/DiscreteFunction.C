/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2016                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file DiscreteFunction.C
  \brief Definition file for the discrete function class

  This file is the definition file for the discrete function class. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#include <DiscreteFunction.h>

//!Default constructor for the coupleReal class
//-----------------------------------------------------------------------------
coupleReal::coupleReal()
//-----------------------------------------------------------------------------
{
}

//!Constructor for the coupleReal class with initailisation of the coordinates of the point
/*!
  \param abscisse X coordinate of the new point
  \param ordonnee Y coordinate of the new point
*/
//-----------------------------------------------------------------------------
coupleReal::coupleReal(Real abscisse, Real ordonnee)
//-----------------------------------------------------------------------------
{
    x = abscisse;
    y = ordonnee;
}

//!Copy constructor for the coupleReal class
//-----------------------------------------------------------------------------
coupleReal::coupleReal(const coupleReal &X)
//-----------------------------------------------------------------------------
{
}

//!Destructor for the coupleReal class
//-----------------------------------------------------------------------------
coupleReal::~coupleReal()
//-----------------------------------------------------------------------------
{
}

//!Default constructor for the DiscreteFunction class
//-----------------------------------------------------------------------------
DiscreteFunction::DiscreteFunction()
//-----------------------------------------------------------------------------
{
    style = 0;
    style2 = 0;
    Sort = True;
}

//!Copy constructor for the DiscreteFunction class
//-----------------------------------------------------------------------------
DiscreteFunction::DiscreteFunction(const DiscreteFunction &X)
//-----------------------------------------------------------------------------
{
}

//!Add a new point (X,Y) to the current discrete function
/*!
  This method add a new point to the current discrete function.
  Depending of the \ref Sort variable, the new point is added at the end of the curve or inserted at it's right place into the curve depending on it's X value.
  \param abscisse X coordinate of the new point
  \param ordonnee Y coordinate of the new point
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::add(Real abscisse, Real ordonnee)
//-----------------------------------------------------------------------------
{
    // init memoire
    coupleReal *ppoint = new coupleReal;

    // affectation des coordonn�es
    ppoint->x = abscisse;
    ppoint->y = ordonnee;

    // ajout � la liste
    lpoints << ppoint;

    if ((lpoints.size() > 1) && (Sort))
        if (abscisse < lpoints(lpoints.size() - 2)->x)
        {
            lpoints.sort(compare);
        }
}

//!Add a new point (X,Y) to the current discrete function
/*!
  This method add a new point as the first point of the current discrete function.
  Depending of the \ref Sort variable, the new point is added at the beginning of the curve or inserted at it's right place into the curve depending on it's X value.
  \param abscisse X coordinate of the new point
  \param ordonnee Y coordinate of the new point
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::startPointAdd(Real abscisse, Real ordonnee)
//-----------------------------------------------------------------------------
{
    // if autosort then add at the end simply
    if (Sort)
    {
        add(abscisse, ordonnee);
        return;
    }

    // init memoire
    coupleReal *ppoint = new coupleReal;

    // affectation des coordonn�es
    ppoint->x = abscisse;
    ppoint->y = ordonnee;

    // ajout � la liste au d�but
    lpoints.insert(ppoint, 0);
}

//!Destructor of the DiscreteFunction class
//-----------------------------------------------------------------------------
DiscreteFunction::~DiscreteFunction()
//-----------------------------------------------------------------------------
{
}

//!Returns the number of points defining the discrete function
/*!
  \see getNumberOfPoints()
*/
//-----------------------------------------------------------------------------
Indice DiscreteFunction::size()
//-----------------------------------------------------------------------------
{
    return lpoints.size();
}

//!Verify that the given X coordinate is in the X range of the function.
/*!
  \param abscisse X coordinate of the point to test
  \return True if the given coordinate X in in the correct range
*/
//-----------------------------------------------------------------------------
Boolean DiscreteFunction::intoBoundsX(Real abscisse)
//-----------------------------------------------------------------------------
{
    return (abscisse >= lpoints(0)->x && abscisse <= lpoints(lpoints.size() - 1)->x);
}

//!Compute the Y coordinate corresponding to the given X coordinate
/*!
  This method computes the Y coordinate corresponding to the given X coordinate using a linear interpolation method.
  \param abscisse X coordinate of the point to compute
  \return Y coordinate of the corresponding point
*/
//-----------------------------------------------------------------------------
Real DiscreteFunction::getValue(Real abscisse)
//-----------------------------------------------------------------------------
{
    Indice i, nb;
    coupleReal *pt;
    coupleReal *pt0;

#ifdef VERIF_bounds
    if (abscisse < lpoints(0)->x || abscisse > lpoints(lpoints.size() - 1)->x)
        fatalError("DiscreteFunction::getValue(Real abscisse)",
                   "value %lf is out of bounds [%lf,%lf]\n", abscisse,
                   lpoints(0)->x, lpoints(lpoints.size() - 1)->x);
#endif

    nb = lpoints.size();
    pt0 = lpoints.first();

    for (i = 1; i < nb; i++)
    {
        pt = lpoints(i);
        if (pt->x >= abscisse)
        {
            if (pt->x == abscisse)
            {
                return pt->y;
            }
            return ((abscisse - pt0->x) / (pt->x - pt0->x)) * (pt->y - pt0->y) + pt0->y;
        }
        pt0 = pt;
    }

    return 0;
}

//!Compute the X coordinate corresponding to the given Y coordinate
/*!
  This method computes the X coordinate corresponding to the given Y coordinate using a linear interpolation method.
  \param abscisse Y coordinate of the point to compute
  \return X coordinate of the corresponding point
*/
//-----------------------------------------------------------------------------
Real DiscreteFunction::getInverseValue(Real ordonnee)
//-----------------------------------------------------------------------------
{
    Indice i, start = 0, stop = 0;

#ifdef VERIF_bounds
    if (ordonnee > maxY() || ordonnee < minY())
        fatalError("DiscreteFunction::getInverseValue(Real abscisse)",
                   "value %lf is out of bounds [%lf,%lf]\n", ordonnee, minY(),
                   maxY());
#endif

    if (lpoints(0)->y == ordonnee)
    {
        return lpoints(0)->x;
    }
    if (lpoints(0)->y < ordonnee)
    {
        for (i = 1; i < lpoints.size(); i++)
        {
            if (lpoints(i)->y >= ordonnee)
            {
                if (lpoints(i)->y == ordonnee)
                {
                    return lpoints(i)->x;
                }
                start = i - 1;
                i = lpoints.size();
            }
        }
    }
    else
    {
        for (i = 1; i < lpoints.size(); i++)
        {
            if (lpoints(i)->y <= ordonnee)
            {
                if (lpoints(i)->y == ordonnee)
                {
                    return lpoints(i)->x;
                }
                start = i - 1;
                i = lpoints.size();
            }
        }
    }

    stop = start + 1;

    // sinon, on interpole
    return ((ordonnee - lpoints(start)->y) / (lpoints(stop)->y -
                                              lpoints(start)->y)) *
               (lpoints(stop)->x - lpoints(start)->x) +
           lpoints(start)->x;
}

//!Compute the slope of the discrete function at a given X coordinate
/*!
  This method is used to compute the slope of the discrete function at a given X coordinate.
  The slope of the function is more or less equal to the derivative of the function at the given point.
  \param abscisse X coordinate of the point to compute
  \return Slope of the function at the corresponding point
*/
//-----------------------------------------------------------------------------
Real DiscreteFunction::getSlope(Real abscisse)
//-----------------------------------------------------------------------------
{
    Indice i, nb;
    coupleReal *pt;
    coupleReal *pt0;

#ifdef VERIF_bounds
    if (abscisse < lpoints(0)->x || abscisse > lpoints(lpoints.size() - 1)->x)
        fatalError("DiscreteFunction::getSlope(Real abscisse)",
                   "value %lf is out of bounds [%lf,%lf]\n", abscisse,
                   lpoints(0)->x, lpoints(lpoints.size() - 1)->x);
#endif

    nb = lpoints.size();
    pt0 = lpoints.first();

    for (i = 1; i < nb; i++)
    {
        pt = lpoints(i);
        if (pt->x >= abscisse)
        {
            return (pt->y - pt0->y) / (pt->x - pt0->x);
        }
        pt0 = pt;
    }

    return 0;
}

//!Printout the discrete function definition
//-----------------------------------------------------------------------------
void DiscreteFunction::print()
//-----------------------------------------------------------------------------
{
    Indice i;
    printf("Discrete Fonction : %s\n", name.chars());
    for (i = 0; i < lpoints.size(); i++)
    {
        printf("point %ld [%f,%f]\n", i + 1, lpoints(i)->x, lpoints(i)->y);
    }
}

//!Returns the maximum X value of the discrete function
/*!
  \return Maximum X value of the discrete function
*/
//-----------------------------------------------------------------------------
Real DiscreteFunction::maxX()
//-----------------------------------------------------------------------------
{
    return (lpoints(lpoints.size() - 1))->x;
}

//!Returns the minimum X value of the discrete function
/*!
  \return Minimum X value of the discrete function
*/
//-----------------------------------------------------------------------------
Real DiscreteFunction::minX()
//-----------------------------------------------------------------------------
{
    return (lpoints(0))->x;
}

//!Returns the maximum Y value of the discrete function
/*!
  \return Maximum Y value of the discrete function
*/
//-----------------------------------------------------------------------------
Real DiscreteFunction::maxY()
//-----------------------------------------------------------------------------
{
    Real val;
    val = (lpoints(0))->y;
    for (Indice i = 1; i < lpoints.size(); i++)
    {
        val = ((lpoints(i))->y > val ? (lpoints(i))->y : val);
    }
    return val;
}

//!Returns the minimum Y value of the discrete function
/*!
  \return Minimum Y value of the discrete function
*/
//-----------------------------------------------------------------------------
Real DiscreteFunction::minY()
//-----------------------------------------------------------------------------
{
    Real val;
    val = (lpoints(0))->y;
    for (Indice i = 1; i < lpoints.size(); i++)
    {
        val = ((lpoints(i))->y < val ? (lpoints(i))->y : val);
    }
    return val;
}

//-----------------------------------------------------------------------------
void DiscreteFunction::sort()
//-----------------------------------------------------------------------------
{
    lpoints.sort(compare);
}

//-----------------------------------------------------------------------------
Boolean compare(coupleReal *t1, coupleReal *t2)
//-----------------------------------------------------------------------------
{
    // comparaison sur x
    return (t1->x > t2->x);
}

//!Save the current discrete function for using with the Gnuplot software
/*!
  This method saves the current discrete function for using with the Gnuplot software.
  \param filename Name of the datafile to use for writting the discrete function
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::toGnuplot(String filename)
//-----------------------------------------------------------------------------
{
    FILE *pfile;

    pfile = fopen(filename.chars(), "w");
    if (pfile == NULL)
    {
        fatalError("open Gnuplot file", "failed to open file for write");
    }
    fprintf(pfile, "#DynELA_plot :%s\n", name.chars());
    fprintf(pfile, "#plotted :%s\n", name.chars());

    for (Indice i = 0; i < lpoints.size(); i++)
    {
        fprintf(pfile, "%10.6E %10.6E\n", lpoints(i)->x, lpoints(i)->y);
    }
    fclose(pfile);
}

//!Read a discrete function from an ASCII file
/*!
  This method reads a discrete function from an ASCII file.
  The X and Y values of the curve are in separate columns on the same line.
  Several values may be given on the line, the number of values on a line is specified by the number of columns that contain data on X and Y axes.
  \param filename Name of the datafile to use for reading the discrete function
  \param xCol Reference number of the column containing the values in the abscissa
  \param yCol Reference number of the column containing the values in the ordinate
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::fromFile(String filename, int xCol, int yCol)
//-----------------------------------------------------------------------------
{
    FILE *pfile;
    Real x, y;
    String st;

    try
    {
        pfile = fopen(filename.chars(), "r");
        IfException(pfile == NULL, StandardExceptions::FileNotFound);
    }
    catch (Exception &e)
    {
        e.print();
        exit(-1);
    }

    while (st.scanFileLine(pfile))
    {
        x = st.getRealAtPos(xCol);
        y = st.getRealAtPos(yCol);
        add(x, y);
    }
    fclose(pfile);
}

//!Remap the discrete function on X or Y axis
/*!
  This method is used to remap a discrete function on X or Y axis.
  Ranges of the X and Y axis are modified by the user.
  This method returns a new discrete function with the same name as the current one + "_remap".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_remap".
  \param xm Minimum value on the X axis
  \param xM Maximum value on the X axis
  \param ym Minimum value on the Y axis
  \param yM Maximum value on the Y axis
  \return Remapped discrete function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::remapCurve(Real xm, Real xM, Real ym, Real yM)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *remapped = new DiscreteFunction;
    Real x, y;
    Real mx, Mx, my, My;
    mx = minX();
    Mx = maxX();
    my = minY();
    My = maxY();
    Indice sz = lpoints.size();

    // recopie de la fonction
    remapped->name = name + "_remap";

    for (Indice i = 0; i < sz; i++)
    {
        x = lpoints(i)->x;
        y = lpoints(i)->y;
        if (xm != xM)
        {
            x = (x - mx) * (xM - xm) / (Mx - mx) + xm;
        }
        if (ym != yM)
        {
            y = (y - my) * (yM - ym) / (My - my) + ym;
        }

        remapped->add(x, y);
    }

    return remapped;
}

//!Derivate a discrete function
/*!
  This method computes the derivative of a discrete function.
  This method returns a new discrete function with the same name as the current one + "_derivative".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_derivative".
  \return Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::derivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    Indice sz = lpoints.size();

    if (sz < 2)
        fatalError("Derivate function",
                   "can't derivate a function with less than two points");

    derive->add(lpoints(0)->x,
                (lpoints(1)->y - lpoints(0)->y) / (lpoints(1)->x -
                                                   lpoints(0)->x));

    for (Indice i = 1; i < sz - 1; i++)
        derive->add(lpoints(i)->x,
                    (lpoints(i)->y - lpoints(i - 1)->y) / (lpoints(i)->x -
                                                           lpoints(i -
                                                                   1)
                                                               ->x));

    derive->add(lpoints(sz - 1)->x,
                (lpoints(sz - 1)->y -
                 lpoints(sz - 2)->y) /
                    (lpoints(sz - 1)->x - lpoints(sz -
                                                  2)
                                              ->x));

    derive->name = name + "_derivate";

    return derive;
}

//!Euler Backward derivate a discrete function
/*!
  This method computes the Euler Backward derivative of a discrete function.
  The Euler Backward derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{f(x)-f(x-dx)}{dx} \f]
  This method returns a new discrete function with the same name as the current one + "_EBderivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_EBderivate".
  \return Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::EulerBackwardDerivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    Indice sz = lpoints.size();

    if (sz < 2)
        fatalError("Euler Backward Derivate function",
                   "can't derivate a function with less than two points");

    for (Indice i = 1; i < sz; i++)
        derive->add(lpoints(i)->x,
                    (lpoints(i)->y - lpoints(i - 1)->y) / (lpoints(i)->x -
                                                           lpoints(i -
                                                                   1)
                                                               ->x));

    derive->name = name + "_EBderivate";

    return derive;
}

//!Euler Forward derivate a discrete function
/*!
  This method computes the Euler Forward derivative of a discrete function.
  The Euler Forward derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{f(x+dx)-f(x)}{dx} \f]
  This method returns a new discrete function with the same name as the current one + "_EFderivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_EFderivate".
  \return Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::EulerForwardDerivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    Indice sz = lpoints.size();

    if (sz < 2)
        fatalError("Euler Forward Derivate function",
                   "can't derivate a function with less than two points");

    for (Indice i = 0; i < sz - 1; i++)
        derive->add(lpoints(i)->x,
                    (lpoints(i + 1)->y - lpoints(i)->y) / (lpoints(i + 1)->x -
                                                           lpoints(i)->x));

    derive->name = name + "_EFderivate";

    return derive;
}

//!Central-Difference derivate a discrete function
/*!
  This method computes the Central-Difference derivative of a discrete function.
  The Central-Difference derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{f(x+dx)-f(x-dx)}{2dx} \f]
  This method returns a new discrete function with the same name as the current one + "_CDderivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_CDderivate".
  \return Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::CentralDifferenceDerivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    Indice sz = lpoints.size();

    if (sz < 3)
        fatalError("Central Difference Derivate function",
                   "can't derivate a function with less than two points");

    for (Indice i = 1; i < sz - 1; i++)
        derive->add(lpoints(i)->x,
                    (lpoints(i + 1)->y -
                     lpoints(i - 1)->y) /
                        (lpoints(i + 1)->x - lpoints(i -
                                                     1)
                                                 ->x));

    derive->name = name + "_CDderivate";

    return derive;
}

//!Zero Forcing 1 derivate a discrete function
/*!
  This method computes the Zero Forcing 1 derivative of a discrete function.
  The Zero Forcing 1 derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{0.2047f(x+dx)+0.886f(x)-1.386f(x-dx)+0.2953f(x-2dx)}{dx} \f]
  This method returns a new discrete function with the same name as the current one + "_ZF1derivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_ZF1derivate".
  \return Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::ZeroForcing1Derivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    Indice sz = lpoints.size();

    if (sz < 4)
        fatalError("Zero forcing # 1 Derivate function",
                   "can't derivate a function with less than two points");

    for (Indice i = 2; i < sz - 1; i++)
        derive->add(lpoints(i)->x,
                    3. * (0.2047 * lpoints(i + 1)->y + 0.886 * lpoints(i)->y - 1.386 * lpoints(i - 1)->y + 0.2953 * lpoints(i - 2)->y) /
                        (lpoints(i + 1)->x - lpoints(i - 2)->x));

    derive->name = name + "_ZF1derivate";

    return derive;
}

//!Zero Forcing 2 derivate a discrete function
/*!
  This method computes the Zero Forcing 2 derivative of a discrete function.
  The Zero Forcing 2 derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{-0.2047f(x+dx)-0.886f(x)+1.386f(x-dx)-0.2953f(x-2dx)}{dx} \f]
  This method returns a new discrete function with the same name as the current one + "_ZF2derivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_ZF2derivate".
  \return Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::ZeroForcing2Derivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    Indice sz = lpoints.size();

    if (sz < 4)
        fatalError("Zero forcing # 2 Derivate function",
                   "can't derivate a function with less than two points");

    for (Indice i = 1; i < sz - 2; i++)
        derive->add(lpoints(i)->x,
                    3. * (-0.2047 * lpoints(i - 1)->y - 0.886 * lpoints(i)->y + 1.386 * lpoints(i + 1)->y - 0.2953 * lpoints(i + 2)->y) /
                        (lpoints(i + 2)->x - lpoints(i - 1)->x));

    derive->name = name + "_ZF2derivate";

    return derive;
}

//!Integration of a discrete function
/*!
  This method computes the integral of a discrete function using the Simpson method.
  This method returns a new discrete function with the same name as the current one + "_integrate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_integrate".
  \return Integrate of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::integrate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *integr = new DiscreteFunction;
    Indice sz = lpoints.size();
    double intg;

    if (sz < 2)
        fatalError("Integrate function",
                   "can't integrate a function with less than two points");

    integr->add(lpoints(0)->x, 0.);
    intg = 0.;

    for (Indice i = 1; i < sz; i++)
    {
        intg +=
            (lpoints(i)->x - lpoints(i - 1)->x) * (lpoints(i)->y + lpoints(i - 1)->y) / 2.;
        integr->add(lpoints(i)->x, intg);
    }

    integr->name = name + "_integrate";

    return integr;
}

//!Mobile average of a discrete function
/*!
  This method computes the mobile average of a discrete function.
  If \f$ f(x) \f$ is the original discrete function, the the mobile average curve \f$ f_{m}(x) \f$ is defined by the following:
  \f[ f_{m}(x)=\frac{1}{n_{1}+n_{2}+1}\sum ^{i=x+n_{2}}_{i=x-n_{1}}f(i) \f] 
  where \f$ n_{1} \f$ et \f$ n_{2} \f$ are corresponding to the given parameters \c before et \c after respectively.
  If \f$ n_{1}=n_{2} \f$ the mobile average is centered, else it will be non-centered. 
  The number of points of the filtered curve is less than the number of points of the original curve and is defined by \f$ n=1+n_{1}+n_{2} \f$.
  This method returns a new discrete function with the same name as the current one + "_movingAverage".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_movingAverage".
  \return Mobile average of the function
  \param before Number of points to consider on the left side
  \param after Number of points to consider on the right side
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::movingAverage(int before, int after)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *average = new DiscreteFunction;
    Indice sz = lpoints.size();
    double valeur;

    for (Indice i = before; i < sz - after; i++)
    {
        valeur = 0.;
        for (Indice j = i - before; j <= i + after; j++)
        {
            valeur += lpoints(j)->y;
        }
        average->add(lpoints(i)->x, valeur / (after + before + 1));
    }

    average->name = name + "_movingAverage";

    return average;
}

//!Combine two functions into a new one
/*!
  This method combines two functions into a new one.
  From the given two curves \f$ y_{1}=f_{1}(x) \f$ and \f$ y_{2}=f_{2}(x) \f$ we create the new curve \f$ y_{2}=f(y_{1}) \f$
  This method returns a new discrete function with the same name as the current ones + "_vs_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_vs_curve2".
  \return Combined discrete function
  \param fonction Second function to combine with
  \param points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::xyMerge(DiscreteFunction *fonction, Indice points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    Real mX, MX;
    Real dx;

    mX = Max(minX(), fonction->minX());
    MX = Min(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (Real x = mX; x <= MX; x += dx)
    {
        mix->add(getValue(x), fonction->getValue(x));
    }

    mix->name = fonction->name + "_vs_" + name;
    return mix;
}

//!Absolute value of a discrete function
/*!
  This method creates a new fonction from the given one as the absolute value of the function defined by \f$ f2=ABS(f1) \f$
  This method returns a new discrete function with the same name as the current one + "_abs".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_abs".
  \return Absolute value of the discrete function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::absoluteValueCurve()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;

    for (Indice i = 0; i < lpoints.size(); i++)
    {
        mix->add(lpoints(i)->x, Abs(lpoints(i)->y));
    }

    // apply the name
    mix->name = "abs_" + name;

    // returns the function
    return mix;
}

//!Multiplication by a scalar value of a function
/*!
  This method creates a new fonction from the given one as the product of the given function by a scalar value \f$ f2=\lambda.f1 \f$
  This method returns a new discrete function with the same name as the current one + "scale_".
  Therefore, if the current function name is "curve" then the new created function name will be "scale_curve".
  \return The new function
  \param factor Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::scaleCurve(Real factor)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;

    for (Indice i = 0; i < lpoints.size(); i++)
    {
        mix->add(lpoints(i)->x, factor * lpoints(i)->y);
    }

    // apply the name
    mix->name = "scale_" + name;

    // returns the function
    return mix;
}

//!Offset of a discrete function
/*!
  This method creates a new fonction from the given one with a offset on the Y axis by a scalar value \f$ f2=f1+Cte \f$
  This method returns a new discrete function with the same name as the current one + "offset_".
  Therefore, if the current function name is "curve" then the new created function name will be "offset_curve".
  \return The new function
  \param offset Offset of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::offsetCurve(Real offset)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;

    for (Indice i = 0; i < lpoints.size(); i++)
    {
        mix->add(lpoints(i)->x, offset + lpoints(i)->y);
    }

    // apply the name
    mix->name = "offset_" + name;

    // returns the function
    return mix;
}

//!Add two discrete functions
/*!
  This method creates a new fonction from the given two ones as the sum of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=f_1(x)+f_2(x) \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_plus_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_plus_curve2".
  \return The new function
  \param fonction Second function to work with
  \param points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::addCurves(DiscreteFunction *fonction, Indice points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    Real mX, MX;
    Real dx;

    mX = Max(minX(), fonction->minX());
    MX = Min(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (Real x = mX; x <= MX; x += dx)
    {
        mix->add(x, getValue(x) + fonction->getValue(x));
    }

    mix->name = name + "_plus_" + fonction->name;
    return mix;
}

//!Substract two discrete functions
/*!
  This method creates a new fonction from the given two ones as the sustraction of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=f_1(x)-f_2(x) \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_minus_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_minus_curve2".
  \return The new function
  \param fonction Second function to work with
  \param points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::substractCurves(DiscreteFunction *fonction, Indice points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    Real mX, MX;
    Real dx;

    mX = Max(minX(), fonction->minX());
    MX = Min(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (Real x = mX; x <= MX; x += dx)
    {
        mix->add(x, getValue(x) - fonction->getValue(x));
    }

    mix->name = name + "_minus_" + fonction->name;
    return mix;
}

//!Multiply two discrete functions
/*!
  This method creates a new fonction from the given two ones as the product of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=f_1(x)*f_2(x) \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_mult_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_mult_curve2".
  \return The new function
  \param fonction Second function to work with
  \param points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *
DiscreteFunction::multiplyCurves(DiscreteFunction *fonction, Indice points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    Real mX, MX;
    Real dx;

    mX = Max(minX(), fonction->minX());
    MX = Min(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (Real x = mX; x <= MX; x += dx)
    {
        mix->add(x, getValue(x) * fonction->getValue(x));
    }

    mix->name = name + "_mult_" + fonction->name;
    return mix;
}

//!Divides two discrete functions
/*!
  This method creates a new fonction from the given two ones as the division of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=\frac{f_1(x)}{f_2(x)} \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_div_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_div_curve2".
  \return The new function
  \warning Function \f$ f_2(x) \f$ must not cut the X axis or it will generate a division by zero.
  \param fonction Second function to work with
  \param points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::divideCurves(DiscreteFunction *fonction, Indice points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    Real mX, MX;
    Real dx;

    mX = Max(minX(), fonction->minX());
    MX = Min(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (Real x = mX; x <= MX; x += dx)
    {
        mix->add(x, getValue(x) / fonction->getValue(x));
    }

    mix->name = name + "_div_" + fonction->name;
    return mix;
}

//!Relative error of two discrete functions
/*!
  This method creates a new fonction from the given two ones as the relative error of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=\frac {(f_2(x) - f_1(x))}{ f_1(x)} \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_error_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_error_curve2".
  \return The new function
  \param fonction Second function to work with
  \param points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::errorCurves(DiscreteFunction *fonction, Indice points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    Real mX, MX;
    Real dx;

    mX = Max(minX(), fonction->minX());
    MX = Min(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (Real x = mX; x <= MX; x += dx)
    {
        mix->add(x, (fonction->getValue(x) - getValue(x)) / getValue(x));
    }

    mix->name = name + "_error_" + fonction->name;
    return mix;
}

//!Minimal distance between two curves on the X axis
/*!
  This method creates a new fonction from the given two ones as the minimal distance between the two curves on the X axis.
  This method calculates a minimum distance between two curves and returns a curve having the curvilinear abscissa in abscissa and in ordinate the minimum distance between two curves at the point considered.
  This method returns a new discrete function with the same name as the current ones + "_distanceX_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_distanceX_curve2".
  \param fonction Second function to work with
  \return The new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::distanceCurvesX(DiscreteFunction *fonction)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *distfunct = new DiscreteFunction;
    Real dist;
    Real dist2;
    Real x, y;
    Real x0 = 0.0; //,y0=0.0;

    for (Indice i = 0; i < lpoints.size(); i++)
    {
        x = lpoints(i)->x;
        y = lpoints(i)->y;
        dist = SQ(x - fonction->lpoints(0)->x) + SQ(y - fonction->lpoints(0)->y);
        for (Indice ii = 1; ii < fonction->lpoints.size(); ii++)
        {
            dist2 = SQ(x - fonction->lpoints(ii)->x) + SQ(y - fonction->lpoints(ii)->y);
            if (dist2 < dist)
            {
                dist = dist2;
                x0 = (fonction->lpoints(ii)->x + x) / 2.0;
            }
        }
        distfunct->add(x0, sqrt(dist));
    }
    distfunct->name = name + "_distanceX_" + fonction->name;
    return distfunct;
}

//!Minimal distance between two curves on the Y axis
/*!
  This method creates a new fonction from the given two ones as the minimal distance between the two curves on the Y axis.
  This method calculates a minimum distance between two curves and returns a curve having the curvilinear ordinate in abscissa and in ordinate the minimum distance between two curves at the point considered.
  This method returns a new discrete function with the same name as the current ones + "_distanceY_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_distanceY_curve2".
  \param fonction Second function to work with
  \return The new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::distanceCurvesY(DiscreteFunction *fonction)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *distfunct = new DiscreteFunction;
    Real dist;
    Real dist2;
    Real x, y;
    Real y0 = 0.0;

    for (Indice i = 0; i < lpoints.size(); i++)
    {
        x = lpoints(i)->x;
        y = lpoints(i)->y;
        dist = SQ(x - fonction->lpoints(0)->x) + SQ(y - fonction->lpoints(0)->y);
        for (Indice ii = 1; ii < fonction->lpoints.size(); ii++)
        {
            dist2 = SQ(x - fonction->lpoints(ii)->x) + SQ(y - fonction->lpoints(ii)->y);
            if (dist2 < dist)
            {
                dist = dist2;
                y0 = (fonction->lpoints(ii)->y + y) / 2.0;
            }
        }
        distfunct->add(y0, sqrt(dist));
    }
    distfunct->name = name + "_distanceY_" + fonction->name;
    return distfunct;
}

//!Minimal distance between two curves
/*!
  This method creates a new fonction from the given two ones as the minimal distance between the two curves.
  This method calculates a minimum distance between two curves and returns a curve having the curvilinear ordinate in abscissa and in ordinate the minimum distance between two curves at the point considered.
  This method returns a new discrete function with the same name as the current ones + "_distance_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_distance_curve2".
  \param fonction Second function to work with
  \return The new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::distanceCurves(DiscreteFunction *fonction)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *distfunct = new DiscreteFunction;
    Real absc = 0.0;
    Real dist;
    Real dist2;
    Real x, y;
    Real x0 = 0.0, y0 = 0.0, x1, y1;

    for (Indice i = 0; i < lpoints.size(); i++)
    {
        // mise � jour de l'abcisse
        if (i > 0)
        {
            absc += sqrt(SQ(lpoints(i)->x - lpoints(i - 1)->x) + SQ(lpoints(i)->y - lpoints(i - 1)->y));
        }
        x = lpoints(i)->x;
        y = lpoints(i)->y;
        dist = SQ(x - fonction->lpoints(0)->x) + SQ(y - fonction->lpoints(0)->y);
        for (Indice ii = 1; ii < fonction->lpoints.size(); ii++)
        {
            dist2 = SQ(x - fonction->lpoints(ii)->x) + SQ(y - fonction->lpoints(ii)->y);
            if (dist2 < dist)
            {
                dist = dist2;
                x1 = (fonction->lpoints(ii)->x + x) / 2.0;
                y1 = (fonction->lpoints(ii)->y + y) / 2.0;
            }
        }
        if (i != 0)
        {
            absc += sqrt(SQ(x1 - x0) + SQ(y1 - y0));
        }
        distfunct->add(absc, sqrt(dist));
        x0 = x1;
        y0 = y1;
    }
    distfunct->name = name + "_distance_" + fonction->name;
    return distfunct;
}

//!Order 1 low pass filter of a function
/*!
  This method creates a new fonction from the given one using an order 1 low pass filter. This type of filter has low frequency gain of 1. It is defined by the following equation:
  \f[ f(p)=\frac{1}{ap+1} \f]
  where \f$ p \f$ is the Laplace operator, \f$ p=j\omega \f$ with \f$ j^{2}=-1 \f$. Using a Backward Euler derivative the filtered form \f$ f_{f}(x) \f$ is therefore given by:
  \f[ f_{f}(x)=\frac{f(x)+\frac{a}{dx}f_{f}(x-dx)}{\frac{a}{dx}+1} \f]
  with the initial conditions on \f$ f_{f}(-dx) \f$
  This method returns a new discrete function with the same name as the current one + "_lowPass_1".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_lowPass_1".
  \return The new function
  \param A Cutting pulse for the filter
  \param points Number of points to generate for the new function
  \param Init Definition of the first point for the filter (\ref df_noInit: first value is 0, \ref df_averageInit: first value is the average value of the function, \ref df_fisrtValueInit: first value is the first value of the non filtered function)
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::lowPass_1(Real A, Indice points, int Init)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *filtered = new DiscreteFunction;
    Real dx;
    Real vfilter;

    dx = (maxX() - minX()) / points;

    vfilter =
        (Init ==
                 averageInit
             ? average()
             : (Init ==
                        fisrtValueInit
                    ? lpoints(0)->y
                    : 0.));

    for (Real x = minX(); x <= maxX(); x += dx)
    {
        vfilter = (getValue(x) + A / dx * vfilter) / (A / dx + 1.);
        filtered->add(x, vfilter);
    }

    filtered->name = name + "_lowPass_1";
    return filtered;
}

//!Order 2 low pass filter of a function
/*!
  This method creates a new fonction from the given one using an order 2 low pass filter. This type of filter has low frequency gain of 1. It is defined by the following equation:
  \f[ f(p)=\frac{1}{ap^{2}+bp+1} \f]
  where \f$ p \f$ is the Laplace operator, \f$ p=j\omega \f$ with \f$ j^{2}=-1 \f$. Using a Backward Euler derivative the filtered form \f$ f_{f}(x) \f$ is therefore given by:
  \f[ f_{f}(x)=\frac{f(x)+(\frac{2a}{dx^{2}}+\frac{b}{dx})f_{f}(x-dx)-\frac{a}{dx^{2}}f_{f}(x-2dx)}{\frac{a}{dx^{2}}+\frac{b}{dx}+1} \f]
  with the initial conditions on \f$ f_{f}(-2dx) \f$ and \f$ f_{f}(-dx) \f$
  This method returns a new discrete function with the same name as the current one + "_lowPass_2".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_lowPass_2".
  \return The new function
  \param A Cutting pulse for the filter
  \param Q Over value (usually between 0.7 and 1.0)
  \param points Number of points to generate for the new function
  \param Init Definition of the first point for the filter (\ref df_noInit: first value is 0, \ref df_averageInit: first value is the average value of the function, \ref df_fisrtValueInit: first value is the first value of the non filtered function)
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::lowPass_2(Real A, Real Q, Indice points, int Init)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *filtered = new DiscreteFunction;
    Real dx;
    Real vfilter1, vfilter2, vfilter;
    Real B;

    B = sqrt(A) / Q;

    dx = (maxX() - minX()) / points;

    vfilter1 =
        (Init ==
                 averageInit
             ? average()
             : (Init ==
                        fisrtValueInit
                    ? lpoints(0)->y
                    : 0.));
    vfilter2 = vfilter1;

    for (Real x = minX(); x <= maxX(); x += dx)
    {
        vfilter =
            (getValue(x) + (2. * A / SQ(dx) + B / dx) * vfilter1 -
             A / SQ(dx) * vfilter2) /
            (A / SQ(dx) + B / dx + 1.);
        filtered->add(x, vfilter);
        vfilter2 = vfilter1;
        vfilter1 = vfilter;
    }

    filtered->name = name + "_lowPass_2";
    return filtered;
}

//!Average value of a function
/*!
  This method computes the average value of a discrete function.
  \return Average value of a function
*/
//-----------------------------------------------------------------------------
Real DiscreteFunction::average()
//-----------------------------------------------------------------------------
{
    Real moyenne = 0.;

    if (lpoints.size() == 0)
    {
        return moyenne;
    }

    for (Indice i = 1; i < lpoints.size(); i++)
    {
        moyenne +=
            (lpoints(i)->y + lpoints(i - 1)->y) / 2.0 * (lpoints(i)->x - lpoints(i - 1)->x);
    }
    moyenne /= (maxX() - minX());

    return moyenne;
}

//!Constructor of the CrackFunction class
//-----------------------------------------------------------------------------
CrackFunction::CrackFunction()
//-----------------------------------------------------------------------------
{
    style = 0;
    style2 = 0;
    Sort = False;
}

//!Copy constructor of the CrackFunction class
//-----------------------------------------------------------------------------
CrackFunction::CrackFunction(const CrackFunction &X)
//-----------------------------------------------------------------------------
{
}

//!Destructor of the CrackFunction class
//-----------------------------------------------------------------------------
CrackFunction::~CrackFunction()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void CrackFunction::headTipAdd(Real x, Real y)
//-----------------------------------------------------------------------------
{
    DiscreteFunction::add(x, y);
}

//-----------------------------------------------------------------------------
void CrackFunction::tailTipAdd(Real x, Real y)
//-----------------------------------------------------------------------------
{
    DiscreteFunction::startPointAdd(x, y);
}

//-----------------------------------------------------------------------------
Boolean CrackFunction::distanceToSegmentShort(Real &dist, Indice i, Real x, Real y)
//-----------------------------------------------------------------------------
{
    dist = -1;
    Real x1, x2, y1, y2;
    x1 = lpoints(i)->x;
    x2 = lpoints(i + 1)->x;
    y1 = lpoints(i)->y;
    y2 = lpoints(i + 1)->y;

    // cas particulier // x
    if (y1 == y2)
    {
        if (x < Min(x1, x2))
        {
            return False;
        }
        if (x > Max(x1, x2))
        {
            return False;
        }
        dist = (y - y1);
        return True;
    }

    // cas particulier // y
    if (x1 == x2)
    {
        if (y < Min(y1, y2))
        {
            return False;
        }
        if (y > Max(y1, y2))
        {
            return False;
        }
        dist = (x - x1);
        return True;
    }

    if ((x2 - x1) * (x - x1) + (y2 - y1) * (y - y1) < 0)
    {
        return False;
    }
    if ((x1 - x2) * (x - x2) + (y1 - y2) * (y - y2) < 0)
    {
        return False;
    }

    // cas g�n�ral ax-y+b=0
    Real a, c;
    a = (y2 - y1) / (x2 - x1);
    c = y1 - a * x1;
    dist = (a * x - y + c) / sqrt(SQ(a) + 1);
    return True;
}

//-----------------------------------------------------------------------------
Boolean CrackFunction::isOnPositiveSide(Real x, Real y)
//-----------------------------------------------------------------------------
{
    Indice nearSeg, nearPoint;
    Real distMinSeg, distMinPoint, dist;
    Indice i;
    Real x1, y1, x2, y2, vx, vy;

    // moins de 2 points ?
    if (lpoints.size() < 2)
    {
        return False;
    }

    // point le plus proche
    nearSeg = -1;
    distMinSeg = 1e30;
    nearPoint = -1;
    distMinPoint = 1e30;

    // distance mini segment
    for (i = 0; i < lpoints.size() - 1; i++)
    {
        if (distanceToSegmentShort(dist, i, x, y) == True)
        {
            if (Abs(dist) < distMinSeg)
            {
                distMinSeg = Abs(dist);
                nearSeg = i;
            }
        }
    }

    // distance mini point
    for (i = 0; i < lpoints.size(); i++)
    {
        dist = SQ(lpoints(i)->x - x) + SQ(lpoints(i)->y - y);
        if (dist == 0.)
        {
            nearPoint = i;
            distMinPoint = dist;
            i = lpoints.size();
        }
        if (dist < distMinPoint)
        {
            distMinPoint = dist;
            nearPoint = i;
        }
    }
    distMinPoint = sqrt(distMinPoint);

    // traitement distance mini-segment
    if ((nearSeg >= 0) && (distMinSeg <= distMinPoint))
    {
        x1 = lpoints(nearSeg)->x;
        x2 = lpoints(nearSeg + 1)->x;
        y1 = lpoints(nearSeg)->y;
        y2 = lpoints(nearSeg + 1)->y;
        return ((y1 - y2) * (x - x1) + (x2 - x1) * (y - y1) >= 0);
    }

    //   cout << nearSeg << endl;

    // near the tip
    if (nearPoint == 0)
    {
        x2 = x - lpoints(nearPoint)->x;
        y2 = y - lpoints(nearPoint)->y;
        x1 = -(lpoints(nearPoint + 1)->y - lpoints(nearPoint)->y);
        y1 = lpoints(nearPoint + 1)->x - lpoints(nearPoint)->x;
        return (x2 * x1 + y2 * y1 >= 0);
    }

    // nearPoint the other tip
    if (nearPoint == lpoints.size() - 1)
    {
        x2 = x - lpoints(nearPoint)->x;
        y2 = y - lpoints(nearPoint)->y;
        x1 = -(lpoints(nearPoint)->y - lpoints(nearPoint - 1)->y);
        y1 = lpoints(nearPoint)->x - lpoints(nearPoint - 1)->x;
        return (x2 * x1 + y2 * y1 >= 0);
    }

    // vecteurs normaux au point
    vx = x - lpoints(nearPoint)->x;
    vy = y - lpoints(nearPoint)->y;
    x1 = -(lpoints(nearPoint)->y - lpoints(nearPoint - 1)->y);
    y1 = lpoints(nearPoint)->x - lpoints(nearPoint - 1)->x;
    x2 = -(lpoints(nearPoint + 1)->y - lpoints(nearPoint)->y);
    y2 = lpoints(nearPoint + 1)->x - lpoints(nearPoint)->x;
    Real cote = x1 * y2 - y1 * x2;
    Real p1 = x1 * vx + y1 * vy;
    Real p2 = x2 * vx + y2 * vy;

    if (cote >= 0)
    {
        if ((p1 >= 0) && (p2 >= 0))
        {
            return True;
        }
        return False;
    }
    else
    {
        if ((p1 <= 0) && (p2 <= 0))
        {
            return False;
        }
        return True;
    }

    return False;
}

//!Output format to DynELA source file
/*!
  This method is used to store the architecture of the discrete function using a DynELA source file format.
  The returned String can be written directly to the source file of the DynELA FEM code.
  \return The String containing the definition of the discrete function according to the DynELA source file format
*/
//-----------------------------------------------------------------------------
String DiscreteFunction::convertToDynELASourceFile()
//-----------------------------------------------------------------------------
{
    String str = "";
    char sstr[1000];

    sprintf(sstr, "DiscreteFunction %s;\n", name.chars());
    str += sstr;

    for (Indice i = 0; i < getNumberOfPoints(); i++)
    {
        sprintf(sstr, "%s.add(%12.7E, %12.7E);\n", name.chars(), getXPoint(i), getYPoint(i));
        str += sstr;
    }
    return str;
}

//!Constructor of the DiscreteFunctionSet class
//-----------------------------------------------------------------------------
DiscreteFunctionSet::DiscreteFunctionSet()
//-----------------------------------------------------------------------------
{
}

//!Copy constructor of the DiscreteFunctionSet class
//-----------------------------------------------------------------------------
DiscreteFunctionSet::DiscreteFunctionSet(const DiscreteFunctionSet &X)
//-----------------------------------------------------------------------------
{
}

//!Destructor of the DiscreteFunctionSet class
//-----------------------------------------------------------------------------
DiscreteFunctionSet::~DiscreteFunctionSet()
//-----------------------------------------------------------------------------
{
}

//!Reads the set of discrete functions on the disk
/*!
  \param st name of the file to read from
*/
//-----------------------------------------------------------------------------
void DiscreteFunctionSet::read(String st)
//-----------------------------------------------------------------------------
{
    FILE *pfile;
    String line;
    Real x, y;
    int number = 0;
    Boolean end = False;
    char str[100];
    int firstOne;

    firstOne = curves.size();

    if ((pfile = fopen(st.chars(), "r")) == NULL)
    {
        fatalError("Can't open file", "for curves");
    }

    // read the first line
    line.scanFileLine(pfile);

    //scan if it's a DynELA format
    if (line.chars()[0] == '#')
    {
        line = line.after("#");
        line.strip();

        // scan the DynELA logo
        sscanf(line.chars(), "%s", str);
        if (strcmp(str, "DynELA_plot") == 0)
        {
            line.scanFileLine(pfile);
            line = line.after(":");
            line.strip();
            while (!end)
            {
                sscanf(line.chars(), "%s", str);
                if (line.contains(" ") == True)
                {
                    line = line.after(" ");
                    line.strip();
                }
                else
                {
                    end = True;
                }
                DiscreteFunction *f = new DiscreteFunction;
                f->name = str;
                curves << f;
                number++;
            }
        }
    }
    else
    {
        // scan for number of terms
        sscanf(line.chars(), "%lf", &x);
        line = line.after(" ");
        line.strip();
        while (!end)
        {
            sscanf(line.chars(), "%lf", &y);
            if (line.contains(" ") == True)
            {
                line = line.after(" ");
                line.strip();
            }
            else
            {
                end = True;
            }
            DiscreteFunction *f = new DiscreteFunction;
            f->add(x, y);
            sprintf(str, "curve_%d", number + 1);
            f->name = str;
            curves << f;
            number++;
        }
    }

    while (line.scanFileLine(pfile) != False)
    {
        line.popReal(x);
        for (Indice i = 0; i < number; i++)
        {
            line.popReal(y);
            curves(i + firstOne)->add(x, y);
        }
    }

    fclose(pfile);
}

//!Writes the set of discrete functions on the disk
/*!
  \param filename name of the file to write
*/
//-----------------------------------------------------------------------------
void DiscreteFunctionSet::write(String filename)
//-----------------------------------------------------------------------------
{
    String nn;

    for (Indice j = 0; j < curves.size(); j++)
    {

        if (filename.contains(plotFileExtension))
        {
            nn = filename.before(plotFileExtension);
        }
        else
        {
            nn = filename;
        }
        nn += "_" + curves(j)->name + (plotFileExtension);

        curves(j)->toGnuplot(nn);
    }
}

//!Get a discrete function from the set by it's name
/*!
  \return The corresponding discrete function from the set
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunctionSet::get(String name)
//-----------------------------------------------------------------------------
{
    for (Indice i = 0; i < curves.size(); i++)
    {
        if (curves(i)->name == name)
        {
            return curves(i);
        }
    }

    return NULL;
}

//!Get the ith discrete function from the set
/*!
  \return The corresponding discrete function from the set
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunctionSet::get(Indice i)
//-----------------------------------------------------------------------------
{
    return curves(i);
}

//-----------------------------------------------------------------------------
void DiscreteFunctionSet::add(DiscreteFunction *df)
//-----------------------------------------------------------------------------
{
    curves << df;
}

//-----------------------------------------------------------------------------
void DiscreteFunctionSet::del(Indice i)
//-----------------------------------------------------------------------------
{
    curves.del(i, i);
}

//-----------------------------------------------------------------------------
void DiscreteFunctionSet::del(String name)
//-----------------------------------------------------------------------------
{
    for (Indice i = 0; i < curves.size(); i++)
    {
        if (curves(i)->name == name)
        {
            curves.del(i, i);
            return;
        }
    }
}

//!Number of discrete functions in the set
/*!
  \return Number of discrete functions in the set
*/
//-----------------------------------------------------------------------------
Indice DiscreteFunctionSet::size()
//-----------------------------------------------------------------------------
{
    return curves.size();
}
