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

// begin date :
// revision date :

/*
	Class Select definition
*/

#ifndef __Select_h__
#define __Select_h__

#include <upFemSolver.h>

/*!
  \file Select.h
  \brief fichier .h de d�finition des groupes de noeuds et �lements
  \ingroup femLibrary

  Ce fichier sert � la d�finition des groupes de noeuds et �lements de la structure.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

/* #include <upFemSolver.h> */

class Element;
class Node;
class Structure;
class ElementSet;

/*!
  \brief Classe de gestion des groupes de noeuds
  \ingroup femLibrary
  \class NodeSet Select.h

 Cette classe g�re les groupes de noeuds au niveau d'une structure sur DynELA. Elle comporte des m�thodes de cr�ation de groupes de noeuds (par ajout de noeuds g�n�ralement), ainsi que des m�thodes de gestion de ces groupes de noeuds.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
/** @dia:pos 183.065,119.529 */
class NodeSet
{
public:
  /** @dia:route 0,2;h,26,47.25,-21.95,-172.85,-95.25 */
  String name; //!< Nom public du NodeSet
  /** @dia:route 52,11;h,228.8,90.5,205.75,122.429,200.365 */
  ListId<Node *> nodes; //!< Liste des noeuds

public:
  // constructeurs
  NodeSet();
  NodeSet(const NodeSet &X);
  ~NodeSet();

  // fonctions membres
  void add(Node *nd);
  void add(Indice num);
  void add(Indice sNum, Indice eNum, Indice inc = 1);
  void add(NodeSet *);
  void intersect(NodeSet *);
  void substract(NodeSet *);
  void clear();
  Node *getNode(Indice i);
  Indice getNodeNumber(Indice i);
  Indice size();
  void surface(ElementSet *);
};

/*!
  \brief Classe de gestion des groupes d'�l�ments
  \ingroup femLibrary
  \class ElementSet Select.h

 Cette classe g�re les groupes d'�l�ments au niveau d'une structure sur DynELA. Elle comporte des m�thodes de cr�ation de groupes d'�l�ments (par ajout d'�l�ments g�n�ralement), ainsi que des m�thodes de gestion de ces groupes d'�l�ments.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
/** @dia:pos 183.8,104.8 */
class ElementSet
{
public:
  /** @dia:route 0,2;h,26.7353,32.5206,-22.6853,-172.85,-95.25 */
  String name; //!< Nom public de l'ElementSet
  /** @dia:route 39,12;h,149.465,88.3794,172.75,108.5,183.8 */
  ListId<Element *> elements; //!< Liste des �l�ments

public:
  // constructeurs
  ElementSet();
  ElementSet(const ElementSet &X);
  ~ElementSet();

  // fonctions membres
  void add(Element *el);
  void add(Indice num);
  void add(Indice sNum, Indice eNum, Indice inc = 1);
  void add(ElementSet *);
  void intersect(ElementSet *);
  void substract(ElementSet *);
  void clear();
  Element *getElement(Indice i);
  Indice size();
};

/*!
  \brief Classe de gestion des groupes de noeuds et d'�l�ments
  \ingroup femLibrary
  \class Select Select.h

 Cette classe g�re les groupes de noeuds et d'�l�ments au niveau d'une structure sur DynELA. Cette classe g�re donc les NodeSet et les ElementSet et permet de les manipuler � partir de leur nom. En fait, cette classe semble plutot vou�e � disparaitre, je ne sais plus trop. Elle semble pourvoir �tre totalement remplac�e par le nouveau langage de commande orient�-objets de DynELA.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
/** @dia:pos 186.065,92.5294 */
class Select
{
public:
  /** @dia:route 4,4;h,200.365,120.229,203.75,93.2294,200.565 */
  List<NodeSet *> nodesSet; //!< Liste de NodeSet
  /** @dia:route 3,3;h,183.8,105.5,179.75,93.2294,186.065 */
  List<ElementSet *> elementsSet; //!< Liste d'ElementSet

public:
  // constructeurs
  Select();
  Select(const Select &X);
  ~Select();
  void add(NodeSet *ns);
  NodeSet *getNodeSet(String name);
  void add(ElementSet *els);
  ElementSet *getElementSet(String name);
};

#endif
