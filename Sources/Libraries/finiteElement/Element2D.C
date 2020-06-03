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
  Class Element2D definition
*/

#include <IntegrationPoint.h>
#include <Element2D.h>
#include <Node.h>
#include <NodalField.h>
#include <Material.h>
#include <Structure.h>
#include <io_Structure.h>
// #include <Physic.h>
#include <Domain.h>

//-----------------------------------------------------------------------------
Element2D::Element2D(Indice No_) : Element(No_)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Element2D::Element2D(const Element2D &el) : Element(el)
//-----------------------------------------------------------------------------
{
  cout << "recopie d'un Element2D\n";
}

//-----------------------------------------------------------------------------
Element2D::~Element2D()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Element2D::computeInternalMatrices(Boolean reference)
//-----------------------------------------------------------------------------
{
  Indice k;
  Node *pnd;

  Indice pt;

  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration courant
    getIntegrationPoint(pt);

    //    ref->JxW0 = ref->InvJxW;
    //    ref->radius0 = ref->radius;

    // appel du calcul de N
    //   getShapeFunctionAtPoint (ref->N, ref->coords);
    // remplac� par:    elementData->integrationPoint[pt].shapeFunction
    //   ref->N = elementData->integrationPoint[pt].shapeFunction;

    // appel du calcul de dN (1ere partie)
    //   getDerShapeFunctionAtPoint (ref->dN, ref->coords);
    // remplac� par:    elementData->integrationPoint[pt].derShapeFunction
    //    ref->dN = elementData->integrationPoint[pt].derShapeFunction;
    //   ref->dN=ref->dN.Transpose();

    // initialisation de J
    ref->JxW = 0.;

    // calcul de J
    for (k = 0; k < getNumberOfNodes(); k++)
    {
      pnd = nodes(k);
      ref->JxW(0, 0) += elementData->integrationPoint[pt].derShapeFunction(k, 0) * pnd->coords(0);
      ref->JxW(0, 1) += elementData->integrationPoint[pt].derShapeFunction(k, 1) * pnd->coords(0);
      ref->JxW(1, 0) += elementData->integrationPoint[pt].derShapeFunction(k, 0) * pnd->coords(1);
      ref->JxW(1, 1) += elementData->integrationPoint[pt].derShapeFunction(k, 1) * pnd->coords(1);
    }

    // determinant de J
    ref->detJ =
        (ref->JxW(0, 0) * ref->JxW(1, 1)) - (ref->JxW(0, 1) * ref->JxW(1, 0));
    if (reference)
      ref->detJ0 = ref->detJ;

    // test de positivite du Jacobien
    if (ref->detJ < 0.)
    {
      cerr << "Negative value of detJ encountered in element " << number << endl;
      cerr << "at time : " << Global_Structure->domains.current()->currentTime << endl;
      cerr << "Emergency save of the last result\n";
      cerr << "Program aborted\n";
      Global_Structure->resultFile->transfert();
      exit(-1);
    }

    // calcul de l'inverse de J
    ref->InvJxW(0, 0) = ref->JxW(1, 1) / ref->detJ;
    ref->InvJxW(0, 1) = -ref->JxW(0, 1) / ref->detJ;
    ref->InvJxW(1, 0) = -ref->JxW(1, 0) / ref->detJ;
    ref->InvJxW(1, 1) = ref->JxW(0, 0) / ref->detJ;

    // recalcul de dN
    ref->dN = elementData->integrationPoint[pt].derShapeFunction * ref->InvJxW;

    //    getRadius_atIntPoint (ref->radius);
    //     if (reference) {
    // //      ref->JxW0= ref->InvJxW;
    // //      ref->radius0= ref->radius;
    //     }
  }
}

//-----------------------------------------------------------------------------
void Element2D::computeElasticStiffnessMatrix(Matrix &K, Boolean under)
//-----------------------------------------------------------------------------
{
  Indice pt;
  Real WxdJ, R;
  Indice i, I, j, J;
  Indice pts;
  IntegrationPointBase *pInt;

  // initialisation
  K.redim(getNumberOfDimensions() * getNumberOfNodes(), getNumberOfDimensions() * getNumberOfNodes());
  K = 0;
  Matrix C;

  // chargement de la matrice de comportement C
  if (getFamily() == Element::Axisymetric)
    C = material->getHookeMatrix(Material::axisymetric);
  else
    C = material->getHookeMatrix(Material::planeStrain);

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
    if (getFamily() == Element::Axisymetric)
    {
      getRadius_atIntPoint(R);
      WxdJ *= 2 * PI * R;
    }

    // calcul de C.B
    for (i = 0; i < getNumberOfNodes(); i++)
    {
      I = getNumberOfDimensions() * i;
      CB(0, I) = (C(0, 0) * pInt->dN(i, 0) + C(0, 2) * pInt->dN(i, 1));
      CB(1, I) = (C(1, 0) * pInt->dN(i, 0) + C(1, 2) * pInt->dN(i, 1));
      CB(2, I) = (C(2, 0) * pInt->dN(i, 0) + C(2, 2) * pInt->dN(i, 1));
      CB(0, I + 1) = (C(0, 1) * pInt->dN(i, 1) + C(0, 2) * pInt->dN(i, 0));
      CB(1, I + 1) = (C(1, 1) * pInt->dN(i, 1) + C(1, 2) * pInt->dN(i, 0));
      CB(2, I + 1) = (C(2, 1) * pInt->dN(i, 1) + C(2, 2) * pInt->dN(i, 0));
      if (getFamily() == Element::Axisymetric)
      {
        CB(3, I) = (C(3, 0) * pInt->dN(i, 0) + C(3, 2) * pInt->dN(i, 1));
        CB(3, I + 1) = (C(3, 1) * pInt->dN(i, 1) + C(3, 2) * pInt->dN(i, 0));
        CB(0, I) += C(0, 3) * pInt->data->shapeFunction(i) / R;
        CB(1, I) += C(1, 3) * pInt->data->shapeFunction(i) / R;
        CB(2, I) += C(2, 3) * pInt->data->shapeFunction(i) / R;
        CB(3, I) += C(3, 3) * pInt->data->shapeFunction(i) / R;
      }
    }

    // calcul de BT [C B]
    for (i = 0; i < getNumberOfNodes(); i++)
      for (j = 0; j < getNumberOfNodes(); j++)
      {
        I = getNumberOfDimensions() * i;
        J = getNumberOfDimensions() * j;
        K(I, J) += (pInt->dN(i, 0) * CB(0, J) + pInt->dN(i, 1) * CB(2, J)) * WxdJ;
        K(I, J + 1) += (pInt->dN(i, 0) * CB(0, J + 1) + pInt->dN(i, 1) * CB(2, J + 1)) * WxdJ;
        K(I + 1, J) += (pInt->dN(i, 1) * CB(1, J) + pInt->dN(i, 0) * CB(2, J)) * WxdJ;
        K(I + 1, J + 1) += (pInt->dN(i, 1) * CB(1, J + 1) + pInt->dN(i, 0) * CB(2, J + 1)) * WxdJ;
        if (getFamily() == Element::Axisymetric)
        {
          K(I, J) += CB(3, J) * pInt->data->shapeFunction(i) / R * WxdJ;
          K(I, J + 1) += CB(3, J + 1) * pInt->data->shapeFunction(i) / R * WxdJ;
        }
      }
  }
}

//!R�cup�ration de la valeur du rayon sur un point d'int�gration
/*!
Cette m�thode permet de r�cup�rer la valeur du rayon de la structure sur un point d'int�gration. La valeur fournie est \f$ R \f$, elle est d�finie par: \f[ R = \sum_{i=1}^{nNodes} \varphi_i x^{i}\f] 
*/
//-----------------------------------------------------------------------------
void Element2D::getRadius_atIntPoint(Real &R)
//-----------------------------------------------------------------------------
{
  Indice i;

  // initialisation a zero de v
  R = 0.;

  // calcul de v courant
  for (i = 0; i < nodes.size(); i++)
  {
    R += ref->data->shapeFunction(i) * nodes(i)->coords(0);
  }
}

//-----------------------------------------------------------------------------
void Element2D::getV_atIntPoint(Vec3D &v, int t)
//-----------------------------------------------------------------------------
{
  Indice i;
  NodalField *field;

  // initialisation a zero de v
  v = 0.;

  // calcul de v courant
  for (i = 0; i < nodes.size(); i++)
  {
    field = nodes(i)->getFieldAtTime(t);
    v(0) += ref->data->shapeFunction(i) * (field->mat_speed(0));
    v(1) += ref->data->shapeFunction(i) * (field->mat_speed(1));
  }
}

//-----------------------------------------------------------------------------
void Element2D::getU_atIntPoint(Vec3D &u, int t)
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
  }
}
