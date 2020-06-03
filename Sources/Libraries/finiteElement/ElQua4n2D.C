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

// begin date : 13/07/1997

/*
  Class ElQua4n2D definition
*/

#include <Domain.h>
#include <io_Structure.h>
#include <Structure.h>
#include <IntegrationPoint.h>
#include <Node.h>
#include <ElQua4n2D.h>
//#include <Physic.h>

const ElementData ElQua4n2D::elementData = {
    // description de param�tres de l'�lement
    "ElQua4n2D",
    Element::ElQua4n2D,
    Element::Bidimensional,
    2,
    2,

    // description des noeuds de l'�lement
    4,
    {
#define uns2 1.0 / 2.0
#define unPR3s2 1.0 + sqrt(3.0) / 2.0 // 1.8660254037844386468
#define unMR3s2 1.0 - sqrt(3.0) / 2.0 // 0.13397459621556135324
        {Vec3D(-1, -1, 0), 2, {1, 3}, Vector(4, unPR3s2, -uns2, unMR3s2, -uns2)},
        {Vec3D(+1, -1, 0), 2, {0, 2}, Vector(4, -uns2, unPR3s2, -uns2, unMR3s2)},
        {Vec3D(+1, +1, 0), 2, {1, 3}, Vector(4, unMR3s2, -uns2, unPR3s2, -uns2)},
        {Vec3D(-1, +1, 0), 2, {0, 2}, Vector(4, -uns2, unMR3s2, -uns2, unPR3s2)}},

    // description des points d'int�gration
    4,
    {
#define deuxPR3s6 (2.0 + sqrt(3.0)) / 6.0    // 0.62200846792814621559
#define deuxMR3s6 (2.0 - sqrt(3.0)) / 6.0    // 0.044658198738520451079
#define uns6 1.0 / 6.0                       // 0.16666666666666666667
#define troisPR3s12 (3.0 + sqrt(3.0)) / 12.0 // 0.39433756729740644113
#define troisMR3s12 (3.0 - sqrt(3.0)) / 12.0 // 0.10566243270259355887
        {Vec3D(-invSqrt3, -invSqrt3, 0),
         1,
         Vector(4, deuxPR3s6, uns6, deuxMR3s6, uns6),
         Matrix(4, 2, -troisPR3s12, -troisPR3s12, troisPR3s12, -troisMR3s12, troisMR3s12, troisMR3s12, -troisMR3s12, troisPR3s12)},
        {Vec3D(+invSqrt3, -invSqrt3, 0),
         1,
         Vector(4, uns6, deuxPR3s6, uns6, deuxMR3s6),
         Matrix(4, 2, -troisPR3s12, -troisMR3s12, troisPR3s12, -troisPR3s12, troisMR3s12, troisPR3s12, -troisMR3s12, troisMR3s12)},
        {Vec3D(+invSqrt3, +invSqrt3, 0),
         1,
         Vector(4, deuxMR3s6, uns6, deuxPR3s6, uns6),
         Matrix(4, 2, -troisMR3s12, -troisMR3s12, troisMR3s12, -troisPR3s12, troisPR3s12, troisPR3s12, -troisPR3s12, troisMR3s12)},
        {Vec3D(-invSqrt3, +invSqrt3, 0),
         1,
         Vector(4, uns6, deuxMR3s6, uns6, deuxPR3s6),
         Matrix(4, 2, -troisMR3s12, -troisPR3s12, troisMR3s12, -troisMR3s12, troisPR3s12, troisMR3s12, -troisPR3s12, troisPR3s12)}},

    // description des points de sous int�gration
    1,
    {{Vec3D(0.0, 0.0, 0.0),
      4.0,
      Vector(4, 0.25, 0.25, 0.25, 0.25),
      Matrix(4, 2, -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25)}},

    // description des faces de l'�lement
    1,
    {{4, {0, 1, 2, 3}}},

    // description des sidefaces de l'�lement
    4,
    {{2, {0, 1}},
     {2, {1, 2}},
     {2, {2, 3}},
     {2, {3, 0}}},

    // description des ar�tes de l'�lement
    4,
    {{2, {0, 1}},
     {2, {1, 2}},
     {2, {2, 3}},
     {2, {3, 0}}},

    // end of data
    "end"};

//-----------------------------------------------------------------------------
ElQua4n2D::ElQua4n2D(Indice No_) : ElementPlane(No_)
//-----------------------------------------------------------------------------
{
  // affectation de taille pour glob2loc
  glob2loc.redim(2, 4);

  Element::elementData = &elementData;
}

//-----------------------------------------------------------------------------
ElQua4n2D::ElQua4n2D(const ElQua4n2D &el) : ElementPlane(el)
//-----------------------------------------------------------------------------
{
  cout << "recopie d'un ElQua4n2D\n";
}

//-----------------------------------------------------------------------------
ElQua4n2D::~ElQua4n2D()
//-----------------------------------------------------------------------------
{
  nodes.flush();
}

//!Calcul de la longueur caract�ristique d'un �l�ment
/*!
Cette m�thode calcule la longueur caract�ristique d'un �l�ment � partir de la d�finition de la g�om�trie de cet �l�ment.
La relation utilis�e pour ce calcul est donn�e par:
\f[ l=\frac{x_{31}*y_{42}+x_{24}*y_{31}}{\sqrt{x_{24}^2+y_{42}^2+x_{31}^2+y_{31}^2}} \f] avec \f$ x_{ij} \f$ distance horizontale entre les points i et j et \f$ y_{ij} \f$ distance verticale entre les points i et j. 
\return longueur caract�ristique de l'�l�ment
*/
//-----------------------------------------------------------------------------
Real ElQua4n2D::getLength()
//-----------------------------------------------------------------------------
{
  Real x31, y42, x24, y31;
  x31 = nodes(2)->coords(0) - nodes(0)->coords(0);
  x24 = nodes(1)->coords(0) - nodes(3)->coords(0);
  y42 = nodes(3)->coords(1) - nodes(1)->coords(1);
  y31 = nodes(2)->coords(1) - nodes(0)->coords(1);

  // modif DynELA 0.9.5
  // correction d'un bug, merci Maxima
  return ((x31 * y42 + x24 * y31) / sqrt(2. * (SQ(x24) + SQ(y42) + SQ(x31) + SQ(y31))));
}

//-----------------------------------------------------------------------------
Real ElQua4n2D::getVolume()
//-----------------------------------------------------------------------------
{
  Real l1, l2, l3, l4, l5;

  l1 = nodes(0)->coords.distance(nodes(1)->coords);
  l2 = nodes(1)->coords.distance(nodes(2)->coords);
  l3 = nodes(2)->coords.distance(nodes(3)->coords);
  l4 = nodes(3)->coords.distance(nodes(0)->coords);
  l5 = nodes(0)->coords.distance(nodes(2)->coords);

  return (Surf_Tri(l1, l2, l5) + Surf_Tri(l3, l4, l5));
}

//-----------------------------------------------------------------------------
void ElQua4n2D::getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
  Real k = point(0);
  Real n = point(1);

  N(0) = (1. - k) * (1. - n) / 4.;
  N(1) = (1. + k) * (1. - n) / 4.;
  N(2) = (1. + k) * (1. + n) / 4.;
  N(3) = (1. - k) * (1. + n) / 4.;
}

//-----------------------------------------------------------------------------
void ElQua4n2D::getDerShapeFunctionAtPoint(Matrix &N, const Vec3D &point)
//-----------------------------------------------------------------------------
{
  Real k = point(0);
  Real n = point(1);

  N(0, 0) = -(1. - n) / 4.;
  N(0, 1) = -(1. - k) / 4.;
  N(1, 0) = +(1. - n) / 4.;
  N(1, 1) = -(1. + k) / 4.;
  N(2, 0) = +(1. + n) / 4.;
  N(2, 1) = +(1. + k) / 4.;
  N(3, 0) = -(1. + n) / 4.;
  N(3, 1) = +(1. - k) / 4.;
}

// //-----------------------------------------------------------------------------
// void
// ElQua4n2D::getIntgtoNodes (Vector & N, const Vec3D & point) const
// //-----------------------------------------------------------------------------
// {
//   const Real pos = 1.0 / sqrt (3.0);
//   Real k = point (0);
//   Real n = point (1);

//   N (0) = 3. * (pos - k) * (pos - n) / 4.;
//   N (1) = 3. * (pos + k) * (pos - n) / 4.;
//   N (2) = 3. * (pos + k) * (pos + n) / 4.;
//   N (3) = 3. * (pos - k) * (pos + n) / 4.;
// }

//-----------------------------------------------------------------------------
void ElQua4n2D::computeGlob2Loc()
//-----------------------------------------------------------------------------
{
  Matrix nds(getNumberOfNodes(), getNumberOfNodes());
  Vector crds(getNumberOfNodes());
  Indice i, j;
  Node *pnd;

  // calcul de la matrice
  for (i = 0; i < getNumberOfNodes(); i++)
  {
    pnd = nodes(i);
    nds(i, 0) = 1.;
    nds(i, 1) = pnd->coords(0);
    nds(i, 2) = pnd->coords(1);
    nds(i, 3) = pnd->coords(0) * pnd->coords(1);
  }

  // inversion de la matrice
  Matrix inv = nds.inverse();
  // cout << inv;

  // init
  glob2loc = 0.;

  // calcul des coefficients en x
  crds(0) = -1.;
  crds(1) = +1.;
  crds(2) = +1.;
  crds(3) = -1.;
  for (i = 0; i < getNumberOfNodes(); i++)
    for (j = 0; j < getNumberOfNodes(); j++)
      glob2loc(0, i) += inv(i, j) * crds(j);

  // calcul des coefficients en y
  crds(0) = -1.;
  crds(1) = -1.;
  crds(2) = +1.;
  crds(3) = +1.;
  for (i = 0; i < getNumberOfNodes(); i++)
    for (j = 0; j < getNumberOfNodes(); j++)
      glob2loc(1, i) += inv(i, j) * crds(j);
}

//-----------------------------------------------------------------------------
void ElQua4n2D::glob2Loc(const Vec3D &point, Vec3D &local)
//-----------------------------------------------------------------------------
{
  local(0) = glob2loc(0, 0) + point(0) * glob2loc(0, 1) + point(1) * glob2loc(0, 2) + point(0) * point(1) * glob2loc(0, 3);
  local(1) = glob2loc(1, 0) + point(0) * glob2loc(1, 1) + point(1) * glob2loc(1, 2) + point(0) * point(1) * glob2loc(1, 3);
  local(2) = 0.;
}
