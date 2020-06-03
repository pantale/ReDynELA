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

// begin date : 22/05/1997

/*
  Class Contact definition
*/

#ifndef __Contact_h__
#define __Contact_h__

/* #include <Side.h> */
/* class Side; */
class SideFace;

/* #include <NodeMotion.h> */
#include <NodeMotion.h>

/**Classe de gestion du contact.
Classe de base de la d�finition des contacts su DynELA. Cette classe est detin�e � g�rer tous les types de contact de type point � point. Cette classe sert de classe de base pour les classe de contact de Coulomb... Selon le type de loi de contact utilis�e, cette classe fait appel � la fonction sp�cialis�e de la classe d�riv�e computeTangentialForce qui permet d'�valuer la valeur de la composante tangentielle de la force au noeud consid�r�.
see Node, NodeMotion
author Olivier PANTALE
version DynELA v. 0.9.0
*/
/** @dia:pos 214.8,119.8 */
/** @dia:route NodeMotion;v,233.575,113.2,115.85,224.85,119.8 */
class Contact : public NodeMotion
{
public:
  /** @dia:route 10,3;h,214.8,122.9,208.75,183.5,222.8 */
  SideFace *pside;

public:
  // constructeurs
  Contact();
  Contact(const Contact &X);
  virtual ~Contact();

  // fonctions membres
  Indice getMotion();
  Boolean attachSideFace(SideFace *pf);
  Boolean detachSideFace();
  void updateNode(Real timeStep);
  void Create();
  void computeForces(Real timeStep);
  virtual void computetangentialForce(Real fn, Vec3D &Ft) = 0;
  //virtual void computeTangentialForce()=0;
  //virtual void computeContactLaw(Real timeStep)=0;
};

#endif

// ---------- Documentation automatique ----------

/*Class:Contact
NAME: Contact - classe de d\'efinition des noeuds de contact

SYNTAX: Contact

KEYWORDS:

contact, motion

DESCRIPTION:

Cette classe est destin\'ee a g\'erer les contacts entre surfaces.

CONSTRUCTORS AND INITIALIZATION:
	
MEMBER FUNCTIONS:
 	
EXAMPLES:

SEE ALSO:
class "nodeMotion" 
DEVELOPED BY:
	Equipe C.M.A.O 
	Laboratoire G\'enie de production
	Ecole Nationale d''Ing\'enieurs de Tarbes
	BP 1629 - 65016 TARBES cedex

AUTHOR:
	Olivier PANTALE
	
End:
*/
