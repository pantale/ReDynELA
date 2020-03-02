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

// begin date : 12/03/1997

/*
  Class Domain definition
*/

#include <Domain.h>
#include <Structure.h>
#include <HistoryFile.h>
#include <Node.h>
#include <NodalField.h>
#include <Material.h>
#include <IntegrationPoint.h>
#include <lagMotion.h>
#include <Element.h>
#include <Interface.h>
#include <Solver.h>
#include <Boundary.h>

// Inclusion conditionnelle de OpenMP
#ifdef _OPENMP
#include <omp.h>
#include <Jobs.h>
#endif

extern String parsedFileName;
extern CPUrecord recordTimes;

Jobs mach;
Jobs timeStepWork;
Jobs internalMatricesWork;
// Real cpu[8][8];
// Real lastcpu[8][8];

//-----------------------------------------------------------------------------
Domain::Domain()
//-----------------------------------------------------------------------------
{
  name = "defaultDomain";

  // initialisation par defaut
  numberOfDim = 0;

  // attacher a global domain
  history_file = NULL;

  massComputed = False;
  fmax0 = 0.0;

  // redimensionner les listes pour gagner du temps dans les lectures
  // on utilise des blocs de 100 allocations pour les noeuds et les �l�ments
  nodes.stackIncrement() = 100;
  elements.stackIncrement() = 100;

  nodes.redim(100);
  elements.redim(100);
}

//-----------------------------------------------------------------------------
Domain::Domain(const Domain &dom)
//-----------------------------------------------------------------------------
{

  numberOfDim = dom.numberOfDim;
  cerr << "erreur copie de Domain\n";
  exit(-1);
}

//-----------------------------------------------------------------------------
Domain::~Domain()
//-----------------------------------------------------------------------------
{
}

//!Calcule le déterminant du Jacobien de tous les éléments de la grille
/*!
  Cette méthode calcule le Jacobien de tous les éléments de la grille.
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Domain::computeInternalMatrices()
//-----------------------------------------------------------------------------
{
#ifdef PRINT_Execution_Solve
  cout << "Calcul des matrices internes\n";
#endif

  // map the work on processors
  internalMatricesWork.mapInitial(elements);

#pragma omp parallel
  {
    Job *work;
    Element *pel;

    // get the work from the processor
    work = internalMatricesWork.getJob();

    // get the first
    work->first();

    // loop over the elements of the work
    while ((pel = work->currentUp()) != NULL)
    {

      //calcul des matrices internes
      pel->computeInternalMatrices();
    }
  }
}

//-----------------------------------------------------------------------------
void Domain::computeInternalForces()
//-----------------------------------------------------------------------------
{
  Indice el, i, j;
  Vector Fe;
  Element *pel;
  Indice glob;

  Indice noddl = numberOfDim * nodes.size();

#ifdef PRINT_Execution_Solve
  cout << "Calcul des matrices internes\n";
#endif

  // mise à zéro des forces internes
  Fint.redim(noddl);
  Fint = 0.0;

  Unix un;
  String st;
  Indice cas;
  st = un.getCurrEnvValue("computeInternalForces");
  st.popInt(cas);

  switch (cas)
  {
    //Parall�lisation de base boucle for simple avec omp critical sur le calcul de Fint

  case 0:
    // BOUCLE FOR
    {
      if (currentTime == 0)
        printf("Case 0\n");
      Indice el, i, j, glob;
      Vector Fe;
      Element *pel;

      // calcul des forces internes
      Indice loop = elements.size();
#pragma omp parallel for private(i, j, Fe, pel, glob)
      for (el = 0; el < loop; el++)
      {
        // recuperation de l'�l�ment
        pel = elements(el);

        // calcul des forces internes de l'�l�ment
        pel->computeInternalForces(Fe);

        // assemblage de la matrice de masse
        // et du vecteur
        for (i = 0; i < pel->nodes.size(); i++)
        {

          // recuperation du numero global
          glob = (pel->nodes(i)->Id) * numberOfDim;

          for (j = 0; j < numberOfDim; j++)
          {
            // assemblage de F
#pragma omp critical
            Fint(glob + j) += Fe(i * numberOfDim + j);
          }
        }
      }
    }
    break;

    // Parall�lisation de base boucle for simple avec vecteur local par processeur et omp critical sur l'assemblage � chaque it�ration par processeur
  case 1:
    // BOUCLE FOR
    {
      if (currentTime == 0)
        printf("Case 1\n");
      Indice el, i, j, glob;
      Vector Fe;
      Element *pel;

      // calcul des forces internes
      Indice loop = elements.size();
#pragma omp parallel for private(i, j, Fe, pel, glob)
      for (el = 0; el < loop; el++)
      {
        Vector FintLocal(noddl);

        // recuperation de l'�l�ment
        pel = elements(el);

        // calcul des forces internes de l'�l�ment
        pel->computeInternalForces(Fe);

        // assemblage de la matrice de masse
        // et du vecteur
        for (i = 0; i < pel->nodes.size(); i++)
        {

          // recuperation du numero global
          glob = (pel->nodes(i)->Id) * numberOfDim;

          for (j = 0; j < numberOfDim; j++)
          {
            // assemblage de F
            FintLocal(glob + j) += Fe(i * numberOfDim + j);
          }
        }
#pragma omp critical
        Fint += FintLocal;
      }
    }

    break;

    // PROCEDURE STANDARD
    // Parall�lisation de zone de programme avec vecteur local propre � chaque processeur, synchronisation des diff�rents processseurs sur la liste des �l�ments restant � traiter dans la liste globale. Chaque processeur traite les �l�ments jusqu'� ce qu'il n'en reste plus dans la liste globale.
    // L'assemblage des donn�es de chaque processeur est faite � la fin du calcul en parall�le par tous les processeurs.
  case 2:
  {
    if (currentTime == 0)
      printf("Case 2\n");
    List<Element *> lel = elements;

    // first element of list
    lel.first();

    const Indice threads = omp_get_max_threads();
    //   Indice threadsNoddl = threads*noddl;
    //   Real *FintLocal = new Real [threadsNoddl];
    Vector *pFintLocal = new Vector[threads];

    // calcul des forces internes
#pragma omp parallel
    {
      Element *pel;
      //   Vector FintLocal(noddl);
      Vector Fe;

      int thread = omp_get_thread_num();
      Vector *pFint = &(pFintLocal[thread]);
      pFint->redim(noddl);
      (*pFint) = 0.;

      //     int thread = omp_get_thread_num();
      //     Indice offset = thread*noddl;

#pragma omp critical
      {
        pel = lel.currentUp();
      }

      while (pel != NULL)
      {

        // calcul des forces internes de l'�l�ment
        pel->computeInternalForces(Fe);

        // assemblage de la matrice de masse
        // et du vecteur
        for (Indice i = 0; i < pel->nodes.size(); i++)
        {

          // recuperation du numero global
          Indice glob = (pel->nodes(i)->Id) * numberOfDim; //+ offset;

          for (Indice j = 0; j < numberOfDim; j++)
          {
            // assemblage de F
            (*pFint)(glob + j) += Fe(i * numberOfDim + j);
          }
        }

#pragma omp critical
        {
          pel = lel.currentUp();
        }
      }

      // #pragma omp critical
      //        Fint += FintLocal;
    }

#pragma omp parallel for
    for (Indice i = 0; i < noddl; i++)
    {
      Real val = 0;
      for (Indice j = 0; j < threads; j++)
        val += pFintLocal[j](i);
      Fint(i) = val;
    }

    delete[] pFintLocal;
    // FIN PROCEDURE STANDARD
  }
  break;

  case 5:
    // EQUILIBRAGE DYNAMIQUE et COLLECTION PARALLELE

    {

      if (currentTime == 0)
        printf("Case 5\n");
      // -------------------------------------------------------------------------
      // This one works

      // allocation de variable par thread
      const short threads = omp_get_max_threads();
      Vector *pFintLocal = new Vector[threads];

      // get number of processors
      mach.mapInitial(elements);
      mach.resetSync();

#pragma omp parallel
      {
        Job *proc;
        proc = mach.getJob();

        int thread = omp_get_thread_num();
        Vector *pFint = &(pFintLocal[thread]);
        pFint->redim(noddl);
        (*pFint) = 0.;

        // get the first
        proc->first();

        //	  Vector FintLocal(noddl);
        Vector Fe;
        Element *pel;
        //       for (Indice el = sta[pr]; el <= sto[pr]; el++)
        while ((pel = proc->currentUp()) != NULL)
        {
          // recuperation de l'�l�ment
          //	  Element * pel=elements (el);

          // calcul des forces internes de l'�l�ment
          pel->computeInternalForces(Fe);

          // assemblage de la matrice de masse
          // et du vecteur
          // assemblage du vecteur
          Indice loop_I = pel->nodes.size();
          Indice *ind = new Indice[loop_I];
          for (Indice I = 0; I < loop_I; I++)
            ind[I] = pel->nodes(I)->Id;
          pFint->gatherFrom(Fe, ind, numberOfDim);
          delete[] ind;
          // 	    for ( Indice i = 0; i < pel->nodes.size(); i++)
          // 	      {

          // 		// recuperation du numero global
          // 		Indice glob = (pel->nodes (i)->Id) * numberOfDim;

          // 		for ( Indice j = 0; j < numberOfDim; j++)
          // 		  {
          // 		    // assemblage de F
          // 		    (*pFint) (glob + j) += Fe (i * numberOfDim + j);
          // 		  }
          // 	      }
        }

#pragma omp critical
        mach.stopSync();

        // phase d'assemblage final critique
        // #pragma omp critical
        // 	  Fint += FintLocal;

        proc->waitOthers();
      }

#pragma omp parallel for
      for (Indice i = 0; i < noddl; i++)
      {
        Real val = 0;
        for (Indice j = 0; j < threads; j++)
          val += pFintLocal[j](i);
        Fint(i) = val;
      }

      delete[] pFintLocal;

      // equilibrage des charges
      mach.equilibrate();

      // -------------------------------------------------------------------------
    }
    break;

  case 6:
    // SANS EQUILIBRAGE DYNAMIQUE et COLLECTION PARALLELE

    {
      if (currentTime == 0)
        printf("Case 6\n");
      //	const Indice loop=elements.size ()-1;

      // allocation de variable par thread
      const Indice threads = omp_get_max_threads();
      Vector *pFintLocal = new Vector[threads];

      // get number of processors
      mach.mapInitial(elements);

      // calcul des forces internes
#pragma omp parallel
      {
        Job *proc;
        proc = mach.getJob();

        int thread = omp_get_thread_num();
        Vector *pFint = &(pFintLocal[thread]);
        pFint->redim(noddl);
        (*pFint) = 0.;

        // get the first
        proc->first();

        Vector Fe;
        Element *pel;
        //       for (Indice el = sta[pr]; el <= sto[pr]; el++)
        while ((pel = proc->currentUp()) != NULL)
        {

          // calcul des forces internes de l'�l�ment
          pel->computeInternalForces(Fe);

          // assemblage du vecteur
          Indice loop_I = pel->nodes.size();
          Indice *ind = new Indice[loop_I];
          for (Indice I = 0; I < loop_I; I++)
            ind[I] = pel->nodes(I)->Id;
          pFint->gatherFrom(Fe, ind, numberOfDim);
          delete[] ind;

          // 	      for ( Indice i = 0; i < pel->nodes.size(); i++)
          // 	      {

          // 	      // recuperation du numero global
          // 	      Indice glob = (pel->nodes (i)->Id) * numberOfDim;

          // 	      for ( Indice j = 0; j < numberOfDim; j++)
          // 	      {
          // 	      // assemblage de F
          // 	      (*pFint)  (glob + j) += Fe (i * numberOfDim + j);
          // 	      }
          // 	      }
        }

        // #pragma omp critical
        // 	  mach.stopSync();

        // phase d'assemblage final critique
        // #pragma omp critical
        // 	  Fint += FintLocal;

        // 	  proc->waitOthers();
      }

#pragma omp parallel for
      for (Indice i = 0; i < noddl; i++)
      {
        Real val = 0;
        for (Indice j = 0; j < threads; j++)
          val += pFintLocal[j](i);
        Fint(i) = val;
      }

      delete[] pFintLocal;

      // equilibrage des charges
      // 	mach.equilibrate();
    }
    break;

  case 7:
    // SANS EQUILIBRAGE DYNAMIQUE et COLLECTION PARALLELE avec TABLEAU FORTRAN PUR

    {

      if (currentTime == 0)
        printf("Case 7\n");
      // -------------------------------------------------------------------------

      // allocation de variable par thread
      const Indice threads = omp_get_max_threads();
      //	Vector* pFintLocal = new Vector [threads];
      Real **pFintLocal = new Real *[threads];

      // get number of processors
      mach.mapInitial(elements);
      //	mach.resetSync();

      // calcul des forces internes
      //  for (pr=0; pr<np; pr++)
#pragma omp parallel
      //private(,pel)
      {
        Job *proc;
        proc = mach.getJob();

        int thread = omp_get_thread_num();
        //	  Vector* pFint=&(pFintLocal[thread]);
        pFintLocal[thread] = new Real[noddl];
        Real *pFint = pFintLocal[thread];
        //	  pFint = new Real [noddl];
        for (Indice i = 0; i < noddl; i++)
          pFint[i] = 0.0;
        //	  pFint->redim(noddl);
        //	  (*pFint)=0.;

        // get the first
        proc->first();

        //	  Vector FintLocal(noddl);
        Vector Fe;
        Element *pel;
        //       for (Indice el = sta[pr]; el <= sto[pr]; el++)
        while ((pel = proc->currentUp()) != NULL)
        {
          // recuperation de l'�l�ment
          //	  Element * pel=elements (el);

          // calcul des forces internes de l'�l�ment
          pel->computeInternalForces(Fe);

          // assemblage de la matrice de masse
          // et du vecteur
          for (Indice i = 0; i < pel->nodes.size(); i++)
          {

            // recuperation du numero global
            Indice glob = (pel->nodes(i)->Id) * numberOfDim;

            for (Indice j = 0; j < numberOfDim; j++)
            {
              // assemblage de F
              pFint[glob + j] += Fe(i * numberOfDim + j);
            }
          }
        }

        // #pragma omp critical
        // 	  mach.stopSync();

        // phase d'assemblage final critique
        // #pragma omp critical
        // 	  Fint += FintLocal;

        // 	  proc->waitOthers();
      }

#pragma omp parallel for
      for (Indice i = 0; i < noddl; i++)
      {
        Real val = 0;
        for (Indice j = 0; j < threads; j++)
          val += pFintLocal[j][i];
        Fint(i) = val;
      }

      //	for (Indice i=0;i<noddl;i++) delete [] pFintLocal[i];
      delete[] pFintLocal;

      // equilibrage des charges
      // 	mach.equilibrate();
      // -------------------------------------------------------------------------
    }
    break;

  case 4:
    // Parall�lisation de zone de programme avec vecteur local propre � chaque process
    // eur et r�partition dynamique de la charge de la jobs sur les diff�rents processeurs.
    // L'assemblage est fait � la fin du calcul en utilisant une directive omp critical.
    // EQUILIBRAGE DYNAMIQUE

    {
      // -------------------------------------------------------------------------
      if (currentTime == 0)
        printf("Case 4\n");

      // get number of processors
      mach.mapInitial(elements);
      mach.resetSync();

      // calcul des forces internes
      //  for (pr=0; pr<np; pr++)
#pragma omp parallel
      //private(,pel)
      {
        Job *proc;
        proc = mach.getJob();

        // get the first
        proc->first();

        Vector FintLocal(noddl);
        Vector Fe;
        Element *pel;
        //       for (Indice el = sta[pr]; el <= sto[pr]; el++)
        while ((pel = proc->currentUp()) != NULL)
        {
          // recuperation de l'�l�ment
          //	  Element * pel=elements (el);

          // calcul des forces internes de l'�l�ment
          pel->computeInternalForces(Fe);

          // assemblage de la matrice de masse
          // et du vecteur
          for (Indice i = 0; i < pel->nodes.size(); i++)
          {

            // recuperation du numero global
            Indice glob = (pel->nodes(i)->Id) * numberOfDim;

            for (Indice j = 0; j < numberOfDim; j++)
            {
              // assemblage de F
              FintLocal(glob + j) += Fe(i * numberOfDim + j);
            }
          }
        }

#pragma omp critical
        mach.stopSync();

        // phase d'assemblage final critique
#pragma omp critical
        Fint += FintLocal;

        proc->waitOthers();
      }

      // equilibrage des charges
      mach.equilibrate();
      // -------------------------------------------------------------------------
    }
    break;

    // SANS EQUILIBRAGE DYNAMIQUE
    // Parall�lisation de zone de programme avec vecteur local propre � chaque process
    // eur sans r�partition dynamique de la charge. Chaque processeur traite une taille de calcul identique.
    // L'assemblage est fait � la fin du calcul en utilisant une directive omp critical.
  case 3:
  {
    // -------------------------------------------------------------------------
    if (currentTime == 0)
      printf("Case 3\n");

    // get number of processors
    mach.mapInitial(elements);
    mach.resetSync();

    // calcul des forces internes
    //  for (pr=0; pr<np; pr++)
#pragma omp parallel
    //private(,pel)
    {
      Job *proc;
      proc = mach.getJob();

      // get the first
      proc->first();

      Vector FintLocal(noddl);
      Vector Fe;
      Element *pel;
      //       for (Indice el = sta[pr]; el <= sto[pr]; el++)
      while ((pel = proc->currentUp()) != NULL)
      {
        // recuperation de l'�l�ment
        //	  Element * pel=elements (el);

        // calcul des forces internes de l'�l�ment
        pel->computeInternalForces(Fe);

        // assemblage de la matrice de masse
        // et du vecteur
        for (Indice i = 0; i < pel->nodes.size(); i++)
        {

          // recuperation du numero global
          Indice glob = (pel->nodes(i)->Id) * numberOfDim;

          for (Indice j = 0; j < numberOfDim; j++)
          {
            // assemblage de F
            FintLocal(glob + j) += Fe(i * numberOfDim + j);
          }
        }
      }

      // #pragma omp critical
      // 	  mach.stopSync();

      // phase d'assemblage final critique
#pragma omp critical
      Fint += FintLocal;

      // 	  proc->waitOthers();
    }
  }
  break;

    // -------------------------------------------------------------------------

  default:
    fatalError("Unknown procedure", "");
  }
}

//-----------------------------------------------------------------------------
Real Domain::getMaximumFrequency()
//-----------------------------------------------------------------------------
{
/*  Real fmax = 0.0;
  Boolean ok = False;
  Indice iteration = 0;
  Real convergence;
  Real limite = maximumFrequencyConvergence;
  Indice i, el;
  Element *pel;

  // matrices globales
  Indice noddl = numberOfDim * nodes.size();

  Indice loop_el = elements.size();
  for (el = 0; el < loop_el; el++)
  {
    // recuperation de l'�l�ment
    pel = elements(el);

    //calcul des forces elastiques
    pel->computeElasticStiffnessMatrix(pel->Ke, False);
  }

  // initialisation du vecteur si besoin
  if ((fmax0 == 0) || (EV_TimeStep.size() != noddl))
  {
    EV_TimeStep.redim(noddl);
    EV_TimeStep(0) = 1.0;
    for (i = 1; i < noddl; i++)
      EV_TimeStep(i) = EV_TimeStep(i - 1) - 2. / (noddl - 1);
  }

  Vector loc;
  Vector EV0;

  while (!ok)
  {
    iteration++;
    EV0 = EV_TimeStep;
    EV_TimeStep = 0.0;
    for (el = 0; el < loop_el; el++)
    {
      // recuperation de l'�l�ment
      pel = elements(el);
      loc.redim(pel->Ke.rows());
      loc = 0.;
      Indice loop_I = pel->nodes.size();
      Indice *ind = new Indice[loop_I];
      for (Indice I = 0; I < loop_I; I++)
        ind[I] = pel->nodes(I)->Id;
      loc.scatterFrom(EV0, ind, numberOfDim);
      loc = pel->Ke * loc;
      EV_TimeStep.gatherFrom(loc, ind, numberOfDim);
      delete[] ind;
    }

    Mass.divideBy(EV_TimeStep);
    fmax = EV_TimeStep.maxAbsoluteValue();

    EV_TimeStep /= fmax;
    convergence = Abs(fmax - fmax0) / fmax;

    // verifier si on a convergé
    if (convergence < limite)
      ok = True;

    fmax0 = fmax;

    if (iteration > maximumFrequencyMaxIterations)
    {
      fatalError("Domain::getMaximumFrequency", "power iteration method not converged %10.4E\n", convergence);
    }
  }

  return sqrt(fmax);*/
  Real fmax = 0.0;
  Boolean ok = False;
  Indice iteration = 0;
  Real convergence;
  Real limite = maximumFrequencyConvergence;
  Indice i, el;
  //Grid *pgrid = grids.current();
  // Real ray;
  Element *pel;
  Indice noddl = numberOfDim * nodes.size();
  List<Element *> listOfElements = elements;

  recordTimes.start("LOOP1");

  //  Initial maping of processors
  timeStepWork.mapInitial(elements);

#pragma omp parallel private(pel)
  {
    Job *work;
    work = timeStepWork.getJob();

    // get the first
    work->first();
    while ((pel = work->currentUp()) != NULL)
    {
      //calcul des forces elastiques
      pel->computeElasticStiffnessMatrix(pel->Ke, False);
    }
  }

  recordTimes.stop("LOOP1");

  Indice loop_el = elements.size();

  // initialisation du vecteur si besoin
  if ((fmax0 == 0) || (EV_TimeStep.size() != noddl))
  {
    EV_TimeStep.redim(noddl);
    EV_TimeStep(0) = 1.0;
    for (i = 1; i < noddl; i++)
      EV_TimeStep(i) = EV_TimeStep(i - 1) - 2. / (noddl - 1);
  }

  Vector EV0;

  // allocation de variable par thread
  const Indice threads = omp_get_max_threads();
  Vector *gEV_TimeStepLoc = new Vector[threads];
  Real Fmax[noddl];

  while (!ok)
  {
    iteration++;
    EV0 = EV_TimeStep;

    recordTimes.start("LOOP2");

#pragma omp parallel private(pel)
    {
      Vector loc;
      int thread = omp_get_thread_num();
      Vector *pEV_TimeStepLoc = &(gEV_TimeStepLoc[thread]);
      pEV_TimeStepLoc->redim(noddl);
      (*pEV_TimeStepLoc) = 0.;

      Job *work;
      work = timeStepWork.getJob();

      // get the first one
      work->first();

      while ((pel = work->currentUp()) != NULL)
      {
        loc.redim(pel->Ke.rows());
        loc = 0.;
        Indice loop_I = pel->nodes.size();
        Indice *ind = new Indice[loop_I];
        for (Indice I = 0; I < loop_I; I++)
          ind[I] = pel->nodes(I)->Id;
        loc.scatterFrom(EV0, ind, numberOfDim);
        pEV_TimeStepLoc->gatherFrom(pel->Ke * loc, ind, numberOfDim);
        delete[] ind;
      }
    }
    recordTimes.stop("LOOP2");
    recordTimes.start("LOOP3");
    for (Indice i = 0; i < threads; i++)
      Fmax[i] = 0.0;

#pragma omp parallel for schedule(static)
    for (Indice i = 0; i < noddl; i++)
    {
      Real val = 0.0;
      int thread = omp_get_thread_num();
      for (Indice j = 0; j < threads; j++)
        val += gEV_TimeStepLoc[j](i);
      val /= Mass(i);
      if (fabs(val) > Fmax[thread])
        Fmax[thread] = fabs(val);
      EV_TimeStep(i) = val;
    }

    fmax = Fmax[0];
    for (Indice i = 1; i < threads; i++)
      if (Fmax[i] > fmax)
        fmax = Fmax[i];

    EV_TimeStep /= fmax;
    recordTimes.stop("LOOP3");

    convergence = Abs(fmax - fmax0) / fmax;

    // verifier si on a converg�
    if (convergence < limite)
      ok = True;

    fmax0 = fmax;

    if (iteration > maximumFrequencyMaxIterations)
    {
      fatalError("Domain::getMaximumFrequency", "power iteration method not converged %10.4E\n", convergence);
    }
  }

  delete[] gEV_TimeStepLoc;

  return sqrt(fmax);
}

//-----------------------------------------------------------------------------
void Domain::computeMassMatrix(Boolean force)
//-----------------------------------------------------------------------------
{
  if (massComputed && !force)
    return;

  Element *pel;
  Node *pnd;
  Indice nonodes;
  Indice i, j, el, no, glob;

  // matrice Masse avec initialisation
  Indice noddl = numberOfDim * nodes.size();
  Mass.redim(noddl, noddl);
  Mass = 0.;

  // matrice elementaire
  MatrixDiag Me;

  for (el = 0; el < elements.size(); el++)
  {
    // chargement de l'element en cours
    pel = elements(el);

    // nombre de noeuds de l'element
    nonodes = pel->getNumberOfNodes();

    // matrice Me
    Me.redim(nonodes, nonodes);

    // calcul de la matrice de masse sur l'élément courant
    pel->computeMassMatrix(Me);

    // assemblage de la matrice de masse
    for (i = 0; i < nonodes; i++)
    {
      // recuperation du numero global
      glob = pel->nodes(i)->Id;

      // assemblage de M
      for (j = 0; j < numberOfDim; j++)
        Mass(glob * numberOfDim + j) += Me(i);
    }
  }

  // flag de la masse
  massComputed = True;

  // redistribution des masses sur les noeuds
  for (no = 0; no < nodes.size(); no++)
  {
    // chargement de l'element en cours
    pnd = nodes(no);

    // affectatio de la masse au noeud
    pnd->mass = Mass(pnd->Id * numberOfDim);
  }
}

//!Initialise les structures mémoire pour la résolution
/*!
  Cette méthode est utilisée pour initialiser les structures mémoire après lecture des données dans le fichier source. C'est la phase de préprocessing de la structure. Un ensemble de vérifications sont effectuées dans cette méthode.

  La première de ces vérifications concerne la topologie des grilles. On doit avoir la même topologie des éléments dans chacune des grilles de la structure. On fait ainsi appel à la méthode Grid::initSolve() pour vérifier la cohérence des grilles et initialiser les données.

  \return True si la méthode n'a généré aucune erreur
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean Domain::initSolve()
//-----------------------------------------------------------------------------
{
  //  Indice noDim;
  Indice family;
  Indice i, j;
  Element *pel;
  Node *pnd;
  Material *pmat;
  NodalField *pnd_field;
  Indice set;
  Solver *psolver;

  // si pas d'�l�ments alors rien � faire ici
  if (elements.size() == 0)
  {
    // il faudrait faire plus car c'est idiot de faire une grille totalement vide d'�l�ments
    return False;
  }

  // dimension de la grille (par d�faut c'est la dimension du premier �l�ment de la grille)
  dimension = elements(0)->getNumberOfDimensions();

  // verification de la coh�rence de la famille d'elements
  family = elements(0)->getFamily();
  for (i = 1; i < elements.size(); i++)
    if (family != elements(i)->getFamily())
    {
      cout << "Passing from ";
      if (family == Element::Bidimensional)
        cout << "2D";
      if (family == Element::Axisymetric)
        cout << "2D Axi";
      if (family == Element::Threedimensional)
        cout << "3D";
      cout << " to ";
      if (elements(i)->getFamily() == Element::Bidimensional)
        cout << "2D\n";
      if (elements(i)->getFamily() == Element::Axisymetric)
        cout << "2D Axi\n";
      if (elements(i)->getFamily() == Element::Threedimensional)
        cout << "3D\n";
      fatalError("Grid topology verification\n",
                 "Mixed Topology found in element %d \n", i);
    }

  // log file
  (*Global_Structure->logFile) << "Grid topology : " << (elements(0)->getFamily() == Element::Bidimensional ? "2D" : elements(0)->getFamily() == Element::Axisymetric ? "2D Axi" : "3D") << "\n";

  // log file
  (*Global_Structure->logFile) << elements.size() << " elements in current domain\n"
                               << nodes.size() << " nodes in current domain\n";

  // up the level of the logFile
  Global_Structure->logFile->upLevel();

  // attacher un champ de donnees a chaque noeud et initialiser en fonction
  // des donnees materiau
  for (i = 0; i < elements.size(); i++)
  {

    // recuperation de l'element
    pel = elements(i);
#ifdef VERIF_assert
    assert(pel != NULL);
#endif

    // recuperation du materiau
    pmat = pel->material;
#ifdef VERIF_assert
    assert(pmat != NULL);
#endif

    for (j = 0; j < pel->getNumberOfNodes(); j++)
    {

      // recherche du noeud
      pnd = pel->nodes(j);
#ifdef VERIF_assert
      assert(pnd != NULL);
#endif

      // recherche du champ associ� au noeud
      pnd_field = pnd->Current;
      pnd_field->ro = pmat->density();
      pnd->T0 = pmat->initTemp();
      pnd_field->T = pmat->initTemp();

      // ajout de la reference croisee noeud->element
      pnd->addElement(pel);
    }

    (*Global_Structure->logFile) << "Creation of " << (Indice)pel->getNumberOfIntegrationPoints() << " integration points ... ";

    // creer les points d'int�gration les attacher � l'�l�m�nt s'ils n'existent pas d�ja
    if (pel->intPoint.size() == 0)
    {
      for (Indice pt = 0; pt < pel->getNumberOfIntegrationPoints(); pt++)
      {
        IntegrationPoint *pintPt = new IntegrationPoint(pel->getNumberOfDimensions(), pel->getNumberOfNodes());
        pel->addIntegrationPoint(pintPt, pt);
      }
      for (Indice pt = 0; pt < pel->numberOfUnderIntegrationPoints(); pt++)
      {
        UnderIntegrationPoint *pintPt = new UnderIntegrationPoint(pel->getNumberOfDimensions(), pel->getNumberOfNodes());
        pel->addIntegrationPoint(pintPt, pt);
      }
      (*Global_Structure->logFile) << "ok\n";
    }
  }

  // down the level of the logFile
  Global_Structure->logFile->downLevel();

  // Sauvegarde dans le vecteur coords0 des coordonn�es des noeuds � l'origine
  (*Global_Structure->logFile) << "Saving initial coordinates ... ";
  for (i = 0; i < nodes.size(); i++)
  {
    nodes(i)->coords0 = nodes(i)->coords;
  }
  (*Global_Structure->logFile) << "ok\n";

  // application des conditions initiales
  (*Global_Structure->logFile) << "Applying initial conditions ... ";
  for (i = 0; i < nodes.size(); i++)
  {
    if (nodes(i)->boundary != NULL)
      nodes(i)->boundary->applyInitial_V0(nodes(i));
  }
  (*Global_Structure->logFile) << "ok\n";

  // verification des elements par methode interne
  (*Global_Structure->logFile) << "Verification of elements ... ";
  for (i = 0; i < elements.size(); i++)
  {
    elements(i)->check();
  }
  (*Global_Structure->logFile) << "ok\n";

  // affectation des Id des noeuds et �lements
  // compactage des structures
  nodes.compact();
  elements.compact();

  // verification des interfaces
  for (i = 0; i < interfaces.size(); i++)
  {
    (*Global_Structure->logFile) << "Interface " << i << " verification ...\n";
    interfaces(i)->Init();
    interfaces(i)->check();
  }

  // verification des solveurs
  Real sTime = 0;
  if (solvers.size() > 0)
    sTime = solvers(0)->getEndTime();
  for (i = 0; i < solvers.size(); i++)
  {
    // get the solver
    psolver = solvers(i);

    // init the solver
    psolver->initTime();

    // reverse link to this
    psolver->setDomain(this);

    (*Global_Structure->logFile) << "Solver " << i << " initialization ...\n";

    if (i > 0)
    {
      if (sTime != solvers(i)->getStartTime())
      {
        fatalError("Solvers chain", "Solver %d has %lf start-time while previous end at %lf",
                   i, solvers(i)->getStartTime(), sTime);
      }
      sTime = solvers(i)->getEndTime();
    }
  }

  // ajout des motions Lagrangienes pour tous les noeuds non definis
  set = 0;
  for (j = 0; j < nodes.size(); j++)
  {
    if (nodes(j)->motion == NULL)
    {
      lagMotion *lag = new lagMotion;
      lag->attachNode(nodes(j));
      nodes(j)->attachNodeMotion(lag);
      set++;
    }
  }
  cout << set << " Lagrangian nodes created\n";

  // initialisation of time
  if (solvers.size() > 0)
    currentTime = solvers(0)->getStartTime();

  // dimension de la grille
  numberOfDim = elements(0)->getNumberOfDimensions();

  elements.first();
  while ((pel = elements.currentUp()) != NULL)
  {
    pel->computeInternalMatrices(True);
  }

  return (True);
}

//-----------------------------------------------------------------------------
Real Domain::getEndSolveTime()
//-----------------------------------------------------------------------------
{
  return solvers.last()->getEndTime();
}

//-----------------------------------------------------------------------------
Boolean
Domain::solve(Real upTime)
//-----------------------------------------------------------------------------
{
  Indice i;
  Boolean ok = False;

  // test si le fichier historique est ouvert
  if (history_file == NULL)
  {
    String nm;
    if (name == "")
      nm = parsedFileName.before(sourceFileExtension) + ".plot";
    else
      nm = parsedFileName.before(sourceFileExtension) + "_" + name + ".plot";
    history_file = fopen(nm.chars(), "w");

    fprintf(history_file,
            "#DynELA_plot : Time history of computation\n#plotted : Used_timeStep timeStep kinetic_energy\n");
  }

  if (upTime == -1.)
  {
    // end time is far away
    upTime = solvers.last()->getEndTime();
  }
  else
  {
    // check to see if time in bounds
    if (currentTime >= upTime)
    {
      return False;
    }
  }

  // check the right solver
  for (i = 0; i < solvers.size(); i++)
  {
    // get the solver
    currentSolver = solvers(i);
    if (currentSolver->betweenBounds(currentTime))
    {
      ok = True;
      break;
    }
  }

  if (!ok)
  {
    fatalError("No solver available", "to compute at time=%10.3E\nup to time %10.3E", currentTime, upTime);
  }

  currentSolver->solve(upTime);

  mach.recordCharges();
  //  mach.recordRanks();

  return True;
}

//-----------------------------------------------------------------------------
void Domain::transfertQuantities()
//-----------------------------------------------------------------------------
{
  Indice no;

#ifdef PRINT_Execution_Solve
  cout << "Quantities transfert\n";
#endif

  for (no = 0; no < nodes.size(); no++)
  {
    nodes(no)->transfertNodalField();
  }
}

//-----------------------------------------------------------------------------
Real Domain::getTotalMass()
//-----------------------------------------------------------------------------
{
  Indice no;
  Real mass;
  Node *pnd;

  mass = 0.;
  for (no = 0; no < nodes.size(); no++)
  {
    pnd = nodes(no);
    mass += pnd->mass;
  }
  return (mass);
}

//-----------------------------------------------------------------------------
Real Domain::getTotalKineticEnergy()
//-----------------------------------------------------------------------------
{
  Indice no;
  Real energy;
  Node *pnd;

  energy = 0.;
  for (no = 0; no < nodes.size(); no++)
  {
    pnd = nodes(no);
    energy += .5 * pnd->mass * (pnd->New->mat_speed.norm2());
  }
  return (energy);
}

//-----------------------------------------------------------------------------
void Domain::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "Nodes list" << endl;
  os << "Elements list\n";
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, Domain &domain)
//-----------------------------------------------------------------------------
{
  domain.print(os);
  return os;
}

//-----------------------------------------------------------------------------
Real Domain::getReadTimeData(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  Real tim;

  if (Global_Structure->checkBinaryVersion(pfile, 0) != Ok)
    fatalError("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read((char *)&tim, sizeof(Real));

  return tim;
}

//-----------------------------------------------------------------------------
void Domain::readData(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  Indice i;

  if (Global_Structure->checkBinaryVersion(pfile, 0) != Ok)
    fatalError("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read((char *)&currentTime, sizeof(Real));

  for (i = 0; i < nodes.size(); i++)
  {
    pfile >> *(nodes(i));
  }

  if (Global_Structure->checkBinaryVersion(pfile, 1) != Ok)
    fatalError("nodal datas", "Read error");

  for (i = 0; i < elements.size(); i++)
  {
    pfile >> *(elements(i));
  }

  if (Global_Structure->checkBinaryVersion(pfile, 1) != Ok)
    fatalError("element datas", "Read error");
}

//-----------------------------------------------------------------------------
void Domain::writeData(ofstream &pfile)
//-----------------------------------------------------------------------------
{
  Indice i;

  Global_Structure->checkBinaryVersionWrite(pfile, 0);

  // save current time
  pfile.write((char *)&currentTime /*times.time*/, sizeof(Real));

  // save the nodal values
  for (i = 0; i < nodes.size(); i++)
  {
    nodes(i)->write(pfile);
  }

  Global_Structure->checkBinaryVersionWrite(pfile, 1);

  // save the element values
  for (i = 0; i < elements.size(); i++)
  {
    elements(i)->write(pfile);
  }
  Global_Structure->checkBinaryVersionWrite(pfile, 1);
}

//-----------------------------------------------------------------------------
void Domain::getGlobalBox(Vec3D &min, Vec3D &max)
//-----------------------------------------------------------------------------
{
  Vec3D coords;
  Indice i, j;
  // affectation par defaut au commencement
  max = min = nodes(0)->coords;

  // boucle de recherche
  for (i = 1; i < nodes.size(); i++)
  {
    coords = nodes(i)->coords;

    for (j = 0; j < 3; j++)
    {
      if (coords(j) < min(j))
        min(j) = coords(j);
      if (coords(j) > max(j))
        max(j) = coords(j);
    }
  }
}

//-----------------------------------------------------------------------------
void Domain::createNode(Indice num, Real x, Real y, Real z)
//-----------------------------------------------------------------------------
{
  Node *pnd;

  if ((pnd = (nodes).AppN(num)) == NULL)
  {
    // cr�er un nouveau noeud
    pnd = new Node(num);

    // l'ajouter au domaine
    nodes << pnd;
  }
  else
  {
    //      cerr << "Warning: node " << num << " redefined line " << scanned_line <<
    //	endl;
  }

  // chargement des coordonnees
  pnd->coords(0) = x;
  pnd->coords(1) = y;
  pnd->coords(2) = z;
}

//-----------------------------------------------------------------------------
Node *Domain::getNodeByNumber(Indice num)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < nodes.size(); i++)
  {
    if (nodes(i)->number == num)
      return nodes(i);
  }

  return NULL;
}

//-----------------------------------------------------------------------------
Real Domain::getCurrentTime()
//-----------------------------------------------------------------------------
{
  return currentTime;
}

//-----------------------------------------------------------------------------
void Domain::addInterface(Interface *inter)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(inter != NULL);
#endif

  interfaces << inter;
}

//-----------------------------------------------------------------------------
void Domain::writeHistoryFiles()
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < historyFiles.size(); i++)
  {
    historyFiles(i)->save(currentTime);
  }
}

//-----------------------------------------------------------------------------
void Domain::starterWrite(String name)
//-----------------------------------------------------------------------------
{
}

//!Ajout d'un noeud � la grille courante
/*!
  Cette m�thode ajoute un noeud � la grille courante. Cette m�thode effectue des v�rifications de base comme par exemple la pr�sence d'un noeud portant le m�me num�ro que le nouveau noeud dans la liste. En cas de doublon, cette m�thode stoppe le processus de construction et renvoie une erreur.
  \param pnd pointeur sur le nouveau noeud � ajouter � la grille
  \return True si l'ajout du noeud est correct
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean Domain::add(Node *pnd)
//-----------------------------------------------------------------------------
{
  // search if not already in the list
  if (nodes.dichotomySearch(diffNodeNumber, pnd->number) != NULL)
  {
    fatalError("Domain::add", "Node %ld already exists in the node list of this grid\n", pnd->number);
  }

  if ((nodes.size() >= 1) && (pnd->number < nodes.last()->number))
  {
    // add the node to the grid
    nodes << pnd;

    // qsort of the list if not sorted
    nodes.sort(compareNodeNumbers);
  }
  else
  {
    // add the node to the grid
    nodes << pnd;
  }

  // return happy
  return True;
}

//!Ajout d'un �l�ment � la grille courante
/*!
  Cette m�thode ajoute un �l�ment � la grille courante. Cette m�thode effectue des v�rifications de base comme par exemple la pr�sence d'un �l�ment portant le m�me num�ro que le nouvel �l�ment dans la liste. En cas de doublon, cette m�thode stoppe le processus de construction et renvoie une erreur.
  \param pel pointeur sur le nouvel �l�ment � ajouter � la grille
  \return True si l'ajout du noeud est correct
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Boolean Domain::add(Element *pel)
//-----------------------------------------------------------------------------
{
  // search if not already in the list
  //  if ((elements.size()>1) && (pel->number < elements.last()->number))
  if (elements.dichotomySearch(diffElementNumber, pel->number) != NULL)
  {
    fatalError("Domain::add", "Element %ld already exists in the element list of this grid\n", pel->number);
  }

  if ((elements.size() >= 1) && (pel->number < elements.last()->number))
  {
    // add the element to the grid
    elements << pel;

    // qsort of the list if not sorted
    elements.sort(compareElementNumbers);
  }
  else
  {
    // add the element to the grid
    elements << pel;
  }

  // return happy
  return True;
}

//!Calcul du time step de minimal de la grille (Courant)
/*!
  Cette m�thode calcule le time step minimal de la grille � partir de la d�finition de la g�om�trie des �l�ments et de la vitesse de propagation du son dans les �l�ments de la structure. Cette relation est bas�e sur le crit�re de stabilit� de Courant.
  La relation utilis�e pour ce calcul est donn�e par:
  \f[ \Delta t = \Delta t_{crit} \f] avec \f[ \Delta t_{crit}=\frac{l}{C_s} \f] calcul� pour tous les �l�ments de la grille dans laquelle \f$ l \f$ est la longueur caract�ristique de l'�l�ment calcul�e par Element::getLength() et \f$ C_s \f$ est la vitesse de propagation d'une onde longitudinale dans l'�l�ment calcul�e par la m�thode Element::getSoundSpeed().
  \return valeur num�rique de la valeur du timeStep critique de la grille
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
Real Domain::computeCourantTimeStep()
//-----------------------------------------------------------------------------
{
  Real len;
  Real Cs, time, crit;

  // initialisation de la longueur caract�ristique de l'�l�ment 0
  len = elements(0)->getLength();

  // vitesse du son dans l'element 0
  Cs = elements(0)->getSoundSpeed();

  // valeur critique du time step
  crit = len / (Cs);

  for (Indice i = 1; i < elements.size(); i++)
  {

    // longueur caract�ristique de l'�l�ment
    len = elements(i)->getLength();

    // vitesse du son dans l'element
    Cs = elements(i)->getSoundSpeed();

    // valeur critique
    time = len / (Cs);

    // minimum des valeurs
    if (time < crit)
      crit = time;
  }

  return crit;
}

//!Cr�e un �l�ment et l'ajoute � la grille courante
/*!
  Cette m�thode cr�e un �l�ment et l'ajoute � la grille courante.

  Elle cr�e les noeuds de l'�lement en v�rifiant que ces noeuds sont bien pr�sents sur la grille courante. Dans le cas contraire, une erreur est g�n�r�e. Le num�ro de l'�l�ment doit avoir �t� cr�e avec l'�l�ment. Cette m�thode ajoute alors les noeuds en fonction des noeuds pr�sents sur la grille.

  \param pel pointeur sur le nouvel �lement que l'on vient de g�n�rer.
  \param nNodes liste des num�ros de noeuds qui composent l'�l�ment
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Domain::createElement(Element *pel, Indice *nNodes)
//-----------------------------------------------------------------------------
{
  Node *pnd;

  // ajout de l'�l�ment � la grille et v�rification de sa non pr�sence
  add(pel);

  // add the reference to the grid of the element
  pel->domain = this;

  // construction de l'�l�ment et de ses noeuds
  Indice nbOk = 0;

  for (Indice j = 0; j < pel->getNumberOfNodes(); j++)
  {
    if ((pnd = nodes.dichotomySearch(diffNodeNumber, nNodes[j])) != NULL)
    {
      pel->addNode(pnd);
      nNodes[j] = -1;
      nbOk++;
    }
  }

  if (nbOk != pel->getNumberOfNodes())
  {
    for (Indice j = 0; j < pel->getNumberOfNodes(); j++)
    {
      if (nNodes[j] != -1)
        cout << "Node " << nNodes[j] << " doesn't exist in this grid\n";
    }
    fatalError("Domain::createElement", "some nodes not exist in this grid");
  }
}
