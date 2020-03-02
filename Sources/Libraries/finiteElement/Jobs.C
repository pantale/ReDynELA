
#ifdef _OPENMP

#include <Jobs.h>
#include <Element.h>
#include <Node.h>

//-----------------------------------------------------------------------------
Job::Job(Jobs *m)
//-----------------------------------------------------------------------------
{
  mach = m;
  delay = 0;
}

//-----------------------------------------------------------------------------
Job::~Job()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Job::waitOthers()
//-----------------------------------------------------------------------------
{
  //delay = 0;
  while (!mach->sync)
#pragma omp atomic
    delay++;
  //#pragma omp critical
  //  cout << id << "->"<<delay << endl;
}

// //-----------------------------------------------------------------------------
// void Job::stopCPU()
// //-----------------------------------------------------------------------------
// {
//   tms now;
//   times (&now);
//   delay=now.tms_utime-time.tms_utime;
// }

//-----------------------------------------------------------------------------
Jobs::Jobs()
//-----------------------------------------------------------------------------
{
  // construct the list of processors
  for (Indice i = 0; i < omp_get_max_threads(); i++)
  {
    Job *proc = new Job(this);
    procs << proc;
    proc->id = i;
  }

  // init the rank table
  rank = new int[omp_get_max_threads()];
  ind = rank;

  pfileCharges = fopen("PROC", "w");
  pfileRanks = fopen("RANK", "w");
  loopC = loopR = 0;

  equilibrateFrequency = 10;
  iteration = 0;

  faster = slower = NULL;

  init = False;
}

//-----------------------------------------------------------------------------
void Jobs::forceJobsNumber(int pr)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < procs.size(); i++)
  {
    delete procs(i);
  }
  procs.flush();
  delete[] rank;

  for (Indice i = 0; i < pr; i++)
  {
    Job *proc = new Job(this);
    procs << proc;
    proc->id = i;
  }

  // init the rank table
  rank = new int[omp_get_max_threads()];
  ind = rank;

  // mapInitial();
}

//-----------------------------------------------------------------------------
Jobs::~Jobs()
//-----------------------------------------------------------------------------
{
  fclose(pfileCharges);
  fclose(pfileRanks);
}

Boolean compareElementJobsSort(Element *pel1, Element *pel2)
{
  Vec3D v1, v2;
  Indice i;
  v1 = 0.0;
  v2 = 0.0;

  for (i = 0; i < pel1->nodes.size(); i++)
    v1 += pel1->nodes(i)->coords;
  for (i = 0; i < pel2->nodes.size(); i++)
    v2 += pel2->nodes(i)->coords;
  v1 /= pel1->nodes.size();
  v2 /= pel2->nodes.size();

  if (v1(1) < v2(1))
    return False;
  if (v1(1) > v2(1))
    return True;
  if (v1(0) < v2(0))
    return False;
  return True;
}

#define REPORT
//-----------------------------------------------------------------------------
void Jobs::mapInitial(List<Element *> lel)
//-----------------------------------------------------------------------------
{
  if (init)
    return;

  List<Element *> lelLoc;

  Indice nb = lel.size();
  Indice i, j, p;

  for (i = 0; i < nb; i++)
    lelLoc << lel(i);
  //   // z sort
  lelLoc.sort(compareElementJobsSort);

  // nombre de processeurs
  Indice np = procs.size();

  // nombre par proc
  Real nppr = ((Real)(nb)) / np;

  for (i = 0; i < nb; i++)
  {
    procs((i + 1) / nppr <= np - 1 ? (i + 1) / nppr : np - 1)->elementsList << lelLoc(i);
  }

  init = True;

#ifdef REPORT
  char st[20];
  sprintf(st, "procs_%06d%s", iteration, resultAsciiFileExtension);

  saveRepartition(st);
#endif
    displayRepartition();
}

//-----------------------------------------------------------------------------
void Jobs::displayRepartition()
//-----------------------------------------------------------------------------
{
  Indice i, j;

  for (i = 0; i < procs.size(); i++)
  {
    cout << "Job " << i << " -> " << procs(i)->elementsList.size() << " elements" << endl;
    cout << procs(i)->elementsList(0)->number;
    for (j = 1; j < procs(i)->elementsList.size(); j++)
      cout << ", " << procs(i)->elementsList(j)->number;
    cout << endl;
  }
}

//-----------------------------------------------------------------------------
void Jobs::displayCPU()
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < procs.size(); i++)
  {
    cout << "Job " << i << " -> " << procs(i)->elementsList.size() << " elements" << endl;
  }
}

//-----------------------------------------------------------------------------
void Jobs::recordCharges()
//-----------------------------------------------------------------------------
{
  fprintf(pfileCharges, "%d ", loopC++);
  for (Indice i = 0; i < procs.size(); i++)
  {
    fprintf(pfileCharges, "%d ", procs(i)->elementsList.size());
  }
  fprintf(pfileCharges, "\n");
  fflush(pfileCharges);
  char st[20];
  sprintf(st, "procs_%06d%s", iteration, resultAsciiFileExtension);

  saveRepartition(st);
}

//-----------------------------------------------------------------------------
void Jobs::recordRanks()
//-----------------------------------------------------------------------------
{
  fprintf(pfileRanks, "%d ", loopR++);
  for (Indice i = 0; i < procs.size(); i++)
  {
    fprintf(pfileRanks, "%d %d ", rank[i], procs(i)->delay);
  }
  fprintf(pfileRanks, "\n");
  fflush(pfileRanks);
}

//-----------------------------------------------------------------------------
Job *Jobs::getJob()
//-----------------------------------------------------------------------------
{
  Job *pr;

#pragma omp critical
  pr = procs(omp_get_thread_num());

  return pr;
}

//-----------------------------------------------------------------------------
void Jobs::resetSync()
//-----------------------------------------------------------------------------
{
  if (omp_get_thread_num() != 0)
    fatalError("Jobs::resetSync", "must be called within a non parallel region");
  ind = rank;
  left = procs.size();
  sync = False;
}

//-----------------------------------------------------------------------------
void Jobs::stopSync()
//-----------------------------------------------------------------------------
{
  *ind++ = omp_get_thread_num();
  if (--left == 0)
    sync = True;
}

//-----------------------------------------------------------------------------
void Jobs::displaySync()
//-----------------------------------------------------------------------------
{
  cout << rank[0];
  for (Indice i = 1; i < procs.size(); i++)
    cout << "," << rank[i];
  cout << endl;
}

//-----------------------------------------------------------------------------
void Jobs::equilibrate()
//-----------------------------------------------------------------------------
{
  if (iteration++ % equilibrateFrequency)
    return;

  if (procs.size() <= 1)
    return;

  Job *fast = procs(rank[0]);
  Job *slow = procs(rank[procs.size() - 1]);

  if (slower == slow)
  {
    Element *pel;

    //cout << fast->delay << endl;
    if (fast->delay > 1000 * equilibrateFrequency)
    {

      // transfert one entity from slow to fast
      if (slow->elementsList.size() != 0)
      {
        // 	      Unix un;
        // 	      if (un.existCurrEnvValue("DYNAMIC"))
        // 		{
        Indice supress = slow->elementsList.size() - 1;
        fast->elementsList << slow->elementsList(supress);
        slow->elementsList.del(supress);
        // 		}
      }
    }
  }
  //   if (iteration % 100) recordRanks();
  // reset the delay of all processors
  for (Indice i = 0; i < procs.size(); i++)
    procs(i)->delay = 0;

  faster = fast;
  slower = slow;
}

//-----------------------------------------------------------------------------
void Jobs::saveRepartition(String name)
//-----------------------------------------------------------------------------
{
  Indice i, j;
  Job *proc;
  Element *pel;
  FILE *pfile;

  pfile = fopen(name.chars(), "w");
  fprintf(pfile, "#Element Real\n");

  for (i = 0; i < procs.size(); i++)
  {
    proc = procs(i);
    proc->first();

    while ((pel = proc->currentUp()) != NULL)
    {
      fprintf(pfile, "%d %d\n", pel->number, i + 1);
    }
  }
  fflush(pfile);
  fclose(pfile);
}

#endif // OPENMP
