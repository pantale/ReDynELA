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
  Class MatrixSparse implementation
*/

/*!
  \file MatrixSparse.C
  \brief fichier .C de d�finition des matrices peu denses
  \ingroup linearAlgebra

  Ce fichier sert � la d�finition de la classe de matrices peu denses. Les matrices peu denses sont des matrices qui contiennent peu de valeurs num�riques non nulles.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#include <MatrixSparse.h>

//!constructeur par d�faut de la classe MatrixSparse
/*!
  Cette m�thode construit une nouvelle matrice avec une taille (nombre de lignes et de colonnes) donn�e. Cette m�thode alloue la m�moire n�cessaire pour la matrice en utilisant une taille par d�faut. Cette taille m�moire est calcul�e pour stocker un nombre de valeurs d�fini par la relation  \f$ nb_{min}=Max(rows,cols) \f$. Cette valeur peut �tre chang�e en utilisant la m�thode setNumberOfVals(). 

  Bien entendu, si au cours du remplissage de la matrice, la taille des vecteurs d'allocation est insufisante, la classe MatrixSparse dispose des m�thodes ad�quates (et automatiques) pour augmenter l'espace d'allocation m�moire, si bien que l'utilisateur n'a pas du tout besoin de se pr�ocuper de la taille m�moire et des allocations dynamiques de m�moire.
  \param lignes nombre de lignes de la matrice
  \param colonnes nombre de lignes de la matrice
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
MatrixSparse::MatrixSparse(const Indice lignes, const Indice colonnes)
//-----------------------------------------------------------------------------
{
  // allocation memoire;
  stored = True;
  lastPos = 0;
  lastVal = 0;
  nbVals = 0;
  allocate(lignes, colonnes);
}

//!constructeur par recopie de la classe MatrixSparse
/*!
  Cette m�thode est utilis�e pour faire une copie d'une instance de la classe MatrixSparse. Cette m�thode est utilis�e dans les op�rations du type \f$ M=N \f$ pour lesquelles \f$M\f$ et \f$N\f$ sont deux instances de la classe MatrixSparse.
  \param mat matrice � recopier
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
MatrixSparse::MatrixSparse(const MatrixSparse &mat)
//-----------------------------------------------------------------------------
{
}

//!destructeur de la classe MatrixSparse
/*!
  Cette m�thode lib�re l'espace m�moire allou� pour une matrice peu dense.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
MatrixSparse::~MatrixSparse()
//-----------------------------------------------------------------------------
{
  row.flush();
  col.flush();
  val.flush();
}

//-----------------------------------------------------------------------------
void MatrixSparse::allocate(const Indice lignes, const Indice colonnes)
//-----------------------------------------------------------------------------
{
  li = lignes;
  co = colonnes;
  Indice ssz = Max(li, co);

  // set a default size of max(li,co) for the 3 arrays
  val.redim(ssz);
  row.redim(ssz);
  col.redim(ssz);
  val.stackIncrement() = ssz;
  row.stackIncrement() = ssz;
  col.stackIncrement() = ssz;
}

//!affichage du contenu d'une matrice sur stdout
/*!
  Cette m�thode affiche le contenu d'une matrice sur le flux stdout.
  \param os output stream d'ecriture de la matrice
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
void MatrixSparse::print(ostream &os)
//-----------------------------------------------------------------------------
{
  Indice i, j;
  Real v;
  Indice tab;

  storeVal();

  os << "matrix sparse (" << nbVals << ") " << li << "x" << co << " ={{";
  for (i = 0; i < li; i++)
  {
    for (j = 0; j < co; j++)
    {
      if (j != 0)
        os << ",";
      v = 0.;
      if (existVal(i, j, tab))
        v = val(tab);
      os << v;
    }
    if (i != li - 1)
      os << "},{";
  }
  os << "}}\n";
}

//!affichage du contenu d'une matrice sur ostream
/*!
  Cette m�thode affiche le contenu d'une matrice sur le flux ostream.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, MatrixSparse &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//!acc�s en lecture et �criture des �lements de la matrice
/*!
  Cette m�thode est sens�e donn�e acc�s en lecture et �criture aux �l�ments de la matrice. Cette m�thode n'est jamais appel�e, ou plutot, j'ai jamais r�ussi � l'appeler. Peut �tre quelqu'un y arrivera-t-il ?
  \warning Cette m�thode g�n�re un message d'erreur automatique. C'est normal car elle ne sert � rien.
  \param li_ num�ro de ligne
  \param co_ num�ro de colonne
  \return la valeur correspondante A(li_,co_)
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
Real MatrixSparse::operator()(Indice li_, Indice co_)
    const
//-----------------------------------------------------------------------------
{
  fatalError("Real MatrixSparse::operator()(Indice li_,Indice co_) const",
             "Oups, on y passe ?\n");
  Indice tab;
  return (existVal(li_, co_, tab) == True ? val(tab) : 0);
}

//!acc�s en lecture et �criture des �lements de la matrice
/*!
  Cette m�thode est sens�e donn�e acc�s en lecture et �criture aux �l�ments de la matrice. Il y a deux types d'utilisation de cette m�thode, la premi�re en lecture, la seconde en �criture.

  La classe MatrixSparse dispose d'un algorithme complexe d'adressage qui permet la lecture et l'�criture directe en utilisant le m�me op�rateur (). En mode lecture, le r�sultat de cet op�rateur est la valeur contenue dans la matrice. Dans le cas de l'�criture, on renvoie un pointeur sur l'�l�ment interne de la matrice. Si cet �l�ment n'existe pas d�ja dans la matrice, un nouvel �lement est cr�� et ins�r�. 
  \warning En raison de l'algorithme interne utilis�, certaines op�rations ne sont pas autoris�es ou plutot, votre compilateur les acceptera mais le r�sultat sera catastrophique. Voir ci-dessous sur un exemple:
  example:
  \code
  MatrixSparse M(5,5);
  M(1,2)=2;            // correct
  M(2,1)=M(1,2)=2;     // correct
  M(3,1)=M(1,3)=3;     // incorrect car cr�ation simultan�e de deux �lements de la matrice
                       // cette m�thode est alors �quivalente au r�sultat suivant
		       // M(3,1)=0; M(1,3)=3;
		       // qui n'a rien a voir avec ce que l'on attendait
  \endcode
  Ce r�sultat est obtenu car dans la quatri�me instruction, M(3,1) est alors �gal � M(1,3) qui est 0 car jamais utilis� pour le moment. \b ENSUITE M(1,3)=3 est effectu� d'o� le r�sultat final:
  M(3,1)=0; et M(1,3)=3;

  \param li_ num�ro de ligne
  \param co_ num�ro de colonne
  \return la valeur correspondante A(li_,co_)
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
Real &
MatrixSparse::operator()(Indice li_, Indice co_)
//-----------------------------------------------------------------------------
{
  Indice tab;
  storeVal();
  if (!existVal(li_, co_, tab))
  {

    // insert at the correct place
    lastPos = tab;
    lastVal = 0;
    lastRow = li_;
    lastCol = co_;
    stored = False;
    return lastVal;
  }
  return val(tab);
}

//-----------------------------------------------------------------------------
Boolean
MatrixSparse::existVal(Indice li_, Indice co_, Indice &tab) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(li_, co_);
#endif

  Indice i, g, d;
  // first value
  if (nbVals == 0)
  {
    tab = 0;
    return False;
  }

  Boolean found = False;

  // row search
  g = 0;
  d = row.size() - 1;
  do
  {
    i = (g + d) / 2;
    if (row(i) == li_)
      found = True;
    if (li_ < row(i))
      d = i - 1; // a droite de i
    else
      g = i + 1; // a gauche de i
  } while ((!found) && (g <= d));

  // row not found
  if (!found)
  {
    tab = g;
    return False;
  }

  while ((row(i) == li_) && (i > 0))
  {
    i--;
  }

  for (tab = i; tab < nbVals; tab++)
  {
    if (row(tab) == li_)
    {
      if (col(tab) == co_)
      {
        return True;
      }
      if (col(tab) > co_)
      {
        return False;
      }
    }
    if (row(tab) > li_)
    {
      return False;
    }
  }
  return False;
}

//!affichage de la structure interne de stockage
/*!
  Cette m�thode n'est pas � utiliser, elle n'est propos�e que pour des fin de d�bug.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
void MatrixSparse::internalPrint()
//-----------------------------------------------------------------------------
{
  storeVal();
  for (Indice i = 0; i < nbVals; i++)
    printf("%ld,%ld=%lf\n", row(i), col(i), val(i));
}

//!r�duction de l'espace m�moire n�cessaire
/*!
  Cette m�thode r�duit l'espace m�moire n�cessaire pour le stockage de la matrice.
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
void MatrixSparse::optimizeMemory()
//-----------------------------------------------------------------------------
{
  row.close();
  val.close();
  col.close();
}

//!ajustement de la taille de la matrice
/*!
  Cette m�thode est utilis�e pour ajuster la taille de stockage d'une matrice. Elle est utile si on sait � priori quelle taille va occuper une matrice, c'est � dire le nombre de valeurs non nulles qu'elle contient.
  \param nb nombre de valeurs non nulles de la matrice
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
void MatrixSparse::setNumberOfVals(Indice nb)
//-----------------------------------------------------------------------------
{
  val.redim(nb);
  row.redim(nb);
  col.redim(nb);
}

//!nombre de valeurs non nulles de la matrice
/*!
  Cette m�thode retourne le nombre de valeurs non nulles de la matrice.
  \return nombre de valeurs non nulles de la matrice
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
Indice
MatrixSparse::numberOfNonZero()
//-----------------------------------------------------------------------------
{
  return nbVals;
}

//!multiplication d'une MatrixSparse par un Vector
/*!
  Cette m�thode calcule le produit d'une matrice peu dense par un vecteur. Le produit de matrice est fait entre une matrice A de taille N-x-M et un vecteur x de taille M. Ceci conduit � un vecteur y de taille N. Cette m�thode utilise une fonction de la librairie Blas2 et calcule le r�sultat de: \f[ y=A.x \f]

  Exemple :
  \code
  MatrixSparse t1;
  Vector v1,v2;
  v2=t1*v1; // produit
  \endcode
  \param vec vecteur par lequel on multiplie la matrice
  \return vecteur r�sultant de l'op�ration
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
Vector
    MatrixSparse::operator*(const Vector &vec) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (co != vec.size())
    fatalError("MatrixSparse::operator *",
               "sparse matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] sparse matrix and a [%d] vector",
               li, co, vec.size());
#endif

  Vector resu(li);

  Indice j;

  for (j = 0; j != nbVals; j++)
    resu.v[row(j)] += vec.v[col(j)] * val(j);

  return resu;
}

//!multiplication de la transpos�e d'une MatrixSparse par un Vector
/*!
  Cette m�thode calcule le produit de la transpos�e d'une matrice peu dense par un vecteur. Le produit de matrice est fait entre une matrice A de taille N-x-M et un vecteur x de taille N. Ceci conduit � un vecteur y de taille M. Cette m�thode utilise une fonction de la librairie Blas2 et calcule le r�sultat de: \f[ y=A^{T}.x \f]

  Exemple :
  \code
  MatrixSparse t1;
  Vector v1,v2;
  v2=t1.trans_mult(v1); // produit
  \endcode
  \param vec vecteur par lequel on multiplie la matrice
  \return vecteur r�sultant de l'op�ration
  \author &copy; Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
Vector
MatrixSparse::trans_mult(const Vector &vec) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (li != vec.size())
    fatalError("MatrixSparse::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               li, co, vec.size());
#endif

  Vector resu(co);
  Indice j;

  for (j = 0; j != nbVals; j++)
    resu.v[col(j)] += vec.v[row(j)] * val(j);

  return resu;
}
