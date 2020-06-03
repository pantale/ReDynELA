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

// begin date : 12/03/1997

/*
  Class Domain definition
*/

#ifndef __Domain_h__
#define __Domain_h__

/*!
  \file Domain.h
  \brief fichier .h de d�finition des domaines �l�ments finis
  \ingroup femSolver

  Ce fichier sert � la d�finition de la classe Domain.

  \author Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/

#include <upFemSolver.h>
class Domain;
class Node;
class Element;
class Material;
class Interface;
class Times;
class HistoryFile;
class Solver;

/*!
  \class Domain Domain.h
  \brief Classe de gestion et manipulation des domaines �l�ments finis.
  \ingroup femLibrary


  \author Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/
/** @dia:pos 199.8,6.8 */
class Domain
{

private:
  Indice numberOfDim;   //!< Dimension associ�e au domaine
  Boolean massComputed; //!< Flag indiquant que la matrice de masse est d�ja calcul�e
  Vector EV_TimeStep;
  Real fmax0;

public:
  String name;                      //!< Nom du domaine s'il existe
  MatrixDiag Mass;                  //!< Matrice de masse
  Vector Fint;                      //!< Vecteur des forces internes
  Real currentTime;                 //!< Temps actuel du domaine
  Real nextTime;                    //!< Temps actuel du domaine + increment de temps
  List<Solver *> solvers;           //!<liste des solveurs associ�s au domaine
  List<HistoryFile *> historyFiles; //!<liste des history files
  List<Interface *> interfaces;     //!< Liste des interfaces de contact du domaine
  Solver *currentSolver;            //!<Solveur courant associ� au domaine
  FILE *history_file;               //!< Fichier historique associ� au domaine
  ListId<Node *> nodes;             //!< Liste des noeuds de la grille
  ListId<Element *> elements;       //!< Liste des �l�ments de la grille
  Indice dimension;                 //!< Nombre de dimensions de la grille

  // constructeurs
  Domain();
  Domain(const Domain &dom);
  ~Domain();

  // fonctions membres
  Indice &NoDim();
  Boolean initSolve();
  Real getMaximumFrequency();
  void computeMassMatrix(Boolean force = False);

  // gestion du temps
  Real getTime();
  void updateTime();

  // interfaces
  void addInterface(Interface *inter);
  // fonctions de calculs
  Boolean solve(Real upTime = -1.);
  void computeInternalMatrices();
  void computeInternalForces();

  void transfertQuantities();
  Real getTotalMass();
  Real getTotalKineticEnergy();
  void starterWrite(String name);
  Real getCurrentTime();
  Real getTimeStep();
  Real getEndSolveTime();
  void writeHistoryFiles();
  void getGlobalBox(Vec3D &min, Vec3D &max);

  // fonctions entree sortie
  void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, Domain &dom);
  void readData(ifstream &pfile);
  Real getReadTimeData(ifstream &pfile);
  void writeData(ofstream &pfile);

  // m�thodes li�es � la lecture des fichiers
  void createNode(Indice, Real, Real, Real);
  Node *getNodeByNumber(Indice);
  Boolean add(Node *pnd);
  Boolean add(Element *pel);
  void createElement(Element *pel, Indice *nNodes);
  Real computeCourantTimeStep();

  // direct implementation
  Indice getNumberOfDimensions() { return dimension; } //!< Dimension topologique de la grille
};

//-----------------------------------------------------------------------------
inline Indice &
Domain::NoDim()
//-----------------------------------------------------------------------------
{
  return numberOfDim;
}

#endif
