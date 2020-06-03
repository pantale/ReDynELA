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

// begin date : 12/03/1997

/*
  Class Node definition
*/

#include <Node.h>
#include <NodalField.h>
#include <Element.h>
#include <NodeMotion.h>

/*!
  \file Node.C
  \brief fichier .C de d�finition des noeuds du maillage
  \ingroup femLibrary

  Ce fichier sert � la d�finition des noeuds du maillage �l�ments finis sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

//-----------------------------------------------------------------------------
Node::Node(Indice numero)
//-----------------------------------------------------------------------------
{
  // init du numero du noeud
  Id = numero;
  number = numero;

  // init des donnees
  mass = 0.;

  // init par defaut des coordonnees
  coords = 0.;
  normal = 0.;

  // init par defaut de deux champs de donnees
  New = new NodalField;
  Current = new NodalField;
  tmp = NULL;

  // init par defaut d'un node Motion
  motion = NULL;

  boundary = NULL;
}

//-----------------------------------------------------------------------------
Node::Node(const Node &nd)
//-----------------------------------------------------------------------------
{
  coords = nd.coords;
  normal = nd.normal;
}

//-----------------------------------------------------------------------------
Node::~Node()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Node::addElement(Element *el)
//-----------------------------------------------------------------------------
{
  elements << el;
}

//-----------------------------------------------------------------------------
Boolean
Node::operator==(const Node &node) const
//-----------------------------------------------------------------------------
{
  if (Id != node.Id)
    return (False);
  if (coords != node.coords)
    return (False);
  return (True);
}

//-----------------------------------------------------------------------------
Boolean
Node::operator!=(const Node &node) const
//-----------------------------------------------------------------------------
{
  return !(*this == node);
}

//-----------------------------------------------------------------------------
Boolean
Node::operator<(const Node &node) const
//-----------------------------------------------------------------------------
{
  return (Id < node.Id);
}

//-----------------------------------------------------------------------------
Boolean
Node::operator>(const Node &node) const
//-----------------------------------------------------------------------------
{
  return (Id > node.Id);
}

//-----------------------------------------------------------------------------
void Node::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "node n=" << number;
  os << " coords=(" << coords << ")";
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, Node &node)
//-----------------------------------------------------------------------------
{
  node.print(os);
  return os;
}

//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const Node &nd)
//-----------------------------------------------------------------------------
{
  nd.write(os);
  return os;
}

//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, Node &nd)
//-----------------------------------------------------------------------------
{
  nd.read(is);
  return is;
}

//-----------------------------------------------------------------------------
void Node::write(ofstream &pfile) const
//-----------------------------------------------------------------------------
{
  pfile << coords;
  //  pfile << disp;
  Current->write(pfile);
}

//-----------------------------------------------------------------------------
Node &Node::read(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  pfile >> coords;
  //  pfile >> disp;
  Current->read(pfile);

  return *this;
}

//-----------------------------------------------------------------------------
Boolean
Node::attachNodeMotion(NodeMotion *_motion)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(motion == NULL);
#endif
  motion = _motion;
  return (Success);
}

//-----------------------------------------------------------------------------
Boolean
Node::detachNodeMotion()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(motion != NULL);
#endif

  // detachement de l'objet
  motion = NULL;
  return (Success);
}

//-----------------------------------------------------------------------------
Boolean
Node::deleteNodeMotion()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(motion != NULL);
#endif

  // destruction de l'objet
  delete motion;

  // detachement de l'objet
  motion = NULL;
  return (Success);
}

//-----------------------------------------------------------------------------
void Node::transfertNodalField()
//-----------------------------------------------------------------------------
{
  // on transfere les anciennes coordonnees
  // coords=new_coords;

  // ici, on croise les references des NodalFields
  tmp = New;
  New = Current;
  Current = tmp;
}

//-----------------------------------------------------------------------------
void Node::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  fprintf(pfile, "%8ld  %8.5E  %8.5E  %8.5E \n", number, coords(0),
          coords(1), coords(2));
}

//-----------------------------------------------------------------------------
void Node::toFileBound(FILE *pfile)
//-----------------------------------------------------------------------------
{
  if (Current->mat_speed.norm2() != 0.0)
  {
    /*    fprintf(pfile,"%8ld  %8.3E %8.3E %8.3E  %8.3E %8.3E %8.3E\n",
      number,
      Current->mat_speed(0),Current->mat_speed(1),Current->mat_speed(2),
      Current->grid_speed(0),Current->grid_speed(1),Current->grid_speed(2));*/
    fprintf(pfile, "%8ld  %8.3E %8.3E %8.3E\n",
            number,
            Current->mat_speed(0), Current->mat_speed(1),
            Current->mat_speed(2));
  }
}

//-----------------------------------------------------------------------------
Indice
Node::objectSize()
//-----------------------------------------------------------------------------
{
  Indice sz;
  sz = sizeof(*this);
  sz += Current->objectSize();
  sz += New->objectSize();
  return sz;
}

//-----------------------------------------------------------------------------
Boolean compareNodeNumbers(Node *p1, Node *p2)
//-----------------------------------------------------------------------------
{
  return (p1->number > p2->number); // comparaison
}

//-----------------------------------------------------------------------------
Indice diffNodeNumber(Node *p1, const Indice number)
//-----------------------------------------------------------------------------
{
  return (p1->number - number); // comparaison
}

#define getNodalFieldScalarValueLocal(NOM, VAR)                                                  \
  if (field == #NOM)                                                                             \
  {                                                                                              \
    if (component > 1)                                                                           \
      fatalError("Node::getNodalValue::get", "No sense for component >1 for a scalar quantity"); \
    return Current->VAR;                                                                         \
  }
#define getNodalFieldVectorialValueLocal(NOM, VAR)                                                  \
  if (field == #NOM)                                                                                \
  {                                                                                                 \
    if (component == 0)                                                                             \
      return Current->VAR.norm();                                                                   \
    if (component > 3)                                                                              \
      fatalError("Node::getNodalValue::get", "No sense for component >3 for a vectorial quantity"); \
    return Current->VAR(component - 1);                                                             \
  }
#define getNodalFieldVectorialLocal(NOM, VAR) \
  if (field == #NOM)                          \
  {                                           \
    return Current->VAR;                      \
  }
#define getNodalVectorialLocal(NOM, VAR) \
  if (field == #NOM)                     \
  {                                      \
    return VAR;                          \
  }
#define getNodalScalarValueLocal(NOM, VAR)                                                       \
  if (field == #NOM)                                                                             \
  {                                                                                              \
    if (component > 1)                                                                           \
      fatalError("Node::getNodalValue::get", "No sense for component >1 for a scalar quantity"); \
    return VAR;                                                                                  \
  }
#define getNodalVectorialValueLocal(NOM, VAR)                                                       \
  if (field == #NOM)                                                                                \
  {                                                                                                 \
    if (component == 0)                                                                             \
      return VAR.norm();                                                                            \
    if (component > 3)                                                                              \
      fatalError("Node::getNodalValue::get", "No sense for component >3 for a vectorial quantity"); \
    return VAR(component - 1);                                                                      \
  }

//-----------------------------------------------------------------------------
Real Node::getNodalValue(String field, Indice component)
//-----------------------------------------------------------------------------
{
  // nodalfield values
  getNodalFieldScalarValueLocal(density, ro);
  getNodalFieldScalarValueLocal(densityInc, dro);
  getNodalFieldScalarValueLocal(energy, e);
  getNodalFieldScalarValueLocal(energyInc, de);
  getNodalFieldVectorialValueLocal(speed, mat_speed);
  getNodalFieldVectorialValueLocal(speedInc, dmat_speed);
  getNodalFieldVectorialValueLocal(force, fe);
  getNodalFieldScalarValueLocal(temperature, T);
  getNodalFieldVectorialValueLocal(flux, flux);
  getNodalFieldVectorialValueLocal(displacementInc, delta_disp);
  getNodalFieldVectorialValueLocal(displacement, disp);

  // nodal values
  getNodalVectorialValueLocal(coords, coords);
  getNodalVectorialValueLocal(normal, normal);
  getNodalScalarValueLocal(mass, mass);
  getNodalScalarValueLocal(temperatureInit, T0);

  if (field == "stress")
  {
    Tensor2 V, V1;
    Indice i;
    V1 = 0;
    for (i = 0; i < elements.size(); i++)
    {
      elements(i)->get_Sig_atNode(V, elements(i)->getLocalNumber(this));
      V1 += V;
    }
    V1 = V1 / elements.size();
    if (component == 0)
      return V1.misesEquivalent();
    else
      return V1(((component - (component % 10)) / 10) - 1, (component % 10) - 1);
  }

  if (field == "strain")
  {
    Tensor2 V, V1;
    Indice i;
    V1 = 0;
    for (i = 0; i < elements.size(); i++)
    {
      elements(i)->get_Eps_atNode(V, elements(i)->getLocalNumber(this));
      V1 += V;
    }
    V1 = V1 / elements.size();
    if (component == 0)
      return V1.equivalent();
    else
      return V1(((component - (component % 10)) / 10) - 1, (component % 10) - 1);
  }

  if (field == "strainInc")
  {
    Tensor2 V, V1;
    Indice i;
    V1 = 0;
    for (i = 0; i < elements.size(); i++)
    {
      elements(i)->get_dEps_atNode(V, elements(i)->getLocalNumber(this));
      V1 += V;
    }
    V1 = V1 / elements.size();
    if (component == 0)
      return V1.equivalent();
    else
      return V1(((component - (component % 10)) / 10) - 1, (component % 10) - 1);
  }

  if (field == "plasticStrain")
  {
    Tensor2 V, V1;
    Indice i;
    V1 = 0;
    for (i = 0; i < elements.size(); i++)
    {
      elements(i)->get_EpsPlas_atNode(V, elements(i)->getLocalNumber(this));
      V1 += V;
    }
    V1 = V1 / elements.size();
    if (component == 0)
      return V1.equivalent();
    else
      return V1(((component - (component % 10)) / 10) - 1, (component % 10) - 1);
  }

  fatalError("getNodalValue", "undefined field %s\n", field.chars());
  return 0;
}

//-----------------------------------------------------------------------------
Vec3D Node::getNodalVector(String field)
//-----------------------------------------------------------------------------
{
  // nodalfield values
  getNodalFieldVectorialLocal(speed, mat_speed);
  getNodalFieldVectorialLocal(speedInc, dmat_speed);
  getNodalFieldVectorialLocal(force, fe);
  getNodalFieldVectorialLocal(flux, flux);
  getNodalFieldVectorialLocal(displacementInc, delta_disp);
  getNodalFieldVectorialLocal(displacement, disp);

  // nodal values
  getNodalVectorialLocal(coords, coords);
  getNodalVectorialLocal(normal, normal);

  fatalError("getNodalVector", "undefined field %s\n", field.chars());
  Vec3D v;
  return v;
}

//-----------------------------------------------------------------------------
Tensor2 Node::getNodalTensor(String field)
//-----------------------------------------------------------------------------
{

  if (field == "stress")
  {
    Tensor2 V, V1;
    Indice i;
    V1 = 0;
    for (i = 0; i < elements.size(); i++)
    {
      elements(i)->get_Sig_atNode(V, elements(i)->getLocalNumber(this));
      V1 += V;
    }
    V1 = V1 / elements.size();
    return V1;
  }

  if (field == "strain")
  {
    Tensor2 V, V1;
    Indice i;
    V1 = 0;
    for (i = 0; i < elements.size(); i++)
    {
      elements(i)->get_Eps_atNode(V, elements(i)->getLocalNumber(this));
      V1 += V;
    }
    V1 = V1 / elements.size();
    return V1;
  }

  if (field == "strainInc")
  {
    Tensor2 V, V1;
    Indice i;
    V1 = 0;
    for (i = 0; i < elements.size(); i++)
    {
      elements(i)->get_dEps_atNode(V, elements(i)->getLocalNumber(this));
      V1 += V;
    }
    V1 = V1 / elements.size();
    return V1;
  }

  if (field == "plasticStrain")
  {
    Tensor2 V, V1;
    Indice i;
    V1 = 0;
    for (i = 0; i < elements.size(); i++)
    {
      elements(i)->get_EpsPlas_atNode(V, elements(i)->getLocalNumber(this));
      V1 += V;
    }
    V1 = V1 / elements.size();
    return V1;
  }

  fatalError("getValueAtNode", "undefined field %s\n", field.chars());
  Tensor2 V1;
  return V1;
}
