/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2004                                                *
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
	Class PrecondDiag definition
*/

#ifndef __PrecondDiag_h__
#define __PrecondDiag_h__
/*!
  \file PrecondDiag.h
  \brief fichier .h de d�finition d'un pr�conditioneur diagonal
  \ingroup linearAlgebra

  Ce fichier d�finit la classe de pr�conditionneur diagonal pour les matrices. Cette classe est � utiliser avec les m�thodes de r�solution it�ratives.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#include <Vector.h>
#include <Matrices.h>
#include <Matrix.h>

/*!
  \class PrecondDiag PrecondDiag.h
  \brief Diagonal preconditionner Class
  \ingroup linearAlgebra

  Cette classe d�finit un pr�conditioneur diagonal � utiliser dans le cas de m�thodes de r�solution it�ratives de syst�mes lin�aires.

  Le taux de convergence des m�thodes it�ratives d�pend des propri�t�s spectrales des coefficients de matrices. Ainsi, on cherche � transformer le probl�me lineaire pos� en un syst�me �quivalent dans le sens que la solution est la m�me, mais qu'il a des propri�t�s spectrales plus favorables. Un pr�conditionneur est une matrice qui effectue ce type de transformation.

  Par exemple, si une matrice \f$ M \f$ approche les coefficients de la matrice \f$ A \f$ d'une certaine mani�re le syst�me modifi� \f[ M^{-1}Ax=M^{-1}b \f] poss�de les m�mes solutions que le syst�me d'origine, mais les propri�tes spectrales des coefficients de \f$ M^{-1}A \f$ sont plus favorables.

  En parlant de pr�conditionneur, on est face � un probl�me de choix concernant la matrice \f$ M \f$ qui approxime \f$ A \f$, et pour laquelle la r�solution du probl�me est plus facile qu'avec \f$ A \f$, ou trouver une matrice \f$ M \f$ qui approxime \f$ A^{-1} \f$ avec la cons�quence que seule la multiplication de matrice par \f$ M \f$ est raquise. La majorit� de pr�conditionneurs est de la premi�re cat�gorie.

  Le plus simple pr�conditionneur est constitu� des valeurs de la diagonale de la matrice de d�part \f[ M_{ij} = \delta_{ij}A_{ij}\f] il est connu sous le nom de pr�conditionneur de type Jacobi.

  Il est possible d'utiliser ce pr�conditionneur sans n�cessiter de nouveau stockage de la matrice elle m�me. Mais, comme les op�rations de division sont co�teuses en temps CPU, on pr�f�re cr�er une nouvelle matrice diagonale et stocker les inverses de la diagonale de la matrice originelle. Cette strat�gie est appliqu�e � beaucoup de pr�conditionneurs.

  En g�n�ral, les utilisateurs de ce type de m�thodes n'ont pas besoin d'avoir acc�s � beaucoup de m�thodes de la classe except� pour la cr�ation du pr�conditionneur.
  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
/** @dia:pos 34,20 */
/** @dia:route Matrices;v,29.675,13.8,16,43,20 */
class PrecondDiag : public Matrices
{
  Real *v;

private:
  // allocators
  void allocate(const Indice li_, const Indice co_);
  void desallocate();
  void redim(const Indice li_, const Indice co_);
  void fill(Real val);

public:
  // constructors
  PrecondDiag(const Indice li = 3, const Indice co = 3);
  PrecondDiag(const PrecondDiag &X);
  PrecondDiag(const Matrix &X);

  // destructor
  ~PrecondDiag();

  // methods
  Vector solve(const Vector &x) const;
  Vector trans_solve(const Vector &x) const;
  long Memory() const;
};

#endif
