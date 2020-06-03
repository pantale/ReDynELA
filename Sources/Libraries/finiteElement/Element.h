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
// revision date : 19/03/1999

/*
  Class Element definition
*/

/*!
  \file Element.h
  \brief fichier .h de d�finition des �l�ments finis
  \ingroup femLibrary

  Ce fichier sert � la d�finition de la classe Element.

  \author Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/

#ifndef __Element_h__
#define __Element_h__

#include <upFemSolver.h>

// necessary class definition before use
class Node;
class Domain;
class Material;
class IntegrationPointBase;
class IntegrationPoint;
class UnderIntegrationPoint;

//---------------------------------------Definition g�n�rale des �lements
// d�finition des edges (nouveaut� de DynELA 1.0.4)
#define maxNumberOfNodes 10
#define maxNumberOfFaces 6
#define maxNumberOfIntegrationPoints 8
#define maxNumberOfNodesNeighbour 3
#define maxNumberOfNodesByEdge 3
#define maxNumberOfNodesByFace 6
#define maxNumberOfNodesBySideFace 6
#define maxNumberOfSideFaces 6
#define maxNumberOfEdges 12

struct ElementEdge
{
    short numberOfNodes;
    short number[maxNumberOfNodesByEdge];
};

struct ElementIntegrationPoint
{
    Vec3D coords;            // coordonn�es du point d'int�gration
    Real weight;             // poids associ� au point d'int�gration
    Vector shapeFunction;    // fonctions d'interpolation au point d'int�gration
    Matrix derShapeFunction; // d�riv�es des fonctions d'interpolation au point d'int�gration
};

struct ElementFace
{
    short numberOfNodes;
    short number[maxNumberOfNodesByFace];
};

struct ElementSideFace
{
    short numberOfNodes;
    short number[maxNumberOfNodesBySideFace];
};

struct ElementNode
{
    Vec3D localCoords;
    short numberOfNeighbourNodes;
    short neighbour[maxNumberOfNodesNeighbour];
    Vector integrationPointsToNode;
};

// d�finition g�n�rale des �lements finis
struct ElementData
{
    // param�tres de l'�l�ment
    String name;
    short type;
    short family;
    short numberOfDimensions;
    short numberOfDDL;

    // description des noeuds de l'�lement
    int numberOfNodes;
    ElementNode nodes[maxNumberOfNodes];

    // description des points d'int�gration de l'�l�ment
    int numberOfIntegrationPoints;
    ElementIntegrationPoint integrationPoint[maxNumberOfIntegrationPoints];

    int numberOfUnderIntegrationPoints;
    ElementIntegrationPoint underIntegrationPoint[maxNumberOfIntegrationPoints];

    // description des faces de l'�lement
    int numberOfFaces;
    ElementFace faces[maxNumberOfFaces];

    // description des sideFaces de l'�l�ment
    int numberOfSideFaces;
    ElementSideFace sideFaces[maxNumberOfSideFaces];

    // description des aretes de l'�lement
    int numberOfEdges;
    ElementEdge edges[maxNumberOfEdges];

    String endOf;
};

//---------------------------------------Definition g�n�rale des �lements

/*!
  \class Element Element.h
  \brief classe de d�finition des �l�ments finis du maillage
  \ingroup femLibrary

  Classe de base des Elements.
  Cette classe sert de classe de base � tous les types d'�lements de DynELA. Dans cette classe, on peut trouver toutes les impl�mentations de base des �l�ments. Pour DynELA, la composition d'un �l�ment fini est d�finie par:
  - Une liste de noeuds constitutifs.
  - Une liste de points d'int�gration
  - Un mat�riau
  - Un ensemble de param�tres comme le nombre de noeuds, le nombre de points d'int�gration, le nombre de faces...

  \author Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/
class Element
{
    friend class ListId<Element *>;

public:
    enum
    {
        Unknown = 0,
        ElQua4n2D,
        ElTri3n2D,
        ElQua4nAx,
        ElHex8n3D,
        ElTet4n3D,
        ElTet10n3D
    };
    enum
    {
        Bidimensional = 0,
        Axisymetric,
        Threedimensional
    };

private:
    Vec3D NodeMin, NodeMax; //!< D�finition de la bounding box d'un �l�ment. Usage interne

public:
    /**.This always points to the CURRENT Integration Point of the element. You have to use the method getIntegrationPoint to change the current integration point of the element. So, all calculations refers now to the current integration point throw the ref pointer. Example : you have access to the Cauchy stress tensor at the integration point using ref->Sig.*/
    IntegrationPoint *ref;
    UnderIntegrationPoint *underRef;
    Matrix Ke;

protected:
    Matrix glob2loc;
    const ElementData *elementData;

public:
    List<IntegrationPoint *> intPoint;
    List<UnderIntegrationPoint *> underIntPoint;
    Indice Id;
    Indice number;
    Material *material;
    ListId<Node *> nodes;
    Domain *domain;

public:
    /**.Default constructor*/
    Element(Indice No_ = 1);
    Element(const Element &X);
    virtual ~Element();

    void computeMassMatrix(MatrixDiag &M);

    /*OLD*/
    void addIntegrationPoint(IntegrationPoint *field, Indice pt);
    void addIntegrationPoint(UnderIntegrationPoint *field, Indice pt);
    void addNode(Node *nd);
    void attachMaterial(Material *mat);
    Boolean check();
    virtual Boolean checkLevel2() = 0;
    void clearIntegrationPoint();
    void detachMaterial();
    void getIntegrationPoint(Indice pt);
    void getUnderIntegrationPoint(Indice pt);
    Indice getLocalNumber(Node *pnd);
    Boolean isNodeinElement(Node *pnd);
    Boolean isNodeinElement(Vec3D coords);
    Boolean isNodeinBoundBox(Node *pnd);
    Boolean isNodeinBoundBox(Vec3D coords);
    Boolean operator==(const Element &element) const;
    Boolean operator!=(const Element &element) const;
    Boolean operator<(const Element &element) const;
    Boolean operator>(const Element &element) const;
    friend ofstream &operator<<(ofstream &, const Element &);
    friend ifstream &operator>>(ifstream &, Element &);
    void write(ofstream &pfile) const;
    Element &read(ifstream &pfile);
    void toFile(FILE *pfile);
    void computeBoundBox();

    // accessors to data
    inline short getType();
    inline short getFamily();
    inline String getName();
    inline short getNumberOfNodes();
    inline short getNumberOfFaces();
    inline short getNumberOfEdges();
    inline short getNumberOfSideFaces();
    inline short getNumberOfNodesOnSideFace(short i);
    inline short getNumberOfNodesOnFace(short i);
    inline short getNumberOfIntegrationPoints();
    inline short getNumberOfDDL();
    inline short getNumberOfDimensions();
    inline short getNumberOfNodesOnEdge(short i);
    inline short getNumberOfNeighbourNodes(short i);
    inline short getLocalIdOfNodeOnEdge(short edge, short node);
    inline Vec3D getLocalNodeCoords(short node);

    Node *getNodeOnSideFace(short sideFace, short node);
    Node *getNodeOnFace(short face, short node);
    Node *getNodeOnEdge(short edge, short node);
    Node *getNeighbourNode(short node, short neighbour);

    void computeInternalForces(Vector &F);
    void computeStress();
    virtual void computeInternalMatrices(Boolean reference = False) = 0;
    virtual void computeElasticStiffnessMatrix(Matrix &K, Boolean under = True) = 0;
    void computeMassEquation(MatrixDiag &M, Vector &F);
    void computeMassEquation(MatrixDiag &M);
    void computeEnergyEquation(MatrixDiag &M, Vector &F);
    void computeMomentumEquation(MatrixDiag &M, Vector &F);
    void computeConstitutiveIntegration(MatrixDiag &M, Vector &F);
    void computeConstitutiveEquation();
    void computeStateEquation();
    Real getSoundSpeed();
    void computeStrains(Real);
    void getDens_atIntPoint(Real &ro);
    void getFe_atIntPoint(Vec3D &fe);
    void getEnergy(Real &e);
    void getdTemp_atIntPoints(Vec3D &dT);
    void getSigmaAtPoint(Tensor2 &Sig, const Vec3D &point);
    void get_Sig_atNode(Tensor2 &Sig, Indice node);
    void get_Eps_atNode(Tensor2 &Eps, Indice node);
    void get_dEps_atNode(Tensor2 &dEps, Indice node);
    void get_EpsPlas_atNode(Tensor2 &Sig, Indice node);
    void get_evp_atNode(Real &alpha, Indice i);
    virtual Indice numberOfUnderIntegrationPoints() = 0;
    virtual Real getLength() = 0;
    virtual Real getVolume() = 0;
    //  Boolean getIntegrationPointCoords (short, Vec3D & coords, Real& weight);
    Boolean linkIntegrationPointData(short);
    //  virtual  Boolean getUnderIntegrPointCoords (Indice, Vec3D & coords, Real& weight) = 0;
    virtual void getShapeFunctionAtPoint(Vector &N, const Vec3D &point) const = 0;
    virtual void getDerShapeFunctionAtPoint(Matrix &N, const Vec3D &point) = 0;
    virtual void getU_atIntPoint(Vec3D &u, int t) = 0;
    virtual void getV_atIntPoint(Vec3D &v, int t) = 0;
    virtual void getdU_atIntPoint(Tensor2 &du, int t) = 0;
    virtual void getF_atIntPoint(Tensor2 &du, int t) = 0;
    virtual void getdV_atIntPoint(Tensor2 &dv, int t) = 0;
    virtual void getRadius_atIntPoint(Real &R) = 0;
    virtual void computeGlob2Loc() = 0;
    virtual void glob2Loc(const Vec3D &point, Vec3D &local) = 0;
    //  virtual //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;
};

Boolean compareElementNumbers(Element *p1, Element *p2);
Indice diffElementNumber(Element *p1, const Indice ind);

struct LateralSurface
{
    Indice tab[maxNumberOfNodesBySideFace];
    Element *pel;
    short face;
};
Boolean compareLateralSurfaces(LateralSurface *p1, LateralSurface *p2);

//------------------------------- Inline -----------------------------------

//-----------------------------------------------------------------------------
inline String Element::getName()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->name;
}

//-----------------------------------------------------------------------------
inline short Element::getType()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->type;
}

//-----------------------------------------------------------------------------
inline short Element::getFamily()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->family;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfNodes()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->numberOfNodes;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfFaces()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->numberOfFaces;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfEdges()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->numberOfEdges;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfSideFaces()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->numberOfSideFaces;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfDDL()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->numberOfDDL;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfDimensions()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->numberOfDimensions;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfNodesOnSideFace(short i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > elementData->numberOfSideFaces)
        fatalError("Element::getNumberOfNodesOnSideFace",
                   "sideface %d is out of bounds %d-%d\n", i, 0, elementData->numberOfSideFaces);
#endif
    return elementData->sideFaces[i].numberOfNodes;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfNodesOnFace(short i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > elementData->numberOfFaces)
        fatalError("Element::getNumberOfNodesOnFace",
                   "face %d is out of bounds %d-%d\n", i, 0, elementData->numberOfFaces);
#endif
    return elementData->faces[i].numberOfNodes;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfNodesOnEdge(short i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > elementData->numberOfEdges)
        fatalError("Element::getNumberOfNodesOnEdge",
                   "edge %d is out of bounds %d-%d\n", i, 0, elementData->numberOfEdges);
#endif
    return elementData->edges[i].numberOfNodes;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfNeighbourNodes(short i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > elementData->numberOfNodes)
        fatalError("Element::getNumberOfNeighbourNodes",
                   "node %d is out of bounds %d-%d\n", i, 0, elementData->numberOfNodes);
#endif
    return elementData->nodes[i].numberOfNeighbourNodes;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfIntegrationPoints()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return elementData->numberOfIntegrationPoints;
}

//-----------------------------------------------------------------------------
inline short Element::getLocalIdOfNodeOnEdge(short edge, short node)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return (elementData->edges[edge].number[node]);
}

//-----------------------------------------------------------------------------
inline Vec3D Element::getLocalNodeCoords(short node)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(elementData != NULL);
#endif
    return (elementData->nodes[node].localCoords);
}

#endif
