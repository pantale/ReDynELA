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
	Class ExplicitSolver implementation
*/

#include <ExplicitSolver.h>
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

/*!
  \class ExplicitSolver ExplicitSolver.C
  \brief Class 
  \ingroup basicTools
  \version 1.0.0
  \date 1997-2004
  \author Olivier PANTALE


*/

//!constructor of the class ExplicitSolver
/*!

*/
//-----------------------------------------------------------------------------
ExplicitSolver::ExplicitSolver() : Solver()
//-----------------------------------------------------------------------------
{
      name = "ExplicitSolver";

      frequencyReports = 10;

      //  lastInc=0;

      //  timeStepSafetyFactor =.95;

      timeStepMethod = Courant;
}

//!copy constructor of the class ExplicitSolver
/*!

*/
//-----------------------------------------------------------------------------
ExplicitSolver::ExplicitSolver(const ExplicitSolver &X)
//-----------------------------------------------------------------------------
{
}

//!destructor of the class ExplicitSolver
/*!

*/
//-----------------------------------------------------------------------------
ExplicitSolver::~ExplicitSolver()
//-----------------------------------------------------------------------------
{
}

#define computeCpuTimes
//#define PRINT_Execution_Solve

//-----------------------------------------------------------------------------
void ExplicitSolver::solve(Real _upTime)
//-----------------------------------------------------------------------------
{
      // cpu times record
      static CPUrecord recordTimes;

      // first we set the up-time
      upTime = _upTime;

      // premier calcul de determinant
      domain->computeInternalMatrices();

      while (betweenTimeBounds())
      {
            // incrementation du nombre d'incr�ments
            increment++;

            // calcul du time step minimal de la structure
#ifdef computeTimes
            recordTimes.start("Time step");
#endif
            computeTimeStep();
#ifdef computeTimes
            recordTimes.stop("Time step");
#endif

            // affichage de l'incr�ment courant tous les 100 incr�ments
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

            // phase dite de prediction correspondant � l'algorithme explicite
#ifdef computeTimes
            recordTimes.start("Predictions");
#endif
            computePredictions();
#ifdef computeTimes
            recordTimes.stop("Predictions");
#endif

            // mise a jour des coordonn�es de la grille
#ifdef computeTimes
            recordTimes.start("Grid motion");
#endif
            updateGrid();
#ifdef computeTimes
            recordTimes.stop("Grid motion");
#endif

            // calcul des deformations
#ifdef computeTimes
            recordTimes.start("Strains compute");
#endif
            computeStrains();
#ifdef computeTimes
            recordTimes.stop("Strains compute");
#endif

            // calcul du domaine parent et de det J
#ifdef computeTimes
            recordTimes.start("DetJ computation");
#endif
            domain->computeInternalMatrices();
#ifdef computeTimes
            recordTimes.stop("DetJ computation");
#endif

            // integration des lois constitutives
#ifdef computeTimes
            recordTimes.start("Compute constitutive");
#endif
            computeConstitutive();
#ifdef computeTimes
            recordTimes.stop("Compute constitutive");
#endif

            // integration de la loi d'etat
#ifdef computeTimes
            recordTimes.start("State law");
#endif
            computeState();
#ifdef computeTimes
            recordTimes.stop("State law");
#endif

            // calcul des contraintes
#ifdef computeTimes
            recordTimes.start("Stress computation");
#endif
            computeStress();
#ifdef computeTimes
            recordTimes.stop("Stress computation");
#endif

            // integration de la masse
#ifdef computeTimes
            recordTimes.start("Mass integration");
#endif
            computeMass();
#ifdef computeTimes
            recordTimes.stop("Mass integration");
#endif

            // integration de la quantite de mouvement
#ifdef computeTimes
            recordTimes.start("Momentum integration");
#endif
            computeMomentum();
#ifdef computeTimes
            recordTimes.stop("Momentum integration");
#endif

            // integration de l'energie et calcul des masses nodales pour le contact
#ifdef computeTimes
            recordTimes.start("Energy integration");
#endif
            computeEnergy();
#ifdef computeTimes
            recordTimes.stop("Energy integration");
#endif

            // phase de correction
#ifdef computeTimes
            recordTimes.start("Corrections");
#endif
            computeCorrections();
#ifdef computeTimes
            recordTimes.stop("Corrections");
#endif

            // calcul des temperatures
#ifdef computeTimes
            recordTimes.start("Temperatures");
#endif
            computeTemperatures();
#ifdef computeTimes
            recordTimes.stop("Temperatures");
#endif

            // scan des interfaces
#ifdef computeTimes
            recordTimes.start("Interfaces");
#endif
            scanInterfaces();
#ifdef computeTimes
            recordTimes.stop("Interfaces");
#endif

            // calcul des efforts de contact
#ifdef computeTimes
            recordTimes.start("Contact forces");
#endif
            computeContactForces();
#ifdef computeTimes
            recordTimes.stop("Contact forces");
#endif

            // update du time
            updateTime();

            // history file
            fprintf(domain->history_file, "%8.4E %8.4E %8.4E %8.4E\n", domain->getCurrentTime(),
                    timeStep, domain->getTotalMass(), domain->getTotalKineticEnergy());
            fflush(domain->history_file);

#ifdef computeTimes
            recordTimes.start("History files");
#endif
            domain->writeHistoryFiles();
#ifdef computeTimes
            recordTimes.stop("History files");
#endif

            // transfert des valeurs d'un incr�ment a l'autre
#ifdef computeTimes
            recordTimes.start("Transfert increment");
#endif
            domain->transfertQuantities();
#ifdef computeTimes
            recordTimes.stop("Transfert increment");
#endif
      }

      // print the CPU times
      recordTimes.report("CPU-TIMES");
}

//-----------------------------------------------------------------------------
void ExplicitSolver::computeForces()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ExplicitSolver::computePredictions()
//-----------------------------------------------------------------------------
{
      Indice i;
      Node *pnd;

#ifdef PRINT_Execution_Solve
      cout << "Predictions de ro, e, mat_speed et u\n";
#endif

      Real alpha = 0.;
      Real dzeta = 0.;
      Real gamma = 0.;
      Real beta = 0.;

      // phase de prediction
      for (i = 0; i < domain->nodes.size(); i++)
      {

            // recuperation du noeud courant
            pnd = domain->nodes(i);
#ifdef VERIF_assert
            assert(pnd != NULL);
#endif

            // prediction de la densite
            pnd->New->ro = pnd->Current->ro + ((1.0 - alpha) * timeStep) * pnd->Current->dro;

            // prediction de l'energie
            pnd->New->e = pnd->Current->e + ((1.0 - dzeta) * timeStep) * pnd->Current->de;

            // application des conditions aux limites impos�es
            if (pnd->boundary != NULL)
                  pnd->boundary->applyConstant_V0(pnd);

            //if (times.time==0.) cout << pnd->Current->mat_speed << endl;
            // prediction de la vitesse materielle
            pnd->New->mat_speed = pnd->Current->mat_speed + ((1.0 - gamma) * timeStep) * pnd->Current->dmat_speed;

            //    pnd->boundary->applyConstant_V0(pnd);

            // prediction du deplacement
            //    pnd->New->delta_disp=pnd->Current->delta_disp+times.timeStep*pnd->Current->mat_speed+(0.5-beta)*SQ(times.timeStep)*pnd->Current->dmat_speed;

            //    pnd->New->delta_disp=times.timeStep*pnd->Current->mat_speed+(0.5-beta)*SQ(times.timeStep)*pnd->Current->dmat_speed;

            //    pnd->New->delta_disp=pnd->Current->disp-pnd->New->disp;

            pnd->New->delta_disp = 0.;
            pnd->New->delta_disp -= pnd->Current->disp;
            pnd->New->disp = timeStep * pnd->Current->mat_speed + (1. - beta) * SQ(timeStep) * pnd->Current->dmat_speed + pnd->Current->disp;
            pnd->Current->disp = pnd->New->disp;
            pnd->New->delta_disp += pnd->New->disp;
            //    pnd->New->delta_disp=pnd->New->disp-pnd->Current->disp;

            //      +(0.5*(1.0-2.*beta)*SQ(times.timeStep))*pnd->Current->dmat_speed;

            //    pnd->boundary->applyConstant_V0(pnd);

            // calcul du deplacement pour l'increment
            //    pnd->disp+=pnd->New->delta_disp;
      }
}

//-----------------------------------------------------------------------------
void ExplicitSolver::updateGrid()
//-----------------------------------------------------------------------------
{
      Indice no;

#ifdef PRINT_Execution_Solve
      cout << "Grid motion computation\n";
#endif

      // mise � jour de la position des noeuds
      for (no = 0; no < domain->nodes.size(); no++)
            domain->nodes(no)->motion->updateNode(timeStep);
}

//-----------------------------------------------------------------------------
void ExplicitSolver::computeStrains()
//-----------------------------------------------------------------------------
{
      Indice el;

#ifdef PRINT_Execution_Solve
      cout << "Strains computation\n";
#endif

      for (el = 0; el < domain->elements.size(); el++)
      {
            domain->elements(el)->computeStrains(timeStep);
      }
}

//-----------------------------------------------------------------------------
void ExplicitSolver::computeConstitutive()
//-----------------------------------------------------------------------------
{
      Element *pel;
      Indice el;

#ifdef PRINT_Execution_Solve
      cout << "Constitutive equation\n";
#endif

      for (el = 0; el < domain->elements.size(); el++)
      {

            // chargement de l'element en cours
            pel = domain->elements(el);

            // integration de la loi constitutive sur l'element courant
            pel->computeConstitutiveEquation();
      }
}
//-----------------------------------------------------------------------------
void ExplicitSolver::computeState()
//-----------------------------------------------------------------------------
{
      Indice el;

#ifdef PRINT_Execution_Solve
      cout << "State equation\n";
#endif

      for (el = 0; el < domain->elements.size(); el++)
      {

            // integration de la loi d'�tat sur l'element courant
            domain->elements(el)->computeStateEquation();
      }
}

//!Phase de calcul des contraintes
/*!
Cette m�thode effectue la phase de calcul des contraintes sur la structure. Deux cas se pr�sentent selon la nature du mat�riau, �lastique ou �lastoplastique. Dans le cas �lastoplastique, on utilise un algorithme de retour radial.
La m�thode de la rotation finale instantan�e est utilis�e pour traiter les probl�mes d'objectivit� de la loi de comportement.
*/
//-----------------------------------------------------------------------------
void ExplicitSolver::computeStress()
//-----------------------------------------------------------------------------
{
      Element *pel;
      Material *pmat;
      Indice el, pt;
      IntegrationPoint *fpt;

      Tensor2 devEpsPlas;
      Tensor2 Id;
      Tensor2 en1;
      Real f_trial_1;
      Tensor2 n_1;
      Tensor2 ksi_1;
      Real Gamma;

      Id.Identity();

#ifdef PRINT_Execution_Solve
      cout << "Stresses computation\n";
#endif

      // boucle sur les elements de la structure
      for (el = 0; el < domain->elements.size(); el++)
      {

            // pointeur sur l'element courant
            pel = domain->elements(el);
#ifdef VERIF_assert
            assert(pel != NULL);
#endif

            // recuperation du materiau de l'element
            pmat = pel->material;
#ifdef VERIF_assert
            assert(pmat != NULL);
#endif

            // Normalement ce qui est ici ne sert � rien !!!
            /*    for (pt=0;pt<pel->getNumberOfIntegrationPoints();pt++) {
      fpt=pel->intPoint(pt);

      fpt->Sig=fpt->S+fpt->p*Id;
      fpt->S=fpt->Sig.deviator();
      fpt->p=fpt->Sig.moyTrace();
    }*/

            // test de la loi d'�coulement �lastique ou �lastoplastique
            if (!pmat->isYield())
            {
            }
            else
            {
                  // boucle sur les points d'integration
                  Tensor2 fi_e;

                  for (pt = 0; pt < pel->getNumberOfIntegrationPoints(); pt++)
                  {

                        //      Tensor2 fi_e;

                        // recup du pointeur sur le tableau de valeurs
                        fpt = pel->intPoint(pt);

                        // Nouvel algorithme developp� en 2000

                        // decomposition de la contrainte
                        fi_e = fpt->S;

                        // crit�re de plasticit�
                        f_trial_1 =
                            fi_e.norme() - sqrt(2. / 3.) * pmat->getYieldStress(fpt);
                        if (f_trial_1 <= 0.)
                        {
                        }
                        else
                        {
                              // normale ext�rieure
                              n_1 = fi_e / fi_e.norme();

                              // calcul de Gamma cas lin�aire
                              if (pmat->isLinear())
                              {
                                    Gamma =
                                        (f_trial_1) / (2. * pmat->mu() *
                                                       (1. +
                                                        pmat->getDerYieldStress(fpt) / (3. *
                                                                                        pmat->mu())));
                                    //  fpt->alpha+=sqrt(2./3.)*Gamma;
                              }

                              // calcul de Gamma cas non-lin�aire
                              else
                              {
                                    Real fun = 1;
                                    Real dfun;
                                    Real evp0 = fpt->evp;
                                    Real xi = fi_e.norme();
                                    Gamma = 0;
                                    int it = 0;

                                    while (Abs(fun) > 0.0001)
                                    {
                                          //version Simo
                                          fun =
                                              -sqrt(2. / 3.) * pmat->getYieldStress(fpt) +
                                              xi - 2. * pmat->mu() * Gamma;
                                          dfun =
                                              +2. * pmat->mu() +
                                              (2. / 3.) * pmat->getDerYieldStress(fpt);
                                          Gamma += fun / dfun;
                                          if ((it++) > 50)
                                                fatalError("Too many atempts in plastic procedure",
                                                           "not converged");
                                          fpt->evp = evp0 + sqrt(2. / 3.) * Gamma;
                                    }
                                    fpt->evp = evp0;
                              }

                              fpt->S -= (2. * pmat->mu() * Gamma) * n_1;
                              fpt->evp += sqrt(2. / 3.) * Gamma;

                              // mise a jour de la d�formation plastique
                              // ca n'a pas l'air tres au point !!!
                              fpt->EpsPlas += Gamma * n_1;
                              fpt->dEpsPlas = Gamma * n_1 / timeStep;
                        }
                  }
            }

            // RFI
            for (pt = 0; pt < pel->getNumberOfIntegrationPoints(); pt++)
            {

                  fpt = pel->intPoint(pt);
                  //version 2
                  fpt->S = fpt->R * (fpt->S * fpt->R.Transpose());
                  fpt->Sig = fpt->S + fpt->p * Id;

                  // RFI des deformations
                  fpt->Eps = fpt->R * (fpt->Eps * fpt->R.Transpose());
                  //version 2
                  fpt->EpsPlas = fpt->R * (fpt->EpsPlas * fpt->R.Transpose());
                  fpt->dEpsPlas = fpt->R * (fpt->dEpsPlas * fpt->R.Transpose());
            }
      }
}
//-----------------------------------------------------------------------------
void ExplicitSolver::computeMass()
//-----------------------------------------------------------------------------
{
      Element *pel;
      Node *pnd;
      Indice nonodes;
      Indice i, el, glob;

#ifdef PRINT_Execution_Solve
      cout << "Mass equation\n";
#endif

      // matrice M, et vecteur F
      MatrixDiag M(domain->nodes.size(), domain->nodes.size());
      Vector F(domain->nodes.size());
      Vector roInc(domain->nodes.size());

      for (el = 0; el < domain->elements.size(); el++)
      {

            // chargement de l'element en cours
            pel = domain->elements(el);

            // nombre de noeuds de l'element
            nonodes = pel->getNumberOfNodes();

            // matrice Me, et vecteur Fe
            MatrixDiag Me(nonodes, nonodes);
            Vector Fe(nonodes);

            // integration de la conservation de la masse
            // sur l'element courant
            pel->computeMassEquation(Me, Fe);

            // assemblage de la matrice de masse
            // et du vecteur
            for (i = 0; i < nonodes; i++)
            {
                  // recuperation du numero global
                  glob = pel->nodes(i)->Id;

                  // assemblage de M
                  M(glob) += Me(i);

                  // assemblage de F
                  F(glob) += Fe(i);
            }
      }

      // resolution du systeme M x q = F
      roInc = M.Solve(F);

      // update du champ de densites
      for (i = 0; i < domain->nodes.size(); i++)
      {
            pnd = domain->nodes(i);
            pnd->New->dro = roInc(i);
      }
}

//-----------------------------------------------------------------------------
void ExplicitSolver::computeEnergy()
//-----------------------------------------------------------------------------
{
      Element *pel;
      Node *pnd;
      Indice nonodes;
      Indice i, el, glob;

#ifdef PRINT_Execution_Solve
      cout << "Energy equation\n";
#endif

      // matrice M, et vecteur F
      MatrixDiag M(domain->nodes.size(), domain->nodes.size());
      Vector F(domain->nodes.size());
      Vector eInc(domain->nodes.size());

      for (el = 0; el < domain->elements.size(); el++)
      {

            // chargement de l'element en cours
            pel = domain->elements(el);

            // nombre de noeuds de l'element
            nonodes = pel->getNumberOfNodes();

            // matrice Me, et vecteur Fe
            MatrixDiag Me(nonodes, nonodes);
            Vector Fe(nonodes);

            // integration de la conservation de la masse
            // sur l'element courant
            pel->computeEnergyEquation(Me, Fe);

            // assemblage de la matrice de masse
            // et du vecteur
            for (i = 0; i < nonodes; i++)
            {

                  // recuperation du numero global
                  glob = pel->nodes(i)->Id;

                  // assemblage de M
                  M(glob) += Me(i);

                  // assemblage de F
                  F(glob) += Fe(i);
            }
      }

      // resolution du systeme M x q = F
      eInc = M.Solve(F);
      //    cout << eInc <<endl;

      // update du champ des energies
      for (i = 0; i < domain->nodes.size(); i++)
      {
            pnd = domain->nodes(i);
            pnd->New->de = eInc(i);

            // mise a jour de la masse nodale
            pnd->mass = M(i);
      }
}

//-----------------------------------------------------------------------------
void ExplicitSolver::computeMomentum()
//-----------------------------------------------------------------------------
{
      Element *pel;
      Node *pnd;
      Indice nonodes;
      Indice i, j, el, glob;

#ifdef PRINT_Execution_Solve
      cout << "Momentum equation\n";
#endif

      // matrice M, et vecteur F
      MatrixDiag M(domain->NoDim() * domain->nodes.size(), domain->NoDim() * domain->nodes.size());
      Vector F(domain->NoDim() * domain->nodes.size());
      Vector vInc(domain->NoDim() * domain->nodes.size());

      // phase de calcul
      for (el = 0; el < domain->elements.size(); el++)
      {

            // chargement de l'element en cours
            pel = domain->elements(el);

            // nombre de noeuds de l'element
            nonodes = pel->getNumberOfNodes();

            // matrice Me, et vecteur Fe
            MatrixDiag Me(domain->NoDim() * nonodes, domain->NoDim() * nonodes);
            Vector Fe(domain->NoDim() * nonodes);

            // integration de la conservation de la quantite de mouvement
            // sur l'element courant
            pel->computeMomentumEquation(Me, Fe);

            // assemblage de la matrice de masse
            // et du vecteur
            for (i = 0; i < nonodes; i++)
            {

                  // recuperation du numero global
                  glob = (pel->nodes(i)->Id) * domain->NoDim();

                  for (j = 0; j < domain->NoDim(); j++)
                  {

                        // assemblage de M
                        M(glob + j) += Me(i * domain->NoDim() + j);

                        // assemblage de F
                        F(glob + j) += Fe(i * domain->NoDim() + j);
                  }
            }
      }
      // COUT
      //  cout << "M="<<M << endl;
      //  cout << "F="<<F << endl;

      // resolution du systeme M x q = F
      vInc = M.Solve(F);
      //  cout << "acc="<<vInc << endl;

      // update du champ des accelerations
      for (i = 0; i < domain->nodes.size(); i++)
      {
            pnd = domain->nodes(i);

            // mise a jour des accelerations
            for (j = 0; j < domain->NoDim(); j++)
                  pnd->New->dmat_speed(j) = vInc(i * domain->NoDim() + j);

            //   cout << pnd->New->dmat_speed << endl;
            // mise a jour des vitesses
            //    pnd->New->mat_speed=pnd->Current->mat_speed+(times.timeStep*pnd->New->dmat_speed);
      }
      //  exit(0);
}

//-----------------------------------------------------------------------------
void ExplicitSolver::computeCorrections()
//-----------------------------------------------------------------------------
{
      Indice i;
      Node *pnd;

#ifdef PRINT_Execution_Solve
      cout << "Corrections de ro, e, mat_speed et u\n";
#endif

      Real alpha = 0.;
      Real dzeta = 0.;
      Real gamma = 0.;
      Real beta = 0.;

      // phase de correction
      for (i = 0; i < domain->nodes.size(); i++)
      {

            // recuperation du noeud courant
            pnd = domain->nodes(i);

            // correction de la densite
            pnd->New->ro += (alpha * timeStep) * pnd->New->dro;

            // correction de l'energie
            pnd->New->e += (dzeta * timeStep) * pnd->New->de;

            // application des conditions aux limites impos�es
            //    pnd->boundary->applyConstant_V0(pnd);

            pnd->New->mat_speed += (gamma * timeStep) * pnd->New->dmat_speed;
            pnd->New->disp += (beta * SQ(timeStep)) * pnd->New->dmat_speed;

            //   cout << "a"<<i<<"="<<pnd->New->dmat_speed << endl;
            //   cout << "v"<<i<<"="<<pnd->New->mat_speed << endl;
            //   cout << "d"<<i<<"="<<pnd->New->disp << endl;

            // correction de la vitesse materielle
            //    pnd->New->mat_speed=pnd->Current->mat_speed+times.timeStep*pnd->New->dmat_speed;

            //pnd->Current->mat_speed+(times.timeStep)*pnd->New->dmat_speed;
            //    cout << pnd->New->dmat_speed<<endl;
            //    pnd->boundary->applyConstant_V0(pnd);

            // correction du deplacement
      }
}

//-----------------------------------------------------------------------------
void ExplicitSolver::computeTemperatures()
//-----------------------------------------------------------------------------
{
      Node *pnd;
      Material *pmat;
      Indice no;
      Real coeff;

      coeff = 1.0;

      for (no = 0; no < domain->nodes.size(); no++)
      {

            // recuperation du noeud
            pnd = domain->nodes(no);

            // chargement du materiau
            pmat = pnd->elements(0)->material;

            // calcul de la temperature
            pnd->New->T = pnd->T0 + coeff * pnd->New->e / pmat->heatCoeff();
      }
}
//-----------------------------------------------------------------------------
void ExplicitSolver::scanInterfaces()
//-----------------------------------------------------------------------------
{
#ifdef PRINT_Execution_Solve
      cout << "Scanning des interfaces\n";
#endif

      // recherche des penetrations d'elements
      // sur la liste des interfaces du domaine
      for (Indice it = 0; it < domain->interfaces.size(); it++)
      {
            domain->interfaces(it)->ScanIncludeNodes();
      }

      // calculs
      /*  for (Indice it=0;it<interfaces.size();it++) {
    interfaces(it)->computeSpeeds(times.timeStep);
  }*/
}
//-----------------------------------------------------------------------------
void ExplicitSolver::computeContactForces()
//-----------------------------------------------------------------------------
{
      Indice no;

#ifdef PRINT_Execution_Solve
      cout << "contact forces computation\n";
#endif

      for (no = 0; no < domain->nodes.size(); no++)
      {
            domain->nodes(no)->motion->computeForces(timeStep);
      }
}
//-----------------------------------------------------------------------------
void ExplicitSolver::updateTime()
//-----------------------------------------------------------------------------
{
      domain->currentTime += timeStep;
}
