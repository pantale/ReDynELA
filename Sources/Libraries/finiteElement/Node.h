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

#ifndef __Node_h__
#define __Node_h__

#include <upFemSolver.h>

/*!
  \file Node.h
  \brief fichier .h de d�finition des noeuds du maillage
  \ingroup femLibrary

  Ce fichier sert � la d�finition des noeuds du maillage �l�ments finis sur DynELA.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

/* #include <upFemSolver.h> */

/* #include <Element.h> */
class Element;

/* #include <NodalField.h> */
class NodalField;

/* #include <NodeMotion.h> */
class NodeMotion;

/* #include <Boundary.h> */
class BoundaryCondition;

/*!
  \class Node
  \brief Classe de d�finition et de manipulation des noeuds du maillage.
  \ingroup femLibrary

  Cette classe permet de d�finir et de manipuler les noeuds de la structure. Ces noeuds �taient anciennement uniquement attach�s � la notion de domaine d'�tude d�finie par la classe Domain. D�sormais, la nouvelle version permet d'int�grer la gestion automatique du multi-grille et du multi-domaine. Dans cette nouvelle perspective, les noeuds sont g�n�ralement rattach�s � des objets de type Grid.

  \see Domain, Element, NodalField
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/
class Node
{
    friend class ListId<Node *>;

private:
    // champ temporaire interne utile pour le transfert de quantit�s d'un incr�ment � l'autre
    NodalField *tmp;

public:
    /**Identification number. This field represents the internal identification number of the current node.*/
    Indice Id;
    /**Identification number. This field represents the external identification number of the current node (ie. user representation).*/
    Indice number;
    /**Nodal field. New nodal field of the node, ie. nodal field at the end of the increment*/
    NodalField *New;
    /**Nodal field. Current nodal field of the node, ie. nodal field at the begining of the increment*/
    NodalField *Current;
    /**Coordinates. Current coordinates of the corresponding point.*/
    Vec3D coords;
    /**Coordinates. Initial coordinates of the corresponding point.*/
    Vec3D coords0;
    /**Normal vector. This vector represents the normal vector of the current point in 3D space. Such normal vector is compted from the values of the normals of the faces connected to this point.*/
    Vec3D normal;
    /**Mass. This field is useed to store the value of the equivalent nodal mass.*/
    Real mass;
    /**Temperature. This field is used to store the reference value of the temperature of the node at the begining of the calculus*/
    Real T0;
    /**Element list. This pointer reference the list of the elements that contains a reference to the current node (ie: the list of the elements that includes this node).*/
    ListId<Element *> elements;
    /**Node motion. This pointer reference the method used to move the point.*/
    NodeMotion *motion;
    /**Boundary conditions. This pointer reference the list of the boundary conditions on the current node.*/
    BoundaryCondition *boundary;

public:
    /**
       name Class management methods
    */

    //{
    /**.Default constructor*/
    Node(Indice No_ = 1);

    /**.Copy constructor*/
    Node(const Node &X);

    /**.Destructor*/
    ~Node();

    /**Attach an element. This method is used to add a new reference to an element in the list of the elements connected to the current point. There is no limit in the number of elements connected to the current node, so there is no verification procedure to see if this is correct for the structure.*/
    void addElement(Element *el);
    Real getNodalValue(String field, Indice component);
    Vec3D getNodalVector(String field);
    Tensor2 getNodalTensor(String field);

    /**.Number of elements. This method returns the number of elements referenced in the list of the elements connected to the current node.*/
    Indice NoElements();

    /**System number. This method gives a direct access to the system number of the current node.*/
    Indice &systemNumber();

    /**Node motion. This method is used to attach a new node motion control to the current node. See the class NodeMotion for more details about the definition of this motion control.*/
    Boolean attachNodeMotion(NodeMotion *motion);

    /**Node motion. This method is used to detach a new node motion control to the current node. See the class NodeMotion for more details about the definition of this motion control.*/
    Boolean detachNodeMotion();

    /**Node motion. This method is used to delete a new node motion control to the current node. See the class NodeMotion for more details about the definition of this motion control.*/
    Boolean deleteNodeMotion();

    /**Nodal field transfer. This method is used to transfer the New nodal field to the Current one. In fact, this swaps the two fields.*/
    void transfertNodalField();

    /**Get a nodal field. This method returns a pointer with a reference to one of the nodal fields depending on the given parameter time. Settings time=0 returns the Current nodal field, time=1 returns the New nodal field.*/
    inline NodalField *getFieldAtTime(int time);

    /**.*/
    Boolean operator==(const Node &node) const;
    /**.*/
    Boolean operator!=(const Node &node) const;
    /**.*/
    Boolean operator<(const Node &node) const;
    /**.*/
    Boolean operator>(const Node &node) const;

    // fonctions entree sortie
    /**.*/
    friend ostream &operator<<(ostream &os, Node &node);
    /**.*/
    friend ofstream &operator<<(ofstream &, const Node &);
    /**.*/
    friend ifstream &operator>>(ifstream &, Node &);
    /**.*/
    void print(ostream &os) const;
    /**.*/
    void write(ofstream &pfile) const;
    /**.*/
    Node &read(ifstream &pfile);

    /**.*/
    void toFile(FILE *pfile);
    /**.*/
    void toFileBound(FILE *pfile);
    //}
    Indice objectSize();
};

Boolean compareNodeNumbers(Node *p1, Node *p2);
Indice diffNodeNumber(Node *p1, const Indice ind);
//Real getValueAtNode(Node* node,String field,Indice component);

//-----------------------------------------------------------------------------
inline NodalField *
Node::getFieldAtTime(int time)
//-----------------------------------------------------------------------------
{
    return (time == 0 ? Current : New);
}

//-----------------------------------------------------------------------------
inline Indice
Node::NoElements()
//-----------------------------------------------------------------------------
{
    return elements.size();
}

//-----------------------------------------------------------------------------
inline Indice &
Node::systemNumber()
//-----------------------------------------------------------------------------
{
    return (Id);
}

#endif
