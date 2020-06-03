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
  Class Boundary definition
*/

/*!
  \file Boundary.h
  \brief fichier .h de d�finition des conditions aux limites
  \ingroup femSolver

  Ce fichier sert � la d�finition des conditions aux limites. Il a �t� remis � jour en 2003 pour la passage � la version 1.0.0.

  \author Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/

#ifndef __Boundary_h__
#define __Boundary_h__

#include <upFemSolver.h>

class Function;
class Node;
class Boundary;
class NodeSet;

/*!
  \class BoundaryCondition Boundary.h
  \brief Classe de gestion des conditions aux limites.
  \ingroup femLibrary

  Cette classe sert � la gestion des conditions aux limites sur DynELA. Ces conditions aux limites se classent en deux cat�gories, les conditions aux limites dites constantes (qui peuvent en fait �tre variables) et les conditions aux limites initiales. En fait, les conditions aux limites constantes doivent �tre assimil�es � des conditions aux limites constament appliqu�es aux noeuds au cours du calcul. Mais l'intensit� de la condition limite correspondante n'est pas forc�ment constante dans le temps.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 267.8,107.8 */
class BoundaryCondition
{

protected:
  /** @dia:route 9,33;h,283.35,131.7,288.75,119.7,284.4 */
  List<Boundary *> con_Bc; //!< Liste des conditions aux limites constantes
  /** @dia:route 13,29;h,283.35,133.5,289.75,118.1,284.4 */
  List<Boundary *> ini_Bc; //!< Liste des conditions aux limites initiales

public:
  // constructeurs
  BoundaryCondition();
  BoundaryCondition(const BoundaryCondition &X);
  ~BoundaryCondition();

  void addConstant(Boundary *bc);
  void addInitial(Boundary *bc);
  Boundary *getInitial(Indice i);
  Boolean applyInitial_V0(Node *node);
  Boundary *getConstant(Indice i);
  Boolean applyConstant_V0(Node *node);
  Boolean applyConstant(Node *node);
  Indice getNumberOfConstants();
  Indice getNumberOfInitials();
  Boolean isConstant();
  Boolean isInitial();
  Boundary *isConstantCondition(Indice value);
  Boundary *isInitialCondition(Indice value);
};

/*!
  \class Boundary Boundary.h
  \brief Classe virtuelle de d�finition des conditions aux limites de tout type.
  \ingroup femLibrary

  Cette classe sert de classe de base pour tous les types de conditions aux limites sur DynELA.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 267.8,129.8 */
class Boundary
{
public:
  enum
  {
    Speed,
    Acceleration,
    Displacement,
    Restrain,
    Force,
    Temperature,
    Flux,
    Convection
  };

protected:
  /** @dia:route 0,2;h,110.735,57.5206,-106.685,-224.4,112.5 */
  Function *function; //!< Fonction discr�te associ�e si elle existe

public:
  /** @dia:route 8,15;h,267.8,131.7,255.75,142.85,205.75,124.029,200.365 */
  List<NodeSet *> nodeSet;
  List<Boolean> typeSet;

public:
  // constructeurs
  Boundary();
  Boundary(const Boundary &X);
  virtual ~Boundary();
  virtual Indice Type() = 0;

  // fonctions membres
  virtual void applyInitial_V0(Node *node) = 0;
  virtual void applyConstant_V0(Node *node) = 0;
  virtual void applyConstant(Node *node) { fatalError("Pas prevu encore", "applyConstant %c", &node); }
  Function *getFunction();
  void setFunction(Function *df);
};

/*!
  \class BoundarySpeed Boundary.h
  \brief Classe de d�finition des conditions aux limites en vitesses.
  \ingroup femLibrary

  Cette classe sert � la gestion des conditions aux limites de type vitesse sur les noeuds. Ces conditions aux limites se classent en deux cat�gories, les conditions aux limites dites constantes (qui peuvent en fait �tre variables) et les conditions aux limites initiales. En fait, les conditions aux limites constantes doivent �tre assimil�es � des conditions aux limites constament appliqu�es aux noeuds au cours du calcul. Mais l'intensit� de la condition limite correspondante n'est pas forc�ment constante dans le temps.

  Dans le cas pr�sent, on peut facilement cr�er une condition limite variable dans le temps en associant � cette condition limite une fonction discr�te d�finie � partie d'une instance de la classe Function. Cette fonction discr�te sert alors � donner une intensit� globale au vecteur (au travers de la valeur sur l'axe y) d�finie en fonction du temps. Voir manuel utilisateur.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 267.8,167.8 */
/** @dia:route Boundary;v,275.575,140.4,164.05,274.7,167.8 */
class BoundarySpeed : public Boundary
{

private:
  /** @dia:route 0,2;h,110.735,95.5206,-106.685,-168.35,165.05 */
  Vec3D speed; //!< Vecteur contenant la vitesse du point

public:
  // constructeurs
  BoundarySpeed();
  BoundarySpeed(const Vec3D &sp);
  BoundarySpeed(const BoundarySpeed &X);
  ~BoundarySpeed();
  void applyConstant_V0(Node *node);
  void applyConstant(Node *node);
  void applyInitial_V0(Node *node);
  Vec3D get();
  void set(const Vec3D &sp);
  void set(Real x, Real y, Real z);

  // fonctions membres
  Indice Type()
  {
    return Boundary::Speed;
  }
};

/*!
  \class BoundaryAcceleration Boundary.h
  \brief Classe de d�finition des conditions aux limites en acc�l�ration.
  \ingroup femLibrary

  Cette classe sert � la gestion des conditions aux limites de type acc�l�ration sur les noeuds. Ces conditions aux limites se classent en deux cat�gories, les conditions aux limites dites constantes (qui peuvent en fait �tre variables) et les conditions aux limites initiales. En fait, les conditions aux limites constantes doivent �tre assimil�es � des conditions aux limites constament appliqu�es aux noeuds au cours du calcul. Mais l'intensit� de la condition limite correspondante n'est pas forc�ment constante dans le temps.

  Dans le cas pr�sent, on peut facilement cr�er une condition limite variable dans le temps en associant � cette condition limite une fonction discr�te d�finie � partie d'une instance de la classe Function. Cette fonction discr�te sert alors � donner une intensit� globale au vecteur (au travers de la valeur sur l'axe y) d�finie en fonction du temps. Voir manuel utilisateur.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 231.8,150.8 */
/** @dia:route Boundary;v,275.575,140.4,146.05,241.15,150.8 */
class BoundaryAcceleration : public Boundary
{

private:
  /** @dia:route 0,2;h,74.7353,78.5206,-70.6853,-168.35,165.05 */
  Vec3D acc;

public:
  // constructeurs
  BoundaryAcceleration();
  BoundaryAcceleration(const BoundaryAcceleration &X);
  ~BoundaryAcceleration();
  Vec3D get();
  void set(const Vec3D &sp);
  void set(Real x, Real y, Real z);

  // fonctions membres
  Indice Type()
  {
    return Boundary::Acceleration;
  }
  void applyConstant_V0(Node *node) { cout << &node << endl; }
  void applyInitial_V0(Node *node) { cout << &node << endl; }
};

/*!
  \class BoundaryRestrain Boundary.h
  \brief Classe de d�finition des conditions aux limites en d�placement.
  \ingroup femLibrary

  Cette classe sert � la gestion des conditions aux limites de type d�placement sur les noeuds. Ces conditions aux limites se classent en deux cat�gories, les conditions aux limites dites constantes (qui peuvent en fait �tre variables) et les conditions aux limites initiales. En fait, les conditions aux limites constantes doivent �tre assimil�es � des conditions aux limites constament appliqu�es aux noeuds au cours du calcul. Mais l'intensit� de la condition limite correspondante n'est pas forc�ment constante dans le temps.

  Dans le cas pr�sent, on peut facilement cr�er une condition limite variable dans le temps en associant � cette condition limite une fonction discr�te d�finie � partie d'une instance de la classe Function. Cette fonction discr�te sert alors � donner une intensit� globale au vecteur (au travers de la valeur sur l'axe y) d�finie en fonction du temps. Voir manuel utilisateur.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 294.8,150.8 */
/** @dia:route Boundary;v,275.575,140.4,146.05,302.75,150.8 */
class BoundaryRestrain : public Boundary
{

private:
  /** @dia:route 0,2;h,137.735,78.5206,-133.685,-168.35,165.05 */
  Vec3D disp;

public:
  // constructeurs
  BoundaryRestrain();
  BoundaryRestrain(const Vec3D &sp);

  BoundaryRestrain(const BoundaryRestrain &X);
  ~BoundaryRestrain();
  void applyConstant(Node *node);
  void applyConstant_V0(Node *node);
  void applyInitial_V0(Node *node);
  Vec3D get();
  void set(const Vec3D &sp);
  void set(Real x, Real y, Real z);

  // fonctions membres
  Indice Type()
  {
    return Boundary::Restrain;
  }
};

/*!
  \class BoundaryDisplacement Boundary.h
  \brief Classe de d�finition des conditions aux limites en d�placement.
  \ingroup femLibrary

  Cette classe sert � la gestion des conditions aux limites de type d�placement sur les noeuds. Ces conditions aux limites se classent en deux cat�gories, les conditions aux limites dites constantes (qui peuvent en fait �tre variables) et les conditions aux limites initiales. En fait, les conditions aux limites constantes doivent �tre assimil�es � des conditions aux limites constament appliqu�es aux noeuds au cours du calcul. Mais l'intensit� de la condition limite correspondante n'est pas forc�ment constante dans le temps.

  Dans le cas pr�sent, on peut facilement cr�er une condition limite variable dans le temps en associant � cette condition limite une fonction discr�te d�finie � partie d'une instance de la classe Function. Cette fonction discr�te sert alors � donner une intensit� globale au vecteur (au travers de la valeur sur l'axe y) d�finie en fonction du temps. Voir manuel utilisateur.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 253.8,150.8 */
/** @dia:route Boundary;v,275.575,140.4,146.05,263.15,150.8 */
class BoundaryDisplacement : public Boundary
{

private:
  /** @dia:route 0,2;h,96.7353,78.5206,-92.6853,-168.35,165.05 */
  Vec3D disp;

public:
  // constructeurs
  BoundaryDisplacement();
  BoundaryDisplacement(const Vec3D &sp);

  BoundaryDisplacement(const BoundaryDisplacement &X);
  ~BoundaryDisplacement();
  void applyConstant(Node *node);
  void applyConstant_V0(Node *node);
  void applyInitial_V0(Node *node);
  Vec3D get();
  void set(const Vec3D &sp);
  void set(Real x, Real y, Real z);

  // fonctions membres
  Indice Type()
  {
    return Boundary::Displacement;
  }
};

/*!
  \class BoundaryForce Boundary.h
  \brief Classe de d�finition des conditions aux limites en force.
  \ingroup femLibrary

  Cette classe sert � la gestion des conditions aux limites de type force sur les noeuds. Ces conditions aux limites se classent en deux cat�gories, les conditions aux limites dites constantes (qui peuvent en fait �tre variables) et les conditions aux limites initiales. En fait, les conditions aux limites constantes doivent �tre assimil�es � des conditions aux limites constament appliqu�es aux noeuds au cours du calcul. Mais l'intensit� de la condition limite correspondante n'est pas forc�ment constante dans le temps.

  Dans le cas pr�sent, on peut facilement cr�er une condition limite variable dans le temps en associant � cette condition limite une fonction discr�te d�finie � partie d'une instance de la classe Function. Cette fonction discr�te sert alors � donner une intensit� globale au vecteur (au travers de la valeur sur l'axe y) d�finie en fonction du temps. Voir manuel utilisateur.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 284.8,167.8 */
/** @dia:route Boundary;v,275.575,140.4,164.05,291.7,167.8 */
class BoundaryForce : public Boundary
{

private:
  /** @dia:route 0,2;h,127.735,95.5206,-123.685,-168.35,165.05 */
  Vec3D force;

public:
  // constructeurs
  BoundaryForce();
  BoundaryForce(const Vec3D &fr);
  BoundaryForce(const BoundaryForce &X);
  ~BoundaryForce();

  // fonctions membres
  Indice Type()
  {
    return Boundary::Force;
  }
  void applyConstant_V0(Node *node);
  void applyInitial_V0(Node *node);
  Vec3D get();
  void set(const Vec3D &sp);
  void set(Real x, Real y, Real z);
};

/*!
  \class BoundaryTemperature Boundary.h
  \brief Classe de d�finition des conditions aux limites en temp�rature.
  \ingroup femLibrary

  Cette classe sert � la gestion des conditions aux limites de type temp�rature sur les noeuds. Ces conditions aux limites se classent en deux cat�gories, les conditions aux limites dites constantes (qui peuvent en fait �tre variables) et les conditions aux limites initiales. En fait, les conditions aux limites constantes doivent �tre assimil�es � des conditions aux limites constament appliqu�es aux noeuds au cours du calcul. Mais l'intensit� de la condition limite correspondante n'est pas forc�ment constante dans le temps.

  Dans le cas pr�sent, on peut facilement cr�er une condition limite variable dans le temps en associant � cette condition limite une fonction discr�te d�finie � partie d'une instance de la classe Function. Cette fonction discr�te sert alors � donner une intensit� globale au vecteur (au travers de la valeur sur l'axe y) d�finie en fonction du temps. Voir manuel utilisateur.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 225.8,167.8 */
/** @dia:route Boundary;v,275.575,140.4,164.05,234.8,167.8 */
class BoundaryTemperature : public Boundary
{

private:
  Real temperature;

public:
  // constructeurs
  BoundaryTemperature();
  BoundaryTemperature(const Real &fr);
  BoundaryTemperature(const BoundaryTemperature &X);
  ~BoundaryTemperature();

  // fonctions membres
  Indice Type()
  {
    return Boundary::Temperature;
  }
  void applyConstant_V0(Node *node);
  void applyInitial_V0(Node *node);
  Real get();
  void set(const Real &val);
};

/*!
  \class BoundaryConvection Boundary.h
  \brief Classe de d�finition des conditions aux limites de type convection.
  \ingroup femLibrary

  Cette classe sert � la gestion des conditions aux limites de type convection sur les noeuds. Ces conditions aux limites se classent en deux cat�gories, les conditions aux limites dites constantes (qui peuvent en fait �tre variables) et les conditions aux limites initiales. En fait, les conditions aux limites constantes doivent �tre assimil�es � des conditions aux limites constament appliqu�es aux noeuds au cours du calcul. Mais l'intensit� de la condition limite correspondante n'est pas forc�ment constante dans le temps.

  Dans le cas pr�sent, on peut facilement cr�er une condition limite variable dans le temps en associant � cette condition limite une fonction discr�te d�finie � partie d'une instance de la classe Function. Cette fonction discr�te sert alors � donner une intensit� globale au vecteur (au travers de la valeur sur l'axe y) d�finie en fonction du temps. Voir manuel utilisateur.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 246.8,167.8 */
/** @dia:route Boundary;v,275.575,140.4,164.05,255.8,167.8 */
class BoundaryConvection : public Boundary
{

public:
  Real hc;
  Real Tf;

public:
  // constructeurs
  BoundaryConvection();
  BoundaryConvection(const Real hc_, const Real tf_);
  BoundaryConvection(const BoundaryConvection &X);
  ~BoundaryConvection();

  // fonctions membres
  Indice Type() { return Boundary::Convection; }
  void applyConstant_V0(Node *node);
  void applyInitial_V0(Node *node);
  Vec3D get();
  void set(const Vec3D &sp);
  void set(Real x, Real y);
};

/*!
  \class BoundaryFlux Boundary.h
  \brief Classe de d�finition des conditions aux limites de type flux thermique.
  \ingroup femLibrary

  Cette classe sert � la gestion des conditions aux limites de type flux thermique sur les noeuds. Ces conditions aux limites se classent en deux cat�gories, les conditions aux limites dites constantes (qui peuvent en fait �tre variables) et les conditions aux limites initiales. En fait, les conditions aux limites constantes doivent �tre assimil�es � des conditions aux limites constament appliqu�es aux noeuds au cours du calcul. Mais l'intensit� de la condition limite correspondante n'est pas forc�ment constante dans le temps.

  Dans le cas pr�sent, on peut facilement cr�er une condition limite variable dans le temps en associant � cette condition limite une fonction discr�te d�finie � partie d'une instance de la classe Function. Cette fonction discr�te sert alors � donner une intensit� globale au vecteur (au travers de la valeur sur l'axe y) d�finie en fonction du temps. Voir manuel utilisateur.

  \author Olivier PANTALE
  \version 0.9.1
  \date 1997-2004
*/
/** @dia:pos 277.8,150.8 */
/** @dia:route Boundary;v,275.575,140.4,146.05,284.35,150.8 */
class BoundaryFlux : public Boundary
{

private:
  Real flux;

public:
  // constructeurs
  BoundaryFlux();
  BoundaryFlux(const Real &fr);
  BoundaryFlux(const BoundaryFlux &X);
  ~BoundaryFlux();

  // fonctions membres
  Indice Type() { return Boundary::Flux; }
  void applyConstant_V0(Node *node);
  void applyInitial_V0(Node *node);
  Real get();
  void set(const Real &val);
};

#endif
