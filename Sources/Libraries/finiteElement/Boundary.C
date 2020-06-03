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
  Class Boundary implementation
*/

/*!
  \file Boundary.C
  \brief fichier .C de d�finition des conditions aux limites
  \ingroup femSolver

  Ce fichier sert � la d�finition des conditions aux limites. Il a �t� remis � jour en 2003 pour la passage � la version 1.0.0.

  \author Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/

#include <Function.h>
#include <Node.h>
#include <Boundary.h>
#include <Select.h>
#include <NodalField.h>
#include <Structure.h>
//#include <Physic.h>
#include <Domain.h>
#include <Solver.h>

//!constructeur par d�faut de la classe BoundaryCondition
/*!
  Cette m�thode est le constructeur par d�faut de la classe BoundaryCondition. En pratique, ici, on ne fait rien de plus que les allocations de m�moire n�cessaires.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryCondition::BoundaryCondition()
//-----------------------------------------------------------------------------
{
}

//!constructeur par recopie de la classe BoundaryCondition
/*!
  Cette m�thode est le constructeur par recopie de la classe BoundaryCondition.
  \warning La recopie n'est pas pr�vue ici, cette m�thode g�n�re une erreur d'ex�cution
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryCondition::BoundaryCondition(const BoundaryCondition &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely BoundaryCondition class" << /*(int) */
      &X << endl;
  exit(-1);
}

//!destructeur de la classe BoundaryCondition
/*!
  Cette m�thode est le destructeur de la classe BoundaryCondition.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryCondition::~BoundaryCondition()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Boolean
BoundaryCondition::isConstant()
//-----------------------------------------------------------------------------
{
  return (con_Bc.size() != 0);
}

//-----------------------------------------------------------------------------
Boolean
BoundaryCondition::isInitial()
//-----------------------------------------------------------------------------
{
  return (ini_Bc.size() != 0);
}

//-----------------------------------------------------------------------------
void BoundaryCondition::addConstant(Boundary *bc)
//-----------------------------------------------------------------------------
{
  // verification
#ifdef VERIF_alloc
  if (bc == NULL)
    fatalError("BoundaryCondition::addConstant",
               "Error due to a null pointer assignment");
#endif

  con_Bc << bc;
}

//-----------------------------------------------------------------------------
void BoundaryCondition::addInitial(Boundary *bc)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_alloc
  if (bc == NULL)
    fatalError("BoundaryCondition::addInitial",
               "Error due to a null pointer assignment");
#endif

  ini_Bc << bc;
}

//-----------------------------------------------------------------------------
Boundary *
BoundaryCondition::getInitial(Indice i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
  if (i >= ini_Bc.size())
    fatalError("BoundaryCondition::getInitial", "Out of bounds error");
#endif

  return ini_Bc(i);
}

//-----------------------------------------------------------------------------
Boolean
BoundaryCondition::applyInitial_V0(Node *node)
//-----------------------------------------------------------------------------
{
  Indice i;

  // if no initial condition found return false
  if (!isInitial() && !isConstant())
    return False;

  // apply the initial conditions
  for (i = 0; i < ini_Bc.size(); i++)
    ini_Bc(i)->applyInitial_V0(node);
  for (i = 0; i < con_Bc.size(); i++)
    con_Bc(i)->applyInitial_V0(node);

  // return true
  return True;
}

//-----------------------------------------------------------------------------
Boundary *
BoundaryCondition::getConstant(Indice i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
  if (i >= con_Bc.size())
    fatalError("BoundaryCondition::getconstant", "Out of bounds error");
#endif

  return con_Bc(i);
}

//!Application d'une condition limite Impos�e sur un noeud
/*!
Cette m�thode applique des conditions aux limites impos�es sur un noeud. Les conditions limites impos�es sont de type vitesse, d�placement ou force impos�e.
*/
//-----------------------------------------------------------------------------
Boolean
BoundaryCondition::applyConstant_V0(Node *node)
//-----------------------------------------------------------------------------
{
  // if no constant condition found return false
  if (!isConstant())
    return False;

  // apply the constant conditions
  for (Indice i = 0; i < con_Bc.size(); i++)
  {
    con_Bc(i)->applyConstant_V0(node);
  }

  // return true
  return True;
}
//-----------------------------------------------------------------------------
Boolean
BoundaryCondition::applyConstant(Node *node)
//-----------------------------------------------------------------------------
{
  // if no constant condition found return false
  if (!isConstant())
    return False;

  // apply the constant conditions
  for (Indice i = 0; i < con_Bc.size(); i++)
  {
    con_Bc(i)->applyConstant(node);
  }

  // return true
  return True;
}

//-----------------------------------------------------------------------------
Boundary *
BoundaryCondition::isConstantCondition(Indice value)
//-----------------------------------------------------------------------------
{
  Indice j;
  Boundary *pb;

  if (isConstant())
  {
    for (j = 0; j < getNumberOfConstants(); j++)
    {
      pb = getConstant(j);

      if (pb->Type() == value)
        return pb;
    }
  }
  return NULL;
}

//-----------------------------------------------------------------------------
Boundary *
BoundaryCondition::isInitialCondition(Indice value)
//-----------------------------------------------------------------------------
{
  Indice j;
  Boundary *pb;

  if (isInitial())
  {
    for (j = 0; j < getNumberOfInitials(); j++)
    {
      pb = getInitial(j);

      if (pb->Type() == value)
        return pb;
    }
  }
  return NULL;
}

//-----------------------------------------------------------------------------
Indice
BoundaryCondition::getNumberOfConstants()
//-----------------------------------------------------------------------------
{
  return con_Bc.size();
}

//-----------------------------------------------------------------------------
Indice
BoundaryCondition::getNumberOfInitials()
//-----------------------------------------------------------------------------
{
  return ini_Bc.size();
}

/*********************************************************************************************
 *
 ********************************************************************************************/

//!constructeur par d�faut de la classe Boundary
/*!
  Cette m�thode est le constructeur par d�faut de la classe Boundary. En pratique, ici, on ne fait rien de plus que les allocations de m�moire n�cessaires.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
Boundary::Boundary()
//-----------------------------------------------------------------------------
{
}

//!constructeur par recopie de la classe Boundary
/*!
  Cette m�thode est le constructeur par recopie de la classe Boundary.
  \warning La recopie n'est pas pr�vue ici, cette m�thode g�n�re une erreur d'ex�cution
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
Boundary::Boundary(const Boundary &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely Boundary class" << /*(int) */ &X << endl;
  exit(-1);
}

//!destructeur de la classe Boundary
/*!
  Cette m�thode est le destructeur de la classe Boundary.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
Boundary::~Boundary()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Function *
Boundary::getFunction()
//-----------------------------------------------------------------------------
{
  return function;
}

//-----------------------------------------------------------------------------
void Boundary::setFunction(Function *df)
//-----------------------------------------------------------------------------
{
  function = df;
}

/*********************************************************************************************
 *
 ********************************************************************************************/

//!constructeur par d�faut de la classe BoundarySpeed
/*!
  Cette m�thode est le constructeur par d�faut de la classe BoundarySpeed. En pratique, ici, on ne fait rien de plus que les allocations de m�moire n�cessaires.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundarySpeed::BoundarySpeed()
//-----------------------------------------------------------------------------
{
  // initialize a null discretefunction
  function = NULL;
}

//-----------------------------------------------------------------------------
BoundarySpeed::BoundarySpeed(const Vec3D &sp)
//-----------------------------------------------------------------------------
{
  // initialize a null discretefunction
  function = NULL;

  // stores the prescribed speed
  speed = sp;
}

//!constructeur par recopie de la classe BoundarySpeed
/*!
  Cette m�thode est le constructeur par recopie de la classe BoundarySpeed.
  \warning La recopie n'est pas pr�vue ici, cette m�thode g�n�re une erreur d'ex�cution
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundarySpeed::BoundarySpeed(const BoundarySpeed &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely BoundarySpeed class" << /*(int) */ &X << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
void BoundarySpeed::applyInitial_V0(Node *node)
//-----------------------------------------------------------------------------
{
  // apply the curent speed on the material and grid speeds
  node->Current->mat_speed = speed;
  node->New->mat_speed = speed;
}

//!Application d'une condition limite Impos�e en vitesse sur un noeud
/*!
  Cette m�thode applique des conditions aux limites impos�es en vitesse sur un noeud. Deux cas se pr�sentent selon la nature de la vitesse impos�e (constante ou variable dans le temps).
  Dans le cas o� la vitesse impos�e est constante dans le temps, on applique simplement dans la direction contrainte une acc�l�ration nulle et une vitesse impos�e constante �gale � la vitesse de la CL.
  Dans le cas o� la CL est variable dans le temps, on utilise les relations suivantes \f[ \stackrel{\bullet }{v_{0}}=\frac{\overline{v}(t+\Delta t)-\overline{v}(t)}{\Delta t} \f] et  \f[  v_{0}=\overline{v}(t) \f] relations dans lesquelles  \f$ \overline{v}(t) \f$ repr�sente la valeur de la vitesse impos�e � l'instant t d�finie � partir de la fonction discrete. 
  \param node Noeud sur lequel la condition limite est appliqu�e
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
void BoundarySpeed::applyConstant_V0(Node *node)
//-----------------------------------------------------------------------------
{
  Domain *pdomain;
  pdomain = Global_Structure->domains.current();
  /*  Vec3D v1,v2;

  // vitesse a imposer
  v1(0)=(speed(0)==0. ? 1. : 0.);
  v1(1)=(speed(1)==0. ? 1. : 0.);
  v1(2)=(speed(2)==0. ? 1. : 0.);

  // calcul du produit simple
  v2(0)=v1(0)*node->New->mat_speed(0);
  v2(1)=v1(1)*node->New->mat_speed(1);
  v2(2)=v1(2)*node->New->mat_speed(2);

  // application de la vitesse imposee
  if (function==NULL) {
    node->New->mat_speed=v2+speed;
  }
  else {
    node->New->mat_speed=v2+function->getValue(Global_Domain->times.time)*speed;
  }

  // correction de l'acceleration correspondante
  if (speed(0)!=0.) node->New->dmat_speed(0)=0.;
  if (speed(1)!=0.) node->New->dmat_speed(1)=0.;
  if (speed(2)!=0.) node->New->dmat_speed(2)=0.;

  // calcul du nouveau deplacement compatible avec la vitesse
  node->New->delta_disp=Global_Domain->times.timeStep*node->New->mat_speed;
*/

  // deuxieme methode implementee le 6/7/2000
  // appliquer la composante dans la direction ou speed!=0
  // modifi�e le 7/03/2001 avec la r��criture du sch�ma d'int�gration num�rique

  for (Indice i = 0; i < 3; i++)
  {
    // si on a une composante de vitesse impos�e
    if (speed(i) != 0.)
    {

      // si c'est constant
      if (function == NULL)
      {

        // acceleration nulle
        node->Current->dmat_speed(i) = 0.;

        // vitesse imposee
        node->Current->mat_speed(i) = speed(i);

        // deplacement constant
        //      node->Current->delta_disp(i)=node->Current->mat_speed(i)*Global_Domain->times.timeStep;
      }
      else
      {

        // acceleration non nulle
        node->Current->dmat_speed(i) =
            (function->getValue(pdomain->currentTime +
                                (pdomain->currentSolver->getTimeStep())) -
             function->getValue(pdomain->currentTime)) *
            speed(i) / pdomain->currentSolver->getTimeStep();
        //      node->Current->dmat_speed(i)=(function->getValue(pdomain->times.time+(pdomain->times.timeStep/2.))*speed(i))/pdomain->times.timeStep;

        // vitesse imposee
        //      node->Current->mat_speed(i)=function->getValue(pdomain->times.time+(pdomain->times.timeStep/2.))*speed(i);
        node->Current->mat_speed(i) =
            function->getValue(pdomain->currentTime) *
            speed(i);

        // deplacement constant
        //      node->Current->delta_disp(i)=node->Current->mat_speed(i)*Global_Domain->times.timeStep;
      }
    }
  }
}

//-----------------------------------------------------------------------------
void BoundarySpeed::applyConstant(Node *node)
//-----------------------------------------------------------------------------
{
  Domain *pdomain;
  pdomain = Global_Structure->domains.current();

  for (Indice i = 0; i < 3; i++)
  {
    // si on a une composante de vitesse impos�e
    if (speed(i) != 0.)
    {

      // si c'est constant
      if (function == NULL)
      {

        // acceleration nulle
        node->Current->dmat_speed(i) = 0.;
        node->New->dmat_speed(i) = 0.;

        // vitesse imposee
        node->Current->mat_speed(i) = speed(i);
        node->New->mat_speed(i) = speed(i);

        // deplacement constant
        node->Current->delta_disp(i) = node->Current->mat_speed(i) * pdomain->currentSolver->getTimeStep();
        node->New->delta_disp(i) = node->New->mat_speed(i) * pdomain->currentSolver->getTimeStep();
      }
      else
      {

        // acceleration non nulle
        node->Current->dmat_speed(i) =
            (function->getValue(pdomain->nextTime) - function->getValue(pdomain->currentTime)) * speed(i) / pdomain->currentSolver->getTimeStep();
        node->New->dmat_speed(i) = node->Current->dmat_speed(i);

        // vitesse imposee
        node->Current->mat_speed(i) = function->getValue(pdomain->currentTime) * speed(i);
        node->New->mat_speed(i) = function->getValue(pdomain->nextTime) * speed(i);

        // deplacement constant
        node->Current->delta_disp(i) = node->Current->mat_speed(i) * pdomain->currentSolver->timeStep;
        node->New->delta_disp(i) = node->New->mat_speed(i) * pdomain->currentSolver->timeStep;
      }
    }
  }
}

//!destructeur de la classe BoundarySpeed
/*!
  Cette m�thode est le destructeur de la classe BoundarySpeed.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundarySpeed::~BoundarySpeed()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundarySpeed::get()
//-----------------------------------------------------------------------------
{
  return speed;
}

//-----------------------------------------------------------------------------
void BoundarySpeed::set(const Vec3D &sp)
//-----------------------------------------------------------------------------
{
  speed = sp;
}

//-----------------------------------------------------------------------------
void BoundarySpeed::set(Real x, Real y, Real z)
//-----------------------------------------------------------------------------
{
  speed(0) = x;
  speed(1) = y;
  speed(2) = z;
}

/*********************************************************************************************
 *
 ********************************************************************************************/

//!constructeur par d�faut de la classe BoundaryRestrain
/*!
  Cette m�thode est le constructeur par d�faut de la classe BoundaryRestrain. En pratique, ici, on ne fait rien de plus que les allocations de m�moire n�cessaires.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryRestrain::BoundaryRestrain()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryRestrain::BoundaryRestrain(const Vec3D &_disp)
//-----------------------------------------------------------------------------
{
  disp = _disp;
}

//!constructeur par recopie de la classe BoundaryRestrain
/*!
  Cette m�thode est le constructeur par recopie de la classe BoundaryRestrain.
  \warning La recopie n'est pas pr�vue ici, cette m�thode g�n�re une erreur d'ex�cution
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryRestrain::BoundaryRestrain(const BoundaryRestrain &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely BoundaryRestrain class" << /*(int) */ &X << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
void BoundaryRestrain::applyInitial_V0(Node *node)
//-----------------------------------------------------------------------------
{
}

//!Application d'une condition limite Impos�e en d�placement sur un noeud
/*!
Cette m�thode applique une condition aux limites impos�e en vitesse sur un noeud. Dans la direction de la condition impos�e, la vitesse, le d�placement et l'acc�leration mat�rielle du noeud sont impos�s nuls.
*/
//-----------------------------------------------------------------------------
void BoundaryRestrain::applyConstant_V0(Node *node)
//-----------------------------------------------------------------------------
{
  if (disp(0) == 1.)
  {
    //    node->Current->delta_disp(0)=0.;
    node->Current->mat_speed(0) = 0.;
    node->Current->dmat_speed(0) = 0.;
    node->New->dmat_speed(0) = 0.;
  }
  if (disp(1) == 1.)
  {
    //    node->Current->delta_disp(1)=0.;
    node->Current->mat_speed(1) = 0.;
    node->Current->dmat_speed(1) = 0.;
    node->New->dmat_speed(1) = 0.;
  }
  if (disp(2) == 1.)
  {
    //    node->Current->delta_disp(2)=0.;
    node->Current->mat_speed(2) = 0.;
    node->Current->dmat_speed(2) = 0.;
    node->New->dmat_speed(2) = 0.;
  }
}
//-----------------------------------------------------------------------------
void BoundaryRestrain::applyConstant(Node *node)
//-----------------------------------------------------------------------------
{
  if (disp(0) == 1.)
  {
    node->Current->delta_disp(0) = 0.;
    node->New->delta_disp(0) = 0.;
    node->Current->mat_speed(0) = 0.;
    node->New->mat_speed(0) = 0.;
    node->Current->dmat_speed(0) = 0.;
    node->New->dmat_speed(0) = 0.;
  }
  if (disp(1) == 1.)
  {
    node->Current->delta_disp(1) = 0.;
    node->New->delta_disp(1) = 0.;
    node->Current->mat_speed(1) = 0.;
    node->New->mat_speed(1) = 0.;
    node->Current->dmat_speed(1) = 0.;
    node->New->dmat_speed(1) = 0.;
  }
  if (disp(2) == 1.)
  {
    node->Current->delta_disp(2) = 0.;
    node->New->delta_disp(2) = 0.;
    node->Current->mat_speed(2) = 0.;
    node->New->mat_speed(2) = 0.;
    node->Current->dmat_speed(2) = 0.;
    node->New->dmat_speed(2) = 0.;
  }
}

//!destructeur de la classe BoundaryRestrain
/*!
  Cette m�thode est le destructeur de la classe BoundaryRestrain.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryRestrain::~BoundaryRestrain()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundaryRestrain::get()
//-----------------------------------------------------------------------------
{
  return disp;
}

//-----------------------------------------------------------------------------
void BoundaryRestrain::set(const Vec3D &sp)
//-----------------------------------------------------------------------------
{
  disp = sp;
}

//-----------------------------------------------------------------------------
void BoundaryRestrain::set(Real x, Real y, Real z)
//-----------------------------------------------------------------------------
{
  disp(0) = x;
  disp(1) = y;
  disp(2) = z;
}

/*********************************************************************************************
 *
 ********************************************************************************************/

//!constructeur par d�faut de la classe BoundaryDisplacement
/*!
  Cette m�thode est le constructeur par d�faut de la classe BoundaryDisplacement. En pratique, ici, on ne fait rien de plus que les allocations de m�moire n�cessaires.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryDisplacement::BoundaryDisplacement()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryDisplacement::BoundaryDisplacement(const Vec3D &_disp)
//-----------------------------------------------------------------------------
{
  disp = _disp;
}

//!constructeur par recopie de la classe BoundaryDisplacement
/*!
  Cette m�thode est le constructeur par recopie de la classe BoundaryDisplacement.
  \warning La recopie n'est pas pr�vue ici, cette m�thode g�n�re une erreur d'ex�cution
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryDisplacement::BoundaryDisplacement(const BoundaryDisplacement &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely BoundaryDisplacement class" << /*(int) */ &X << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
void BoundaryDisplacement::applyInitial_V0(Node *node)
//-----------------------------------------------------------------------------
{
}

//!Application d'une condition limite Impos�e en d�placement sur un noeud
/*!
Cette m�thode applique une condition aux limites impos�e en vitesse sur un noeud. Dans la direction de la condition impos�e, la vitesse, le d�placement et l'acc�leration mat�rielle du noeud sont impos�s nuls.
*/
//-----------------------------------------------------------------------------
void BoundaryDisplacement::applyConstant(Node *node)
//-----------------------------------------------------------------------------
{
  fatalError("BoundaryDisplacement::applyConstant", "not yet implemented\n");
  Domain *pdomain;
  pdomain = Global_Structure->domains.current();

  for (Indice i = 0; i < 3; i++)
  {
    // si on a une composante de vitesse impos�e
    if (disp(i) != 0.)
    {

      // si c'est constant
      if (function == NULL)
      {
        // deplacement constant
        node->Current->delta_disp(i) = disp(i);
        node->New->delta_disp(i) = disp(i);

        // vitesse imposee
        node->Current->mat_speed(i) = node->Current->delta_disp(i) / pdomain->currentSolver->getTimeStep();
        node->New->mat_speed(i) = node->New->delta_disp(i) / pdomain->currentSolver->getTimeStep();

        // acceleration nulle
        node->Current->dmat_speed(i) = 2.0 * disp(i) / (SQ(pdomain->currentSolver->getTimeStep()));
        node->New->dmat_speed(i) = 2.0 * disp(i) / (SQ(pdomain->currentSolver->getTimeStep()));
      }
      else
      {
        // deplacement constant
        node->Current->delta_disp(i) = disp(i) * (function->getValue(pdomain->currentTime));
        node->New->delta_disp(i) = disp(i) * function->getValue(pdomain->nextTime);

        // vitesse imposee
        node->Current->mat_speed(i) = node->Current->delta_disp(i) / pdomain->currentSolver->getTimeStep() * function->getValue(pdomain->currentTime);
        node->New->mat_speed(i) = node->New->delta_disp(i) / pdomain->currentSolver->getTimeStep() * function->getValue(pdomain->nextTime);

        // acceleration non nulle
        node->Current->dmat_speed(i) = 2.0 * disp(i) / (SQ(pdomain->currentSolver->getTimeStep())) * function->getValue(pdomain->nextTime);
        node->New->dmat_speed(i) = 2.0 * disp(i) / (SQ(pdomain->currentSolver->getTimeStep())) * function->getValue(pdomain->nextTime);
      }
    }
  }
}

//-----------------------------------------------------------------------------
void BoundaryDisplacement::applyConstant_V0(Node *node)
//-----------------------------------------------------------------------------
{
  fatalError("BoundaryDisplacement::applyConstant_V0", "Old one\n");
}

//!destructeur de la classe BoundaryDisplacement
/*!
  Cette m�thode est le destructeur de la classe BoundaryDisplacement.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryDisplacement::~BoundaryDisplacement()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundaryDisplacement::get()
//-----------------------------------------------------------------------------
{
  return disp;
}

//-----------------------------------------------------------------------------
void BoundaryDisplacement::set(const Vec3D &sp)
//-----------------------------------------------------------------------------
{
  disp = sp;
}

//-----------------------------------------------------------------------------
void BoundaryDisplacement::set(Real x, Real y, Real z)
//-----------------------------------------------------------------------------
{
  disp(0) = x;
  disp(1) = y;
  disp(2) = z;
}

/*********************************************************************************************
 *
 *********************************************************************************************/

//!constructeur par d�faut de la classe BoundaryForce
/*!
  Cette m�thode est le constructeur par d�faut de la classe BoundaryForce. En pratique, ici, on ne fait rien de plus que les allocations de m�moire n�cessaires.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryForce::BoundaryForce()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryForce::BoundaryForce(const Vec3D &fr)
//-----------------------------------------------------------------------------
{
  force = fr;
}

//!constructeur par recopie de la classe BoundaryForce
/*!
  Cette m�thode est le constructeur par recopie de la classe BoundaryForce.
  \warning La recopie n'est pas pr�vue ici, cette m�thode g�n�re une erreur d'ex�cution
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryForce::BoundaryForce(const BoundaryForce &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely BoundaryForce class" << /*(int) */ &X << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
void BoundaryForce::applyInitial_V0(Node *node)
//-----------------------------------------------------------------------------
{
  //node->New->fe=force;
}

//!Application d'une condition limite Impos�e sur un noeud
/*!
Cette m�thode applique des conditions aux limites impos�es sur un noeud. Les conditions limites impos�es sont de type vitesse, d�placement ou force impos�e.
*/
//-----------------------------------------------------------------------------
void BoundaryForce::applyConstant_V0(Node *node)
//-----------------------------------------------------------------------------
{
  // l'id�e est la, mais reste � le faire
  Real facteur =
      1.2 * Global_Structure->domains.current()->currentTime / Global_Structure->domains.current()->currentSolver->getEndTime();
  if (facteur > 1.0)
    facteur = 1.0;
  node->New->fe = facteur * force;
  fatalError("BoundaryForce::applyConstant_V0",
             "Pas encore implement� et test�");
}

//!destructeur de la classe BoundaryForce
/*!
  Cette m�thode est le destructeur de la classe BoundaryForce.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryForce::~BoundaryForce()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundaryForce::get()
//-----------------------------------------------------------------------------
{
  return force;
}

//-----------------------------------------------------------------------------
void BoundaryForce::set(const Vec3D &sp)
//-----------------------------------------------------------------------------
{
  force = sp;
}

//-----------------------------------------------------------------------------
void BoundaryForce::set(Real x, Real y, Real z)
//-----------------------------------------------------------------------------
{
  force(0) = x;
  force(1) = y;
  force(2) = z;
}

//!constructeur par d�faut de la classe BoundaryConvection
/*!
  Cette m�thode est le constructeur par d�faut de la classe BoundaryConvection. En pratique, ici, on ne fait rien de plus que les allocations de m�moire n�cessaires.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryConvection::BoundaryConvection()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryConvection::BoundaryConvection(const Real hc_, const Real tf_)
//-----------------------------------------------------------------------------
{
  hc = hc_;
  Tf = tf_;
}

//-----------------------------------------------------------------------------
Vec3D BoundaryConvection::get()
//-----------------------------------------------------------------------------
{
  Vec3D v;
  v(0) = hc;
  v(1) = Tf;
  return v;
}

//-----------------------------------------------------------------------------
void BoundaryConvection::set(const Vec3D &sp)
//-----------------------------------------------------------------------------
{
  //  speed=sp;
  hc = sp(0);
  Tf = sp(1);
}

//-----------------------------------------------------------------------------
void BoundaryConvection::set(Real x, Real y)
//-----------------------------------------------------------------------------
{
  hc = x;
  Tf = y;
}

//!constructeur par recopie de la classe BoundaryConvection
/*!
  Cette m�thode est le constructeur par recopie de la classe BoundaryConvection.
  \warning La recopie n'est pas pr�vue ici, cette m�thode g�n�re une erreur d'ex�cution
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryConvection::BoundaryConvection(const BoundaryConvection &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely BoundaryConvection class" << /*(int) */ &X << endl;
  exit(-1);
}

//!destructeur de la classe BoundaryConvection
/*!
  Cette m�thode est le destructeur de la classe BoundaryConvection.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryConvection::~BoundaryConvection()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void BoundaryConvection::applyInitial_V0(Node *node)
//-----------------------------------------------------------------------------
{
  //node->New->fe=force;
}

//-----------------------------------------------------------------------------
void BoundaryConvection::applyConstant_V0(Node *node)
//-----------------------------------------------------------------------------
{
  //     node->New->hconv=hc;
  //    node->Current->Tfluid=Tf;
}

//!constructeur par d�faut de la classe BoundaryFlux
/*!
  Cette m�thode est le constructeur par d�faut de la classe BoundaryFlux. En pratique, ici, on ne fait rien de plus que les allocations de m�moire n�cessaires.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryFlux::BoundaryFlux()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryFlux::BoundaryFlux(const Real &fr)
//-----------------------------------------------------------------------------
{
  flux = fr;
}

//!constructeur par recopie de la classe BoundaryFlux
/*!
  Cette m�thode est le constructeur par recopie de la classe BoundaryFlux.
  \warning La recopie n'est pas pr�vue ici, cette m�thode g�n�re une erreur d'ex�cution
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryFlux::BoundaryFlux(const BoundaryFlux &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely BoundaryFlux class" << /*(int) */ &X << endl;
  exit(-1);
}

//!destructeur de la classe BoundaryFlux
/*!
  Cette m�thode est le destructeur de la classe BoundaryFlux.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryFlux::~BoundaryFlux()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void BoundaryFlux::applyInitial_V0(Node *node)
//-----------------------------------------------------------------------------
{
  node->New->flux = flux;
  node->Current->flux = flux;
}

//-----------------------------------------------------------------------------
void BoundaryFlux::applyConstant_V0(Node *node)
//-----------------------------------------------------------------------------
{
  node->New->flux = flux;
  node->Current->flux = flux;
}

//-----------------------------------------------------------------------------
Real BoundaryFlux::get()
//-----------------------------------------------------------------------------
{
  return flux;
}

//-----------------------------------------------------------------------------
void BoundaryFlux::set(const Real &sp)
//-----------------------------------------------------------------------------
{
  flux = sp;
}

//!constructeur par d�faut de la classe BoundaryTemperature
/*!
  Cette m�thode est le constructeur par d�faut de la classe BoundaryTemperature. En pratique, ici, on ne fait rien de plus que les allocations de m�moire n�cessaires.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryTemperature::BoundaryTemperature()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryTemperature::BoundaryTemperature(const Real &fr)
//-----------------------------------------------------------------------------
{
  temperature = fr;
}

//!constructeur par recopie de la classe BoundaryTemperature
/*!
  Cette m�thode est le constructeur par recopie de la classe BoundaryTemperature.
  \warning La recopie n'est pas pr�vue ici, cette m�thode g�n�re une erreur d'ex�cution
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryTemperature::BoundaryTemperature(const BoundaryTemperature &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely BoundaryTemperature class" << &X << endl;
  exit(-1);
}

//!destructeur de la classe BoundaryTemperature
/*!
  Cette m�thode est le destructeur de la classe BoundaryTemperature.
  \author Olivier PANTALE
  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryTemperature::~BoundaryTemperature()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void BoundaryTemperature::applyInitial_V0(Node *node)
//-----------------------------------------------------------------------------
{
  //  node->New->temperature=temperature;
  //  node->Current->temperature=temperature;
}

//-----------------------------------------------------------------------------
void BoundaryTemperature::applyConstant_V0(Node *node)
//-----------------------------------------------------------------------------
{
  //  node->New->temperature=temperature;
  //  node->Current->temperature=temperature;
}

//-----------------------------------------------------------------------------
Real BoundaryTemperature::get()
//-----------------------------------------------------------------------------
{
  return temperature;
}

//-----------------------------------------------------------------------------
void BoundaryTemperature::set(const Real &sp)
//-----------------------------------------------------------------------------
{
  temperature = sp;
}

//-----------------------------------------------------------------------------
BoundaryAcceleration::BoundaryAcceleration()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryAcceleration::BoundaryAcceleration(const BoundaryAcceleration &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryAcceleration::~BoundaryAcceleration()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundaryAcceleration::get()
//-----------------------------------------------------------------------------
{
  return acc;
}

//-----------------------------------------------------------------------------
void BoundaryAcceleration::set(const Vec3D &sp)
//-----------------------------------------------------------------------------
{
  acc = sp;
}

//-----------------------------------------------------------------------------
void BoundaryAcceleration::set(Real x, Real y, Real z)
//-----------------------------------------------------------------------------
{
  acc(0) = x;
  acc(1) = y;
  acc(2) = z;
}
