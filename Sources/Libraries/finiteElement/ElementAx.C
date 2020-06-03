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

// begin date : 19/03/1997

/*
  Class ElementAx definition
*/

#include <IntegrationPoint.h>
#include <ElementAx.h>
#include <Node.h>
#include <NodalField.h>
#include <Material.h>
#include <Structure.h>
#include <io_Structure.h>
// #include <Physic.h>
#include <Domain.h>

//-----------------------------------------------------------------------------
ElementAx::ElementAx(Indice No_) : Element2D(No_)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ElementAx::ElementAx(const ElementAx &el) : Element2D(el)
//-----------------------------------------------------------------------------
{
  cout << "recopie d'un ElementAx\n";
}

//-----------------------------------------------------------------------------
ElementAx::~ElementAx()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ElementAx::getdV_atIntPoint(Tensor2 &dv, int t)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  Indice k;
  Vec3D v;
  Real R;

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

  // calcul du terme axisym�trique
  getV_atIntPoint(v, t);
  getRadius_atIntPoint(R);
  dv(2, 2) = v(0) / R;
}

//-----------------------------------------------------------------------------
void ElementAx::getdU_atIntPoint(Tensor2 &du, int t)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  Indice k;
  Real v;
  Real R;

  // initialisation a zero de du
  du = 0.;
  v = 0.;

  // calcul de du
  for (k = 0; k < getNumberOfNodes(); k++)
  {
    field = nodes(k)->getFieldAtTime(t);
    du(0, 0) += ref->dN(k, 0) * field->delta_disp(0);
    du(0, 1) += ref->dN(k, 1) * field->delta_disp(0);
    du(1, 0) += ref->dN(k, 0) * field->delta_disp(1);
    du(1, 1) += ref->dN(k, 1) * field->delta_disp(1);
    v += ref->data->shapeFunction(k) * field->delta_disp(0);
  }

  // calcul du terme axisym�trique
  getRadius_atIntPoint(R);
  du(2, 2) = v / R;
}

//-----------------------------------------------------------------------------
void ElementAx::getF_atIntPoint(Tensor2 &du, int t)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  Indice k;
  Real v;
  Real R;

  // initialisation a zero de du
  du.Identity();
  v = 0.;

  // calcul de du
  for (k = 0; k < getNumberOfNodes(); k++)
  {
    field = nodes(k)->getFieldAtTime(t);
    du(0, 0) += ref->dN(k, 0) * field->delta_disp(0);
    du(0, 1) += ref->dN(k, 1) * field->delta_disp(0);
    du(1, 0) += ref->dN(k, 0) * field->delta_disp(1);
    du(1, 1) += ref->dN(k, 1) * field->delta_disp(1);
    v += ref->data->shapeFunction(k) * field->delta_disp(0);
  }

  // calcul du terme axisym�trique
  getRadius_atIntPoint(R);
  du(2, 2) += v / R;
}

//-----------------------------------------------------------------------------
Boolean
ElementAx::checkLevel2()
//-----------------------------------------------------------------------------
{
  // check for a negative value of x coordinate
  for (Indice i = 0; i < nodes.size(); i++)
  {
    if (nodes(i)->coords(0) < 0.)
    {
      if (nodes(i)->coords(0) > -1e-8)
      {
        warning("Negative r value in an axisymetric element",
                "Element %d has an r value of %10.3E\nThis has been changed to 0.00\n",
                number, nodes(i)->coords(0));
        nodes(i)->coords(0) = 0.;
      }
      else
      {
        cerr << "Error in element " << number << "\nr coordinate (" << nodes(i)->coords(0) << ") of the node " << nodes(i)->number << " is negative\n";
        exit(-1);
      }
    }
  }
  return True;
}
