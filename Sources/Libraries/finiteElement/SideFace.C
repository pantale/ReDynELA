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

// begin date : 22/05/1997

/*
	Class Side definition
*/

#include <Side.h>
#include <SideFace.h>
#include <Element.h>
#include <Node.h>
#include <NodalField.h>
#include <NodeMotion.h>
#include <Coulomb.h>
#include <Interface.h>

//-----------------------------------------------------------------------------
SideFace::SideFace()
//-----------------------------------------------------------------------------
{
  // affectation du numero
  //  Id = num;

  // init element par defaut
  element = NULL;

  // init de la normale
  normal = 0.;
}

//-----------------------------------------------------------------------------
SideFace::SideFace(const SideFace &X)
//-----------------------------------------------------------------------------
{
  cerr << "erreur SideFace copy not implemented" << &X << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
SideFace::~SideFace()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Boolean
SideFace::addNode(Node *pnd)
//-----------------------------------------------------------------------------
{
  nodes << pnd;
  return (Success);
}

//-----------------------------------------------------------------------------
Boolean
SideFace::attachElement(Element *pel)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(element == NULL);
#endif

  element = pel;
  return (Success);
}

//-----------------------------------------------------------------------------
Boolean
SideFace::check()
//-----------------------------------------------------------------------------
{
  Boolean ok;

  for (Indice i = 0; i < nodes.size(); i++)
  {
    ok = False;
    for (Indice j = 0; j < element->nodes.size(); j++)
    {
      if (element->nodes(j) == nodes(i))
        ok = True;
    }
    if (!ok)
      fatalError("Declared contact node error",
                 "node %d is not in element %d\n", nodes(i)->number,
                 element->number);
  }
  return True;
}

//-----------------------------------------------------------------------------
Boolean
SideFace::isNodeinSideFace(Side *side, Node *pnd)
//-----------------------------------------------------------------------------
{
  /*  if (side->isMaster()==True) {
    //   cout <<" Slave Node\n";
  }	
  else {
   //    cout <<" Master Node\n"; 
    return(False); 
  }*/

  // cout << "element "<<element->number<<" node "<<pnd->number<<endl;
  // appel de la fonction specialisee
  if (element->isNodeinElement(pnd) != True)
  {
    return (False);
  }

  //cout << "Node "<<pnd->number<<" in element "<<element->number<<endl;

  //cout << "Node "<<pnd->number()<<" in element "<<element->number()<<endl;

  // on commence par tester si le noeud concerne possede un caractere
  // de noeud de contact, c'est a dire s'il est deja pris en compte
  // par un algorithme de contact
  if (pnd->motion->getMotion() != Con_NodeMotion)
  {

    // donc maintenant, il faut creer un noeud de contact a la place de
    // l'ancien noeud, quelle que soit sa nature ant�rieure

    // detachement de l'ancien controle de mouvement
    pnd->deleteNodeMotion();
    //    pnd->detachNodeMotion();

    // allocation d'un contact control de type Coulomb
    //Contact* pcontact=new Contact;
    Coulomb *pcontact = new Coulomb;

    // initialisation du contact
    pcontact->attachNode(pnd);
    pcontact->attachSideFace(this);

    // creation de la reference croisee
    pnd->attachNodeMotion(pcontact);

    // creation du noeud de contact
    pcontact->Create();

    // quand je me mets a ecrire des trucs pareils, j'ai honte !!!
    pcontact->friction =
        ((CoulombLaw *)side->interface->contactLaw)->friction;
  }
  // dans l'autre cas, il faut v�rifier si le couple d�ja cr�e est toujours
  // valable, c'est a dire si le noeud concerne n'a pas gliss� dans
  // un autre �l�ment
  else
  {

    // quand je me mets a ecrire des trucs pareils, j'ai honte !!!
    Contact *pcontact = (Contact *)pnd->motion;
    //  Coulomb* pcontact=(Coulomb*)pnd->motion;

    // le couple est il deja fait ?
    if (pcontact->pside->element == element)
    {
      return (False);
    }

    // cout << "between element "<<pcontact->pside->element->number;
    //     cout <<" and "<<element->number<<endl;
    //   exit(0);

    // mise a jour du contact
    pcontact->detachSideFace();
    pcontact->attachSideFace(this);

    pcontact->Create();
  }
  return (True);
}

//-----------------------------------------------------------------------------
void SideFace::computeScanVolume()
//-----------------------------------------------------------------------------
{
  // preparation du calcul
  element->computeGlob2Loc();

  // compute bound box
  element->computeBoundBox();
}

//-----------------------------------------------------------------------------
void SideFace::computeNormalVertex()
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < nodes.size(); i++)
    nodes(i)->normal += normal;
}

//-----------------------------------------------------------------------------
Indice
SideFace::NoNodes()
//-----------------------------------------------------------------------------
{
  return nodes.size();
}

//-----------------------------------------------------------------------------
void SideFace::computeNormalatPoint(const Vec3D &point, Vec3D &resu)
//-----------------------------------------------------------------------------
{
  Vec3D coord;
  Real dist;

  // initialiser
  coord = point;
  resu = 0.;

  //    coord+=normal*computeDistanceWithPoint(point);

  for (Indice i = 0; i < nodes.size(); i++)
  {
    dist = nodes(i)->coords.distance(coord);
    if (dist < 1.e-10)
    {
      resu = nodes(i)->normal;
      return;
    }
    resu += nodes(i)->normal / dist;
  }

  resu.normalize();
}

//-----------------------------------------------------------------------------
void SideFace::computeForceatPoint(const Vec3D &point, Vec3D &force)
//-----------------------------------------------------------------------------
{
  Tensor2 Sig;
  Vec3D Normal;

  // calcul des contraintes au point
  element->getSigmaAtPoint(Sig, point);

  force = 0.;

  cout << "tenseur des contraintes " << Sig << endl;

  computeNormalatPoint(point, Normal);
  cout << "normale " << normal << endl;

  for (Indice i = 0; i < 3; i++)
    for (Indice j = 0; j < 3; j++)
      force(i) -= Sig(i, j) * Normal(j);

  cout << "vecteur de force " << force << endl;
}

//-----------------------------------------------------------------------------
Vec3D SideFace::averageSideFaceSpeed()
//-----------------------------------------------------------------------------
{
  Vec3D speed;
  speed = 0.;

  if (nodes.size() == 0)
    fatalError("0 noeuds dans le sideface\n", "");
  for (Indice i = 0; i < nodes.size(); i++)
    speed += nodes(i)->New->mat_speed;
  speed /= nodes.size();

  return speed;
}

//-----------------------------------------------------------------------------
void SideFace::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  fprintf(pfile, "  %6ld  ", element->number);
  for (Indice i = 0; i < nodes.size(); i++)
    fprintf(pfile, "%6ld  ", nodes(i)->number);
  fprintf(pfile, "\n");
}

// *************************************
//
// ****************** SideFace2D
//
// *************************************

//-----------------------------------------------------------------------------
SideFace2D::SideFace2D()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
SideFace2D::SideFace2D(const SideFace2D &X) : SideFace(X)
//-----------------------------------------------------------------------------
{
  cerr << "erreur SideFace2D copy not implemented" << &X << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
SideFace2D::~SideFace2D()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void SideFace2D::computeNormal()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(element != NULL);
#endif

  // calcul de la normale
  normal(0) = nodes(0)->coords(1) - nodes(1)->coords(1);
  normal(1) = nodes(1)->coords(0) - nodes(0)->coords(0);

  // normalisation du vecteur
  normal.normalize();
}

//-----------------------------------------------------------------------------
Vec3D SideFace2D::getTangent()
//-----------------------------------------------------------------------------
{
  Vec3D tangent;

  tangent(0) = normal(1);
  tangent(1) = normal(0);
  tangent(2) = 0.;

  return tangent;
}

//-----------------------------------------------------------------------------
Real SideFace2D::computeDistanceWithPoint(const Vec3D &point)
//-----------------------------------------------------------------------------
{
  Real h;

  // vecteur directeur
  h = normal(0) * nodes(0)->coords(0) + normal(1) * nodes(0)->coords(1);

  // calcul de la distance
  return (-(normal(0) * point(0) + normal(1) * point(1) - h));
}

//-----------------------------------------------------------------------------
Vec3D SideFace2D::computeIntersectionWith(const Vec3D &point, const Vec3D &traj)
//-----------------------------------------------------------------------------
{
  Vec3D resu;

  // cout << "intersection"<<endl;
  // cout << *nodes(0) << endl;
  // cout << *nodes(1) << endl;
  // cout << point << endl;
  // cout << traj << endl;

  Real a1 = normal(0);
  Real b1 = normal(1);
  Real c1 = a1 * nodes(0)->coords(0) + b1 * nodes(0)->coords(1);
  //  Real c1=a1*(nodes(0)->coords(0)+nodes(0)->New->u(0))+b1*(nodes(0)->coords(1)+nodes(0)->New->u(1));
  Real a2 = -traj(1);
  Real b2 = traj(0);
  Real c2 = a2 * point(0) + b2 * point(1);

  if (a1 == 0.)
  {
    resu(1) = c1 / b1;
    resu(0) = (c2 - b2 * resu(1)) / a2;
    //cout << resu << endl;
    return (resu);
  }

  resu(0) = (b1 * c2 - b2 * c1) / (a2 * b1 - a1 * b2);
  resu(1) = (a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1);

  //cout << resu << endl;

  return (resu);
}

// *************************************
//
// ****************** SideFace3D
//
// *************************************

//-----------------------------------------------------------------------------
SideFace3D::SideFace3D()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
SideFace3D::SideFace3D(const SideFace3D &X) : SideFace(X)
//-----------------------------------------------------------------------------
{
  cerr << "erreur SideFace3D copy not implemented" << &X << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
SideFace3D::~SideFace3D()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void SideFace3D::computeNormal()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(element != NULL);
#endif
  Vec3D v1, v2;
  normal = 0.;

  // first one
  v1 = nodes(1)->coords - nodes(0)->coords;
  v2 = nodes.last()->coords - nodes(0)->coords;
  normal = v1.vectorial(v2);

  for (Indice i = 1; i < nodes.size() - 2; i++)
  {
    v1 = nodes(i + 1)->coords - nodes(i)->coords;
    v2 = nodes(i - 1)->coords - nodes(i)->coords;
    normal += v1.vectorial(v2);
  }

  // last one
  v1 = nodes(0)->coords - nodes.last()->coords;
  v2 = nodes(nodes.size() - 2)->coords - nodes.last()->coords;
  normal += v1.vectorial(v2);

  // normalisation du vecteur
  normal.normalize();
}

//-----------------------------------------------------------------------------
Vec3D SideFace3D::getTangent()
//-----------------------------------------------------------------------------
{
  Vec3D tangent;

  cerr << "Pas encore eu d'idee intelligente pour void SideFace3D::getTangent()\n";
  exit(-1);

  return tangent;
}

//-----------------------------------------------------------------------------
Real SideFace3D::computeDistanceWithPoint(const Vec3D &point)
//-----------------------------------------------------------------------------
{
  cerr << "Pas encore eu d'idee intelligente pour void SideFace3D::computeDistanceWithPoint()\n";
  exit(-1);
  return 0;
}

//-----------------------------------------------------------------------------
Vec3D SideFace3D::computeIntersectionWith(const Vec3D &point, const Vec3D &traj)
//-----------------------------------------------------------------------------
{
  Vec3D resu;
  cerr << "Pas encore eu d'idee intelligente pour void SideFace3D::computeIntersectionWith()\n";
  exit(-1);

  return (resu);
}
