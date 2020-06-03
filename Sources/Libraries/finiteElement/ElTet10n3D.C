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
  Class ElTet10n3D definition
*/

#include <Domain.h>
#include <io_Structure.h>
#include <Structure.h>
#include <IntegrationPoint.h>
#include <Node.h>
#include <ElTet10n3D.h>
//#include <Physic.h>
#include <Material.h>

// definitions locales
#define Beta 0.585410196624968
#define Alpha 0.138196601125011
#define Weight 1.0 / 24.0

const ElementData ElTet10n3D::elementData = {
    // description de param�tres de l'�lement
    "ElTet10n3D",
    Element::ElTet10n3D,
    Element::Threedimensional,
    3,
    3,

    // description des noeuds de l'�lement
    10,
    {
#define unP3R5s4 (1.0 + 3.0 * sqrt(5.0)) / 4.0
#define unMR5s4 (1.0 - sqrt(5.0)) / 4.0
#define unPR5s4 (1.0 + sqrt(5.0)) / 4.0
        {Vec3D(0.0, 0.0, 0.0), 3, {4, 6, 7}, Vector(4, unP3R5s4, unMR5s4, unMR5s4, unMR5s4)},
        {Vec3D(1.0, 0.0, 0.0), 3, {4, 5, 8}, Vector(4, unMR5s4, unP3R5s4, unMR5s4, unMR5s4)},
        {Vec3D(0.0, 1.0, 0.0), 3, {6, 5, 9}, Vector(4, unMR5s4, unMR5s4, unP3R5s4, unMR5s4)},
        {Vec3D(0.0, 0.0, 1.0), 3, {7, 8, 9}, Vector(4, unMR5s4, unMR5s4, unMR5s4, unP3R5s4)},
        {Vec3D(0.5, 0.0, 0.0), 2, {0, 1}, Vector(4, unPR5s4, unPR5s4, unMR5s4, unMR5s4)},
        {Vec3D(0.5, 0.5, 0.0), 2, {1, 2}, Vector(4, unMR5s4, unPR5s4, unPR5s4, unMR5s4)},
        {Vec3D(0.0, 0.5, 0.0), 2, {0, 2}, Vector(4, unPR5s4, unMR5s4, unPR5s4, unMR5s4)},
        {Vec3D(0.0, 0.0, 0.5), 2, {0, 3}, Vector(4, unPR5s4, unMR5s4, unMR5s4, unPR5s4)},
        {Vec3D(0.5, 0.0, 0.5), 2, {1, 3}, Vector(4, unMR5s4, unPR5s4, unMR5s4, unPR5s4)},
        {Vec3D(0.0, 0.5, 0.5), 2, {2, 3}, Vector(4, unMR5s4, unMR5s4, unPR5s4, unPR5s4)}},

    // description des points d'int�gration
    4,
    {
#define troissR5 3.0 / sqrt(5.0)
#define unsR5 1.0 / sqrt(5.0)                // 0.44721359549995793928
#define unM1sR5 1.0 - 1.0 / sqrt(5.0)        // 0.55278640450004206072
#define quatresR5 4.0 / sqrt(5.0)            // 1.7888543819998317571
#define unP3sR5 1.0 + 3.0 / sqrt(5.0)        // 2.3416407864998738178
#define unPR5s10 (1.0 + sqrt(5.0)) / 10.0    // 0.32360679774997896964
#define troisMR5s10 (3.0 - sqrt(5.0)) / 10.0 // 0.076393202250021030359
        {Vec3D(Alpha, Alpha, Alpha),
         Weight,
         Vector(10, 0.1, -0.1, -0.1, -0.1, unPR5s10, troisMR5s10, unPR5s10, unPR5s10, troisMR5s10, troisMR5s10),
         Matrix(10, 3, -troissR5, -troissR5, -troissR5, -unsR5, 0.0, 0.0, 0.0, -unsR5, 0.0, 0.0, 0.0, -unsR5, quatresR5, -unM1sR5, -unM1sR5, unM1sR5, unM1sR5, 0.0, -unM1sR5, quatresR5, -unM1sR5, -unM1sR5, -unM1sR5, quatresR5, unM1sR5, 0.0, unM1sR5, 0.0, unM1sR5, unM1sR5)},
        {Vec3D(Beta, Alpha, Alpha),
         Weight,
         Vector(10, -0.1, 0.1, -0.1, -0.1, unPR5s10, unPR5s10, troisMR5s10, troisMR5s10, unPR5s10, troisMR5s10),
         Matrix(10, 3, unsR5, unsR5, unsR5, troissR5, 0.0, 0.0, 0.0, -unsR5, 0.0, 0.0, 0.0, -unsR5, -quatresR5, -unP3sR5, -unP3sR5, unM1sR5, unP3sR5, 0.0, -unM1sR5, 0.0, -unM1sR5, -unM1sR5, -unM1sR5, 0.0, unM1sR5, 0.0, unP3sR5, 0.0, unM1sR5, unM1sR5)},
        {Vec3D(Alpha, Beta, Alpha),
         Weight,
         Vector(10, -0.1, -0.1, 0.1, -0.1, troisMR5s10, unPR5s10, unPR5s10, troisMR5s10, troisMR5s10, unPR5s10),
         Matrix(10, 3, unsR5, unsR5, unsR5, -unsR5, 0.0, 0.0, 0.0, troissR5, 0.0, 0.0, 0.0, -unsR5, 0.0, -unM1sR5, -unM1sR5, unP3sR5, unM1sR5, 0.0, -unP3sR5, -quatresR5, -unP3sR5, -unM1sR5, -unM1sR5, 0.0, unM1sR5, 0.0, unM1sR5, 0.0, unM1sR5, unP3sR5)},
        {Vec3D(Alpha, Alpha, Beta),
         Weight,
         Vector(10, -0.1, -0.1, -0.1, 0.1, troisMR5s10, troisMR5s10, troisMR5s10, unPR5s10, unPR5s10, unPR5s10),
         Matrix(10, 3, unsR5, unsR5, unsR5, -unsR5, 0.0, 0.0, 0.0, -unsR5, 0.0, 0.0, 0.0, troissR5, 0.0, -unM1sR5, -unM1sR5, unM1sR5, unM1sR5, 0.0, -unM1sR5, 0.0, -unM1sR5, -unP3sR5, -unP3sR5, -quatresR5, unP3sR5, 0.0, unM1sR5, 0.0, unP3sR5, unM1sR5)}},

    // description des points de sous int�gration
    1, // Sous integration � 1 point pour la m�canique suffisant
    {
        {Vec3D(0.25, 0.25, 0.25),
         4.0 * Weight,
         Vector(4, 0.25, 0.25, 0.25, 0.25),
         Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)},
    },

    // description des faces de l'�lement
    4,
    {{6, {0, 4, 1, 8, 3, 7}},
     {6, {0, 6, 2, 5, 1, 4}},
     {6, {0, 7, 3, 9, 2, 6}},
     {6, {2, 9, 3, 8, 1, 5}}},

    // description des sidefaces de l'�lement
    4,
    {{6, {0, 4, 1, 8, 3, 7}},
     {6, {0, 6, 2, 5, 1, 4}},
     {6, {0, 7, 3, 9, 2, 6}},
     {6, {2, 9, 3, 8, 1, 5}}},

    // description des ar�tes de l'�lement
    6,
    {{3, {0, 4, 1}},
     {3, {1, 5, 2}},
     {3, {2, 6, 0}},
     {3, {0, 7, 3}},
     {3, {1, 8, 3}},
     {3, {2, 9, 3}}},

    // end of data
    "end"};

//-----------------------------------------------------------------------------
ElTet10n3D::ElTet10n3D(Indice No_) : Element3D(No_)
//-----------------------------------------------------------------------------
{
  // redim du glob2loc ?
  glob2loc.redim(3, 10);

  Element::elementData = &elementData;
}

//-----------------------------------------------------------------------------
ElTet10n3D::ElTet10n3D(const ElTet10n3D &el) : Element3D(el)
//-----------------------------------------------------------------------------
{
  cout << "recopie d'un ElTet10n3D\n";
}

//-----------------------------------------------------------------------------
ElTet10n3D::~ElTet10n3D()
//-----------------------------------------------------------------------------
{
  nodes.flush();
}

//-----------------------------------------------------------------------------
Real ElTet10n3D::getLength()
//-----------------------------------------------------------------------------
{
  Real l1, l2, l3, l4, l5, l6, ll;
  l1 = nodes(0)->coords.distance(nodes(1)->coords);
  l2 = nodes(0)->coords.distance(nodes(2)->coords);
  l3 = nodes(1)->coords.distance(nodes(2)->coords);
  l4 = nodes(0)->coords.distance(nodes(3)->coords);
  l5 = nodes(1)->coords.distance(nodes(3)->coords);
  l6 = nodes(2)->coords.distance(nodes(3)->coords);
  ll = Min(l1, l2);
  ll = Min(ll, l3);
  ll = Min(ll, l4);
  ll = Min(ll, l5);
  ll = Min(ll, l6);
  return ll;
}

//-----------------------------------------------------------------------------
Real ElTet10n3D::getVolume()
//-----------------------------------------------------------------------------
{
  cerr << "ElTet10n3D::getVolume() erreur ElHex8n3D non implemente\n";
  exit(-1);

  Real l1, l2, l3, l4, l5;
  l1 = nodes(1)->coords.distance(nodes(2)->coords);
  l2 = nodes(2)->coords.distance(nodes(3)->coords);
  l3 = nodes(3)->coords.distance(nodes(4)->coords);
  l4 = nodes(4)->coords.distance(nodes(1)->coords);
  l5 = nodes(1)->coords.distance(nodes(3)->coords);
  return (Surf_Tri(l1, l2, l5) + Surf_Tri(l3, l4, l5));
}

/*//-----------------------------------------------------------------------------
Boolean
ElTet10n3D::getUnderIntegrPointCoords (Indice i, Vec3D & coords, Real & weight)
//-----------------------------------------------------------------------------
{
  weight = 1.0/6.0;
  coords = 0.25;

  // retour
  return (Success);
}*/

//-----------------------------------------------------------------------------
void ElTet10n3D::getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
  Real ksi = point(0);
  Real eta = point(1);
  Real zeta = point(2);
  Real xez = 1.0 - ksi - eta - zeta;

  N(0) = xez * (2.0 * xez - 1.0);
  N(1) = ksi * (2.0 * ksi - 1.0);
  N(2) = eta * (2.0 * eta - 1.0);
  N(3) = zeta * (2.0 * zeta - 1.0);
  N(4) = 4.0 * xez * ksi;
  N(5) = 4.0 * ksi * eta;
  N(6) = 4.0 * xez * eta;
  N(7) = 4.0 * xez * zeta;
  N(8) = 4.0 * ksi * zeta;
  N(9) = 4.0 * eta * zeta;
}

//-----------------------------------------------------------------------------
void ElTet10n3D::getDerShapeFunctionAtPoint(Matrix &dN, const Vec3D &point)
//-----------------------------------------------------------------------------
{
  Real ksi = point(0);
  Real eta = point(1);
  Real zeta = point(2);
  Real xez = 1.0 - ksi - eta - zeta;

  // dN represente dN/dksi, dN/deta,...
  dN(0, 0) = 1.0 - 4.0 * xez;
  dN(0, 1) = 1.0 - 4.0 * xez;
  dN(0, 2) = 1.0 - 4.0 * xez;
  dN(1, 0) = 4.0 * ksi - 1.0;
  dN(1, 1) = 0.0;
  dN(1, 2) = 0.0;
  dN(2, 0) = 0.0;
  dN(2, 1) = 4.0 * eta - 1.0;
  dN(2, 2) = 0.0;
  dN(3, 0) = 0.0;
  dN(3, 1) = 0.0;
  dN(3, 2) = 4.0 * zeta - 1.0;
  dN(4, 0) = 4.0 * (xez - ksi);
  dN(4, 1) = -4.0 * ksi;
  dN(4, 2) = -4.0 * ksi;
  dN(5, 0) = 4.0 * eta;
  dN(5, 1) = 4.0 * ksi;
  dN(5, 2) = 0.0;
  dN(6, 0) = -4.0 * eta;
  dN(6, 1) = 4.0 * (xez - eta);
  dN(6, 2) = -4.0 * eta;
  dN(7, 0) = -4.0 * zeta;
  dN(7, 1) = -4.0 * zeta;
  dN(7, 2) = 4.0 * (xez - zeta);
  dN(8, 0) = 4.0 * zeta;
  dN(8, 1) = 0.0;
  dN(8, 2) = 4.0 * ksi;
  dN(9, 0) = 0.0;
  dN(9, 1) = 4.0 * zeta;
  dN(9, 2) = 4.0 * eta;
}

// //-----------------------------------------------------------------------------
// void ElTet10n3D::getIntgtoNodes(Vector& N,const Vec3D& point) const
// //-----------------------------------------------------------------------------
// {
//   cout << N << point << endl;
//   cerr << "unknown ElTet10n3D::getIntgtoNodes\n";
//   exit(-1);
// }

//-----------------------------------------------------------------------------
void ElTet10n3D::computeGlob2Loc()
//-----------------------------------------------------------------------------
{
  Matrix nds(getNumberOfNodes(), getNumberOfNodes());
  Matrix cords(getNumberOfNodes(), 3);
  Indice i, j;
  Node *pnd;

  // calcul de la matrice
  for (i = 0; i < getNumberOfNodes(); i++)
  {
    pnd = nodes(i);
    nds(i, 0) = 1;
    nds(i, 1) = pnd->coords(0);
    nds(i, 2) = pnd->coords(1);
    nds(i, 3) = pnd->coords(2);
  }

  // inversion de la matrice
  Matrix inv = nds.inverse();

  for (i = 0; i < getNumberOfNodes(); i++)
    for (j = 0; j < 3; j++)
      cords(i, j) = getLocalNodeCoords(i)(j);

  glob2loc = (inv * cords).Transpose();
}

//-----------------------------------------------------------------------------
void ElTet10n3D::glob2Loc(const Vec3D &point, Vec3D &local)
//-----------------------------------------------------------------------------
{
  local(0) =
      glob2loc(0, 0) + point(0) * glob2loc(0, 1) + point(1) * glob2loc(0, 2) + point(2) * glob2loc(0, 3);
  local(1) =
      glob2loc(1, 0) + point(0) * glob2loc(1, 1) + point(1) * glob2loc(1, 2) + point(2) * glob2loc(1, 3);
  local(2) =
      glob2loc(2, 0) + point(0) * glob2loc(2, 1) + point(1) * glob2loc(2, 2) + point(2) * glob2loc(2, 3);
}
