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
  Class MatrixSparse definition
*/

/*!
  \file MatrixSparse.h
  \brief fichier .h de d�finition des matrices peu denses
  \ingroup linearAlgebra

  Ce fichier sert � la d�finition de la classe de matrices peu denses. Les matrices peu denses sont des matrices qui contiennent peu de valeurs num�riques non nulles.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#ifndef __MatrixSparse_h__
#define __MatrixSparse_h__
#include <Matrices.h>
#include <Vector.h>

/*!
  \brief Classe de matrice peu dense 
  \ingroup linearAlgebra

  Cette classe sert � la d�finition des matrices peu denses. Les matrices peu denses sont des matrices qui contiennent peu de valeurs num�riques non nulles. Afin d'illustrer le m�canisme de stockage, on pr�sente ci-dessous un exemple de matrice non-sym�trique: \f[ A=\left( \begin{array}{ccccc}
1 & 2 & 0 & 0 & 3\\
4 & 5 & 6 & 0 & 0\\
0 & 7 & 8 & 0 & 9\\
0 & 0 & 0 & 10 & 0\\
11 & 0 & 0 & 0 & 12
\end{array}\right)\f]

La mani�re la plus simple de stocker une matrice peu dense est de stocker toutes ses valeurs non nulles avec en r�f�rence le num�ro de ligne et de colonne � laquelle elles se rattachent. La matrice \f$ A \f$ peut alors etre stock�e � l'aide des tableaux suivants:
\f[\begin{array}{l|c|c|c|c|c|c|c|c|c|c|c|c|}
val() & 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12\\
row() & 0 & 0 & 0 & 1 & 1 & 1 & 2 & 2 & 2 & 3 & 4 & 4\\
col() & 0 & 1 & 4 & 0 & 1 & 2 & 1 & 2 & 4 & 3 & 0 & 4
\end{array}\f]

Dans cette classe, les valeurs sont automatiquement tri�es en fonction de la ligne et de la colonne dans un ordre d'indices croissants. Cette mise en ordre par ligne et par colonnes est sans doute la forme la plus naturelle, de plus elle permet un indexage le plus efficace possible. Ainsi, lorsqu'une nouvelle valeur num�rique se pr�sente, la valeur correspondante est ins�r�e � la bonne position dans les vecteurs de stockage et toute l'architecture de la matrice est mise � jour. Cette fa�on d'op�rer relentit l'ex�cution du programme au moment de la construction (du remplissage) de la matrice, mais optimise �norm�ment les op�ration de r�cup�ration des informations et les op�rations math�matiques.
  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
/** @dia:pos 4,20 */
/** @dia:route Matrices;v,29.675,13.8,16,14.925,20 */
class MatrixSparse : public Matrices
{
private:
  List<Indice> col;
  List<Indice> row;
  List<Real> val;
  Indice nbVals;
  Boolean stored;
  Indice lastPos;
  Real lastVal;
  Indice lastRow;
  Indice lastCol;

  void allocate(const Indice li_, const Indice co_);
  Boolean existVal(Indice li_, Indice co_, Indice &tab) const;
  inline void storeVal(void);
  inline Boolean indexOK(Indice i, Indice j) const;

public:
  // constructor
  MatrixSparse(const Indice li_ = 3, const Indice co_ = 3);
  MatrixSparse(const MatrixSparse &X);
  ~MatrixSparse();

  // member functions
  void print(ostream &os);
  friend ostream &operator<<(ostream &os, MatrixSparse &mat);
  Real &operator()(Indice i, Indice j);
  Real operator()(Indice i, Indice j) const;
  void internalPrint();
  void setNumberOfVals(Indice nb);
  Indice numberOfNonZero();
  void optimizeMemory();
  long Memory() const
  {
    return 0;
  }

  // operations
  Vector operator*(const Vector &vec) const;
  Vector trans_mult(const Vector &vec) const;
};

//!teste les bornes de la matrice
/*!
  Cette m�thode teste les bornes d'une matrice
  \param i indice de ligne
  \param j indice de colonne
  \return True si les indices fournis sont dans les bornes, False dans le cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
inline Boolean
MatrixSparse::indexOK(Indice i, Indice j) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < li) && (j >= 0) && (j < co))
    return (True);

  fatalError("Matrix::indexOK",
             "indice(s) [%d,%d] out of allowed range [0-%d,0-%d]",
             i, j, li - 1, co - 1);

  // fonction inutile, mais necessaire pour le compilateur
  return (False);
}

//-----------------------------------------------------------------------------
inline void
MatrixSparse::storeVal(void)
//-----------------------------------------------------------------------------
{
  // test if something to do ?
  if (stored)
    return;

  if (lastVal != 0)
  {
    // insert at the correct place
    row.insert(lastRow, lastPos);
    col.insert(lastCol, lastPos);
    val.insert(lastVal, lastPos);
    nbVals++;
    stored = True;
  }
}

#endif
