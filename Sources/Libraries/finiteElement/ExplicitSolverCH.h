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
	Class ExplicitSolverCH definition
*/

#ifndef __ExplicitSolverCH_h__
#define __ExplicitSolverCH_h__

#include <upFemSolver.h>

#include <Solver.h>
class Domain;

/*!
  \file ExplicitSolverCH.h
  \brief fichier .h de d�finition des solveurs �l�ments finis
  \ingroup femExplicitSolverCH

  Ce fichier sert � la d�finition de la classe ExplicitSolverCH.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/

/*!
  \class ExplicitSolverCH ExplicitSolverCH.h
  \brief Classe de d�finition et de manipulation des solveurs �l�ments finis.
  \ingroup femExplicitSolverCH

  Cette classe sert � d�finir et � manipuler les solveurs disponibles au niveau d'un domaine. Elle contient toutes les d�finitions de base concernant la gestion des temps de simulation (incr�ment de temps, temps de d�but, de fin ...) ainsi que la gestion compl�te des incr�ments.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
/** @dia:pos 95.6853,-30.4294 */
/** @dia:route Solver;v,90.3853,-31.8794,-27.4706,103.635,-30.4294 */
class ExplicitSolverCH : public Solver
{

private:
  Real alphaM;
  Real alphaF;
  Real beta;
  Real gamma;
  Real rho_b;

public:
  // constructor
  ExplicitSolverCH();
  ExplicitSolverCH(const ExplicitSolverCH &X);
  ~ExplicitSolverCH();

public:
  Real getAlphaM();
  Real getBeta();
  Real getGamma();
  void computeIntegrationParameters();
  void setDissipation(Real val);
  void solve(Real upTime);
  const char *Name() { return "ExplicitSolverCH"; }

private:
  void computePredictions();
  void initStep();
  void endStep();
  void initIteration();
  void explicitSolve();
  void updateGrid();
  void updateTime();
};

#endif
