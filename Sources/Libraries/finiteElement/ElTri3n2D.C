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
  Class ElTri3n2D definition
*/

#include <Domain.h>
#include <io_Structure.h>
#include <Structure.h>
#include <IntegrationPoint.h>
#include <Node.h>
#include <ElTri3n2D.h>
//#include <Physic.h>

const ElementData ElTri3n2D::elementData = {
    // description de param�tres de l'�lement
    "ElTri3n2D",
    Element::ElTri3n2D,
    Element::Bidimensional,
    2,
    2,

    // description des noeuds de l'�lement
    3,
    {{Vec3D(0, 0, 0), 2, {1, 2}, Vector()},
     {Vec3D(0, 0, 0), 2, {0, 2}, Vector()},
     {Vec3D(0, 0, 0), 2, {1, 0}, Vector()}},

    // description des points d'int�gration
    1,
    {{Vec3D(-invSqrt3, -invSqrt3, 0),
      1,
      Vector(),
      Matrix()}},

    // description des points de sous int�gration
    1,
    {{Vec3D(-invSqrt3, -invSqrt3, 0),
      1,
      Vector(),
      Matrix()}},

    // description des faces de l'�lement
    1,
    {{3, {0, 1, 2}}},

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
ElTri3n2D::ElTri3n2D(Indice No_) : ElementPlane(No_)
//-----------------------------------------------------------------------------
{
  // affectation de taille pour glob2loc
  glob2loc.redim(2, 4);

  Element::elementData = &elementData;
}

//-----------------------------------------------------------------------------
ElTri3n2D::ElTri3n2D(const ElTri3n2D &el) : ElementPlane(el)
//-----------------------------------------------------------------------------
{
  cout << "recopie d'un ElTri3n2D\n";
}

//-----------------------------------------------------------------------------
ElTri3n2D::~ElTri3n2D()
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
Real ElTri3n2D::getLength()
//-----------------------------------------------------------------------------
{
  Real x31, y42, x24, y31;
  x31 = nodes(2)->coords(0) - nodes(0)->coords(0);
  x24 = nodes(1)->coords(0) - nodes(3)->coords(0);
  y42 = nodes(3)->coords(1) - nodes(1)->coords(1);
  y31 = nodes(2)->coords(1) - nodes(0)->coords(1);

  // modif DynELA 0.9.5
  // correction d'un bug, merci Maxima
  return ((x31 * y42 +
           x24 * y31) /
          sqrt(2. * (SQ(x24) + SQ(y42) + SQ(x31) +
                     SQ(y31))));
}

//-----------------------------------------------------------------------------
Real ElTri3n2D::getVolume()
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

/*//-----------------------------------------------------------------------------
Boolean
ElTri3n2D::getUnderIntegrPointCoords (Indice i, Vec3D & coords, Real & weight)
//-----------------------------------------------------------------------------
{
  weight = 4.;
  coords = 0.;

  // retour
  return (Success);
}*/

//-----------------------------------------------------------------------------
void ElTri3n2D::getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const
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
void ElTri3n2D::getDerShapeFunctionAtPoint(Matrix &N, const Vec3D &point)
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

//! calcul du jacobien, fonctions d'interpolations ...
/*!
Cette m�thode calcule le jacobien de l'�l�ment, les fonctions d'interpolations de l'�l�ment sur le domaine courant pour chaque point d'int�gration de l'�l�ment ainsi que les d�riv�es des fonctions d'interpolation de l'�l�ment par rapport aux coordonn�es physiques de l'�l�ment
*/
/*//-----------------------------------------------------------------------------
void
ElTri3n2D::computeInternalMatrices (Boolean reference)
//-----------------------------------------------------------------------------
{
  return;

  // coordonn�es de noeuds
  Real x1 = nodes (0)->coords (0);
  Real x2 = nodes (1)->coords (0);
  Real x3 = nodes (2)->coords (0);
  Real x4 = nodes (3)->coords (0);
  Real y1 = nodes (0)->coords (1);
  Real y2 = nodes (1)->coords (1);
  Real y3 = nodes (2)->coords (1);
  Real y4 = nodes (3)->coords (1);

  // variables de calcul
  Real sqrt3 = sqrt (3.0);
  Real sqrt3p1 = sqrt3 + 1.0;
  Real sqrt3p2 = sqrt3 + 2.0;
  Real sqrt3p3 = sqrt3 + 3.0;
  Real sqrt3m1 = sqrt3 - 1.0;
  Real sqrt3m2 = sqrt3 - 2.0;
  Real sqrt3m3 = sqrt3 - 3.0;

  // variables temporaires
  Real denom1, denom2;

  // calcul du point 0
  getUnderIntegrationPoint (0);
  denom1 = ((x3 - x1) * (y4 - y2) + (x2 - x4) * (y3 - y1));
  underRef->dN (0, 0) = (y2 - y4) / denom1;
  underRef->dN (0, 1) = (x4 - x2) / denom1;
  underRef->dN (1, 0) = (y3 - y1) / denom1;
  underRef->dN (1, 1) = (x1 - x3) / denom1;
  underRef->dN (2, 0) = (y4 - y2) / denom1;
  underRef->dN (2, 1) = (x2 - x4) / denom1;
  underRef->dN (3, 0) = (y1 - y3) / denom1;
  underRef->dN (3, 1) = (x3 - x1) / denom1;
  underRef->N = 1. / 4.;
  underRef->detJ = denom1 / 8.0;
  if (reference) underRef->detJ0 = underRef->detJ;

  // calcul du point 1
  getIntegrationPoint (0);
  ref->detJ = -((sqrt3m3 * x3 - 2 * sqrt3 * x2 + sqrt3p3 * x1) * y4 +
		(-sqrt3m3 * x4 + sqrt3m3 * x2) * y3 +
		(2 * sqrt3 * x4 + (3 - sqrt3) * x3 - sqrt3p3 * x1) * y2 +
		((-sqrt3p3) * x4 + sqrt3p3 * x2) * y1) / 24.0;
  if (reference) ref->detJ0 = ref->detJ;
  // test de positivite du Jacobien
  if (ref->detJ < 0.)
    {
      cerr << "Negative value of detJ encountered in element " << number <<
	endl;
      cerr << "at time : " << Global_Structure->physics.current()->domains.current()->currentTime << endl;
      cerr << "Emergency save of the last result\n";
      cerr << "Program aborted\n";
      Global_Structure->resultFile->transfert ();
      exit (-1);
    }
  ref->N (0) = sqrt3p2 / 6.0;
  ref->N (1) = 1.0 / 6.0;
  ref->N (2) = -sqrt3m2 / 6.0;
  ref->N (3) = 1.0 / 6.0;
  denom1 = 1.0 / (sqrt3 * 8.0 * ref->detJ);
  denom2 = 1.0 / (24.0 * ref->detJ);
  ref->dN (0, 0) = -(sqrt3p1 * y4 - sqrt3p1 * y2) * denom1;
  ref->dN (0, 1) = (sqrt3p1 * x4 - sqrt3p1 * x2) * denom1;
  ref->dN (1, 0) = (2.0 * sqrt3 * y4 - sqrt3m3 * y3 - sqrt3p3 * y1) * denom2;
  ref->dN (1, 1) = -(2.0 * sqrt3 * x4 - sqrt3m3 * x3 - sqrt3p3 * x1) * denom2;
  ref->dN (2, 0) = (sqrt3m1 * y4 - sqrt3m1 * y2) * denom1;
  ref->dN (2, 1) = -(sqrt3m1 * x4 - sqrt3m1 * x2) * denom1;
  ref->dN (3, 0) = (sqrt3m3 * y3 - 2.0 * sqrt3 * y2 + sqrt3p3 * y1) * denom2;
  ref->dN (3, 1) = -(sqrt3m3 * x3 - 2.0 * sqrt3 * x2 + sqrt3p3 * x1) * denom2;

  // calcul du point 2
  getIntegrationPoint (1);
  ref->detJ = -((sqrt3m3 * x3 - sqrt3m3 * x1) * y4 +
		(-sqrt3m3 * x4 + (-sqrt3p3) * x2 + 2 * sqrt3 * x1) * y3 +
		(sqrt3p3 * x3 - sqrt3p3 * x1) * y2 +
		(sqrt3m3 * x4 - 2 * sqrt3 * x3 + sqrt3p3 * x2) * y1) / 24.0;
  if (reference) ref->detJ0 = ref->detJ;
  // test de positivite du Jacobien
  if (ref->detJ < 0.)
    {
      cerr << "Negative value of detJ encountered in element " << number <<
	endl;
      cerr << "at time : " << Global_Structure->physics.current()->domains.current()->currentTime << endl;
      cerr << "Emergency save of the last result\n";
      cerr << "Program aborted\n";
      Global_Structure->resultFile->transfert ();
      exit (-1);
    }
  ref->N (0) = 1.0 / 6.0;
  ref->N (1) = sqrt3p2 / 6.0;
  ref->N (2) = 1.0 / 6.0;
  ref->N (3) = -sqrt3m2 / 6.0;
  denom1 = 1.0 / (sqrt3 * 8.0 * ref->detJ);
  denom2 = 1.0 / (24.0 * ref->detJ);
  ref->dN (0, 0) = (sqrt3m3 * y4 - 2.0 * sqrt3 * y3 + sqrt3p3 * y2) * denom2;
  ref->dN (0, 1) = -(sqrt3m3 * x4 - 2.0 * sqrt3 * x3 + sqrt3p3 * x2) * denom2;
  ref->dN (1, 0) = (sqrt3p1 * y3 - sqrt3p1 * y1) * denom1;
  ref->dN (1, 1) = -(sqrt3p1 * x3 - sqrt3p1 * x1) * denom1;
  ref->dN (2, 0) = -(sqrt3m3 * y4 + sqrt3p3 * y2 - 2.0 * sqrt3 * y1) * denom2;
  ref->dN (2, 1) = (sqrt3m3 * x4 + sqrt3p3 * x2 - 2.0 * sqrt3 * x1) * denom2;
  ref->dN (3, 0) = -(sqrt3m1 * y3 - sqrt3m1 * y1) * denom1;
  ref->dN (3, 1) = (sqrt3m1 * x3 - sqrt3m1 * x1) * denom1;

  // calcul du point 3
  getIntegrationPoint (2);
  ref->detJ = ((sqrt3p3 * x3 - 2 * sqrt3 * x2 + sqrt3m3 * x1) * y4 +
	       ((-sqrt3p3) * x4 + sqrt3p3 * x2) * y3 +
	       (2 * sqrt3 * x4 - sqrt3p3 * x3 - sqrt3m3 * x1) * y2 +
	       (-sqrt3m3 * x4 + sqrt3m3 * x2) * y1) / 24.0;
  if (reference) ref->detJ0 = ref->detJ;
  // test de positivite du Jacobien
  if (ref->detJ < 0.)
    {
      cerr << "Negative value of detJ encountered in element " << number <<
	endl;
      cerr << "at time : " << Global_Structure->physics.current()->domains.current()->currentTime << endl;
      cerr << "Emergency save of the last result\n";
      cerr << "Program aborted\n";
      Global_Structure->resultFile->transfert ();
      exit (-1);
    }
  ref->N (0) = -sqrt3m2 / 6.0;
  ref->N (1) = 1.0 / 6.0;
  ref->N (2) = sqrt3p2 / 6.0;
  ref->N (3) = 1.0 / 6.0;
  denom1 = 1.0 / (sqrt3 * 8.0 * ref->detJ);
  denom2 = 1.0 / (24.0 * ref->detJ);
  ref->dN (0, 0) = -(sqrt3m1 * y4 - sqrt3m1 * y2) * denom1;
  ref->dN (0, 1) = (sqrt3m1 * x4 - sqrt3m1 * x2) * denom1;
  ref->dN (1, 0) = -(2.0 * sqrt3 * y4 - sqrt3p3 * y3 - sqrt3m3 * y1) * denom2;
  ref->dN (1, 1) = (2.0 * sqrt3 * x4 - sqrt3p3 * x3 - sqrt3m3 * x1) * denom2;
  ref->dN (2, 0) = (sqrt3p1 * y4 - sqrt3p1 * y2) * denom1;
  ref->dN (2, 1) = -(sqrt3p1 * x4 - sqrt3p1 * x2) * denom1;
  ref->dN (3, 0) = -(sqrt3p3 * y3 - 2.0 * sqrt3 * y2 + sqrt3m3 * y1) * denom2;
  ref->dN (3, 1) = (sqrt3p3 * x3 - 2.0 * sqrt3 * x2 + sqrt3m3 * x1) * denom2;

  // calcul du point 4
  getIntegrationPoint (3);
  ref->detJ = ((sqrt3p3 * x3 - sqrt3p3 * x1) * y4 +
	       ((-sqrt3p3) * x4 - sqrt3m3 * x2 + 2 * sqrt3 * x1) * y3 +
	       (sqrt3m3 * x3 - sqrt3m3 * x1) * y2 +
	       (sqrt3p3 * x4 - 2 * sqrt3 * x3 + sqrt3m3 * x2) * y1) / 24.0;
  if (reference) ref->detJ0 = ref->detJ;
  // test de positivite du Jacobien
  if (ref->detJ < 0.)
    {
      cerr << "Negative value of detJ encountered in element " << number <<
	endl;
      cerr << "at time : " << Global_Structure->physics.current()->domains.current()->currentTime << endl;
      cerr << "Emergency save of the last result\n";
      cerr << "Program aborted\n";
      Global_Structure->resultFile->transfert ();
      exit (-1);
    }
  ref->N (0) = 1.0 / 6.0;
  ref->N (1) = -sqrt3m2 / 6.0;
  ref->N (2) = 1.0 / 6.0;
  ref->N (3) = sqrt3p2 / 6.0;
  denom1 = 1.0 / (sqrt3 * 8.0 * ref->detJ);
  denom2 = 1.0 / (24.0 * ref->detJ);
  ref->dN (0, 0) = -(sqrt3p3 * y4 - 2.0 * sqrt3 * y3 + sqrt3m3 * y2) * denom2;
  ref->dN (0, 1) = (sqrt3p3 * x4 - 2.0 * sqrt3 * x3 + sqrt3m3 * x2) * denom2;
  ref->dN (1, 0) = (sqrt3m1 * y3 - sqrt3m1 * y1) * denom1;
  ref->dN (1, 1) = -(sqrt3m1 * x3 - sqrt3m1 * x1) * denom1;
  ref->dN (2, 0) = (sqrt3p3 * y4 + sqrt3m3 * y2 - 2.0 * sqrt3 * y1) * denom2;
  ref->dN (2, 1) = -(sqrt3p3 * x4 + sqrt3m3 * x2 - 2.0 * sqrt3 * x1) * denom2;
  ref->dN (3, 0) = -(sqrt3p1 * y3 - sqrt3p1 * y1) * denom1;
  ref->dN (3, 1) = (sqrt3p1 * x3 - sqrt3p1 * x1) * denom1;
}*/

// //-----------------------------------------------------------------------------
// void
// ElTri3n2D::getIntgtoNodes (Vector & N, const Vec3D & point) const
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
void ElTri3n2D::computeGlob2Loc()
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
void ElTri3n2D::glob2Loc(const Vec3D &point, Vec3D &local)
//-----------------------------------------------------------------------------
{
  local(0) =
      glob2loc(0, 0) + point(0) * glob2loc(0, 1) + point(1) * glob2loc(0, 2) +
      point(0) * point(1) * glob2loc(0, 3);
  local(1) =
      glob2loc(1, 0) + point(0) * glob2loc(1, 1) + point(1) * glob2loc(1, 2) +
      point(0) * point(1) * glob2loc(1, 3);
  local(2) = 0.;
}
