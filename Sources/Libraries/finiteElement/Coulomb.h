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
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

// begin date :
// revision date :

/*
  Class Coulomb definition
*/

#ifndef __Coulomb_h__
#define __Coulomb_h__

#include <Contact.h>
class Contact;
#include <ContactLaw.h>
class ContactLaw;

/**Classe de gestion des param�tres de la loi de frottement de Coulomb.
see Coulomb
author Olivier PANTALE
version DynELA v. 0.9.0
*/
/** @dia:pos 298.8,59.8 */
/** @dia:route ContactLaw;v,301,48.8,52.85,304.65,59.8 */
class CoulombLaw : public ContactLaw
{
public:
  Real friction;

public:
  // constructeurs
  CoulombLaw();
  CoulombLaw(const CoulombLaw &X);
  ~CoulombLaw();

  int Type()
  {
    return coulombContactLaw;
  }
  void setFriction(Real fric);
  String Name();
  // fonctions membres
  void toFile(FILE *pfile);
};

/**Classe de gestion de la loi de contact de Coulomb.
see CoulombLaw, Node, NodeMotion
author Olivier PANTALE
version DynELA v. 0.9.0
*/
/** @dia:pos 218.8,135.8 */
/** @dia:route Contact;v,224.85,129.8,131.85,227.45,135.8 */
class Coulomb : public Contact
{
public:
  Real friction;
  //  Real penetration;

public:
  // constructeurs
  Coulomb();
  Coulomb(const Coulomb &X);
  ~Coulomb();

  // fonctions membres
  /**.Calcul du vecteur de force � appliquer au noeud afin de contrer la p�n�tration des deux surfaces de contact*/
  void computetangentialForce(Real fn, Vec3D &Ft);
  //  void computeForces(Real timeStep);
  //  void computeSpeeds(Real timeStep);
};
/*>Coulomb:*/

#endif
