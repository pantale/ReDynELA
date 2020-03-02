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

// begin date : 19/03/1997

/*
  Class ElementPlane definition
*/

#include <IntegrationPoint.h>
#include <ElementPlane.h>
#include <Node.h>
#include <NodalField.h>
#include <Material.h>
#include <Structure.h>
#include <io_Structure.h>
//#include <Physic.h>
#include <Domain.h>

//-----------------------------------------------------------------------------
ElementPlane::ElementPlane(Indice No_) : Element2D(No_)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ElementPlane::ElementPlane(const ElementPlane &el) : Element2D(el)
//-----------------------------------------------------------------------------
{
  cout << "recopie d'un ElementPlane\n";
}

//-----------------------------------------------------------------------------
ElementPlane::~ElementPlane()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ElementPlane::getdV_atIntPoint(Tensor2 &dv, int t)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  Indice k;

  // initialisation a zero de dv
  dv = 0.;

  // calcul de dv
  for (k = 0; k < getNumberOfNodes(); k++)
  {
    field = nodes(k)->getFieldAtTime(t);
    dv(0, 0) += ref->dN(k, 0) * field->mat_speed(0);
    dv(0, 1) += ref->dN(k, 1) * field->mat_speed(0);
    dv(1, 0) += ref->dN(k, 0) * field->mat_speed(1);
    dv(1, 1) += ref->dN(k, 1) * field->mat_speed(1);
  }
}

//-----------------------------------------------------------------------------
void ElementPlane::getdU_atIntPoint(Tensor2 &du, int t)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  Indice k;

  // initialisation a zero de du
  du = 0.;

  // calcul de du
  for (k = 0; k < getNumberOfNodes(); k++)
  {
    field = nodes(k)->getFieldAtTime(t);
    du(0, 0) += ref->dN(k, 0) * field->delta_disp(0);
    du(0, 1) += ref->dN(k, 1) * field->delta_disp(0);
    du(1, 0) += ref->dN(k, 0) * field->delta_disp(1);
    du(1, 1) += ref->dN(k, 1) * field->delta_disp(1);
  }
}

//!Calcul du gradient des d�formations F
/*!
Cette m�thode effectue le calcul du gradient des d�formations F au point d'int�gration � l'instant t. La relation utilis�e pour ce calcul est donn�e par:
\f[  F=1+\frac{\partial \overrightarrow{u}}{\partial \overrightarrow{X}} \f]
\param F Tensor de retour pour le gradient des d�formations F
\param t valeur permettant de s�lectionner le champ pour le d�placement (0 r�f�rence, 1 courant).
*/
//-----------------------------------------------------------------------------
void ElementPlane::getF_atIntPoint(Tensor2 &F, int t)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  Indice k;

  // initialisation a Id de F
  F.Identity();

  // calcul de F en ajoutant le terme du/dX
  for (k = 0; k < getNumberOfNodes(); k++)
  {
    field = nodes(k)->getFieldAtTime(t);
    F(0, 0) += ref->dN(k, 0) * field->delta_disp(0);
    F(0, 1) += ref->dN(k, 1) * field->delta_disp(0);
    F(1, 0) += ref->dN(k, 0) * field->delta_disp(1);
    F(1, 1) += ref->dN(k, 1) * field->delta_disp(1);
  }
}

//-----------------------------------------------------------------------------
Boolean
ElementPlane::checkLevel2()
//-----------------------------------------------------------------------------
{
  return True;
}
