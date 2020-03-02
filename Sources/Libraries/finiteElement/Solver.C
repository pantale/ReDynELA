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
	Class Solver implementation
*/

#include <Solver.h>
#include <Domain.h>
#include <Structure.h>
//#include <Grid.h>
extern Structure *Global_Structure;

/*!
  \class Solver Solver.C
  \brief Class 
  \ingroup basicTools
  \version 1.0.0
  \date 1997-2004
  \author Olivier PANTALE


*/
//!default constructor of the class Solver
/*!

*/
//-----------------------------------------------------------------------------
Solver::Solver()
//-----------------------------------------------------------------------------
{
  timeStep = 0;
  startTime = 0;
  endTime = 0;
  increment = 0;
  startIncrement = 0;
  endIncrement = 0;
  upTime = 0;

  name = "Generic solver";

  frequencyReports = 10;

  omegaS = 2.0;

  timeStepSafetyFactor = defaultSafetyFactor;

  // par defaut, la fr�quence de time-step est continue
  computeTimeStepFrequency = defaultComputeTimeStepFrequency;

  lastInc = 0;
  computedTimeStep = 0;
  timeStepComputationDate = 0;
  lastTimeStepComputationDate = 0;
  lastTimeStep = 0;

  // options de r�solution
  RFI = True;
  underIntegratePressure = True;
}

//!copy constructor of the class Solver
/*!

*/
//-----------------------------------------------------------------------------
Solver::Solver(const Solver &X)
//-----------------------------------------------------------------------------
{
}

//!destructor of the class Solver
/*!

*/
//-----------------------------------------------------------------------------
Solver::~Solver()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Solver::initTime()
//-----------------------------------------------------------------------------
{

  // init des starts
  //  currentTime = startTime;
  increment = startIncrement;
}

//-----------------------------------------------------------------------------
Boolean Solver::timeOk()
//-----------------------------------------------------------------------------
{
  // test sur le temps
  if (domain->getCurrentTime() >= endTime)
    return (False);

  // test sur les increments
  if (endIncrement == 0)
    return (True);

  // deuxieme test sur les increments
  if (increment < endIncrement)
    return (True);

  // par defaut
  return (False);
}

//-----------------------------------------------------------------------------
void Solver::setTimes(Real startT, Real stopT)
//-----------------------------------------------------------------------------
{
  startTime = startT;
  endTime = stopT;
}

//-----------------------------------------------------------------------------
void Solver::setIncrements(Indice startI, Indice stopI)
//-----------------------------------------------------------------------------
{
  startIncrement = startI;
  endIncrement = stopI;
}

//-----------------------------------------------------------------------------
Boolean Solver::betweenBounds(Real t)
//-----------------------------------------------------------------------------
{
  if ((startTime <= t) && (t < endTime))
    return True;
  return False;
}

//-----------------------------------------------------------------------------
void Solver::setDomain(Domain *dom)
//-----------------------------------------------------------------------------
{
  domain = dom;
}

//! teste que le temps actuel est inf�rieur � une valeur limite
/*!
Cette m�thode teste que le temps actuel est tel que:
- le temps actuel est inf�rieur au temps maximum autoris�
- l'incr�ment actuel est inf�rieur � l'incr�ment maximal autoris� (si cet incr�ment maximal est diff�rent de z�ro)
\return True si on est entre les bornes
*/
//-----------------------------------------------------------------------------
Boolean
Solver::betweenTimeBounds()
//-----------------------------------------------------------------------------
{
  if ((domain->getCurrentTime() >= startTime) && (domain->getCurrentTime() < Min(endTime, upTime)))
    return True;

  return False;
}

//-----------------------------------------------------------------------------
void Solver::progressWrite()
//-----------------------------------------------------------------------------
{
  String st;
  Real interval;

  interval = theclock.getInterval();

  if (interval < 10)
    frequencyReports *= 10;

  st = "." + parsedFileName.before(sourceFileExtension) + progressFileExtension;
  FILE *fprogress = fopen(st.chars(), "w");
  fprintf(fprogress, "%ld %10.7E %10.7E %10.7E %10.7E %ld\n",
          increment,
          domain->getCurrentTime(),
          timeStep,
          endTime,
          interval,
          increment - lastInc);
  lastInc = increment;

  fclose(fprogress);
}

//!Calcul du time step de minimal du domaine
/*!
  Cette m�thode calcule le time step minimal du domaine en fonction de la grille courante. Cette m�thode fait appel � la m�thode Domain::getMaximumFrequency() pour l'�valuation num�rique de la valeur du time step minimal. La relation utilis�e pour ce calcul est donc donn�e par l'une des �quations ci-dessous selon la valeur d�finie par la m�thode setTimeStepMethod():
Si m�thode pulsation maxi
  \f[ \Delta t = \gamma_s \frac{\Omega_s}{f_{max}} \f]
Si crit�re de Courant:
  \f[ \Delta t = factor * \Delta t_{crit} \f]

  La valeur du timeStep n'est recalcul�e que si l'une des conditions ci-dessous est vraie:
  Le param�tre force est True
  Le num�ro d'incr�ment est un multiple entier de la valeur de computeTimeStepFrequency
  La valeur de l'ancien incr�ment de temps � �t� modifi�e par une routine externe
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Solver::computeTimeStep(Boolean force)
//-----------------------------------------------------------------------------
{
  // teste si le numero d'incr�ment correspond � la fr�quence de recalcul
  if (force || (increment % computeTimeStepFrequency == 0))
  {
    Real fmax = 0;
    switch (timeStepMethod)
    {
    case Courant:
    {
      fmax = 2.0 / domain->computeCourantTimeStep();
    }
    break;
    case Pulse:
    {
      fmax = domain->getMaximumFrequency();
    }
    break;
    default:
      fatalError("Solver::computeTimeStep", "Unknown method\n");
    }

    timeStep = timeStepSafetyFactor * omegaS / fmax;

    // remember old values
    lastTimeStepComputationDate = timeStepComputationDate;
    lastTimeStep = computedTimeStep;

    // on peut corriger �ventuellement en fonction d'un timeStep maximum

    // on peut �galement r�duire le time-step en fonction d'un archivage ult�rieur

    // set actual date
    timeStepComputationDate = domain->currentTime;
    computedTimeStep = timeStep;
  }

  // interpolation lineaire du timeStep si possible
  else
  {
    if (timeStepComputationDate != lastTimeStepComputationDate)
      timeStep = LINEARdbl(lastTimeStep,
                           computedTimeStep,
                           (domain->currentTime - lastTimeStepComputationDate) /
                               (timeStepComputationDate - lastTimeStepComputationDate));
  }
}

//!Permet de d�finir la valeur du facteur de s�curit� appliqu� dans l'integration explicite
/*!
  Cette m�thode permet de d�finir la valeur du coefficient de s�curit� pour le calcul du pas de temps critique dans l'int�gration num�rique explicite.
  \param val valeur du facteur de s�curit�
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Solver::setTimeStepSafetyFactor(Real val)
//-----------------------------------------------------------------------------
{
  if (timeStepSafetyFactor <= 0)
    fatalError("Solver::setTimeStepSafetyFactor", "negative or null safety factor value");

  // affect the value
  timeStepSafetyFactor = val;

  if (Global_Structure != NULL)
  {
    (*Global_Structure->logFile) << "Solver : " << name << " safety factor set to " << timeStepSafetyFactor << "\n";
  }
}

//!Permet de d�finit la fr�quence (en nombre d'it�rations) avec laquelle on refait le calcul du pas de temps critique
/*!
  Cette m�thode permet de d�finir la fr�quence (en nombre d'it�rations) avec laquelle on refait le calcul du pas de temps critique. Ce calcul du pas de temps critique �volue faiblement d'un incr�ment � l'autre, et on peut donc au travers de ce param�tre d�finir la fr�quence � laquelle ce calcul est effectu�.
  \param val de la fr�quence de calcul du pas de temps critique
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Solver::setComputeTimeStepFrequency(Indice val)
//-----------------------------------------------------------------------------
{
  computeTimeStepFrequency = val;

  if (Global_Structure != NULL)
  {
    (*Global_Structure->logFile) << "Solver : " << name << " timestep frequency set to " << computeTimeStepFrequency << "\n";
  }
}

//-----------------------------------------------------------------------------
void Solver::setTimeStepMethod(String meth)
//-----------------------------------------------------------------------------
{
  timeStepMethod = Unknown;
  if (meth == "Courant")
    timeStepMethod = Courant;
  if (meth == "Pulse")
    timeStepMethod = Pulse;

  if (timeStepMethod == Unknown)
    fatalError("setTimeStepMethod", "Unknown method %s \n", meth.chars());
  if (Global_Structure != NULL)
  {
    (*Global_Structure->logFile) << "Solver : " << name << " timestep method set to " << meth << "\n";
  }
}

//-----------------------------------------------------------------------------
void Solver::setRFI(Boolean val)
//-----------------------------------------------------------------------------
{
#ifndef G2TR_STUDY
  warning("Solver::setRFI n'est utilisable que dans le cadre d'une version pour laquelle la variable G2TR_STUDY est active dans le fichier Const.h");
#endif

  RFI = val;
}

//-----------------------------------------------------------------------------
void Solver::setUnderIntegratePressure(Boolean val)
//-----------------------------------------------------------------------------
{
#ifndef G2TR_STUDY
  warning("Solver::setUnderIntegratePressure n'est utilisable que dans le cadre d'une version pour laquelle la variable G2TR_STUDY est active dans le fichier Const.h");
#endif

  underIntegratePressure = val;
}
