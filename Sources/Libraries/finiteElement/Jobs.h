#ifdef _OPENMP

#ifndef __Job_h__
#define __Job_h__

#include <omp.h>
#include <upFemSolver.h>
class Element;
class Jobs;

class Job
{
  tms time; //<! time of the processor
  Indice delay;
  friend class Jobs;
  Jobs *mach;
  int id;

public:
  List<Element *> elementsList;
  Job(Jobs *);
  ~Job();
  Element *first() { return elementsList.first(); }
  Element *last() { return elementsList.last(); }
  Element *next() { return elementsList.next(); }
  Element *currentUp() { return elementsList.currentUp(); }
  void waitOthers();
};

class Jobs
{
private:
  friend class Job;
  Boolean init;
  int *ind;
  int *rank;
  FILE *pfileCharges;
  FILE *pfileRanks;
  int loopC;
  int loopR;
  int equilibrateFrequency;
  Indice iteration;
  Job *faster;
  Job *slower;
  Boolean sync;
  int left;

public:
  List<Job *> procs;

public:
  Jobs();
  ~Jobs();
  Indice getNumberOfJobs() { return procs.size(); }
  void forceJobsNumber(int);
  void mapInitial(List<Element *> lel);
  void displayRepartition();
  Job *getJob();
  void displayCPU();
  void recordCharges();
  void recordRanks();
  void resetSync();
  void stopSync();
  void displaySync();
  void equilibrate();
  void setEquilibrateFrequency(Indice freq) { equilibrateFrequency = freq; }
  void saveRepartition(String name);
};

#endif // Job.h
#endif // OPENMP
