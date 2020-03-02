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

// begin date : 22/05/1997

/*
  Class Interface definition
*/

#ifndef __Interface_h__
#define __Interface_h__

#include <upFemSolver.h>

/* #include <Domain.h> */

/* #include <Side.h> */
class Side;
/* #include <ContactLaw.h> */
class ContactLaw;

/*<Interface:*/
/**
*/
/** @dia:pos 290.8,75.8 */
class Interface
{
  friend class ListId<Interface *>;

  //protected:
public:
  Indice Id;
  /** @dia:route 17,19;h,302.85,81.1,310.75,103.9,306.05 */
  Side *master;
  /** @dia:route 21,15;h,302.85,82.7,309.75,102.3,306.05 */
  Side *slave;
  /** @dia:route 0,2;h,133.735,3.5206,-129.685,-172.85,-95.25 */
  String name;

public:
  /** @dia:route 3,8;h,294.8,43.5,285.75,77.7,290.8 */
  ContactLaw *contactLaw;

public:
  // constructeurs
  Interface(Indice _Id = 0);
  Interface(const Interface &X);
  ~Interface();

  // fonctions membres
  Boolean setMaster(Side *pside);
  Boolean setSlave(Side *pside);
  void ScanIncludeNodes();
  //  void computeSpeeds(Real timestep);
  void Init();
  void toFile(FILE *pfile);
  Boolean check();
  void setContactLaw(ContactLaw *);
};

/*>Interface:*/

#endif
