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

// begin date : 20/03/1997

/*
  Class NodalField definition
*/

#ifndef __NodalField_h__
#define __NodalField_h__

#include <upFemSolver.h>

/** Classe de d�finition des champs de donn�es nodales.
D�finition d'un champ de donn�es aux noeuds de la structure. Chaque noeud de la structure poss�de deux champs de donn�es correspondant respectivement au champ � l'instant t (r�f�renc� par le pointeur Current) et au champ � t+dt (r�f�renc� par le pointeur New). Se reporter � la documentation de la classe Node pour plus de renseignements.
Afin d'acc�l�rer l'acc�s aux informations contenues dans cette classe, les champs sont d�finis comme publics.
see Node
version DynELA v.0.9.0
author Olivier PANTALE
*/
/** @dia:pos 265.8,80.8 */
class NodalField
{

public:
  /**.Masse volumique*/
  Real ro;
  /**.Incr�ment de masse volumique*/
  Real dro;
  /**.Energie interne*/
  Real e;
  /**.Incr�ment d'�nergie interne*/
  Real de;
  /**.Vecteur vitesse materielle du noeud*/
  /** @dia:route 0,2;h,108.735,8.5206,-104.685,-168.35,165.05 */
  Vec3D mat_speed;
  /*Vecteur vitesse de grille du noeud */
  //  Vec3D grid_speed;
  /*Vecteur acc�l�ration mat�rielle du noeud */
  /** @dia:route 0,2;h,108.735,8.5206,-104.685,-168.35,165.05 */
  Vec3D dmat_speed;
  /**.Vecteur des forces ext�rieures appliqu�es au noeud*/
  /** @dia:route 0,2;h,108.735,8.5206,-104.685,-168.35,165.05 */
  Vec3D fe;
  /**.Temp�rature du noeud.*/
  Real T;
  /**.Champ de d�placements au noeud*/
  /** @dia:route 0,2;h,108.735,8.5206,-104.685,-168.35,165.05 */
  Vec3D flux;
  /** @dia:route 0,2;h,108.735,8.5206,-104.685,-168.35,165.05 */
  Vec3D delta_disp;
  /** @dia:route 0,2;h,108.735,8.5206,-104.685,-168.35,165.05 */
  Vec3D disp;

public:
  /**name Constructeurs et destructeur de la classe NodalField*/
  //{
  /**.Constructeur par d�faut*/
  NodalField();
  /**.Constructeur par recopie*/
  NodalField(const NodalField &X);
  /**.Detructeur*/
  ~NodalField();
  //}

  /**name Fonctions membres*/
  //{
  /**.Remise � z�ro de toutes les donn�es au noeud*/
  void Clear();

  // fonctions entree sortie
  ///
  ///
  friend ostream &operator<<(ostream &os, NodalField &field);
  friend ofstream &operator<<(ofstream &, const NodalField &);
  friend ifstream &operator>>(ifstream &, NodalField &);
  void print(ostream &os) const;
  void write(ofstream &pfile) const;
  NodalField &read(ifstream &pfile);
  Indice objectSize();

  //}
};

#endif

// ---------- Documentation automatique ----------

/*Class:NodalField
NAME: NodalField - Classe de gestion des donn\'ees scalaires et tensorielles

SYNTAX: NodalField

KEYWORDS:
champ de vecteurs, champ de scalaires

DESCRIPTION:

Cette classe sert a d\'efinir les donnees scalaires et tensorielles que l'on a au niveau de chaque noeud. Il convient d'initialiser ce champ pour chaque noeud en attachant un champ de fields \`a chaque noeud avant de commencer les calculs. Il faut cr\'eer un objet NodalField et l'attacher au noeud par la fonction attachField().

Les variables suivantes sont repr\'esent\'ees dans NodalField

$Variables scalaires\\
$ro=$\rho_{t}$ (masse volumique) \\
$dro=$\partial \rho_{t}$ (incr\'ement de masse volumique) \\
$new\_ro=$\rho_{t+\Delta t}$ (nouvelle masse volumique) \\
$new\_dro=$\partial \rho_{t+\Delta t}$ (nouvel incr\'ement de masse volumique) \\
$T=$T$ (temperature) \\
$T0=$T_0$ (temperature de reference) \\
$e=$e_{t}$ (\'energie) \\
$de=$\partial e_{t}$ (incr\'ement d'\'energie) \\
$new\_e=$e_{t+\Delta t}$ (nouvelle \'energie) \\
$new\_de=$\partial e_{t+\Delta t}$ (nouvel incr\'ement d'\'energie) \\

$Variables vectorielles \\
$fe= (vecteur des efforts ext\'erieurs appliqu\'es au noeud) \\
$u= (vecteur des d\'eplacements) \\
$dmat\_speed= (vecteur incr\'ement de vitesse mat\'erielle) \\
$new\_dmat\_speed= (vecteur du nouvel incr\'ement de vitesse mat\'erielle) \\

CONSTRUCTORS AND INITIALIZATION:
	
MEMBER FUNCTIONS:

"Clear" - \'efface toutes les donn\'ees et fait une remise a z\'ero des variables.

EXAMPLES:
|// creer un field en 3D
|NodalField* pfield=new NodalField(3);
|				
|// l'attacher au noeud pointe par pnd
|pnd->attachField(pfield);
|
|// initialiser les donnees masse volumique et temperature
|pfield->ro=7.8E+6;
|pfield->T=300.;

SEE ALSO:
class "Node" 
DEVELOPED BY:
	Equipe C.M.A.O 
	Laboratoire G\'enie de production
	Ecole Nationale d'Ing\'enieurs de Tarbes
	BP 1629 - 65016 TARBES cedex

AUTHOR:
	Olivier PANTALE
	
End:
*/
