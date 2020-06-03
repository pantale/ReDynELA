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
  Class PrecondDiag implementation
*/
/*!
  \file PrecondDiag.C
  \brief fichier .C de d�finition d'un pr�conditioneur diagonal
  \ingroup linearAlgebra

  Ce fichier d�finit la classe de pr�conditionneur diagonal pour les matrices. Cette classe est � utiliser avec les m�thodes de r�solution it�ratives.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#include <PrecondDiag.h>

//!constructeur par d�faut de la classe PrecondDiag
/*!
  Cette m�thode est le constructeur par d�faut de la classe PrecondDiag. Le nouveau pr�conditionneur est une matrice diagonale remplie de 0.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
PrecondDiag::PrecondDiag(const Indice li_, const Indice co_)
//-----------------------------------------------------------------------------
{
  // allocation memoire;
  allocate(li_, co_);

  // initialisation
  fill(0.);
}

//!constructeur par recopie de la classe PrecondDiag
/*!
  Cette m�thode recopie le contenu d'un pr�conditionneur diagonal dans un autre de m�me type. Cette m�thode effectue les op�rations de copie de valeurs entre les deux matrices.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
PrecondDiag::PrecondDiag(const PrecondDiag &X)
//-----------------------------------------------------------------------------
{
  allocate(X.li, X.co);

#ifdef MEM_funct
  memcpy(v, X.v, li * sizeof(Real));
#else
  for (Indice i = 0; i < li; i++)
    v[i] = X.v[i];
#endif
}

//!creation d'un pr�conditionneur PrecondDiag � partir d'une matrice quelconque
/*!
  Cette m�thode construit un pr�conditionneur diagonal � partir d'une matrice donn�e A:  \f[ M_{ij} = \delta_{ij}A_{ij}\f] il est connu sous le nom de pr�conditionneur de type Jacobi.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
PrecondDiag::PrecondDiag(const Matrix &X)
//-----------------------------------------------------------------------------
{
  // allocate memory for the preconditioner
  allocate(X.rows(), X.columns());

  for (Indice i = 0; i < li; i++)
    v[i] = X(i, i);
}

//!destructeur de la classe PrecondDiag
/*!
  Cette m�thode lib�re l'espace m�moire occup� par un PrecondDiag.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
PrecondDiag::~PrecondDiag()
//-----------------------------------------------------------------------------
{
  desallocate();
}

//-----------------------------------------------------------------------------
void PrecondDiag::allocate(const Indice li_, const Indice co_)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (li_ != co_)
  {
    internalFatalErrorLine("PrecondDiag::allocate()",
                           "tried to declare a non SQUARE Preconditioner with size [%d,%d]",
                           li_, co_);
  }
#endif

  li = li_;
  co = co_;

  // allocation dynamique de memoire
  v = new Real[li];

#ifdef VERIF_alloc
  if (!v)
  {
    internalFatalErrorLine("PrecondDiag::allocate()",
                           "new Real[%d] Error\n"
                           "Seems to have an overflow memory error\n"
                           "Check your memory size, and memory consumption first",
                           li);
  }
#endif
}

//-----------------------------------------------------------------------------
void PrecondDiag::desallocate()
//-----------------------------------------------------------------------------
{
  // destruction du tableau de pointeurs de vecteurs
  // en meme temps, les vecteurs sont detruits
  li = 0;
  co = 0;

  delete[] v;

#ifdef VERIF_assert
// else assert(v==NULL);
#endif
}

//-----------------------------------------------------------------------------
void PrecondDiag::redim(const Indice li_, const Indice co_)
//-----------------------------------------------------------------------------
{
  if ((li_ == li) && (co_ == co))
  {
    fill(0.);
    return;
  }
  desallocate();
  allocate(li_, co_);

  // initialisation
  fill(0.);
}

//-----------------------------------------------------------------------------
void PrecondDiag::fill(Real val)
//-----------------------------------------------------------------------------
{
  Indice i;
  for (i = 0; i < li; i++)
    v[i] = val;
}

//!r�solution en utilisant le pr�conditionneur
/*!
  Cette m�thode r�soud le probl�me pos� et retourne la veleur solution du syst�me lin�aire \f$ y_{i} = M_{ij}.b_j \f$.
  \param vec le vecteur b du second membre du stsy�me lin�aire
  \return le vecteur r�sultat de l'op�ration
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
Vector
PrecondDiag::solve(const Vector &vec) const
//-----------------------------------------------------------------------------
{
  Vector y(vec.size());

  for (int i = 0; i < vec.size(); i++)
    y(i) = vec(i) * v[i];

  return y;
}

//!r�solution en utilisant la transpos�e du pr�conditionneur
/*!
  Cette m�thode r�soud le probl�me pos� et retourne la veleur solution du syst�me lin�aire \f$ y_{i} = M_{ij}.b_j \f$. Cette m�thode est totalement identique � solve() dans le cas du pr�conditionneur diagonal.
  \param vec le vecteur b du second membre du stsy�me lin�aire
  \return le vecteur r�sultat de l'op�ration
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
Vector
PrecondDiag::trans_solve(const Vector &vec) const
//-----------------------------------------------------------------------------
{
  Vector y(vec.size());

  for (int i = 0; i < vec.size(); i++)
    y(i) = vec(i) * v[i];

  return y;
}

//-----------------------------------------------------------------------------
long PrecondDiag::Memory() const
//-----------------------------------------------------------------------------
{
  return (0);
}
