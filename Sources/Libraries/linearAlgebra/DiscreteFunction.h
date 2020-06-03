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

/*!
  \file DiscreteFunction.h
  \brief Declaration file for the discrete function class

  This file is the declaration file for the discrete function class. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __DiscreteFunction_h__
#define __DiscreteFunction_h__
#include <upLinearAlgebra.h>
#include <Function.h>

/*!
  \class coupleReal
  \brief This is a private class used in the DiscreteFunction class to define a couple of points. This one is not directly refered by the user.
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/
class coupleReal
{
    friend class List<coupleReal *>;
    friend class DiscreteFunction;
    friend class CrackFunction;
    friend Boolean compare(coupleReal *t1, coupleReal *t2);
    Real x, y;

    coupleReal();
    coupleReal(Real x, Real y);
    coupleReal(const coupleReal &X);
    ~coupleReal();
};

// fonction de comparison pour le tri des points
Boolean compare(coupleReal *t1, coupleReal *t2);

/*!
  \class DiscreteFunction
  \brief Definition of a discrete function in the DynELA FEM code

  This class defines a discrete function for using in the DynELA FEM code. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  These functions are for example used in the DynELA computer code to create boundaries conditions to varying with time limits.
  Combining times to the variable x, we can define a function to vary the boundary condition over time.
  One can also use this function to create nonlinear behavior laws of discretely defined materials with the following form: \f[ \sigma=f(\varepsilon) \f]
  Of course it's also used in the application \b DynELA \b curves for managing digital results as curves.
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/
class DiscreteFunction : public Function
{
public:
    enum
    {
        noInit,
        averageInit,
        fisrtValueInit
    };

protected:
    List<coupleReal *> lpoints;
    Boolean Sort;

public:
    char style;  //!< First style variable for the discrete function. This is useful for storing information about this feature
    char style2; //!< Second style variable for the discrete function. This is useful for storing information about this feature

private:
    void sort();

public:
    DiscreteFunction();
    DiscreteFunction(const DiscreteFunction &X);
    ~DiscreteFunction();
    void add(Real x, Real y);
    void startPointAdd(Real x, Real y);
    Indice size();
    Boolean intoBoundsX(Real X);
    Real getValue(Real x);
    Real getInverseValue(Real y);
    Real getSlope(Real x);
    void print();
    Real maxX();
    Real minX();
    Real maxY();
    Real minY();
    Real average();
    void autoSort(Boolean sr)
    {
        Sort = sr;
    } //!< Auto sort x coordinates (default value is \b True)
    Indice getNumberOfPoints()
    {
        return lpoints.size();
    } //!< Returns the number of points defining the discrete function
    void flush()
    {
        lpoints.flush();
    } //!< Delete all the points in the current dicrete function
    Real getXPoint(Indice i)
    {
        return lpoints(i)->x;
    } //!< Return the X coordinate of the ith point
    Real getYPoint(Indice i)
    {
        return lpoints(i)->y;
    } //!< Return the Y coordinate of the ith point
    void toGnuplot(String name);
    void fromFile(String name, int xCol = 1, int yCol = 2);
    String convertToDynELASourceFile();
    DiscreteFunction *derivate();
    DiscreteFunction *EulerBackwardDerivate();
    DiscreteFunction *EulerForwardDerivate();
    DiscreteFunction *CentralDifferenceDerivate();
    DiscreteFunction *ZeroForcing1Derivate();
    DiscreteFunction *ZeroForcing2Derivate();
    DiscreteFunction *integrate();
    DiscreteFunction *movingAverage(int before, int after);
    DiscreteFunction *xyMerge(DiscreteFunction *fu, Indice points);
    DiscreteFunction *addCurves(DiscreteFunction *fu, Indice points);
    DiscreteFunction *errorCurves(DiscreteFunction *fu, Indice points);
    DiscreteFunction *distanceCurvesX(DiscreteFunction *fu);
    DiscreteFunction *distanceCurvesY(DiscreteFunction *fu);
    DiscreteFunction *distanceCurves(DiscreteFunction *fu);
    DiscreteFunction *absoluteValueCurve();
    DiscreteFunction *scaleCurve(Real factor);
    DiscreteFunction *offsetCurve(Real factor);
    DiscreteFunction *substractCurves(DiscreteFunction *fu, Indice points);
    DiscreteFunction *multiplyCurves(DiscreteFunction *fu, Indice points);
    DiscreteFunction *divideCurves(DiscreteFunction *fu, Indice points);
    DiscreteFunction *lowPass_1(Real A, Indice points, int Init);
    DiscreteFunction *lowPass_2(Real A, Real B, Indice points, int Init);
    DiscreteFunction *remapCurve(Real xm, Real xM, Real ym, Real yM);
};

/*!
  \class CrackFunction
  \brief This special type of discrete function is used to define cracks in an X-FEM formulation.
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/
class CrackFunction : public DiscreteFunction
{

public:
    CrackFunction();
    CrackFunction(const CrackFunction &X);
    ~CrackFunction();

    void headTipAdd(Real x, Real y);
    void tailTipAdd(Real x, Real y);
    Boolean isOnPositiveSide(Real x, Real y);
    Boolean distanceToSegmentShort(Real &dist, Indice i, Real x, Real y);
};

/*!
  \class DiscreteFunctionSet
  \brief This is a set of discrete functions.
  
  This class is used to manage sets of discrete functions and group them. This is a usefull class for the \b DynELA \b curves application.
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/
class DiscreteFunctionSet
{
    List<DiscreteFunction *> curves;

public:
    DiscreteFunctionSet();
    DiscreteFunctionSet(const DiscreteFunctionSet &X);
    ~DiscreteFunctionSet();

    void read(String);
    void write(String);
    DiscreteFunction *get(String);
    DiscreteFunction *get(Indice);
    void add(DiscreteFunction *);
    void del(Indice);
    void del(String);
    Indice size();
};

#endif
