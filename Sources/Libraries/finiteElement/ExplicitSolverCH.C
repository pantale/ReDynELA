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

// begin date :
// revision date :

/*
	Class ExplicitSolverCH implementation
*/

#include <ExplicitSolverCH.h>
//#include <Grid.h>
#include <Structure.h>
#include <Node.h>
#include <Element.h>
#include <Domain.h>
#include <Material.h>
#include <Interface.h>
#include <NodalField.h>
#include <IntegrationPoint.h>
#include <Boundary.h>
#include <NodeMotion.h>

extern Structure *Global_Structure;
extern String parsedFileName;

// cpu times record
CPUrecord recordTimes;
#define computeCpuTimes

/*!
  \class ExplicitSolverCH ExplicitSolverCH.C
  \brief Class 
  \ingroup basicTools
  \version 1.0.0
  \date 1997-2004
  \author Olivier PANTALE


*/

//!constructeur par défaut de la classe ExplicitSolverCH
/*!
  Cette méthode est le constructeur par défaut de la classe ExplicitSolverCH.
En pratique, ici, on ne fait rien de plus que les allocations de mémoire nécessaires. On définit ici des valeurs par défaut pour ce solveur explicite, par exemple \f$\rho_b=0.8182\f$ et \f$\gamma_s=0.9\f$.
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
ExplicitSolverCH::ExplicitSolverCH() : Solver()
//-----------------------------------------------------------------------------
{
  name = "ExplicitSolverCH";

  // valeur par défaut du rayon spectral
  rho_b = defaultSpectralRadius;
  //  timeStepSafetyFactor = defaultSafetyFactor;

  // calcul des param�tres initiaux du sch�ma de Chung-Hulbert
  computeIntegrationParameters();

  // par defaut, la fr�quence de time-step est continue
  //  computeTimeStepFrequency=defaultComputeTimeStepFrequency;

  //  computedTimeStep=0;
  //  timeStepComputationDate=0;
  //  lastTimeStepComputationDate=0;
  //  lastTimeStep=0;

  timeStepMethod = Pulse;
}

//!constructeur par recopie de la classe ExplicitSolverCH
/*!
  Cette méthode est le constructeur par recopie de la classe ExplicitSolverCH.
  \warning La recopie n'est pas prévue ici, cette méthode génère une erreur d'exécution
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
ExplicitSolverCH::ExplicitSolverCH(const ExplicitSolverCH &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely ExplicitSolverCH" << /*(int) */
      &X << endl;
  exit(-1);
}

//!destructeur de la classe ExplicitSolverCH
/*!
  Cette méthode est le destructeur de la classe ExplicitSolverCH.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
ExplicitSolverCH::~ExplicitSolverCH()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ExplicitSolverCH::solve(Real _upTime)
//-----------------------------------------------------------------------------
{
  Boolean runStep;

  // first we set the up-time
  upTime = _upTime;

  // verification que l'on doit résoudre
  runStep = betweenTimeBounds();
  if (!runStep)
    return;

    // calcul préliminaire des matrices internes
    // initialisation du step de calcul
#ifdef computeCpuTimes
  recordTimes.start("Internal_Matrices");
  Global_Structure->cpuTimes.timer("computeInternalMatrices")->start();
#endif
  domain->computeInternalMatrices();
  // initialisation du step de calcul
#ifdef computeCpuTimes
  Global_Structure->cpuTimes.timer("computeInternalMatrices")->stop();
  recordTimes.stop("Internal_Matrices");
#endif

  // calcul de la matrice de masse
#ifdef computeCpuTimes
  recordTimes.start("Mass_Matrices");
#endif
  domain->computeMassMatrix();
#ifdef computeCpuTimes
  recordTimes.stop("Mass_Matrices");
#endif

  // initial contact

  // calcul des forces internes
#ifdef computeCpuTimes
  recordTimes.start("Internal_Forces");
  Global_Structure->cpuTimes.timer("computeInternalForces")->start();
#endif
  domain->computeInternalForces();
#ifdef computeCpuTimes
  Global_Structure->cpuTimes.timer("computeInternalForces")->stop();
  recordTimes.stop("Internal_Forces");
#endif

  // calcul des forces d'inertie

  // calcul des forces d'inertie par une mise en rotation de la structure

  // calcul du pas de temps critique de la structure
#ifdef computeCpuTimes
  recordTimes.start("Time_Step");
  Global_Structure->cpuTimes.timer("computeTimeStep")->start();
#endif
  computeTimeStep(True);
#ifdef computeCpuTimes
  Global_Structure->cpuTimes.timer("computeTimeStep")->stop();
  recordTimes.stop("Time_Step");
#endif

  // boucle sur le temps
  while (runStep)
  {
    // initialisation du step de calcul
#ifdef computeCpuTimes
    recordTimes.start("Init_Step");
#endif
    initStep();
#ifdef computeCpuTimes
    recordTimes.stop("Init_Step");
#endif

    // affichage de l'incrément courant tous les 100 incréments
#ifndef PRINT_Execution_Solve
    if ((increment % frequencyReports == 0) || (increment == 1))
    {
#endif
      printf("%s inc=%ld time=%8.4E timeStep=%8.4E\n", domain->name.chars(), increment,
             domain->getCurrentTime(), timeStep);

      // write the progress file
      progressWrite();
#ifndef PRINT_Execution_Solve
    }
#endif

    // prediction des quantités
    // phase dite de prediction correspondant à l'algorithme explicite
#ifdef computeCpuTimes
    recordTimes.start("Predictions");
#endif
    computePredictions();
#ifdef computeCpuTimes
    recordTimes.stop("Predictions");
#endif

    // detection du contact

    // initialisation de l'iteration de calcul
#ifdef computeCpuTimes
    recordTimes.start("Init_Iteration");
#endif
    initIteration();
#ifdef computeCpuTimes
    recordTimes.stop("Init_Iteration");
#endif

    // r�solution implicite ou explicite de l'iteration
#ifdef computeCpuTimes
    recordTimes.start("Explicit_Solve");
#endif
    explicitSolve();
#ifdef computeCpuTimes
    recordTimes.stop("Explicit_Solve");
#endif

    // fin du step
#ifdef computeCpuTimes
    recordTimes.start("End_Step");
#endif
    endStep();
#ifdef computeCpuTimes
    recordTimes.stop("End_Step");
#endif

    // test de fin de calcul
    if (!betweenTimeBounds())
    {
      runStep = False;
    }

    // si on continue le calcul
    if (runStep)
    {
#ifdef computeCpuTimes
      recordTimes.start("Internal_Matrices");
      Global_Structure->cpuTimes.timer("computeInternalMatrices")->start();
#endif
      domain->computeInternalMatrices();
      // initialisation du step de calcul
#ifdef computeCpuTimes
      Global_Structure->cpuTimes.timer("computeInternalMatrices")->stop();
      recordTimes.stop("Internal_Matrices");
#endif

      // calcul du pas de temps critique de la structure
#ifdef computeCpuTimes
      recordTimes.start("Time_Step");
      Global_Structure->cpuTimes.timer("computeTimeStep")->start();
#endif
      computeTimeStep();
#ifdef computeCpuTimes
      Global_Structure->cpuTimes.timer("computeTimeStep")->stop();
      recordTimes.stop("Time_Step");
#endif
    }
  }

  // print the CPU times
  //  recordTimes.report("CPU-TIMES");
}

//!Renvoie le paramètre \f$\alpha_M\f$ de l'intégration de Chung-Hulbert
/*!
  Cette méthode renvoie la valeur du paramètre \f$\alpha_M\f$ pour le schéma d'intégration de Chung-Hulbert.
  \see computeIntegrationParameters()
  \return valeur numérique de \f$\alpha_M\f$
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Real ExplicitSolverCH::getAlphaM()
//-----------------------------------------------------------------------------
{
  return alphaM;
}

//!Renvoie le paramètre \f$\beta\f$ de l'intégration de Chung-Hulbert
/*!
  Cette méthode renvoie la valeur du paramètre \f$\beta\f$ pour le schéma d'intégration de Chung-Hulbert.
  \see computeIntegrationParameters()
  \return valeur numérique de \f$\beta\f$
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Real ExplicitSolverCH::getBeta()
//-----------------------------------------------------------------------------
{
  return beta;
}

//!Renvoie le paramètre \f$\gamma\f$ de l'intégration de Chung-Hulbert
/*!
  Cette méthode renvoie la valeur du paramètre \f$\gamma\f$ pour le schéma d'intégration de Chung-Hulbert.
  \see computeIntegrationParameters()
  \return valeur numérique de \f$\gamma\f$
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Real ExplicitSolverCH::getGamma()
//-----------------------------------------------------------------------------
{
  return gamma;
}

//!Calcul des paramètres de l'intégration de Chung-Hulbert
/*!
  Cette méthode calcule les valeurs des paramètres \f$\alpha_M\f$, \f$\beta\f$ et \f$\gamma\f$ pour le schéma d'intégration de Chung-Hulbert.
  Le paramètre \f$\alpha_M\f$ est défini à partir de la valeur de \f$\rho_b\f$ par la relation suivante: \f[\alpha_M=\frac{2\rho_b-1}{1+\rho_b}\f]
  Le paramètre \f$\beta\f$ est défini à partir de la valeur de \f$\rho_b\f$ par la relation suivante: \f[\beta=\frac{5-3\rho_b}{(1+\rho_b)^2(2-\rho_b)}\f]
  Le paramètre \f$\gamma\f$ est défini à partir de la valeur de \f$\alpha_M\f$ par la relation suivante: \f[\gamma=\frac{3}{2}-\alpha_M\f]
  Le paramètre \f$\Omega_s\f$ est défini à partir de la valeur de \f$\rho_b\f$ par la relation suivante: \f[\Omega_s=\sqrt{\frac{12(1+rho_b)^3(2-\rho_b)}{10+15\rho_b-\rho_b^2+\rho_b^3-\rho_b^4}}\f]
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void ExplicitSolverCH::computeIntegrationParameters()
//-----------------------------------------------------------------------------
{
  alphaM = (2.0 * rho_b - 1.0) / (1.0 + rho_b);
  beta = (5.0 - 3.0 * rho_b) / ((1.0 + rho_b) * (1.0 + rho_b) * (2.0 - rho_b));
  gamma = 1.5 - getAlphaM();
  omegaS = sqrt((12.0 * pow((1 + rho_b), 3) * (2.0 - rho_b)) / (10 + 15 * rho_b - rho_b * rho_b + pow(rho_b, 3) - pow(rho_b, 4)));
}

//!Permet de définir l'amortissement du schéma explicite
/*!
  Cette méthode permet de définir l'ammortissement numérique introduit dans le schéma d'intégration numérique explicite en modifiant la valeur du rayon spectral à la fréquence de bifurcation. La valeur numérique doit être comprise dans l'intervalle: \f$\rho_{b}\in[0.0:1.0]\f$. Cette méthode fait appel à la methode computeIntegrationParameters() afin de remettre à jour les paramètres en fonction du nouveau rayon spectral.
  \param val valeur du rayon spectral
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void ExplicitSolverCH::setDissipation(Real val)
//-----------------------------------------------------------------------------
{
  if (val < 0.0 || val > 1.0)
    fatalError("ExplicitSolverCH::setDissipation", "dissipation value out of range [0.0, 1.0]");

  // affect the value
  rho_b = val;

  // recompute parameters
  computeIntegrationParameters();

  // report operation in log file
  if (Global_Structure != NULL)
  {
    (*Global_Structure->logFile) << "ExplicitSolverCH : " << name << " dissipation factor set to " << rho_b << "\n";
    (*Global_Structure->logFile) << "  alphaM = " << alphaM << "\n";
    (*Global_Structure->logFile) << "  beta   = " << beta << "\n";
    (*Global_Structure->logFile) << "  gamma  = " << gamma << "\n";
    (*Global_Structure->logFile) << "  OmegaS = " << omegaS << "\n";
  }
}

//!Phase de pr�diction des d�placements, vitesses et acc�l�rations nodales
/*!
  La pr�diction se fait au niveau des d�placements, vitesses et acc�l�rations nodales � partir des relations suivantes:
  \f[\stackrel{\bullet\bullet}{x}_{n+1}=0\f]
  \f[\stackrel{\bullet}{x}_{n+1}=\stackrel{\bullet}{x}_{n}+\Delta t(1-\gamma)\stackrel{\bullet\bullet}{x}_{n}\f]
  \f[x_{n+1}=x_{n}+\Delta t\stackrel{\bullet}{x}_{n}+\Delta t^{2}\left(\frac{1}{2}-\beta\right)\stackrel{\bullet\bullet}{x}_{n}\f]
  On applique également le respect des conditions aux limites imposées au travers de la méthode BoundaryCondition::applyConstant_V0().
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void ExplicitSolverCH::computePredictions()
//-----------------------------------------------------------------------------
{
  Indice i;
  Node *pnd;

#ifdef PRINT_Execution_Solve
  cout << "Predictions de disp, mat_speed et dmat_speed\n";
#endif

  // boucle sur les noeuds du domaine
  for (i = 0; i < domain->nodes.size(); i++)
  {

    // recuperation du noeud courant
    pnd = domain->nodes(i);

    // verification d'assertion
#ifdef VERIF_assert
    assert(pnd != NULL);
#endif

    // prediction du déplacement
    //    pnd->New->disp = pnd->Current->disp + (timeStep*pnd->Current->mat_speed) + (timeStep*timeStep*(0.5-beta))*pnd->Current->dmat_speed;
    pnd->New->delta_disp = pnd->Current->dmat_speed;
    pnd->New->delta_disp *= timeStep * (0.5 - beta);
    pnd->New->delta_disp += pnd->Current->mat_speed;
    pnd->New->delta_disp *= timeStep;

    pnd->New->disp = pnd->Current->disp + pnd->New->delta_disp;

    // prediction de la vitesse
    //    pnd->New->mat_speed = pnd->Current->mat_speed + (timeStep*(1.0-gamma))*pnd->Current->dmat_speed;
    pnd->New->mat_speed = pnd->Current->dmat_speed;
    pnd->New->mat_speed *= timeStep * (1 - gamma);
    pnd->New->mat_speed += pnd->Current->mat_speed;

    // prediction de l'acceleration
    pnd->New->dmat_speed = 0.0;

    // application des conditions aux limites imposées
    if (pnd->boundary != NULL)
      pnd->boundary->applyConstant(pnd);
  }
}

//-----------------------------------------------------------------------------
void ExplicitSolverCH::initStep()
//-----------------------------------------------------------------------------
{
  // verification de la projection du temps par rapport au temps final
  // pour na pas dépasse la bonne date
  if (domain->currentTime + timeStep > upTime)
    timeStep = upTime - domain->currentTime;

  // mise a jour du temps
  domain->nextTime = domain->currentTime + timeStep;

  // incrementation du nombre d'incréments
  increment++;
}

//-----------------------------------------------------------------------------
void ExplicitSolverCH::endStep()
//-----------------------------------------------------------------------------
{
  // calcul des reactions

  // update du time
  updateTime();

  // history file
  fprintf(domain->history_file, "%8.4E %8.4E %8.4E %8.4E\n", domain->getCurrentTime(),
          timeStep, computedTimeStep, domain->getTotalKineticEnergy());
  fflush(domain->history_file);

  // sauvegarde des history file
  domain->writeHistoryFiles();

  // swap des valeurs nodales
  domain->transfertQuantities();
}

//-----------------------------------------------------------------------------
void ExplicitSolverCH::initIteration()
//-----------------------------------------------------------------------------
{
  // calcul des forces internes
  Global_Structure->cpuTimes.timer("computeInternalForces")->start();

  domain->computeInternalForces();
  Global_Structure->cpuTimes.timer("computeInternalForces")->stop();

  // calcul des forces externes
}

//!R�solution explicite de l'incr�ment
/*!
  Cette m�thode effectue la r�solution explicite de l'incr�ment de temps. La m�thode explicite �tant une m�thode directe, aucune it�ration n'est necessaire ici, les quantit�s peuvent �tre calcul�es directement en utilisante les relations suivantes:
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void ExplicitSolverCH::explicitSolve()
//-----------------------------------------------------------------------------
{
  Indice i, j;
  Node *pnd;

#ifdef PRINT_Execution_Solve
  cout << "Resolution explicite du pas de temps\n";
#endif

  // phase de calcul du terme d'accélération global
  //  $GLOBAL$ nd->New->dmat_speed = $M^-1$ * (Fext - Fint)
  Vector acc;
  acc = domain->Mass.Solve(domain->Fint);

  // update du champ des accelerations
  for (i = 0; i < domain->nodes.size(); i++)
  {
    pnd = domain->nodes(i);

    // mise a jour des accelerations
    for (j = 0; j < domain->NoDim(); j++)
      pnd->New->dmat_speed(j) = acc(i * domain->NoDim() + j);
  }

  // phase de résolution
  for (i = 0; i < domain->nodes.size(); i++)
  {

    // recuperation du noeud courant
    pnd = domain->nodes(i);

    // mise à jour de l'accélération matérielle
    pnd->New->dmat_speed -= alphaM * pnd->Current->dmat_speed;
    pnd->New->dmat_speed /= (1.0 - alphaM);

    // mise à jour de la vitesse matérielle
    pnd->New->mat_speed += (timeStep * gamma) * pnd->New->dmat_speed;

    // mise à jour du déplacement
    pnd->New->delta_disp += (SQ(timeStep) * beta) * pnd->New->dmat_speed;

    // application des conditions aux limites imposées
    if (pnd->boundary != NULL)
      pnd->boundary->applyConstant(pnd);

    // prise en compte du contact

    // prise en compte des conditions aux limites (2)

    pnd->New->disp = pnd->Current->disp + pnd->New->delta_disp;
  }

  // mise a jour des coordonnées
  updateGrid();
}

//-----------------------------------------------------------------------------
void ExplicitSolverCH::updateTime()
//-----------------------------------------------------------------------------
{
  domain->currentTime += timeStep;
}

//-----------------------------------------------------------------------------
void ExplicitSolverCH::updateGrid()
//-----------------------------------------------------------------------------
{
  Indice no;

#ifdef PRINT_Execution_Solve
  cout << "Grid motion computation\n";
#endif

  // mise à jour de la position des noeuds
  for (no = 0; no < domain->nodes.size(); no++)
    domain->nodes(no)->motion->updateNode(timeStep);
}
