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
  Class ElTet4n3D definition
*/

#include <Domain.h>
#include <io_Structure.h>
#include <Structure.h>
#include <IntegrationPoint.h>
#include <Node.h>
#include <ElTet4n3D.h>
//#include <Physic.h>
#include <Material.h>

// definitions locales
#define Beta 0.58541020
#define Alpha 0.13819660
#define Weight 1. / 6.
#define Weight4 1. / 24.

const ElementData ElTet4n3D::elementData = {
    // description de param�tres de l'�lement
    "ElTet4n3D",
    Element::ElTet4n3D,
    Element::Threedimensional,
    3,
    3,

    // description des noeuds de l'�lement
    4,
    {
        {Vec3D(0, 0, 0), 3, {1, 2, 3}, Vector(1, 1.0)},
        {Vec3D(1, 0, 0), 3, {0, 2, 3}, Vector(1, 1.0)},
        {Vec3D(0, 1, 0), 3, {0, 1, 3}, Vector(1, 1.0)},
        {Vec3D(0, 0, 1), 3, {0, 1, 2}, Vector(1, 1.0)}
        //     {Vec3D(0,0,0) , 3 , {1,2,3}, Vector(4, 1.9270509831248422723, -0.30901699437494742410, -0.30901699437494742410, -0.30901699437494742410)},
        //     {Vec3D(1,0,0) , 3 , {0,2,3}, Vector(4, -0.30901699437494742410, 1.9270509831248422723, -0.30901699437494742410, -0.30901699437494742410)},
        //     {Vec3D(0,1,0) , 3 , {0,1,3}, Vector(4, -0.30901699437494742410, -0.30901699437494742410, 1.9270509831248422723, -0.30901699437494742410)},
        //     {Vec3D(0,0,1) , 3 , {0,1,2}, Vector(4, -0.30901699437494742410, -0.30901699437494742410, -0.30901699437494742410, 1.9270509831248422723)}
    },

    // description des points d'int�gration
    1, // Sous integration � 1 point pour la m�canique suffisant
    {
        {Vec3D(0.25, 0.25, 0.25),
         Weight,
         Vector(4, 0.25, 0.25, 0.25, 0.25),
         Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)},
    },
    /* 4, // Integration � 4 point pour la m�canique
  {
    {
      Vec3D(Alpha,Alpha,Alpha),
      Weight4,
      Vector(4, 0.58541019662496845446, 0.13819660112501051518, 0.13819660112501051518, 0.13819660112501051518),
      Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)
    },
    {
      Vec3D(Beta,Alpha,Alpha),
      Weight4,
      Vector(4, 0.13819660112501051518, 0.58541019662496845446, 0.13819660112501051518, 0.13819660112501051518),
      Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)
    },
    {
      Vec3D(Alpha,Beta,Alpha),
      Weight4,
      Vector(4, 0.13819660112501051518, 0.13819660112501051518, 0.58541019662496845446, 0.13819660112501051518),
      Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)
    },
    {
      Vec3D(Alpha,Alpha,Beta),
      Weight4,
      Vector(4, 0.13819660112501051518, 0.13819660112501051518, 
0.13819660112501051518, 0.58541019662496845446),
      Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)
    },
  },*/

    // description des points de sous int�gration
    1,
    {
        {Vec3D(0.25, 0.25, 0.25),
         Weight,
         Vector(4, 0.25, 0.25, 0.25, 0.25),
         Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)},
    },

    // description des faces de l'�lement
    4,
    {{3, {0, 1, 3}},
     {3, {0, 2, 1}},
     {3, {0, 3, 2}},
     {3, {2, 3, 1}}},

    // description des sidefaces de l'�lement
    4,
    {{3, {0, 1, 3}},
     {3, {0, 2, 1}},
     {3, {0, 3, 2}},
     {3, {2, 3, 1}}},

    // description des ar�tes de l'�lement
    6,
    {{2, {0, 1}},
     {2, {1, 2}},
     {2, {2, 0}},
     {2, {0, 3}},
     {2, {1, 3}},
     {2, {2, 3}}},

    // end of data
    "end"};

//-----------------------------------------------------------------------------
ElTet4n3D::ElTet4n3D(Indice No_) : Element3D(No_)
//-----------------------------------------------------------------------------
{
  // redim du glob2loc ?
  glob2loc.redim(3, 4);

  Element::elementData = &elementData;
}

//-----------------------------------------------------------------------------
ElTet4n3D::ElTet4n3D(const ElTet4n3D &el) : Element3D(el)
//-----------------------------------------------------------------------------
{
  cout << "recopie d'un ElTet4n3D\n";
}

//-----------------------------------------------------------------------------
ElTet4n3D::~ElTet4n3D()
//-----------------------------------------------------------------------------
{
  nodes.flush();
}

//-----------------------------------------------------------------------------
Real ElTet4n3D::getLength()
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
Real ElTet4n3D::getVolume()
//-----------------------------------------------------------------------------
{
  cerr << "ElTet4n3D::getVolume() erreur ElHex8n3D non implemente\n";
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
ElTet4n3D::getUnderIntegrPointCoords (Indice i, Vec3D & coords, Real & weight)
//-----------------------------------------------------------------------------
{
  weight = 4./6.;
  coords = 0.;

  // retour
  return (Success);
}*/

//-----------------------------------------------------------------------------
void ElTet4n3D::getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
  Real k = point(0);
  Real n = point(1);
  Real z = point(2);
  N(0) = 1 - k - n - z;
  N(1) = k;
  N(2) = n;
  N(3) = z;
}

//-----------------------------------------------------------------------------
void ElTet4n3D::getDerShapeFunctionAtPoint(Matrix &dN, const Vec3D &point)
//-----------------------------------------------------------------------------
{
  // dN represente dN/dksi, dN/deta,...
  dN(0, 0) = -1.;
  dN(0, 1) = -1.;
  dN(0, 2) = -1.;
  dN(1, 0) = 1.;
  dN(1, 1) = 0.;
  dN(1, 2) = 0.;
  dN(2, 0) = 0.;
  dN(2, 1) = 1.;
  dN(2, 2) = 0.;
  dN(3, 0) = 0.;
  dN(3, 1) = 0.;
  dN(3, 2) = 1.;
}

// //-----------------------------------------------------------------------------
// void ElTet4n3D::getIntgtoNodes(Vector& N,const Vec3D& point) const
// //-----------------------------------------------------------------------------
// {
//   cout << N << point << endl;
//   cerr << "unknown ElTet4n3D::getIntgtoNodes\n";
//   exit(-1);
// }

//-----------------------------------------------------------------------------
void ElTet4n3D::computeGlob2Loc()
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
void ElTet4n3D::glob2Loc(const Vec3D &point, Vec3D &local)
//-----------------------------------------------------------------------------
{
  local(0) =
      glob2loc(0, 0) + point(0) * glob2loc(0, 1) + point(1) * glob2loc(0, 2) + point(2) * glob2loc(0, 3);
  local(1) =
      glob2loc(1, 0) + point(0) * glob2loc(1, 1) + point(1) * glob2loc(1, 2) + point(2) * glob2loc(1, 3);
  local(2) =
      glob2loc(2, 0) + point(0) * glob2loc(2, 1) + point(1) * glob2loc(2, 2) + point(2) * glob2loc(2, 3);
}
