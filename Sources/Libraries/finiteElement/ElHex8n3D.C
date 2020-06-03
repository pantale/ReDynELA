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
  Class ElHex8n3D definition
*/

#include <ElHex8n3D.h>
#include <Node.h>

const ElementData ElHex8n3D::elementData = {
    // description de param�tres de l'�lement
    "ElHex8n3D",
    Element::ElHex8n3D,
    Element::Threedimensional,
    3,
    3,

// description des noeuds de l'�lement
#define cinqP3R3s4 (5.0 + 3.0 * sqrt(3.0)) / 4.0
#define cinqM3R3s4 (5.0 - 3.0 * sqrt(3.0)) / 4.0
#define R3P1s4 (sqrt(3.0) + 1.0) / 4.0
#define R3M1s4 (sqrt(3.0) - 1.0) / 4.0

    8,
    {{Vec3D(-1, -1, -1),
      3,
      {1, 3, 4},
      Vector(8, cinqP3R3s4, -R3P1s4, R3M1s4, -R3P1s4, -R3P1s4, R3M1s4, cinqM3R3s4, R3M1s4)},
     {Vec3D(+1, -1, -1),
      3,
      {0, 5, 2},
      Vector(8, -R3P1s4, cinqP3R3s4, -R3P1s4, R3M1s4, R3M1s4, -R3P1s4, R3M1s4, cinqM3R3s4)},
     {Vec3D(+1, +1, -1),
      3,
      {1, 6, 3},
      Vector(8, R3M1s4, -R3P1s4, cinqP3R3s4, -R3P1s4, cinqM3R3s4, R3M1s4, -R3P1s4, R3M1s4)},
     {Vec3D(-1, +1, -1),
      3,
      {2, 0, 7},
      Vector(8, -R3P1s4, R3M1s4, -R3P1s4, cinqP3R3s4, R3M1s4, cinqM3R3s4, R3M1s4, -R3P1s4)},
     {Vec3D(-1, -1, +1),
      3,
      {0, 5, 7},
      Vector(8, -R3P1s4, R3M1s4, cinqM3R3s4, R3M1s4, cinqP3R3s4, -R3P1s4, R3M1s4, -R3P1s4)},
     {Vec3D(+1, -1, +1),
      3,
      {1, 4, 6},
      Vector(8, R3M1s4, -R3P1s4, R3M1s4, cinqM3R3s4, -R3P1s4, cinqP3R3s4, -R3P1s4, R3M1s4)},
     {Vec3D(+1, +1, +1),
      3,
      {5, 7, 2},
      Vector(8, cinqM3R3s4, R3M1s4, -R3P1s4, R3M1s4, R3M1s4, -R3P1s4, cinqP3R3s4, -R3P1s4)},
     {Vec3D(-1, +1, +1),
      3,
      {3, 6, 4},
      Vector(8, R3M1s4, cinqM3R3s4, R3M1s4, -R3P1s4, -R3P1s4, R3M1s4, -R3P1s4, cinqP3R3s4)}},

    // description des points d'int�gration
    8,
    {{
#define deuxPR3s12 (2.0 + sqrt(3.0)) / 12.0
#define uns12 1.0 / 12.0
#define deuxMR3s12 (2.0 - sqrt(3.0)) / 12.0
#define neufP5R3s36 (9.0 + 5.0 * sqrt(3.0)) / 36.0
#define troisPR3s36 (3.0 + sqrt(3.0)) / 36.0
#define troisMR3s36 (3.0 - sqrt(3.0)) / 36.0
#define neufM5R3s36 (9.0 - 5.0 * sqrt(3.0)) / 36.0

         Vec3D(-invSqrt3, -invSqrt3, -invSqrt3),
         1,
         Vector(8, neufP5R3s36, troisPR3s36, troisMR3s36, troisPR3s36, troisPR3s36, troisMR3s36, neufM5R3s36, troisMR3s36),
         Matrix(8, 3, -deuxPR3s12, -deuxPR3s12, -deuxPR3s12, deuxPR3s12, -uns12, -uns12, uns12, uns12, -deuxMR3s12, -uns12, deuxPR3s12, -uns12, -uns12, -uns12, deuxPR3s12, uns12, -deuxMR3s12, uns12, deuxMR3s12, deuxMR3s12, deuxMR3s12, -deuxMR3s12, uns12, uns12)},
     {Vec3D(+invSqrt3, -invSqrt3, -invSqrt3),
      1,
      Vector(8, troisPR3s36, neufP5R3s36, troisPR3s36, troisMR3s36, troisMR3s36, troisPR3s36, troisMR3s36, neufM5R3s36),
      Matrix(8, 3, -deuxPR3s12, -uns12, -uns12, deuxPR3s12, -deuxPR3s12, -deuxPR3s12, uns12, deuxPR3s12, -uns12, -uns12, uns12, -deuxMR3s12, -uns12, -deuxMR3s12, uns12, uns12, -uns12, deuxPR3s12, deuxMR3s12, uns12, uns12, -deuxMR3s12, deuxMR3s12, deuxMR3s12)},
     {Vec3D(+invSqrt3, +invSqrt3, -invSqrt3),
      1,
      Vector(8, troisMR3s36, troisPR3s36, neufP5R3s36, troisPR3s36, neufM5R3s36, troisMR3s36, troisPR3s36, troisMR3s36),
      Matrix(8, 3, -uns12, -uns12, -deuxMR3s12, uns12, -deuxPR3s12, -uns12, deuxPR3s12, deuxPR3s12, -deuxPR3s12, -deuxPR3s12, uns12, -uns12, -deuxMR3s12, -deuxMR3s12, deuxMR3s12, deuxMR3s12, -uns12, uns12, uns12, uns12, deuxPR3s12, -uns12, deuxMR3s12, uns12)},
     {Vec3D(-invSqrt3, +invSqrt3, -invSqrt3),
      1,
      Vector(8, troisPR3s36, troisMR3s36, troisPR3s36, neufP5R3s36, troisMR3s36, neufM5R3s36, troisMR3s36, troisPR3s36),
      Matrix(8, 3, -uns12, -deuxPR3s12, -uns12, uns12, -uns12, -deuxMR3s12, deuxPR3s12, uns12, -uns12, -deuxPR3s12, deuxPR3s12, -deuxPR3s12, -deuxMR3s12, -uns12, uns12, deuxMR3s12, -deuxMR3s12, deuxMR3s12, uns12, deuxMR3s12, uns12, -uns12, uns12, deuxPR3s12)},
     {Vec3D(-invSqrt3, -invSqrt3, +invSqrt3),
      1,
      Vector(8, troisPR3s36, troisMR3s36, neufM5R3s36, troisMR3s36, neufP5R3s36, troisPR3s36, troisMR3s36, troisPR3s36),
      Matrix(8, 3, -uns12, -uns12, -deuxPR3s12, uns12, -deuxMR3s12, -uns12, deuxMR3s12, deuxMR3s12, -deuxMR3s12, -deuxMR3s12, uns12, -uns12, -deuxPR3s12, -deuxPR3s12, deuxPR3s12, deuxPR3s12, -uns12, uns12, uns12, uns12, deuxMR3s12, -uns12, deuxPR3s12, uns12)},
     {Vec3D(+invSqrt3, -invSqrt3, +invSqrt3),
      1,
      Vector(8, troisMR3s36, troisPR3s36, troisMR3s36, neufM5R3s36, troisPR3s36, neufP5R3s36, troisPR3s36, troisMR3s36),
      Matrix(8, 3, -uns12, -deuxMR3s12, -uns12, uns12, -uns12, -deuxPR3s12, deuxMR3s12, uns12, -uns12, -deuxMR3s12, deuxMR3s12, -deuxMR3s12, -deuxPR3s12, -uns12, uns12, deuxPR3s12, -deuxPR3s12, deuxPR3s12, uns12, deuxPR3s12, uns12, -uns12, uns12, deuxMR3s12)},
     {Vec3D(+invSqrt3, +invSqrt3, +invSqrt3),
      1,
      Vector(8, neufM5R3s36, troisMR3s36, troisPR3s36, troisMR3s36, troisMR3s36, troisPR3s36, neufP5R3s36, troisPR3s36),
      Matrix(8, 3, -deuxMR3s12, -deuxMR3s12, -deuxMR3s12, deuxMR3s12, -uns12, -uns12, uns12, uns12, -deuxPR3s12, -uns12, deuxMR3s12, -uns12, -uns12, -uns12, deuxMR3s12, uns12, -deuxPR3s12, uns12, deuxPR3s12, deuxPR3s12, deuxPR3s12, -deuxPR3s12, uns12, uns12)},
     {Vec3D(-invSqrt3, +invSqrt3, +invSqrt3),
      1,
      Vector(8, troisMR3s36, neufM5R3s36, troisMR3s36, troisPR3s36, troisPR3s36, troisMR3s36, troisPR3s36, neufP5R3s36),
      Matrix(8, 3, -deuxMR3s12, -uns12, -uns12, deuxMR3s12, -deuxMR3s12, -deuxMR3s12, uns12, deuxMR3s12, -uns12, -uns12, uns12, -deuxPR3s12, -uns12, -deuxPR3s12, uns12, uns12, -uns12, deuxMR3s12, deuxPR3s12, uns12, uns12, -deuxPR3s12, deuxPR3s12, deuxPR3s12)}},

    1,
    {
        {Vec3D(0.0, 0.0, 0.0),
         8.0,
         Vector(8, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125),
         Matrix(8, 3, -0.125, 0.125, 0.125, -0.125, -0.125, 0.125, 0.125, -0.125, -0.125, -0.125, 0.125, 0.125, -0.125, -0.125, 0.125, 0.125, -0.125, -0.125, -0.125,
                -0.125, 0.125, 0.125, 0.125, 0.125)},
    },

    // description des faces de l'�lement
    6,
    {{4, {0, 1, 5, 4}},
     {4, {1, 2, 6, 5}},
     {4, {2, 3, 7, 6}},
     {4, {3, 0, 4, 7}},
     {4, {7, 4, 5, 6}},
     {4, {2, 1, 0, 3}}},

    // description des sidefaces de l'�lement
    6,
    {{4, {0, 1, 5, 4}},
     {4, {1, 2, 6, 5}},
     {4, {2, 3, 7, 6}},
     {4, {3, 0, 4, 7}},
     {4, {7, 4, 5, 6}},
     {4, {2, 1, 0, 3}}},

    // description des ar�tes de l'�lement
    12,
    {{2, {0, 1}},
     {2, {1, 2}},
     {2, {2, 3}},
     {2, {3, 0}},
     {2, {4, 5}},
     {2, {5, 6}},
     {2, {6, 7}},
     {2, {7, 4}},
     {2, {0, 4}},
     {2, {1, 5}},
     {2, {2, 6}},
     {2, {3, 7}}},

    // end of data
    "end"};

//-----------------------------------------------------------------------------
ElHex8n3D::ElHex8n3D(Indice No_) : Element3D(No_)
//-----------------------------------------------------------------------------
{
  // redim glob2loc ?
  glob2loc.redim(3, 8);

  Element::elementData = &elementData;
}

//-----------------------------------------------------------------------------
ElHex8n3D::ElHex8n3D(const ElHex8n3D &el) : Element3D(el)
//-----------------------------------------------------------------------------
{
  cout << "recopie d'un ElHex8n3D\n";
}

//-----------------------------------------------------------------------------
ElHex8n3D::~ElHex8n3D()
//-----------------------------------------------------------------------------
{
  nodes.flush();
}

//-----------------------------------------------------------------------------
Real ElHex8n3D::getLength()
//-----------------------------------------------------------------------------
{
  cerr << "erreur ElHex8n3D non implemente\n";
  exit(-1);

  Real l1, l2, l3, l4, l5, ll;
  l1 = nodes(1)->coords.distance(nodes(2)->coords);
  l2 = nodes(2)->coords.distance(nodes(3)->coords);
  l3 = nodes(3)->coords.distance(nodes(4)->coords);
  l4 = nodes(4)->coords.distance(nodes(1)->coords);
  l5 = nodes(1)->coords.distance(nodes(3)->coords);
  ll = Max(l1, l2);
  ll = Max(ll, l3);
  ll = Max(ll, l4);
  return ((Surf_Tri(l1, l2, l5) + Surf_Tri(l3, l4, l5)) / ll);
}

//-----------------------------------------------------------------------------
Real ElHex8n3D::getVolume()
//-----------------------------------------------------------------------------
{
  cerr << "erreur ElHex8n3D non implemente\n";
  exit(-1);

  Real l1, l2, l3, l4, l5;
  l1 = nodes(1)->coords.distance(nodes(2)->coords);
  l2 = nodes(2)->coords.distance(nodes(3)->coords);
  l3 = nodes(3)->coords.distance(nodes(4)->coords);
  l4 = nodes(4)->coords.distance(nodes(1)->coords);
  l5 = nodes(1)->coords.distance(nodes(3)->coords);
  return (Surf_Tri(l1, l2, l5) + Surf_Tri(l3, l4, l5));
}

//! calcul du jacobien, fonctions d'interpolations ...
/*!
Cette m�thode calcule le jacobien de l'�l�ment, les fonctions d'interpolations de l'�l�ment sur le domaine courant pour chaque point d'int�gration de l'�l�ment ainsi que les d�riv�es des fonctions d'interpolation de l'�l�ment par rapport aux coordonn�es physiques de l'�l�ment
*/
// //-----------------------------------------------------------------------------
// void
// ElHex8n3D::computeInternalMatrices (Boolean reference)
// //-----------------------------------------------------------------------------
// {
//   Element::computeInternalMatrices(reference);
// }

/*//-----------------------------------------------------------------------------
Boolean
ElHex8n3D::getUnderIntegrPointCoords (Indice i, Vec3D & coords, Real & weight)
//-----------------------------------------------------------------------------
{
  weight = 8.;
  coords = 0.;

  // retour
  return (Success);
}*/

//-----------------------------------------------------------------------------
void ElHex8n3D::getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
  Real k = point(0);
  Real n = point(1);
  Real z = point(2);

  N(0) = (1.0 - k) * (1.0 - n) * (1.0 - z) / 8.0;
  N(1) = (1.0 + k) * (1.0 - n) * (1.0 - z) / 8.0;
  N(2) = (1.0 + k) * (1.0 + n) * (1.0 - z) / 8.0;
  N(3) = (1.0 - k) * (1.0 + n) * (1.0 - z) / 8.0;
  N(4) = (1.0 - k) * (1.0 - n) * (1.0 + z) / 8.0;
  N(5) = (1.0 + k) * (1.0 - n) * (1.0 + z) / 8.0;
  N(6) = (1.0 + k) * (1.0 + n) * (1.0 + z) / 8.0;
  N(7) = (1.0 - k) * (1.0 + n) * (1.0 + z) / 8.0;
}

//-----------------------------------------------------------------------------
void ElHex8n3D::getDerShapeFunctionAtPoint(Matrix &N, const Vec3D &point)
//-----------------------------------------------------------------------------
{
  Real k = point(0);
  Real n = point(1);
  Real z = point(2);

  N(0, 0) = -(1.0 - n) * (1.0 - z) / 8.0;
  N(0, 1) = -(1.0 - k) * (1.0 - z) / 8.0;
  N(0, 2) = -(1.0 - k) * (1.0 - n) / 8.0;
  N(1, 0) = +(1.0 - n) * (1.0 - z) / 8.0;
  N(1, 1) = -(1.0 + k) * (1.0 - z) / 8.0;
  N(1, 2) = -(1.0 + k) * (1.0 - n) / 8.0;
  N(2, 0) = +(1.0 + n) * (1.0 - z) / 8.0;
  N(2, 1) = +(1.0 + k) * (1.0 - z) / 8.0;
  N(2, 2) = -(1.0 + k) * (1.0 + n) / 8.0;
  N(3, 0) = -(1.0 + n) * (1.0 - z) / 8.0;
  N(3, 1) = +(1.0 - k) * (1.0 - z) / 8.0;
  N(3, 2) = -(1.0 - k) * (1.0 + n) / 8.0;
  N(4, 0) = -(1.0 - n) * (1.0 + z) / 8.0;
  N(4, 1) = -(1.0 - k) * (1.0 + z) / 8.0;
  N(4, 2) = +(1.0 - k) * (1.0 - n) / 8.0;
  N(5, 0) = +(1.0 - n) * (1.0 + z) / 8.0;
  N(5, 1) = -(1.0 + k) * (1.0 + z) / 8.0;
  N(5, 2) = +(1.0 + k) * (1.0 - n) / 8.0;
  N(6, 0) = +(1.0 + n) * (1.0 + z) / 8.0;
  N(6, 1) = +(1.0 + k) * (1.0 + z) / 8.0;
  N(6, 2) = +(1.0 + k) * (1.0 + n) / 8.0;
  N(7, 0) = -(1.0 + n) * (1.0 + z) / 8.0;
  N(7, 1) = +(1.0 - k) * (1.0 + z) / 8.0;
  N(7, 2) = +(1.0 - k) * (1.0 + n) / 8.0;
}

// //-----------------------------------------------------------------------------
// void
// ElHex8n3D::getIntgtoNodes (Vector & N, const Vec3D & point) const
// //-----------------------------------------------------------------------------
// {
//   cout << N << point << endl;
//   cerr << "unknown ElHex8n3D::getIntgtoNodes\n";
//   exit (-1);
// }

//-----------------------------------------------------------------------------
void ElHex8n3D::computeGlob2Loc()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ElHex8n3D::glob2Loc(const Vec3D &point, Vec3D &local)
//-----------------------------------------------------------------------------
{
  cout << point << local << endl;
}
