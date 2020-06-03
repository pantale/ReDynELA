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

// bebgin date : 12/03/1997

/*
  Class Element definition
*/

/*!
  \file Element.C
  \brief fichier .C de d�finition des �l�ments finis
  \ingroup femLibrary

*/

#include <Element.h>
#include <Node.h>
#include <Material.h>
#include <IntegrationPoint.h>
#include <NodalField.h>

extern CPUrecord recordTimes;

#ifdef G2TR_STUDY
#include <Domain.h>
// #include <Physic.h>
#include <Structure.h>
#include <Solver.h>
#endif

//-----------------------------------------------------------------------------
Element::Element(Indice num)
//-----------------------------------------------------------------------------
{
  // affectation du numero d'element
  Id = num;
  number = num;

  // affectation par defaut NULL sur les materiaux
  material = NULL;

  // affectation par defaut NULL sur les ref
  ref = NULL;

  // affectation par defaut NULL sur les ref
  underRef = NULL;

  // affectation par defaut edges
  elementData = NULL;
}

//-----------------------------------------------------------------------------
Element::Element(const Element &el)
//-----------------------------------------------------------------------------
{
  cout << "recopie de l'element" << el.number << endl;
}

//-----------------------------------------------------------------------------
Element::~Element()
//-----------------------------------------------------------------------------
{
  nodes.flush();
}

//!Ajout d'un point d'int�gration � l'�l�ment courant
/*!
  Cette m�thode ajoute un point d'int�gration � l'�l�ment courant.
  \param field point d'int�gration � ajouter
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Element::addIntegrationPoint(IntegrationPoint *field, Indice pt)
//-----------------------------------------------------------------------------
{
  // v�rifier la nature du champ donn� en argument
#ifdef VERIF_assert
  if (field == NULL)
    fatalError("Element::addIntegrationPoint",
               "A NULL pointer has been passed to the function\n"
               "You are not allowed to insert a non existing integration point to the element %d",
               number);
#endif

  // ajout du point d'integration
  intPoint << field;

#ifdef VERIF_assert
  assert(elementData != NULL);
#endif
#ifdef VERIF_bounds
  if (pt > elementData->numberOfIntegrationPoints)
    fatalError("Element::addIntegrationPoint",
               "integration point %d is out of bounds %d-%d\n", pt, 0, elementData->numberOfIntegrationPoints);
#endif

  // recuperation des coordonnees du point d'integration (pt)
  //  getIntegrationPointCoords (pt, field->coords, field->weight);
  field->data = &(elementData->integrationPoint[pt]);
  //  linkIntegrationPointData(pt);

  // stockage du poids
  //  getIntegrPointWeight (pt, underIntPoint->weight);
}

//-----------------------------------------------------------------------------
void Element::addIntegrationPoint(UnderIntegrationPoint *field, Indice pt)
//-----------------------------------------------------------------------------
{
  // v�rifier la nature du champ donn� en argument
#ifdef VERIF_assert
  if (field == NULL)
    fatalError("Element::addIntegrationPoint",
               "A NULL pointer has been passed to the function\n"
               "You are not allowed to insert a non existing integration point to the element %d",
               number);
#endif

  // ajout du point d'integration
  underIntPoint << field;

#ifdef VERIF_assert
  assert(elementData != NULL);
#endif
#ifdef VERIF_bounds
  if (pt > elementData->numberOfUnderIntegrationPoints)
    fatalError("Element::addIntegrationPoint",
               "underIntegration point %d is out of bounds %d-%d\n", pt, 0, elementData->numberOfUnderIntegrationPoints);
#endif

  field->data = &(elementData->underIntegrationPoint[pt]);
  //  linkUnderIntegrationPointData(pt);MODIF
  // recuperation des coordonnees du point d'integration (pt)
  //  getUnderIntegrPointCoords (pt, field->coords, field->weight);

  // stockage du poids
  //  getUnderIntegrPointWeight (pt, underIntPoint->weight);
}

//-----------------------------------------------------------------------------
void Element::clearIntegrationPoint()
//-----------------------------------------------------------------------------
{
  // vider la liste
  intPoint.flush();
}

//-----------------------------------------------------------------------------
void Element::computeInternalForces(Vector &F)
//-----------------------------------------------------------------------------
{
  Indice pt, I, i, j;
  Real WxdJ;
  Real R;

  // calcul des contraintes au sein de l'�l�ment
  computeStress();

  // redim du vecteur F
  F.redim(elementData->numberOfNodes * getNumberOfDimensions());
  F = 0.0;

  // Boucle sur les points d'int�gration
  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {
    // recuperation du point d'integration
    getIntegrationPoint(pt);

    // calcul du terme d'integration numerique
    WxdJ = ref->data->weight * ref->detJ;
    if (getFamily() == Element::Axisymetric)
    {
      getRadius_atIntPoint(R);
      WxdJ *= deuxPI * R;
    }

    // calcul des forces internes
    for (I = 0; I < elementData->numberOfNodes; I++)
    {
      for (i = 0; i < getNumberOfDimensions(); i++)
      {
        for (j = 0; j < getNumberOfDimensions(); j++)
        {
          F(I * getNumberOfDimensions() + i) -=
              ref->dN(I, j) * ref->Sig(j, i) * WxdJ;
        }
      }
    }

    //contribution axisym�trique
    if (getFamily() == Element::Axisymetric)
    {
      for (I = 0; I < elementData->numberOfNodes; I++)
      {
        F(I * getNumberOfDimensions()) -= (elementData->integrationPoint[pt].shapeFunction(I) * ref->Sig(2, 2) / R) * WxdJ;
      }
    }
  }
}

//-----------------------------------------------------------------------------
void Element::computeStress()
//-----------------------------------------------------------------------------
{
  Tensor2 F;
  Indice pt;
  Real pressure;
  Tensor2 Identity;
  Identity.Identity();

  pressure = 0.0;

  Real dt = domain->currentSolver->timeStep;

  // Calcul des contraintes volumiques et deviatoriques

  Real K = material->young() / (3. * (1. - 2. * material->nu()));
  Real G = material->mu();

  // Boucle sur les points d'int�gration
  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {
    // recuperation du point d'integration
    getIntegrationPoint(pt);

    // calcul de F
    getF_atIntPoint(F, 0);

    // decomposition polaire et recuperation de U et R
    F.polarDecomposeLnU(ref->dEps, ref->R);

    // calcul de Eps
    ref->Eps += ref->dEps;

    // calcul de epsilon Equivalent total
    //      ref->EpsEqv = ref->Eps.equivalent ();

    // calcul de la contribution en pression
    pressure += ref->dEps.Trace();

    // ici on entre dans le sch�ma du retour radial

    // phase de pr�diction

    // calcul des contraintes pr�dites
    //      ref->dS = (2.0 * G) * (ref->dEps.deviator ());
    ref->S += (2.0 * G) * (ref->dEps.deviator());

    // rotation finale instantan�e de la contrainte
    //      ref->S = ref->R * (ref->S * ref->R.Transpose ());

    // declaration de gamma
    Real gamma = 0.0;
    Real h = 0.0;

    // phase de correction
    // test de la loi d'�coulement an�lastique
    if (material->isYield())
    {

      //calcul de la limite �lastique initiale
      Real Sigv0 = material->getIsotropicYieldStress(this);

      // crit�re de plasticit�
      // 	  if (fi_e.norme () - sqrt (2. / 3.) * pmat->getYieldStress (fpt) > 0.)
      Real Strial = ref->S.J2();
      // cout << Strial << " "<<Sigv0<<endl;
      if (Strial > Sigv0)
      {
        // calcul de la correction plastique
        //calcul de la valeur de h
        h = material->getIsotropicYieldHardening(this);
        //cout << h << endl;
        //	      exit (-1);

        // calcul de gamma dans le cas lin�aire
        gamma = (ref->S.norme() - Sqrt23 * Sigv0) / (2.0 * G * (1.0 + (h /* + eta/Deltat*/) / (3.0 * G)));

        // initialisations
        Real dgamma = 0;
        Real Sigv;
        // 	      Real evpl;
        Real dfun;
        Real fun;
        Boolean run = True;
        int iteration = 0;

        // Boucle iterative
        while (run)
        {
          // calcul de la limite elastique et de l'�crouissage
          Sigv = material->getIsotropicYieldStress(this, gamma);
          h = material->getIsotropicYieldHardening(this, gamma);

          // calcul de evpl
          // 		  evpl = ref->evp + Sqrt23 * gamma;
          dfun = Sqrt32 * 2.0 * G + Sqrt23 * h;
          fun = dfun * gamma - (Strial - Sigv + h * Sqrt23 * gamma);

          if (Abs(fun / Sigv) > radialReturnPrecision)
          {
            dgamma = -fun / dfun;
            if (dgamma < 0 && Abs(dgamma) > gamma)
              dgamma = -gamma * 0.9;
            gamma += dgamma;
          }
          else
          {
            run = False;
          }
          iteration++;
          if (iteration > radialReturnMaxIterations)
          {
            fatalError("Element::computeInternalForces", "Too many atempts in plastic procedure");
          }
        }

        Tensor2 n_1 = ref->S / (ref->S.norme());

        // update cumulated visco plastic strain
        ref->evp += Sqrt23 * gamma;

        // update stresses
        ref->S -= (2.0 * G * gamma) * n_1;

        // update eplastic deformations
        ref->EpsPlas += gamma * n_1;
      }
      else
      {
        // mise a jour de viscoplastique
        //GP1->evpl = GP0->evpl +sq23*gamma;
      }
    }

    // fin de calcul de la plasticit� du mat�riau
  }

  // calcul de la valeur moyenne de la pression sur l'�l�ment
  pressure /= getNumberOfIntegrationPoints();

#ifdef G2TR_STUDY
  Solver *psolver = Global_Structure->domains.current()->solvers.current();
#endif

  // calcul des contraintes finales
  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration
    getIntegrationPoint(pt);

    // ICI ON MODIFIE LE CODE POUR LES TP G2TR
#ifdef G2TR_STUDY
    // sans la sous-integration en pression
    if (psolver->underIntegratePressure == False)
    {
      // pression
      ref->p += K * ref->dEps.Trace();
    }
    else
    {
      // pression
      ref->p += K * pressure;
    }

    // avec la RFI
    if (psolver->RFI == True)
    {
      // rotation finale instantan�e des tenseurs
      ref->S = ref->R * (ref->S * ref->R.Transpose());             // RFI du d�viateur des contraintes
      ref->Eps = ref->R * (ref->Eps * ref->R.Transpose());         // RFI des deformations
      ref->EpsPlas = ref->R * (ref->EpsPlas * ref->R.Transpose()); // RFI des deformations plastiques
    }

#else
    // pression
    ref->p += K * pressure;
    // rotation finale instantan�e des tenseurs
    ref->S = ref->R * (ref->S * ref->R.Transpose());             // RFI du d�viateur des contraintes
    ref->Eps = ref->R * (ref->Eps * ref->R.Transpose());         // RFI des deformations
    ref->EpsPlas = ref->R * (ref->EpsPlas * ref->R.Transpose()); // RFI des deformations plastiques
#endif

    // contrainte
    ref->Sig = ref->S + ref->p * Identity;
  }
}

//!Calcul de la matrice de masse
/*!
Cette m�thode effectue le calcul de la matrice de masse au niveau d'un �l�ment
\param M matrice de masse de taille (n_nodes x n_nodes)
*/
//-----------------------------------------------------------------------------
void Element::computeMassMatrix(MatrixDiag &M)
//-----------------------------------------------------------------------------
{
  //Vector ro (elementData->numberOfNodes);
  Indice i, pt;
  Real WxdJ;

#ifdef VERIF_math
  // verifier la taille de la matrice M
  if ((M.rows() != elementData->numberOfNodes) || (M.columns() != elementData->numberOfNodes))
  {
    cerr << "Error in computeMassEquation()\nincompatible size of matrices M\n";
    cerr << "expected " << elementData->numberOfNodes << "x" << elementData->numberOfNodes << endl;
    cerr << "getting  " << M.rows() << "x" << M.columns() << endl;
    exit(-1);
  }
#endif

  // initialiser M
  M = 0.;

  // chargement des densites nodales initiales
  //  for (i = 0; i < elementData->numberOfNodes; i++)
  //    ro (i) = nodes (i)->Current->ro;

  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {
    // recuperation du point d'integration
    getIntegrationPoint(pt);

    // calcul du terme d'integration numerique
    WxdJ = ref->data->weight * ref->detJ;
    if (getFamily() == Element::Axisymetric)
    {
      Real R;
      getRadius_atIntPoint(R);
      WxdJ *= 2. * PI * R;
    }

    // calcul de la matrice M
    for (i = 0; i < elementData->numberOfNodes; i++)
      M(i) += /*nodes (i)->Current->ro*/ material->density() * (elementData->integrationPoint[pt].shapeFunction(i)) * WxdJ;
  }
}

/*//-----------------------------------------------------------------------------
Boolean Element::isIntegrationPoint()
//-----------------------------------------------------------------------------
{
  return (intPoint.size()==getNumberOfIntegrationPoints() ? Yes : No);
}*/

/*OLD*/
//-----------------------------------------------------------------------------
Boolean
Element::check()
//-----------------------------------------------------------------------------
{

  //  dN0.redim (elementData->numberOfNodes, getNumberOfDimensions());
  //  N0.redim (elementData->numberOfNodes);

  // check the number of nodes of the element
  if (nodes.size() != elementData->numberOfNodes)
    fatalError("",
               "Element %d has only %d nodes while this must have %d nodes\n",
               number, nodes.size(), elementData->numberOfNodes);

  // Version 5.0 (mis en commentaires)
  // check the number of integration points of the element
  //   if (intPoint.size()!=getNumberOfIntegrationPoints())
  //     fatalError("",
  //             "Element %d has only %d integration points while this must have %d integration points\n"
  //             ,number,intPoint.size(),getNumberOfIntegrationPoints());

  // check for the material
  if (material == NULL)
    fatalError("",
               "Element %d has no material attached to it\n"
               "this is a fault, you must attach a material definition to all the elements of the structure\n");

  // check for distorded elements
  checkLevel2();

  return True;
}

//-----------------------------------------------------------------------------
void Element::attachMaterial(Material *mater)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  // verif de la non nullite du pointeur passe
  assert(mater != NULL);

  if (material != NULL)
  {
    if (mater == material)
      cerr << "The two materials are the same\n";
    else
      cerr << "The two materials are the differents\n";
    fatalError("Element::attachMaterial",
               "This function have been called more than one time for the element %d.\n"
               "This is not allowed since you tried to attach two materials to the same element\n"
               "You must make a call to Element::detachMaterial before doing this\n",
               number);
  }
#endif

  material = mater;

  //  return(Success);
}

//-----------------------------------------------------------------------------
void Element::detachMaterial()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  if (material == NULL)
    fatalError("Element::detachMaterial",
               "This function have been called while there is no material attached to this element.\n"
               "This is not allowed since you tried to detach a non existing material to the element\n"
               "You must make a call to Element::attachMaterial before doing this\n");
#endif

  material = NULL;

  //  return(Success);
}

//!Positionne le point d'int�gration courant
/*!
Cette m�thode permet de r�f�rencer le point d'int�gration courant pour les calculs. On choisit le num�ro du point d'int�gration, puis on peut ensuite obtenir directement ce point par Element->ref->.
\param pt indice du point d'int�gration courant dans la liste des points d'int�gration de l'�l�ment.
*/
//-----------------------------------------------------------------------------
void Element::getIntegrationPoint(Indice pt)
//-----------------------------------------------------------------------------
{
  // pointage sur le point d'int�gration
  ref = intPoint(pt);

#ifdef VERIF_assert
  if (ref == NULL)
  {
    fatalError("Element::getIntegrationPoint",
               "Integration point [%d] doesn't exist for the element %d",
               pt, Id);
  }
#endif
}

//!Positionne le point de sous-int�gration courant
/*!
Cette m�thode permet de r�f�rencer le point d'int�gration courant pour les calculs dans les sous-integration. On choisit le num�ro du point de sous-int�gration, puis on peut ensuite obtenir directement ce point par Element->ref->.
\param pt indice du point d'int�gration courant dans la liste des points d'int�gration de l'�l�ment.
*/
//-----------------------------------------------------------------------------
void Element::getUnderIntegrationPoint(Indice pt)
//-----------------------------------------------------------------------------
{
  // pointage sur le point d'int�gration
  underRef = underIntPoint(pt);

#ifdef VERIF_assert
  if (underRef == NULL)
  {
    fatalError("Element::getIntegrationPoint",
               "Integration point [%d] doesn't exist for the element %d",
               pt, Id);
  }
#endif
}

//-----------------------------------------------------------------------------
Indice
Element::getLocalNumber(Node *nd)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < nodes.size(); i++)
  {
    if (nodes(i) == nd)
      return i;
  }
  cerr << "indice " << nd->number << " not found";
  return 0;
}

//!Calcul du de la vitesse de propagation d'une onde.
/*!
Cette m�thode calcule la vitesse de propagation d'une onde longitudinale dans un �l�ment.
La relation utilis�e pour ce calcul est donn�e par:
\f[ C_s=\sqrt{ \frac{E(1-\nu)}{\rho(1+\nu)(1-2\nu)}} \f]
*/
//-----------------------------------------------------------------------------
Real Element::getSoundSpeed()
//-----------------------------------------------------------------------------
{
  Real E = material->young();
  Real Nu = material->nu();

  // calcul de la densit� moyenne de l'�l�ment (est-ce vraiment n�cessaire ?)
  Real ro = 0.;
  for (Indice i = 0; i < elementData->numberOfNodes; i++)
    ro += nodes(i)->Current->ro;
  ro /= elementData->numberOfNodes;

  // vitesse du son dans le materiau
  return sqrt((E * (1 - Nu)) / (ro * (1 + Nu) * (1 - 2 * Nu)));
}

/*//-----------------------------------------------------------------------------
Real Element::getThermalSpeed()
//-----------------------------------------------------------------------------
{
  Real	Cp=material->heatCoeff();
  Real	K=material->conductivity();

  // calcul de la densit� moyenne de l'element
  Real	ro=0.;
  for (Indice i=0;i<elementData->numberOfNodes;i++) ro+=nodes(i)->Current->ro;
  ro/=elementData->numberOfNodes;
  
  // vitesse du son dans le materiau
  return ((ro*Cp)/(2.*K));
//  return sqrt((E*(1-Nu))/(ro*(1+Nu)*(1-2*Nu)));
}*/

//!Phase d'int�gration de la loi de conservation de la masse
/*!
Cette m�thode effectue la phase d'int�gration de la loi de conservation de la masse pour un �l�ment.
\param M matrice de masse de taille (n_nodes x n_nodes)
\param F vecteur de taille (n_nodes)
*/
//-----------------------------------------------------------------------------
void Element::computeMassEquation(MatrixDiag &M, Vector &F)
//-----------------------------------------------------------------------------
{
  Vector ro(elementData->numberOfNodes);
  Tensor2 dv;
  Indice i, j, pt;
  Real /*mass,alpha, */ WxdJ;

#ifdef VERIF_math
  // verifier la taille de la matrice M
  if ((M.rows() != elementData->numberOfNodes) || (M.columns() != elementData->numberOfNodes))
  {
    cerr << "Error in computeMassEquation()\nincompatible size of matrices M\n";
    cerr << "expected " << elementData->numberOfNodes << "x" << elementData->numberOfNodes << endl;
    cerr << "getting  " << M.rows() << "x" << M.columns() << endl;
    exit(-1);
  }

  // verifier la taille du vecteur F
  if (F.size() != elementData->numberOfNodes)
  {
    cerr << "Error in computeMassEquation()\nincompatible size of vector F\n";
    cerr << "expected " << elementData->numberOfNodes << endl;
    cerr << "getting  " << F.size() << endl;
    exit(-1);
  }
#endif

  // initialiser M, F
  M = 0.;
  F = 0.;
  //  mass=0.;

  // chargement des densites nodales pr�dites
  for (i = 0; i < elementData->numberOfNodes; i++)
    ro(i) = nodes(i)->New->ro;
  // cout << "ro="<<ro<<endl;

  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration
    getIntegrationPoint(pt);

    // calcul de la vitesse convective au point
    //    getConvVel(c);
    // cout << "c="<<c<<endl;

    // calcul du gradient de vitesses au point (Version 5 vitesse a la fin de l'increment)
    getdV_atIntPoint(dv, 1);

    // cout << "dv="<<dv<<endl;

    // calcul du terme d'integration numerique
    WxdJ = ref->data->weight * ref->detJ;
    if (getFamily() == Element::Axisymetric)
    {
      Real R;
      getRadius_atIntPoint(R);
      WxdJ *= 2. * PI * R;
    }

    // calcul de l'increment de masse pour le lumping
    //    mass+=WxdJ;

    // calcul de la matrice M consistante
    for (i = 0; i < elementData->numberOfNodes; i++)
      M(i) += /*SQ*/ (elementData->integrationPoint[pt].shapeFunction(i)) * WxdJ;

    // calcul des vecteurs de densites
    for (i = 0; i < elementData->numberOfNodes; i++)
    {

      // calcul de Ni * WxdJ
      //      NiWxdJ=WxdJ*ref->N(i);

      for (j = 0; j < elementData->numberOfNodes; j++)
      {
        //      NiroWxdJ=NiWxdJ*ro(j);

        //F(i)-=dv.Trace()*ref->N(j)*NiroWxdJ;
        F(i) -= elementData->integrationPoint[pt].shapeFunction(i) * elementData->integrationPoint[pt].shapeFunction(j) * dv.Trace() * ro(j) * WxdJ;
      }
    }
  }

  // lumping de la matrice de masse
  //   alpha=mass/M.Trace();
  //   for (i=0;i<elementData->numberOfNodes;i++) M(i)*=alpha;
  //cout << "M="<<M<<endl;
}

//!Calcul de la matrice de masse d'un �l�ment
/*!
  Cette m�thode calcule la matrice de masse d'un �l�ment fini.
  \param M matrice de masse de taille quelconque (modifi�e par cette m�thode si besoin)
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void Element::computeMassEquation(MatrixDiag &M)
//-----------------------------------------------------------------------------
{
  Indice i, pt;
  Real WxdJ;

  // redim de la matrice
  M.redim(elementData->numberOfNodes, elementData->numberOfNodes);

  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {
    // recuperation du point d'integration
    getIntegrationPoint(pt);

    // calcul du terme d'integration numerique
    WxdJ = ref->data->weight * ref->detJ;
    if (getFamily() == Element::Axisymetric)
    {
      Real R;
      getRadius_atIntPoint(R);
      WxdJ *= deuxPI * R;
    }

    // calcul de la matrice M consistante
    for (i = 0; i < elementData->numberOfNodes; i++)
      M(i) += (elementData->integrationPoint[pt].shapeFunction(i)) * WxdJ;
  }
}

//!Phase d'int�gration de la loi de conservation de la quantit� de mouvement
/*!
Cette m�thode effectue la phase d'int�gration de la loi de conservation de la quantit� de mouvement d'un �l�ment.
\param M matrice de masse de taille (n_nodes*n_dim x n_nodes*n_dim)
\param F vecteur de taille (n_nodes*n_dim)
*/
//-----------------------------------------------------------------------------
void Element::computeMomentumEquation(MatrixDiag &M, Vector &F)
//-----------------------------------------------------------------------------
{
  //  Vec3D       fe;
  Indice pt;
  Indice i, j, I;
  Real ro, WxdJ, R;

#ifdef VERIF_math
  // verifier la taille de la matrice M
  if ((M.rows() != elementData->numberOfNodes * getNumberOfDimensions()) || (M.columns() != elementData->numberOfNodes * getNumberOfDimensions()))
  {
    cerr << "Error in computeMomentumEquation()\nincompatible size of matrices M\n";
    cerr << "expected " << getNumberOfDimensions() * elementData->numberOfNodes << "x" << getNumberOfDimensions() * elementData->numberOfNodes << endl;
    cerr << "getting  " << M.rows() << "x" << M.columns() << endl;
    exit(-1);
  }

  // verifier la taille du vecteur F
  if (F.size() != elementData->numberOfNodes * getNumberOfDimensions())
  {
    cerr << "Error in computeMomentumEquation()\nincompatible size of vector F\n";
    cerr << "expected " << elementData->numberOfNodes * getNumberOfDimensions() << endl;
    cerr << "getting  " << F.size() << endl;
    exit(-1);
  }
#endif

  // initialiser M, F
  M = 0.;
  F = 0.;
  //  mass=0.;

  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration
    getIntegrationPoint(pt);

    // calcul de la masse volumique au point
    getDens_atIntPoint(ro);

    //    Sig=0.;
    //    for (i=0;i<elementData->numberOfNodes;i++) Sig+=ref->N(i)*nodes(i)->Sig;

    // calcul des efforts externes
    //    getFe_atIntPoint(fe);

    // calcul du terme d'integration numerique
    WxdJ = ref->data->weight * ref->detJ;
    if (getFamily() == Element::Axisymetric)
    {
      getRadius_atIntPoint(R);
      WxdJ *= 2 * PI * R;
      //      RR+=R;
    }

    // calcul de l'increment de masse pour le lumping
    //    mass+=ro*WxdJ;

    // calcul de M consistent et de la contribution en masse
    for (I = 0; I < elementData->numberOfNodes; I++)
      for (i = 0; i < getNumberOfDimensions(); i++)
        M(I * getNumberOfDimensions() + i) += /*SQ*/ (elementData->integrationPoint[pt].shapeFunction(I)) * (ro * WxdJ);

    //     for (I=0;I<elementData->numberOfNodes;I++)
    //     for (J=0;J<elementData->numberOfNodes;J++)
    //       for (i=0;i<getNumberOfDimensions();i++)
    //      MC(2*I+i,2*J+i)+=(ref->N(I)*ref->N(J))*(ro*WxdJ);

    // calcul des forces internes
    for (I = 0; I < elementData->numberOfNodes; I++)
    {
      for (i = 0; i < getNumberOfDimensions(); i++)
      {
        for (j = 0; j < getNumberOfDimensions(); j++)
        {
          F(I * getNumberOfDimensions() + i) -=
              ref->dN(I, j) * ref->Sig(j, i) * WxdJ;
        }
      }
    }

    //contribution axisym�trique
    if (getFamily() == Element::Axisymetric)
    {
      // R deja calcul�
      getRadius_atIntPoint(R);

      for (I = 0; I < elementData->numberOfNodes; I++)
      {
        F(I * getNumberOfDimensions()) -= (elementData->integrationPoint[pt].shapeFunction(I) * ref->Sig(2, 2) / R) * WxdJ;
      }
    }
  }
}

//!Phase d'int�gration de la loi de comportement
/*!
Cette m�thode effectue la phase d'int�gration de la loi de comportement du mat�riau sur l'�l�ment. Elle fournit les quantit�s suivantes pour un �l�ment: \f[ \stackrel{\bullet}{S_1} = 2 G dev[E^N] \f] et \f[ S_1 = S_0 + \stackrel{\bullet}{S_1} \f]
*/
//-----------------------------------------------------------------------------
void Element::computeConstitutiveEquation()
//-----------------------------------------------------------------------------
{
  Indice pt;

  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration
    getIntegrationPoint(pt);

    //      ref->dS = (2.0 * material->mu ()) * (ref->dEps.deviator ());
    ref->S += (2.0 * material->mu()) * (ref->dEps.deviator());
  }
}

//!Phase d'int�gration de la loi d'�tat
/*!
Cette m�thode effectue la phase d'int�gration de la loi d'�tat du mat�riau sur l'�l�ment. Ce calcul fait intervenir une sous int�gration s�lective de l'�l�ment pour la loi d'�tat. Cette sous int�gration est tr�s simple, et ne fonctionne que pour des �l�ments � formulation lin�aire (sous int�gration � 1 point central), elle devra etre reprise pour des �l�ments plus complets si besoin. Normalement non, car seuls les lin�aires sont utilis�s en explicite. La relation de bas est la suivante: \f[ p_1 = p_0+K tr[E^N] \f]
*/
//-----------------------------------------------------------------------------
void Element::computeStateEquation()
//-----------------------------------------------------------------------------
{
  Indice pt;
  Real K;

  K = material->young() / (3. * (1. - 2. * material->nu()));

  // version 0.9.5
  // cette modif est destin�e � moyenner la pression sur l'�l�ment
  // au moment du calcul de l'integration (num�rique locale)
  Real pp = 0;
  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration
    getIntegrationPoint(pt);

    pp += ref->dEps.Trace();
  }
  pp /= getNumberOfIntegrationPoints();
  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration
    getIntegrationPoint(pt);

    ref->p += K * pp;
  }
  // fin de modif 0.9.5
}

//!Phase d'int�gration de la loi de conservation de l'�nergie
/*!
Cette m�thode effectue la phase d'int�gration de la loi de conservation de l'�nergie d'un �l�ment.
\param M matrice de masse de taille (n_nodes x n_nodes)
\param F vecteur de taille (n_nodes)
*/
//-----------------------------------------------------------------------------
void Element::computeEnergyEquation(MatrixDiag &M, Vector &F)
//-----------------------------------------------------------------------------
{
  Vector e(elementData->numberOfNodes); // vecteur des densites aux noeuds
  Vec3D dT;
  //  Vec3D       c; // vitesses convectives
  Tensor2 Sig;
  Tensor2 dv;  // matrice gradient des vitesses
  Tensor2 dvs; // matrice gradient des vitesses
  Indice pt;
  Indice i, k;
  Real /*mass,alpha, */ WxdJ, ro;
  Real SigklVkl, lambda;

#ifdef VERIF_math
  // verifier la taille de la matrice M
  if ((M.rows() != elementData->numberOfNodes) || (M.columns() != elementData->numberOfNodes))
  {
    cerr << "Error in computeEnergyEquation()\nincompatible size of matrices M\n";
    cerr << "expected " << elementData->numberOfNodes << "x" << elementData->numberOfNodes << endl;
    cerr << "getting  " << M.rows() << "x" << M.columns() << endl;
    exit(-1);
  }

  // verifier la taille du vecteur F
  if (F.size() != elementData->numberOfNodes)
  {
    cerr << "Error in computeEnergyEquation()\nincompatible size of vector F\n";
    cerr << "expected " << elementData->numberOfNodes << endl;
    cerr << "getting  " << F.size() << endl;
    exit(-1);
  }
#endif

  // initialiser M, F
  M = 0.;
  F = 0.;
  //  mass=0.;

  // chargement des energies nodales
  for (i = 0; i < elementData->numberOfNodes; i++)
    e(i) = nodes(i)->New->e;
  // cout << "e="<<e<<endl;

  // calcul de la conductivite thermique du materiau
  lambda = material->conductivity();

  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration
    getIntegrationPoint(pt);

    // calcul de la vitesse convective au point
    //    getConvVel(c);
    // cout << "c="<<c<<endl;

    // calcul de la densite au point
    getDens_atIntPoint(ro);
    // cout << "ro="<<ro<<endl;

    // calcul de la temperature au point
    getdTemp_atIntPoints(dT);

    // calcul du tenseur des contraintes au point
    Sig = intPoint(pt)->Sig;
    // cout << "Sig="<<Sig<<endl;

    // calcul du gradient de vitesses au point
    // getdV(dv);
    //    getdV_atIntPoint(dv,1);
    //    dv.getSymetricPart(dvs);
    // cout << "dv="<<dv<<endl;
    // cout << "dvs="<<dvs<<endl;

    // calcul de SigklVkl
    SigklVkl = Sig.prod2(intPoint(pt)->dEpsPlas);

    // calcul du terme d'integration numerique
    WxdJ = ref->data->weight * ref->detJ;
    if (getFamily() == Element::Axisymetric)
    {
      Real R;
      getRadius_atIntPoint(R);
      WxdJ *= 2 * PI * R;
    }

    // calcul de l'increment de masse pour le lumping
    //  mass+=ro*WxdJ;

    // calcul de M consistent et de la contribution en masse
    for (i = 0; i < elementData->numberOfNodes; i++)
      M(i) += ro * /*SQ*/ (elementData->integrationPoint[pt].shapeFunction(i)) * WxdJ;

    // calcul des vecteurs de densites
    for (i = 0; i < elementData->numberOfNodes; i++)
    {
      /*                tmpConv=0.;
	     for (j=0;j<elementData->numberOfNodes;j++) {
	     for (k=0;k<getNumberOfDimensions();k++) {
	     tmpConv+=c(k)*ref->dN(j,k)*e(j);
	     }
	     }      */

      // contribution de la partie convective
      //        F(i)-=ro*ref->N(i)*tmpConv*WxdJ;

      // contribution de la deformation
      F(i) += elementData->integrationPoint[pt].shapeFunction(i) * SigklVkl * WxdJ;

      // partie conductive
      for (k = 0; k < getNumberOfDimensions(); k++)
        F(i) -= lambda * ref->dN(i, k) * dT(k) * WxdJ;
    }
  }

  // lumping de la matrice de masse
  //   alpha=mass/M.Trace();
  //   for (i=0;i<elementData->numberOfNodes;i++) M(i)*=alpha;
}

//!Phase de calcul des d�formations
/*!
Cette m�thode effectue le calcul des d�formations au sein de l'�l�ment � l'instant t. Cette m�thode utilise la m�thode de calcul Element::getF_atIntPoint(Tensor2 & F,int t) pour d�terminer la valeur du tenseur gradient des d�formations au sein de l'�l�ment, puis la m�thode Tensor2::polarDecomposeLnU(Tensor2 &U,Tensor2& R) pour la d�composition polaire du tenseur gradient des d�formations F en partie d�formation pure U et rotation pure V. On met � jour dEps, R, Eps et EpsEqv.
\param timeStep valeur du time step courant pour la structure (fourni par Domain).
*/
//-----------------------------------------------------------------------------
void Element::computeStrains(Real timeStep)
//-----------------------------------------------------------------------------
{
  Tensor2 F;
  Indice pt;

  for (pt = 0; pt < getNumberOfIntegrationPoints(); pt++)
  {

    // recuperation du point d'integration
    getIntegrationPoint(pt);

    // calcul de F
    getF_atIntPoint(F, 1);

    // decomposition polaire et recup de U et R
    F.polarDecomposeLnU(ref->dEps, ref->R);

    // calcul de Eps
    ref->Eps += ref->dEps;

    // calcul de epsilon Equivalent total
    //      ref->EpsEqv = ref->Eps.equivalent ();
  }
}

//!Calcul du gradient de temp�rature sur un point d'int�gration
/*!
Cette m�thode permet de calculer le gradient de temp�rature sur un point d'int�gration. On calcule le gradient de temp�rature au point d'int�gration courant par la relation: \f[ T_{,i}^p = \sum_{j=1}^{nNodes} \varphi_{j,i} T^{j}\f] 
*/
//-----------------------------------------------------------------------------
void Element::getdTemp_atIntPoints(Vec3D &dT)
//-----------------------------------------------------------------------------
{
  Indice i, k;

  dT = 0.;
  for (i = 0; i < getNumberOfDimensions(); i++)
    for (k = 0; k < elementData->numberOfNodes; k++)
      dT(i) += ref->dN(k, i) * nodes(k)->Current->T;
}

//!R�cup�ration de la densit� sur un point d'int�gration
/*!
Cette m�thode permet de transferer les densit�s (quantit�s nodales) aux points d'int�gration. Elle transfere au point d'int�gration courant la valeur de la densit� interpol�e � l'int�rieur de l'�lement. La valeur fournie est \f$ \rho_1 \f$, elle est d�finie par: \f[ \rho_1^p = \sum_{i=1}^{nNodes} \varphi_i \rho_1^{i}\f] 
*/
//-----------------------------------------------------------------------------
void Element::getDens_atIntPoint(Real &ro)
//-----------------------------------------------------------------------------
{
  Indice i;

  ro = 0.;
  for (i = 0; i < elementData->numberOfNodes; i++)
    ro += ref->data->shapeFunction(i) * nodes(i)->New->ro;
}

//!R�cup�ration du vecteur des efforts externes sur un point d'int�gration
/*!
Cette m�thode permet de transferer les efforts externes (quantit�s nodales) aux points d'int�gration. Elle transfere au point d'int�gration courant la valeur des efforts externes interpol�s � l'int�rieur de l'�lement. La valeur fournie est \f$ fe_1 \f$, elle est d�finie par: \f[ fe_1^p = \sum_{I=1}^{nNodes} \varphi_I fe_1^{I}\f] 
*/
//-----------------------------------------------------------------------------
void Element::getFe_atIntPoint(Vec3D &fe)
//-----------------------------------------------------------------------------
{
  Indice i, j;

  fe = 0.;
  for (i = 0; i < elementData->numberOfNodes; i++)
    for (j = 0; j < getNumberOfDimensions(); j++)
      fe(j) += ref->data->shapeFunction(i) * nodes(i)->New->fe(j);
}

//-----------------------------------------------------------------------------
void Element::getEnergy(Real &e)
//-----------------------------------------------------------------------------
{
  Indice i;

  e = 0.;
  for (i = 0; i < elementData->numberOfNodes; i++)
    e += ref->data->shapeFunction(i) * nodes(i)->New->e;
}

//-----------------------------------------------------------------------------
void Element::getSigmaAtPoint(Tensor2 &Sig, const Vec3D &point)
//-----------------------------------------------------------------------------
{
  Indice i;
  Vec3D loc;
  Vector N(getNumberOfIntegrationPoints());

  // calcul global local
  glob2Loc(point, loc);

  // calcul des fonctions d'interpolation
  //  getIntgtoNodes (N, loc);
  fatalError("Element::getSigmaAtPoint", "Not implemented\n");

  // initialisation
  Sig = 0.;

  // interpolation du tenseur des contraintes
  for (i = 0; i < getNumberOfIntegrationPoints(); i++)
    Sig += N(i) * intPoint(i)->Sig;
}

// d�finition de l'algorithme de calcul des valeurs nodales en fonction des valeurs stock�es au point d'int�gration
#define get_IntegrationValue_atNode(VAR, NODE)                                                    \
  {                                                                                               \
    Node *pnd = NODE;                                                                             \
    Element *pel;                                                                                 \
    Indice j, pt, nb = 0;                                                                         \
    VAR = 0.;                                                                                     \
    for (j = 0; j < pnd->elements.size(); j++)                                                    \
    {                                                                                             \
      pel = pnd->elements(j);                                                                     \
      Indice loc = pel->nodes.IAppN(pnd->Id);                                                     \
      for (pt = 0; pt < pel->getNumberOfIntegrationPoints(); pt++)                                \
      {                                                                                           \
        VAR += pel->elementData->nodes[loc].integrationPointsToNode(pt) * pel->intPoint(pt)->VAR; \
      }                                                                                           \
      nb++;                                                                                       \
    }                                                                                             \
    VAR = VAR / nb;                                                                               \
  }

//-----------------------------------------------------------------------------
void Element::get_Sig_atNode(Tensor2 &Sig, Indice i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(Sig, nodes(i));
}

//-----------------------------------------------------------------------------
void Element::get_Eps_atNode(Tensor2 &Eps, Indice i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(Eps, nodes(i));
}

//-----------------------------------------------------------------------------
void Element::get_dEps_atNode(Tensor2 &dEps, Indice i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(dEps, nodes(i));
}

//-----------------------------------------------------------------------------
void Element::get_EpsPlas_atNode(Tensor2 &EpsPlas, Indice i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(EpsPlas, nodes(i));
}

//-----------------------------------------------------------------------------
void Element::get_evp_atNode(Real &evp, Indice i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(evp, nodes(i));
}

//-----------------------------------------------------------------------------
Boolean
Element::operator==(const Element &element) const
//-----------------------------------------------------------------------------
{
  if (Id != element.Id)
    return (False);
  if (nodes != element.nodes)
    return (False);
  return (True);
}

//-----------------------------------------------------------------------------
Boolean
Element::operator!=(const Element &element) const
//-----------------------------------------------------------------------------
{
  return !(*this == element);
}

//-----------------------------------------------------------------------------
Boolean
Element::operator>(const Element &element) const
//-----------------------------------------------------------------------------
{
  return (Id > element.Id);
}

//-----------------------------------------------------------------------------
Boolean
Element::operator<(const Element &element) const
//-----------------------------------------------------------------------------
{
  return (Id < element.Id);
}

//-----------------------------------------------------------------------------
void Element::addNode(Node *nd)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  // verif de ce que l'on a passe a la fonction
  if (nd == NULL)
    fatalError("Element::addNode",
               "Tried to insert a non existing node in the element %d\n"
               "the corresponding node is a NULL pointer",
               number);
#endif

  // add the corresponding node in the element
  nodes << nd;
}

//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const Element &nd)
//-----------------------------------------------------------------------------
{
  nd.write(os);
  return os;
}

//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, Element &nd)
//-----------------------------------------------------------------------------
{
  nd.read(is);
  return is;
}

//-----------------------------------------------------------------------------
void Element::write(ofstream &pfile) const
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < intPoint.size(); i++)
  {
    intPoint(i)->write(pfile);
  }
}

//-----------------------------------------------------------------------------
Element &Element::read(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < intPoint.size(); i++)
  {
    pfile >> *(intPoint(i));
  }

  return *this;
}

//-----------------------------------------------------------------------------
void Element::computeBoundBox()
//-----------------------------------------------------------------------------
{
  Indice i, j;
  Node *pnd;

  // initialisation du vecteur
  NodeMin = NodeMax = nodes(0)->coords;

  for (i = 1; i < nodes.size(); i++)
  {
    pnd = nodes(i);
    for (j = 0; j < 3; j++)
    {
      if (pnd->coords(j) < NodeMin(j))
        NodeMin(j) = pnd->coords(j);
      if (pnd->coords(j) > NodeMax(j))
        NodeMax(j) = pnd->coords(j);
    }
  }
}

//-----------------------------------------------------------------------------
Boolean
Element::isNodeinBoundBox(Vec3D coords)
//-----------------------------------------------------------------------------
{
  if ((coords(0) < NodeMin(0)) || (coords(0) > NodeMax(0)))
    return (False);
  if ((coords(1) < NodeMin(1)) || (coords(1) > NodeMax(1)))
    return (False);
  if ((coords(2) < NodeMin(2)) || (coords(2) > NodeMax(2)))
    return (False);
  return (True);
}

//-----------------------------------------------------------------------------
Boolean
Element::isNodeinBoundBox(Node *pnd)
//-----------------------------------------------------------------------------
{
  return isNodeinBoundBox(pnd->coords);
}

//-----------------------------------------------------------------------------
Boolean
Element::isNodeinElement(Node *pnd)
//-----------------------------------------------------------------------------
{
  return isNodeinElement(pnd->coords);
}

//-----------------------------------------------------------------------------
Boolean
Element::isNodeinElement(Vec3D coords)
//-----------------------------------------------------------------------------
{
  // test rapide
  if (isNodeinBoundBox(coords) == False)
    return (False);

  Vec3D loc;
  Vector shapes(getNumberOfNodes());

  // calcul du passage global en local
  glob2Loc(coords, loc);

  // calcul des fonctions d'interpolation au point
  getShapeFunctionAtPoint(shapes, loc);

  // recherche des bornes
  for (Indice i = 0; i < getNumberOfNodes(); i++)
  {

    // recherche avec une tol�rance 1e-7
    if ((shapes(i) < -0.0000001) || (shapes(i) > 1.0000001))
      return (False);
  }

  // ok, le noeud est dedans
  return (True);
}

//-----------------------------------------------------------------------------
void Element::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  fprintf(pfile, "%8ld  %s  ", number, getName().chars());
  for (Indice i = 0; i < elementData->numberOfNodes; i++)
  {
    fprintf(pfile, "%6ld ", nodes(i)->number);
  }
  fprintf(pfile, "\n");
}

//-----------------------------------------------------------------------------
Boolean compareElementNumbers(Element *p1, Element *p2)
//-----------------------------------------------------------------------------
{
  return (p1->number > p2->number); // comparaison
}

//-----------------------------------------------------------------------------
Indice diffElementNumber(Element *p1, const Indice number)
//-----------------------------------------------------------------------------
{
  return (p1->number - number); // comparaison
}

//-----------------------------------------------------------------------------
Node *Element::getNodeOnEdge(short edge, short node)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(elementData != NULL);
#endif
  return nodes(elementData->edges[edge].number[node]);
}

//-----------------------------------------------------------------------------
Node *Element::getNodeOnSideFace(short sideface, short node)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(elementData != NULL);
#endif
  return nodes(elementData->sideFaces[sideface].number[node]);
}

//-----------------------------------------------------------------------------
Node *Element::getNodeOnFace(short face, short node)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(elementData != NULL);
#endif
  return nodes(elementData->faces[face].number[node]);
}

//-----------------------------------------------------------------------------
Node *Element::getNeighbourNode(short node, short neighbour)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(elementData != NULL);
#endif
  return nodes(elementData->nodes[node].neighbour[neighbour]);
}

/*//-----------------------------------------------------------------------------
//Boolean Element::getIntegrationPointCoords (short i, Vec3D & coords, Real & weight)
Boolean Element::linkIntegrationPointData (short i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(elementData!=NULL);
#endif
#ifdef VERIF_bounds
    if (i > elementData->numberOfIntegrationPoints) 
      fatalError("Element::getIntegrPointCoords",
		 "integration point %d is out of bounds %d-%d\n",i,0,elementData->numberOfIntegrationPoints);
#endif

  data = elementData->integrationPoint[i];

    return True;
}*/

//-----------------------------------------------------------------------------
Boolean compareLateralSurfaces(LateralSurface *p1, LateralSurface *p2)
//-----------------------------------------------------------------------------
{
  short i;

  // test if elements are from the same domain
  if (p1->pel->domain != p2->pel->domain)
    return (p1->pel->domain > p2->pel->domain);

  for (i = 0; i < maxNumberOfNodesBySideFace - 1; i++)
  {
    if (p1->tab[i] < p2->tab[i])
      return 0;
    if (p1->tab[i] > p2->tab[i])
      return 1;
  }
  return (p1->tab[i] > p2->tab[i]);
}
