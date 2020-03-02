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
  Class Coulomb implementation
*/

#include <Coulomb.h>
//#include <Physic.h>
#include <Structure.h>
#include <Solver.h>
#include <Node.h>
#include <NodalField.h>
#include <SideFace.h>
#include <Domain.h>

//-----------------------------------------------------------------------------
CoulombLaw::CoulombLaw()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
CoulombLaw::CoulombLaw(const CoulombLaw &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely CoulombLaw class" << /*(int) */ &X << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
CoulombLaw::~CoulombLaw()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void CoulombLaw::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  fprintf(pfile, "COULOMB CONTACT LAW\n");
  fprintf(pfile, "   FRICTION\n");
  fprintf(pfile, "  %8.5E\n", friction);
}

//-----------------------------------------------------------------------------
String
CoulombLaw::Name()
//-----------------------------------------------------------------------------
{
  String name;
  name = "Coulomb";
  return name;
}

//-----------------------------------------------------------------------------
void CoulombLaw::setFriction(Real fric)
//-----------------------------------------------------------------------------
{
  friction = fric;
}

//-----------------------------------------------------------------------------
Coulomb::Coulomb()
//-----------------------------------------------------------------------------
{
  // friction=0.5;
  //  penetration=0.;
}

//-----------------------------------------------------------------------------
Coulomb::Coulomb(const Coulomb &X)
//-----------------------------------------------------------------------------
{
  cerr << "can't copy directely Coulomb class" << /*(int) */ &X << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
Coulomb::~Coulomb()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Coulomb::computetangentialForce(Real fn, Vec3D &Ft)
//-----------------------------------------------------------------------------
{
  Vec3D Vt;
  Real ft;
  // Vec3D       force;

  // Vitesse tangentielle
  Vt =
      node->New->mat_speed -
      (node->New->mat_speed.dot(pside->normal)) * pside->normal;
  //  cout << "Vt =" << Vt << endl;

  // test dans la cas ou pas de composante tangentielle
  if (Vt.norm2() == 0.0)
  {
    Ft = 0.;
    return;
  }

  // Force tangentielle
  Ft = -(node->mass / Global_Structure->domains.current()->/*times.timeStep*/ currentSolver->getTimeStep()) * Vt;
  //  cout << "Ft =" << Ft << endl;

  // normalisation de Vt
  Vt.normalize();

  ft = Ft.dot(Vt);
  //  cout << "ft =" << ft << endl;

  // friction
  if (friction * Abs(fn) < Abs(ft))
    Ft = -(friction * Abs(fn)) * Vt;
  else
    Ft = -Abs(ft) * Vt;
  //  cout << "Ft =" << Ft << endl;
}
