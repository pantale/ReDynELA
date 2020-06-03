/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2016                                                *
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

// begin date : 13/03/1997
// revision date : 24/09/2002

/*
  Class CpuClock definition
*/

#ifndef __CPUclock_h__
#define __CPUclock_h__

#include <Const.h>
/* #include <time.h> */
/* #include <bits/time.h> */
#include <unistd.h>
#include <sys/times.h>
#include <String.h>
#include <List.h>
#include <Unix.h>

/*!
  \file CPUclock.h
  \brief fichier .h de d�finition des mesures de temps CPU
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2016
  \ingroup basicTools
*/

#define RDTSC(X) __asm__ __volatile__("rdtsc" \
                                      : "=A"(X))

/*!
  \class CPUclock CPUclock.h
  \brief classe de mesure de temps d'ex�cution CPU
  \ingroup basicTools

  Cette classe est utilis�e pour mesurer les temps d'ex�cution des routines de calcul en C++ dans un programme. C'est une classe relativement simple qui fournit les informations relatives aux temps d'ex�cution CPU. L'information est obtenue � partir de l'horloge interne de la machine sur laquelle le processus est ex�cut�. L'information fournie par cette classe est pr�cise � la microseconde pr�s.

Les valeurs de temps d'ex�cution fournies au niveau de cette classe sont des valeur de temps CPU, c'est � dire le temps r�el utilis� par le programme. L'ex�cution similtan�e de plusieurs application n'a pas d'influence sur le r�sultat issu des m�thodes de cette classe.

Une utilisation simple est illustr�e par le morceau de code C++ ci-dessous:

Example:
\code
CPUclock timer;                               // cr�e une instance de la classe CPUclock
timer.initTime();                             // initialise la mesure de temps
........                                      // ici on place l'appel des diff�rentes m�thodes dont on veut mesurer le temps d'ex�cution
cout << "time : "<<timer.getInterval()<<endl; // et on r�cup�re le temps CPU �coul� depuis le dernier appel.
\endcode

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2016
*/
/** @dia:pos 110,24 */
class CPUclock
{
  long tunits; // number of timing units per second for struct tms

  tms last; // previous clock value
  tms now;  // present clock value
  tms diff; // diff = now - last
  tms total;
  tms deb;
  tms on;
  tms off;
  Boolean flag;

  void swap(); // last = now
  void subt(); // subtract: now - last, store in diff

  double convert2sec(clock_t tm)
  { // convert to seconds
    return double(tm) / double(tunits);
  }

public:
  CPUclock();
  ~CPUclock();

  double getOn();
  double getOff();
  double getTotal();
  void startTime();
  void stopTime(tms now_);
  double getInterval(); // returns user time
};

/*!
  \class CPUmeasure CPUclock.h
  \brief classe d'enregistrement de temps d'ex�cution CPU
  \ingroup basicTools

  Cette classe est utilis�e par la classe CPUrecord.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2016
*/
/** @dia:pos 28,44 */
class CPUmeasure
{

  friend class CPUrecord;
  friend Boolean compareCpuTimes(CPUmeasure *p1, CPUmeasure *p2);
  friend Boolean compareNames(CPUmeasure *p1, CPUmeasure *p2);

  unsigned long long last; // previous clock value
  unsigned long long now;  // present clock value
  unsigned long long diff; // diff = now - last
  Boolean flag;
  /** @dia:route 4,82;h,42.15,44.7,48,39.9,54 */
  String name;
  unsigned long long cpuTime;
  Indice calls;
  Boolean run;
  List<CPUmeasure *> childs;
  Boolean child;
  Boolean possibleChilds;
  Boolean master;

public:
  CPUmeasure(String _name);
  ~CPUmeasure();
  void startTime();
  void stopTime(unsigned long long now);
  unsigned long long getInterval(); // returns user time
  unsigned long long getTotal()
  {
    return cpuTime; // returns user time
  }
};

/*!
  \class CPUrecord CPUclock.h
  \brief classe d'enregistrement de temps d'ex�cution CPU de plusieurs routines
  \ingroup basicTools

  Cette classe est utilis�e pour mesurer les temps d'ex�cution des routines de calcul en C++ dans un programme. Elle se compose d'un enregistrement multi-pistes permettant de mesurer les temps CPU de plusieurs routines de calcul en parall�le. L'information est obtenue � partir de l'horloge interne de la machine sur laquelle le processus est ex�cut�. L'information fournie par cette classe est pr�cise � la microseconde pr�s.

  Les valeurs de temps d'ex�cution fournies au niveau de cette classe sont des valeur de temps CPU, c'est � dire le temps r�el utilis� par le programme. L'ex�cution similtan�e de plusieurs application n'a pas d'influence sur le r�sultat issu des m�thodes de cette classe.

  Une utilisation simple est illustr�e par le morceau de code C++ ci-dessous:

Example:
\code
CPUrecord recordTimes;            // cr�e un enregistrement

recordTimes.start("Mass matrix"); // d�but de routine
........                          // ici on place l'appel des diff�rentes m�thodes dont on veut mesurer le temps d'ex�c
recordTimes.stop("Mass matrix");  // fin de routine

recordTimes.start("DetJ");        // d�but de routine
........                          // ici on place l'appel des diff�rentes m�thodes dont on veut mesurer le temps d'ex�c
recordTimes.stop("DetJ");         // fin de routine

recordTimes.report("CPU-TIMES");  // sortie dans le fichier "CPU-TIMES"
\endcode

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2016
*/
/** @dia:pos 28,62 */
class CPUrecord
{

  /** @dia:route 10,10;h,28,46.7,22,64.7,28 */
  List<CPUmeasure *> cpuMeasures;
  /** @dia:route 14,3;h,28,48.3,24,62.7,28 */
  List<CPUmeasure *> cpuMeasuresRunning;
  Real clockReference;
  unsigned long long startReference;
  tms deb;
  long tunits; // number of timing units per second for struct tms
  Real getSeconds(CPUmeasure *mes);

public:
  CPUrecord();
  ~CPUrecord();
  void report(String);
  void archive(String);
  void start(String);
  void stop(String);
  Real getTime(String);
  void computeClockReference();
};

#endif
