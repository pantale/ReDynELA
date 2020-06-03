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

// begin date : 16/07/1997
// revision date : 24/09/2002

/*!
  \file CPUclock.C
  \brief fichier .C de d�finition des mesures de temps CPU
  \ingroup basicTools

  Ce fichier contient les implementation des m�thodes de la classe CPUclock. Cette classe est utilis�e pour mesurer les temps d'ex�cution des routines de calcul en C++ dans un programme. C'est une classe relativement simple qui fournit les informations relatives aux temps d'ex�cution CPU. L'information est obtenue � partir de l'horloge interne de la machine sur laquelle le processus est ex�cut�. L'information fournie par cette classe est pr�cise � la microseconde pr�s.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#include <ctime>
#include <sys/times.h>
#include <CPUclock.h>
#include <Boolean.h>
#include <Error.h>

//-----------------------------------------------------------------------------
CPUclock::CPUclock()
//-----------------------------------------------------------------------------
{
#ifdef _SC_CLK_TCK
    tunits = sysconf(_SC_CLK_TCK);
#elif defined CLK_TCK
    tunits = CLK_TCK;
#else
#warning not defined CLOCKS_PER_SECOND
    tunits = 100;
#endif
    times(&total);
    times(&deb);
    times(&last);
    flag = False;
    on.tms_utime = 0;
    off.tms_utime = 0;
}

//!destructeur par d�faut
/*!
  Ce destructeur ne fait rien.
*/
//-----------------------------------------------------------------------------
CPUclock::~CPUclock()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double CPUclock::getOn()
//-----------------------------------------------------------------------------
{
    return convert2sec((total.tms_utime - deb.tms_utime - off.tms_utime + on.tms_utime) / 2);
}

//-----------------------------------------------------------------------------
double CPUclock::getOff()
//-----------------------------------------------------------------------------
{
    return convert2sec((total.tms_utime - deb.tms_utime - on.tms_utime + off.tms_utime) / 2);
}

//-----------------------------------------------------------------------------
double CPUclock::getTotal()
//-----------------------------------------------------------------------------
{
    return convert2sec(total.tms_utime - deb.tms_utime);
}

//-----------------------------------------------------------------------------
void CPUclock::startTime()
//-----------------------------------------------------------------------------
{
    off.tms_utime -= total.tms_utime;
    times(&total);
    off.tms_utime += total.tms_utime;
    if (flag == True)
    {
        fatalError("False time for start method\n");
    }
    flag = True;
}

//-----------------------------------------------------------------------------
void CPUclock::stopTime(tms now_)
//-----------------------------------------------------------------------------
{
    on.tms_utime -= total.tms_utime;
    times(&total);
    on.tms_utime += total.tms_utime;
    if (flag == False)
    {
        fatalError("False time for stop method\n");
    }
    flag = False;
}

//-----------------------------------------------------------------------------
void CPUclock::swap()
//-----------------------------------------------------------------------------
{
    last.tms_utime = now.tms_utime;
    last.tms_stime = now.tms_stime;
    last.tms_cutime = now.tms_cutime;
    last.tms_cstime = now.tms_cstime;
}

//-----------------------------------------------------------------------------
void CPUclock::subt()
//-----------------------------------------------------------------------------
{
    diff.tms_utime = now.tms_utime - last.tms_utime;    // user time
    diff.tms_stime = now.tms_stime - last.tms_stime;    // system time
    diff.tms_cutime = now.tms_cutime - last.tms_cutime; // user time, children
    diff.tms_cstime = now.tms_cstime - last.tms_cstime; // system time, children
}

//!renvoie le temps d'ex�cution + le temps syst�me
/*!
  Cette m�thode renvoie la valeur du temps d'ex�cution + le temps syst�me depuis le dernier appel � cette m�me m�thode ou � la m�thode initTime(). La valeur retourn�e est exprim�e en secondes.
\return temps d'ex�cution +  temps syst�me en secondes
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
*/
//-----------------------------------------------------------------------------
double
CPUclock::getInterval()
//-----------------------------------------------------------------------------
{
    times(&now);
    subt();
    swap();
    return convert2sec(diff.tms_utime + diff.tms_stime);
}

//-----------------------------------------------------------------------------
void CPUmeasure::startTime()
//-----------------------------------------------------------------------------
{
    if (flag == True)
    {
        fatalError("False time for start method\n");
    }

    flag = True;
    RDTSC(last);
}

//-----------------------------------------------------------------------------
void CPUmeasure::stopTime(unsigned long long now_)
//-----------------------------------------------------------------------------
{
    diff = now_ - last;
    cpuTime += diff;

    if (flag == False)
    {
        fatalError("False time for stop method\n");
    }

    flag = False;
}

//!renvoie le temps d'ex�cution + le temps syst�me
/*!
  Cette m�thode renvoie la valeur du temps d'ex�cution + le temps syst�me depuis le dernier appel � cette m�me m�thode ou � la m�thode initTime(). La valeur retourn�e est exprim�e en secondes.
\return temps d'ex�cution +  temps syst�me en secondes
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
*/
//-----------------------------------------------------------------------------
unsigned long long
CPUmeasure::getInterval()
//-----------------------------------------------------------------------------
{
    RDTSC(now);
    stopTime(now);
    return diff;
}

//!constructeur
/*!
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
CPUmeasure::CPUmeasure(String _name)
//-----------------------------------------------------------------------------
{
    name = _name;
    cpuTime = 0;
    calls = 0;
    run = False;
    flag = False;
    child = False;
    possibleChilds = False;
    master = True;
    cpuTime = 0;
    last = 0;
    diff = 0;
}

//!destructeur
/*!
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
CPUmeasure::~CPUmeasure()
//-----------------------------------------------------------------------------
{
}

//!constructeur
/*!
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
CPUrecord::CPUrecord()
//-----------------------------------------------------------------------------
{
#ifdef _SC_CLK_TCK
    tunits = sysconf(_SC_CLK_TCK);
#elif defined CLK_TCK
    tunits = CLK_TCK;
#else
#warning not defined CLOCKS_PER_SECOND
    tunits = 100;
#endif

    RDTSC(startReference);
    times(&deb);
}

//!destructeur
/*!
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
CPUrecord::~CPUrecord()
//-----------------------------------------------------------------------------
{
    // delete objects
    for (Indice i = 0; i < cpuMeasures.size(); i++)
    {
        delete cpuMeasures(i);
    }

    // flush list
    cpuMeasures.flush();
}

Boolean compareCpuTimes(CPUmeasure *p1, CPUmeasure *p2)
{
    return (p1->cpuTime < p2->cpuTime);
}

Boolean compareNames(CPUmeasure *p1, CPUmeasure *p2)
{
    return (p1->name < p2->name);
}

//-----------------------------------------------------------------------------
void CPUrecord::computeClockReference()
//-----------------------------------------------------------------------------
{
    unsigned long long stopref;
    tms sto;
    RDTSC(stopref);
    times(&sto);

    clockReference = ((double)(stopref - startReference)) / ((sto.tms_utime + sto.tms_cutime - deb.tms_utime - deb.tms_cutime) / double(tunits));
}

Real CPUrecord::getTime(String _name)
{
    Indice i;
    CPUmeasure *cp;

    for (i = 0; i < cpuMeasures.size(); i++)
    {
        cp = cpuMeasures(i);
        if (cp->name == _name)
        {
            return getSeconds(cp);
        }
    }
    return 0;
}

//!sortie du rapport de mesure dans un fichier
/*!
  Cette m�thode �crit le rapport des mesurs CPU dans un fichier texte sp�cifi� par son nom.
  \param _name nom du fichier texte
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void CPUrecord::report(String _name)
//-----------------------------------------------------------------------------
{
    FILE *pfile;
    Real cpuTot = 0;
    Real cpuLocTot;
    Indice i, j;
    List<CPUmeasure *> lcp;
    CPUmeasure *cp;

    computeClockReference();

    pfile = fopen(_name.chars(), "w");
    if (pfile == NULL)
    {
        fatalError("CPUrecord::report", "Unable to open file %s for write mode", _name.chars());
    }

    fprintf(pfile, "DynELA v. %d.%d.%d-%d\n\n", VERSION, RELEASE, UNDER_RELEASE, ALPHA_VERSION);
    fprintf(pfile, "Complete report of CPU times of routines\n\n");

    // create a master cpu list
    lcp.flush();
    for (i = 0; i < cpuMeasures.size(); i++)
    {
        cp = cpuMeasures(i);
        if (cp->master)
        {
            lcp << cp;
        }
    }

    lcp.sort(compareCpuTimes);

    // temps total CPU des process maitres
    for (i = 0; i < lcp.size(); i++)
        if (!lcp(i)->run)
        {
            cpuTot += getSeconds(lcp(i));
        }

    fprintf(pfile, "Total master time reported : %10.3f s\n", cpuTot);
    fprintf(pfile, "\n---------------------------------------------------\n");
    fprintf(pfile, "Master CPU times detail\n");

    for (i = 0; i < lcp.size(); i++)
    {
        fprintf(pfile, "\n%s : %10.3f %%\n", lcp(i)->name.chars(), 100 * getSeconds(lcp(i)) / cpuTot);
        fprintf(pfile, "total calls      : %ld\n", lcp(i)->calls);
        fprintf(pfile, "CPU time         : %lf s\n", getSeconds(lcp(i)));
        fprintf(pfile, "average CPU time : %lf s\n", (lcp(i)->calls != 0 ? getSeconds(lcp(i)) / lcp(i)->calls : 0));
        if (lcp(i)->run)
        {
            fprintf(pfile, "WARNING : Record still running !!!\n  no call to stop routine\n");
        }
    }

    fprintf(pfile, "\n---------------------------------------------------\n");
    fprintf(pfile, "Slaves CPU times detail\n");
    for (i = 0; i < lcp.size(); i++)
    {
        if (lcp(i)->childs.size() != 0)
        {
            cpuLocTot = 0;
            fprintf(pfile, "\n%s : %10.3f %% -> %lf s\n", lcp(i)->name.chars(), 100 * getSeconds(lcp(i)) / cpuTot, getSeconds(lcp(i)));
            lcp(i)->childs.sort(compareCpuTimes);
            for (j = 0; j < lcp(i)->childs.size(); j++)
                if (!lcp(i)->childs(j)->run)
                {
                    cpuLocTot += getSeconds(lcp(i)->childs(j));
                }

            for (j = 0; j < lcp(i)->childs.size(); j++)
            {
                fprintf(pfile, "%s : %10.3f %% local, %10.3f %% total\n", lcp(i)->childs(j)->name.chars(), 100 * getSeconds(lcp(i)->childs(j)) / cpuLocTot, 100 * getSeconds(lcp(i)->childs(j)) / cpuTot);
                fprintf(pfile, "total calls      : %ld\n", lcp(i)->childs(j)->calls);
                fprintf(pfile, "CPU time         : %lf s\n", getSeconds(lcp(i)->childs(j)));
                fprintf(pfile, "average CPU time : %lf s\n", (lcp(i)->childs(j)->calls != 0 ? getSeconds(lcp(i)->childs(j)) / lcp(i)->childs(j)->calls : 0));
            }
        }
    }

    fclose(pfile);

    archive(_name);
}

//-----------------------------------------------------------------------------
void CPUrecord::archive(String _name)
//-----------------------------------------------------------------------------
{
    // creates an archive report
    CPUmeasure *cp;
    FILE *pfile;
    Indice i;
    List<CPUmeasure *> lcp;
    Real ttm;
    Unix un;
    String nn = _name;
    nn += ".archive_new";
    pfile = fopen(nn.chars(), "w");
    String nn2 = _name;
    nn2 += ".archive";
    FILE *pfile2 = fopen(nn2.chars(), "r");
    String ll;

    // create a master cpu list
    lcp.flush();
    for (i = 0; i < cpuMeasures.size(); i++)
    {
        cp = cpuMeasures(i);
        if (cp->master)
        {
            lcp << cp;
        }
    }

    lcp.sort(compareNames);
    fprintf(pfile, "#DynELA_plot : Time history of computation\n");
    fprintf(pfile, "#plotted :");
    for (i = 0; i < lcp.size(); i++)
    {
        fprintf(pfile, "%s ", lcp(i)->name.chars());
    }
    fprintf(pfile, "total clockRef");
    fprintf(pfile, "\n");

    // copy old
    if (pfile2 != NULL)
    {
        ll.scanFileLine(pfile2);
        ll.scanFileLine(pfile2);
        while (ll.scanFileLine(pfile2))
        {
            fprintf(pfile, "%s\n", ll.chars());
        }
    }

    if (un.existCurrEnvValue("OMP_NUM_THREADS"))
    {
        fprintf(pfile, "%s ", un.getCurrEnvValue("OMP_NUM_THREADS").chars());
    }
    else
    {
        fprintf(pfile, "%s ", un.getCurrDate().before("\n").chars());
    }

    ttm = 0;
    for (i = 0; i < lcp.size(); i++)
    {
        fprintf(pfile, "%10.7E ", getSeconds(lcp(i)));
        ttm += getSeconds(lcp(i));
    }
    fprintf(pfile, "%10.7E %10.7E", ttm, clockReference);
    fprintf(pfile, "\n");

    fclose(pfile);
    if (pfile2 != NULL)
    {
        fclose(pfile2);
    }

    rename(nn.chars(), nn2.chars());

    lcp.flush();
}

//!d�part de la mesure sur un canal donn�
/*!
  Cette m�thode lance la mesure du temps CPU sur un canal donn�, et incr�mente automatiquement le nombre d'appels � la routine.
  \param _name nom du canal associ�
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void CPUrecord::start(String _name)
//-----------------------------------------------------------------------------
{
    Boolean ok = False;
    CPUmeasure *p = NULL;
    Indice i, j;

    // list all started cpumeasures

    for (i = 0; i < cpuMeasures.size(); i++)
    {
        if (cpuMeasures(i)->name == _name)
        {
            ok = True;
            p = cpuMeasures(i);
            i = cpuMeasures.size();
        }
    }

    if (!ok)
    {
        p = new CPUmeasure(_name);
        cpuMeasures << p;
    }

#ifdef VERIF_assert
    assert(p != NULL);
#endif

    // start the counters
    p->calls++;
    p->run = True;

    // ajouter celui-ci � la liste de ceux qui tournent en provisoire
    for (i = 0; i < cpuMeasuresRunning.size(); i++)
    {
        ok = False;
        for (j = 0; j < cpuMeasuresRunning(i)->childs.size(); j++)
        {
            if (cpuMeasuresRunning(i)->childs(j) == p)
            {
                ok = True;
                break;
            }
        }
        if (!ok)
        {
            cpuMeasuresRunning(i)->childs << p;
            cpuMeasuresRunning(i)->possibleChilds = True;
        }
    }

    // run this one
    cpuMeasuresRunning << p;

    // start the counter
    p->startTime();
}

//!arret de la mesure sur un canal donn�
/*!
  Cette m�thode arr�te la mesure du temps CPU sur un canal donn�, et incr�mente automatiquement le temps total CPU.
  \param _name nom du canal associ�
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void CPUrecord::stop(String _name)
//-----------------------------------------------------------------------------
{
    // catch the final time
    unsigned long long now;
    RDTSC(now);

    Boolean ok = False;
    CPUmeasure *p = NULL;
    Indice i, j;

    for (i = 0; i < cpuMeasuresRunning.size(); i++)
    {
        p = cpuMeasuresRunning(i);
        if (p->name == _name)
        {
            ok = True;

            // stop the cpu measure
            p->stopTime(now);
            p->run = False;

            // delete the running occurence
            cpuMeasuresRunning.del(i);

            // boucler sur la liste des fils et verifier s'ils tournent toujours
            if (p->possibleChilds)
                for (j = 0; j < p->childs.size(); j++)
                {
                    // le fils est arret�, c'est un vrai fils
                    if (p->childs(j)->run == False)
                    {
                        p->childs(j)->child = True;
                        p->childs(j)->master = False;
                        p->possibleChilds = False;
                        for (Indice k = 0; k < p->childs.size(); k++)
                            if (p->childs(k)->child == False)
                            {
                                p->possibleChilds = True;
                            }
                    }
                }

            break;
        }
    }
    if (!ok)
    {
        fatalError("CPUrecord::start", "%s not running", _name.chars());
    }
}

//-----------------------------------------------------------------------------
Real CPUrecord::getSeconds(CPUmeasure *mes)
//-----------------------------------------------------------------------------
{
    return (mes->cpuTime / clockReference);
}
