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
  Class SideFace definition
*/

#ifndef __SideFace_h__
#define __SideFace_h__

#include <upFemSolver.h>

class SideFace;
class Node;

/* #include <Domain.h> */
class Side;
class Element;

//#include <Coulomb.h>
//class Coulomb;

/*<Side:*/
/**
*/
/** @dia:pos 222.8,182.8 */
class SideFace
{
  friend class List<SideFace *>;

protected:
  //  Indice Id;			// pour traiter avec ListId
  //    Element*        element;// element de volume

public:
  /// liste des noeuds composant le SideFace
  /** @dia:route 60,14;h,228.8,93.7,207.75,187.3,222.8 */
  ListId<Node *> nodes;
  /// normale unitaire ext�rieure au SideFace
  /** @dia:route 0,2;h,65.7353,110.521,-61.6853,-168.35,165.05 */
  Vec3D normal;
  /// element sur lequel est construit le sideFace
  /** @dia:route 179,26;h,149.465,144.379,204.75,192.1,222.8 */
  Element *element;
  /** @dia:route 25,17;h,306.05,106.3,317.75,188.1,250.25 */
  Side *side;

public:
  // constructeurs
  SideFace();
  SideFace(const SideFace &X);
  virtual ~SideFace();

  // fonctions membres
  //Node* getNode(Indice num);
  Boolean addNode(Node *pnd);
  Boolean isNodeinSideFace(Side *side, Node *pnd);
  // Element* getElement();
  Boolean attachElement(Element *pel);
  void computeScanVolume();
  void computeNormalVertex();
  void scanIncludeNodes(Side *slave);
  virtual void computeNormal() = 0;
  virtual Vec3D getTangent() = 0;
  virtual Real computeDistanceWithPoint(const Vec3D &point) = 0;
  Indice NoNodes();
  void computeNormalatPoint(const Vec3D &point, Vec3D &resu);
  virtual Vec3D computeIntersectionWith(const Vec3D &point,
                                        const Vec3D &traj) = 0;
  void computeForceatPoint(const Vec3D &point, Vec3D &force);
  Vec3D averageSideFaceSpeed();
  void toFile(FILE *pfile);
  Boolean check();
};

/**
*/
/** @dia:pos 211.8,207.8 */
/** @dia:route SideFace;v,236.525,200.6,203.25,224.125,207.8 */
class SideFace2D : public SideFace
{

public:
  // constructeurs
  SideFace2D();
  SideFace2D(const SideFace2D &X);
  ~SideFace2D();

  // fonctions membres
  void computeNormal();
  Vec3D getTangent();
  Real computeDistanceWithPoint(const Vec3D &point);
  Vec3D computeIntersectionWith(const Vec3D &point, const Vec3D &traj);
};

/**
*/
/** @dia:pos 258.8,208.8 */
/** @dia:route SideFace;v,236.525,200.6,203.25,271.125,208.8 */
class SideFace3D : public SideFace
{

public:
  // constructeurs
  SideFace3D();
  SideFace3D(const SideFace3D &X);
  ~SideFace3D();

  // fonctions membres
  void computeNormal();
  Vec3D getTangent();
  Real computeDistanceWithPoint(const Vec3D &point);
  Vec3D computeIntersectionWith(const Vec3D &point, const Vec3D &traj);
};
/*>SideFace:*/

#endif
