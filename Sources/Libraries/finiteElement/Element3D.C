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

// begin date : 28/04/1997

/*
  Class Element3D definition
*/

#include <IntegrationPoint.h>
#include <Element3D.h>
#include <Node.h>
#include <NodalField.h>
#include <Material.h>
#include <Structure.h>
#include <io_Structure.h>
// #include <Physic.h>
#include <Domain.h>

//-----------------------------------------------------------------------------
Element3D::Element3D(Indice No_) : Element(No_)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Element3D::Element3D(const Element3D &el) : Element(el)
//-----------------------------------------------------------------------------
{
  cout << "recopie d'un Element3D\n";
}

//-----------------------------------------------------------------------------
Element3D::~Element3D()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Element3D::computeInternalMatrices(Boolean reference)
//-----------------------------------------------------------------------------
{
  register Indice k;
  Node *pnd;

  // appel du calcul de N
  Indice pt;

  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration courant
    getIntegrationPoint(pt);

    //    getShapeFunctionAtPoint(ref->N,ref->coords);
    //    ref->N = elementData->integrationPoint[pt].shapeFunction;

    // appel du calcul de dN
    //    getDerShapeFunctionAtPoint(ref->dN,ref->coords);
    ref->dN = elementData->integrationPoint[pt].derShapeFunction;

    // initialisation de J
    ref->JxW = 0.;

    for (k = 0; k < getNumberOfNodes(); k++)
    {
      pnd = nodes(k);
      ref->JxW(0, 0) += ref->dN(k, 0) * pnd->coords(0);
      ref->JxW(0, 1) += ref->dN(k, 1) * pnd->coords(0);
      ref->JxW(0, 2) += ref->dN(k, 2) * pnd->coords(0);
      ref->JxW(1, 0) += ref->dN(k, 0) * pnd->coords(1);
      ref->JxW(1, 1) += ref->dN(k, 1) * pnd->coords(1);
      ref->JxW(1, 2) += ref->dN(k, 2) * pnd->coords(1);
      ref->JxW(2, 0) += ref->dN(k, 0) * pnd->coords(2);
      ref->JxW(2, 1) += ref->dN(k, 1) * pnd->coords(2);
      ref->JxW(2, 2) += ref->dN(k, 2) * pnd->coords(2);
    }

    // determinant de J
    ref->detJ = (ref->JxW(0, 0) * ref->JxW(1, 1) * ref->JxW(2, 2)) + (ref->JxW(1, 0) * ref->JxW(2, 1) * ref->JxW(0, 2)) + (ref->JxW(2, 0) * ref->JxW(0, 1) * ref->JxW(1, 2)) - (ref->JxW(2, 0) * ref->JxW(1, 1) * ref->JxW(0, 2)) - (ref->JxW(0, 0) * ref->JxW(2, 1) * ref->JxW(1, 2)) - (ref->JxW(1, 0) * ref->JxW(0, 1) * ref->JxW(2, 2));
    if (reference)
      ref->detJ0 = ref->detJ;

    // test de positivite du Jacobien
    if (ref->detJ < 0.)
    {
      cerr << "Negative value of detJ encountered in element " << number << " at integration point " << pt + 1 << endl;
      cerr << "at time : " << Global_Structure->domains.current()->currentTime << endl;
      cerr << "Emergency save of the last result\n";
      cerr << "Program aborted\n";
      //      Global_Structure->resultFile->transfert ();
    }

    // calcul de l'inverse de J
    ref->InvJxW = ref->JxW.inverse();

    // recalcul de dN
    ref->dN = ref->dN * ref->InvJxW;
  }
}

//-----------------------------------------------------------------------------
void Element3D::getV_atIntPoint(Vec3D &v, int t)
//-----------------------------------------------------------------------------
{
  Indice i;
  NodalField *field;

  // initialisation a zero de v
  v = 0.;

  // calcul de v
  for (i = 0; i < nodes.size(); i++)
  {
    field = nodes(i)->getFieldAtTime(t);
    v(0) += ref->data->shapeFunction(i) * (field->mat_speed(0));
    v(1) += ref->data->shapeFunction(i) * (field->mat_speed(1));
    v(2) += ref->data->shapeFunction(i) * (field->mat_speed(2));
  }
}

//-----------------------------------------------------------------------------
void Element3D::getU_atIntPoint(Vec3D &u, int t)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  Indice i;

  // initialisation a zero de du
  u = 0.;

  // calcul de du
  for (i = 0; i < nodes.size(); i++)
  {
    field = nodes(i)->getFieldAtTime(t);
    u(0) += ref->data->shapeFunction(i) * field->delta_disp(0);
    u(1) += ref->data->shapeFunction(i) * field->delta_disp(1);
    u(2) += ref->data->shapeFunction(i) * field->delta_disp(2);
  }
}

//-----------------------------------------------------------------------------
void Element3D::getdV_atIntPoint(Tensor2 &dv, int t)
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
    dv(0, 2) += ref->dN(k, 2) * field->mat_speed(0);
    dv(1, 0) += ref->dN(k, 0) * field->mat_speed(1);
    dv(1, 1) += ref->dN(k, 1) * field->mat_speed(1);
    dv(1, 2) += ref->dN(k, 2) * field->mat_speed(1);
    dv(2, 0) += ref->dN(k, 0) * field->mat_speed(2);
    dv(2, 1) += ref->dN(k, 1) * field->mat_speed(2);
    dv(2, 2) += ref->dN(k, 2) * field->mat_speed(2);
  }
}

//-----------------------------------------------------------------------------
void Element3D::getF_atIntPoint(Tensor2 &du, int t)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  Indice k;

  // initialisation a zero de du
  du.Identity();

  // calcul de du
  for (k = 0; k < getNumberOfNodes(); k++)
  {
    field = nodes(k)->getFieldAtTime(t);
    du(0, 0) += ref->dN(k, 0) * field->delta_disp(0);
    du(0, 1) += ref->dN(k, 1) * field->delta_disp(0);
    du(0, 2) += ref->dN(k, 2) * field->delta_disp(0);
    du(1, 0) += ref->dN(k, 0) * field->delta_disp(1);
    du(1, 1) += ref->dN(k, 1) * field->delta_disp(1);
    du(1, 2) += ref->dN(k, 2) * field->delta_disp(1);
    du(2, 0) += ref->dN(k, 0) * field->delta_disp(2);
    du(2, 1) += ref->dN(k, 1) * field->delta_disp(2);
    du(2, 2) += ref->dN(k, 2) * field->delta_disp(2);
  }
}

//-----------------------------------------------------------------------------
void Element3D::getdU_atIntPoint(Tensor2 &du, int t)
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
    du(0, 2) += ref->dN(k, 2) * field->delta_disp(0);
    du(1, 0) += ref->dN(k, 0) * field->delta_disp(1);
    du(1, 1) += ref->dN(k, 1) * field->delta_disp(1);
    du(1, 2) += ref->dN(k, 2) * field->delta_disp(1);
    du(2, 0) += ref->dN(k, 0) * field->delta_disp(2);
    du(2, 1) += ref->dN(k, 1) * field->delta_disp(2);
    du(2, 2) += ref->dN(k, 2) * field->delta_disp(2);
  }
}

//-----------------------------------------------------------------------------
Boolean
Element3D::checkLevel2()
//-----------------------------------------------------------------------------
{
  return True;
}

//-----------------------------------------------------------------------------
void Element3D::computeElasticStiffnessMatrix(Matrix &K, Boolean under)
//-----------------------------------------------------------------------------
{
  Indice pt;
  Real WxdJ;
  Indice i, I, j, J;
  Indice pts;
  IntegrationPointBase *pInt;

  // initialisation
  K.redim(getNumberOfDimensions() * getNumberOfNodes(), getNumberOfDimensions() * getNumberOfNodes());
  K = 0;
  Matrix C;

  C = material->getHookeMatrix(Material::threedimensional);

  // matrice temporaire
  Matrix CB(C.rows(), getNumberOfDimensions() * getNumberOfNodes());

  // nomre de points d'int�gration
  if (under)
  {
    pts = numberOfUnderIntegrationPoints();
  }
  else
  {
    pts = getNumberOfIntegrationPoints();
  }

  // parallelisation
  //#pragma omp parallel for private(WxdJ),shared(K)
  // boucle sur les points d'int�gration
  for (pt = 0; pt < pts; pt++)
  {

    // recuperation du point d'integration
    if (under)
    {
      getUnderIntegrationPoint(pt);
      pInt = underRef;
    }
    else
    {
      getIntegrationPoint(pt);
      pInt = ref;
    }

    // calcul du terme d'integration numerique
    WxdJ = pInt->data->weight * pInt->detJ;

    // calcul de C.B
    for (i = 0; i < getNumberOfNodes(); i++)
    {
      I = getNumberOfDimensions() * i;
      CB(0, I) = (C(0, 0) * pInt->dN(i, 0) + C(0, 3) * pInt->dN(i, 1) + C(0, 5) * pInt->dN(i, 2));
      CB(1, I) = (C(1, 0) * pInt->dN(i, 0) + C(1, 3) * pInt->dN(i, 1) + C(1, 5) * pInt->dN(i, 2));
      CB(2, I) = (C(2, 0) * pInt->dN(i, 0) + C(2, 3) * pInt->dN(i, 1) + C(2, 5) * pInt->dN(i, 2));
      CB(3, I) = (C(3, 0) * pInt->dN(i, 0) + C(3, 3) * pInt->dN(i, 1) + C(3, 5) * pInt->dN(i, 2));
      CB(4, I) = (C(4, 0) * pInt->dN(i, 0) + C(4, 3) * pInt->dN(i, 1) + C(4, 5) * pInt->dN(i, 2));
      CB(5, I) = (C(5, 0) * pInt->dN(i, 0) + C(5, 3) * pInt->dN(i, 1) + C(5, 5) * pInt->dN(i, 2));
      CB(0, I + 1) = (C(0, 1) * pInt->dN(i, 1) + C(0, 3) * pInt->dN(i, 0) + C(0, 4) * pInt->dN(i, 2));
      CB(1, I + 1) = (C(1, 1) * pInt->dN(i, 1) + C(1, 3) * pInt->dN(i, 0) + C(1, 4) * pInt->dN(i, 2));
      CB(2, I + 1) = (C(2, 1) * pInt->dN(i, 1) + C(2, 3) * pInt->dN(i, 0) + C(2, 4) * pInt->dN(i, 2));
      CB(3, I + 1) = (C(3, 1) * pInt->dN(i, 1) + C(3, 3) * pInt->dN(i, 0) + C(3, 4) * pInt->dN(i, 2));
      CB(4, I + 1) = (C(4, 1) * pInt->dN(i, 1) + C(4, 3) * pInt->dN(i, 0) + C(4, 4) * pInt->dN(i, 2));
      CB(5, I + 1) = (C(5, 1) * pInt->dN(i, 1) + C(5, 3) * pInt->dN(i, 0) + C(5, 4) * pInt->dN(i, 2));
      CB(0, I + 2) = (C(0, 2) * pInt->dN(i, 2) + C(0, 4) * pInt->dN(i, 1) + C(0, 5) * pInt->dN(i, 0));
      CB(1, I + 2) = (C(1, 2) * pInt->dN(i, 2) + C(1, 4) * pInt->dN(i, 1) + C(1, 5) * pInt->dN(i, 0));
      CB(2, I + 2) = (C(2, 2) * pInt->dN(i, 2) + C(2, 4) * pInt->dN(i, 1) + C(2, 5) * pInt->dN(i, 0));
      CB(3, I + 2) = (C(3, 2) * pInt->dN(i, 2) + C(3, 4) * pInt->dN(i, 1) + C(3, 5) * pInt->dN(i, 0));
      CB(4, I + 2) = (C(4, 2) * pInt->dN(i, 2) + C(4, 4) * pInt->dN(i, 1) + C(4, 5) * pInt->dN(i, 0));
      CB(5, I + 2) = (C(5, 2) * pInt->dN(i, 2) + C(5, 4) * pInt->dN(i, 1) + C(5, 5) * pInt->dN(i, 0));
    }

    // calcul de BT [C B]
    for (i = 0; i < getNumberOfNodes(); i++)
      for (j = 0; j < getNumberOfNodes(); j++)
      {
        I = getNumberOfDimensions() * i;
        J = getNumberOfDimensions() * j;
        K(I, J) += (pInt->dN(i, 0) * CB(0, J) + pInt->dN(i, 1) * CB(3, J) + pInt->dN(i, 2) * CB(5, J)) * WxdJ;
        K(I, J + 1) += (pInt->dN(i, 0) * CB(0, J + 1) + pInt->dN(i, 1) * CB(3, J + 1) + pInt->dN(i, 2) * CB(5, J + 1)) * WxdJ;
        K(I, J + 2) += (pInt->dN(i, 0) * CB(0, J + 2) + pInt->dN(i, 1) * CB(3, J + 2) + pInt->dN(i, 2) * CB(5, J + 2)) * WxdJ;
        K(I + 1, J) += (pInt->dN(i, 1) * CB(1, J) + pInt->dN(i, 0) * CB(3, J) + pInt->dN(i, 2) * CB(4, J)) * WxdJ;
        K(I + 1, J + 1) += (pInt->dN(i, 1) * CB(1, J + 1) + pInt->dN(i, 0) * CB(3, J + 1) + pInt->dN(i, 2) * CB(4, J + 1)) * WxdJ;
        K(I + 1, J + 2) += (pInt->dN(i, 1) * CB(1, J + 2) + pInt->dN(i, 0) * CB(3, J + 2) + pInt->dN(i, 2) * CB(4, J + 2)) * WxdJ;
        K(I + 2, J) += (pInt->dN(i, 2) * CB(2, J) + pInt->dN(i, 1) * CB(4, J) + pInt->dN(i, 0) * CB(5, J)) * WxdJ;
        K(I + 2, J + 1) += (pInt->dN(i, 2) * CB(2, J + 1) + pInt->dN(i, 1) * CB(4, J + 1) + pInt->dN(i, 0) * CB(5, J + 1)) * WxdJ;
        K(I + 2, J + 2) += (pInt->dN(i, 2) * CB(2, J + 2) + pInt->dN(i, 1) * CB(4, J + 2) + pInt->dN(i, 0) * CB(5, J + 2)) * WxdJ;
      }
  }
}
