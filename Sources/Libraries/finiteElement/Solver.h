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
// revision date :

/*
	Class Solver definition
*/

#ifndef __Solver_h__
#define __Solver_h__

#include <upFemSolver.h>
class Domain;
extern String parsedFileName;

/*!
  \file Solver.h
  \brief fichier .h de d�finition des solveurs �l�ments finis
  \ingroup femSolver

  Ce fichier sert � la d�finition de la classe Solver.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/

/*!
  \class Solver Solver.h
  \brief Classe de d�finition et de manipulation des solveurs �l�ments finis.
  \ingroup femSolver

  Cette classe sert � d�finir et � manipuler les solveurs disponibles au niveau d'un domaine. Elle contient toutes les d�finitions de base concernant la gestion des temps de simulation (incr�ment de temps, temps de d�but, de fin ...) ainsi que la gestion compl�te des incr�ments.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
/** @dia:pos 238.8,5.8 */
class Solver
{

public:
  enum
  {
    Unknown,
    Courant,
    Pulse
  };

private:
  /** @dia:route 0,2;h,-85.6,-130.75,43,-66.4794,99.0353 */
  CPUclock theclock;

protected:
  Indice frequencyReports;
  Indice lastInc;
  Real timeStepSafetyFactor;
  Indice computeTimeStepFrequency;
  Real lastTimeStepComputationDate;
  Real timeStepComputationDate;
  Real lastTimeStep;
  Real computedTimeStep;
  Real omegaS;
  short timeStepMethod;

public:
  Real timeStep;
  Real startTime;
  Real endTime;
  //  Real timeStepFactor;
  Indice increment;
  Indice startIncrement;
  Indice endIncrement;
  Real upTime;
  /** @dia:route 9,10;h,217.45,8.7,226.75,8.5,238.8 */
  Domain *domain;
  Boolean RFI;
  Boolean underIntegratePressure;

public:
  /** @dia:route 0,2;h,81.7353,-66.4794,-77.6853,-172.85,-95.25 */
  String name;

public:
  // constructor
  Solver();
  Solver(const Solver &X);
  virtual ~Solver();

  // member functions
  void initTime();
  Boolean timeOk();
  void setTimes(Real startT, Real stopT);
  void setIncrements(Indice startI, Indice stopI);
  Real getStartTime() { return startTime; }
  Real getEndTime() { return endTime; }
  Real getTimeStep() { return timeStep; }
  Boolean betweenBounds(Real t);
  Boolean betweenTimeBounds();
  void setDomain(Domain *dom);
  void progressWrite();
  void computeTimeStep(Boolean force = False);
  void setComputeTimeStepFrequency(Indice val);
  void setTimeStepSafetyFactor(Real val);
  void setTimeStepMethod(String);
  void setRFI(Boolean);
  void setUnderIntegratePressure(Boolean);

  // virtual methods
  virtual void solve(Real upTime) = 0;
  virtual const char *Name() = 0;
};

#endif
