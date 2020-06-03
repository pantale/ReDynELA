/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2004                                                *
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
#include <femLibrary.h>
#include <InputDyn.h>

//-----------------------------------------------------------------------------
void Presentation()
//-----------------------------------------------------------------------------
{
  printf("\n%s v %d.%d.%d-%d\n",PROGRAM_NAME,VERSION,RELEASE,UNDER_RELEASE,ALPHA_VERSION);
#ifdef __STDC__
  printf("date: %s\n\n",__DATE__);
#endif	
  printf("PANTALE Olivier\n\n");
  printf("Laboratoire Genie de Production C.M.A.O\n");
  printf("Ecole Nationale d'Ingenieurs de TARBES\n");
  printf("Avenue d'Azereix\n65016 TARBES Cedex\n\n");
}

//-----------------------------------------------------------------------------
void syntaxePrint()
//-----------------------------------------------------------------------------
{
  cout << "DynELA [options] file\n\n";
  cout << "options disponibles:\n";
  cout << "    -help        affichage de cet ecran d'aide\n";
  cout << "    -restart <n> permet de continuer un calcul interrompu en sp�cifiant un num�ro de sauvegarde\n"
          "                 exemple: DynELA run -restart 20 Test (relance Test depuis la sauvegarde 20)\n";
//  cout << "    -times       affichage des temps d'ex�cution des fonctions principales\n";
//  cout << "    -no_history  desactive l'ecriture du fichier de time history\n";
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // r�glage interne
//  Boolean displayExecutionTimesFlag=False;

  // affichage du message de presentation
  Presentation();

  // parsing des lignes d'arguments
//  if (ArgumentsLineParse("times",argc,argv)==True) displayExecutionTimesFlag=True;
//  if (ArgumentsLineParse("no_history",argc,argv)==True) historyFile=False;
  if (ArgumentsLineParse("help",argc,argv)==True) {
    syntaxePrint(); 
    exit(0);
  }

  // comptage des arguments
  if (argc < 2) {
    syntaxePrint();
    exit(-1);
  }

  // recuperation du nom de l'ex�cutable
  String filename=argv[argc-1];

  // cr�er une instance du fichier d'entr�e
  inputDyn = new InputDyn;

  // nom du fichier de donn�es
//  String dyn_name=filename+sourceFileExtension;

  // lire le fichier de donn�es
  parsedFileName=filename+sourceFileExtension;
  parseInputDyn (InputDyn::Solve);
//  Global_Structure->logFile->setVerbosity(2);

  // init solve de la structure
  Global_Structure->initSolve();

  // creating a file recording the pid of the process
  FILE* pfilePid;
  String pid_name="."+filename+pidFileExtension;
  pfilePid=fopen(pid_name.chars(),"w");
  fprintf(pfilePid,"%d\n",getpid());
  fclose(pfilePid);

  // ecriture des fichiers intermediaires
//  Global_Structure->resultFile=new io_Data;
//  String inter_name=filename+".";
//  Global_Structure->resultFile->binaryFile()=True;
//  Global_Structure->resultFile->link(inter_name);
//  Global_Structure->resultFile->link(Global_Structure);
  Global_Structure->resultFile->setMode(Write);
  Global_Structure->resultFile->incrementalFile()=True;

  // restart analyse
  // nouveaut� de la version 0.9.5
  if (ArgumentsLineParse("restart",argc,argv)==True) {
    String tmpStr=ArgumentsLineGet("restart",argc,argv);
    Indice restartInc;
    sscanf(tmpStr.chars(),"%ld",&restartInc);
    cout << "Restarting from previous save number "<<restartInc<<endl;

    // transfert des donn�es
    Global_Structure->resultFile->startCurrentSave(restartInc);
    Global_Structure->resultFile->setMode(Read);
    Global_Structure->resultFile->transfert();

    // remise en mode ecriture
    Global_Structure->resultFile->setMode(Write);
    
    // affichage
    cout << "\nRestarting solve from time "<<Global_Structure->currentTime<<endl;
 //   cout << pdomain->times << endl;
    
    // mise a jour du start time
//    Global_Structure->startTime=pdomain->times.time;

    // calcul de la prochaine sauvegarde
    Global_Structure->startSaveTime=Global_Structure->currentTime;
    Global_Structure->nextSaveTime=(restartInc+1)*Global_Structure->saveTime;
  }  

  // lancement du solveur g�n�ral
  Global_Structure->solve();

  Global_Structure->resultFile->transfert();
  
/*
  // creer un domaine
  Domain* pdomain;
  pdomain=new Domain;
  pdomain->typeOfPreprocessing=preprocessingSingle;

  if (historyFile==True) {
    String plot_name=filename+plotFileExtension;
    pdomain->history_file=fopen(plot_name.chars(),"w");	
  }

  // creer input Data
  pdomain->io_data=new InputData;
  String dyn_name=filename+sourceFileExtension;
  pdomain->io_data->link(dyn_name);
  pdomain->io_data->link(pdomain);
  pdomain->io_data->setMode(Read);
  pdomain->io_data->transfert();
  
  // initialisation des donnees aux noeuds
  pdomain->initSolve();

  // initialisation du fichier sta
  String sta_name=filename+statusFileExtension;
  pdomain->starterWrite(sta_name);

  // scan des interfaces
  pdomain->initInterfaces();
  
  // celle la on n'y touche pas
  pdomain->times.endSaveTime=pdomain->times.endTime;
  
  // ecriture des fichiers intermediaires
  pdomain->io_result=new io_Data;
  String inter_name=filename+".";
  pdomain->io_result->binaryFile()=True;
  pdomain->io_result->link(inter_name);
  pdomain->io_result->link(pdomain);
  pdomain->io_result->setMode(Write);
  pdomain->io_result->incrementalFile()=True;
  
  // restart analyse
  // nouveaut� de la version 0.9.5
  if (ArgumentsLineParse("restart",argc,argv)==True) {
    String tmpStr=ArgumentsLineGet("restart",argc,argv);
    Indice restartInc;
    sscanf(tmpStr.chars(),"%ld",&restartInc);
    cout << "Restarting from previous save number "<<restartInc<<endl;

    // transfert des donn�es
    pdomain->io_result->startCurrentSave(restartInc);
    pdomain->io_result->setMode(Read);
    pdomain->io_result->transfert();

    // remise en mode ecriture
    pdomain->io_result->setMode(Write);
    
    // affichage
    cout << "Restarting solve from time "<<pdomain->times.time<<endl;
 //   cout << pdomain->times << endl;
    
    // mise a jour du start time
    pdomain->times.startTime=pdomain->times.time;

    // calcul de la prochaine sauvegarde
    pdomain->times.startSaveTime=(restartInc+1)*pdomain->times.saveTime;

//    cout << pdomain->times << endl;
//    exit(0);
  }  


  // creating a file recording the pid of the process
  FILE* pfilePid;
  String pid_name="."+filename+pidFileExtension;
  pfilePid=fopen(pid_name.chars(),"w");
  fprintf(pfilePid,"%d\n",getpid());
  fclose(pfilePid);

  // Solve the problem
  pdomain->solve();
  
  // fin de calcul
  // affichage
  cout << "\nEnd of computation ...\n";
  cout << pdomain->times << endl;

  pdomain->io_result->transfert();

  // ecriture finale
  pdomain->io_final = new io_Data;
  String final_name=filename;
  pdomain->io_final->link(final_name);
  pdomain->io_final->link(pdomain);
  pdomain->io_final->setMode(Write);
  pdomain->io_final->transfert();

  // fermeture du fichier history
  if (historyFile==True)  fclose (pdomain->history_file);

  // affichage des temps d'execution
  if (displayExecutionTimesFlag==True) displayExecutionTimes();
*/

  // fin de programme
  return (0);
}
