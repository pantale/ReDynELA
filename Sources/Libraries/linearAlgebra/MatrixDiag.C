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

// begin date : 25/03/1997

/*
  Class MatrixDiag implementation
*/

/*!
  \file MatrixDiag.C
  \brief fichier .C de d�finition des matrices diagonales
  \ingroup linearAlgebra

  Ce fichier sert � la d�finition de la classe de matrices diagonales.

  \author &copy; Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/

#include <MatrixDiag.h>

//!constructeur de la classe MatrixDiag
/*!
  Cette m�thode construit une matrice de lignes/colonnes elements. Par d�faut, le contenu de la matrice est mis � z�ro
  \param li_ nombre de lignes
  \param co_ nombre de colonnes 
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag::MatrixDiag(const Indice li_, const Indice co_)
//-----------------------------------------------------------------------------
{
  // allocation memoire;
  allocate(li_, co_);

  // initialisation
  fill(0.);
}

//!constructeur par recopie d'une matrice
/*!
  Ce constructeur recopie les valeurs contenues dans une matrice. Selon la valeur du flag \ref MEM_funct, la copie est faite soit terme � terme (flag non d�fini) ou par appel � memcopy() (flag d�fini).
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag::MatrixDiag(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  allocate(mat.li, mat.co);

#ifdef MEM_funct
  memcpy(v, mat.v, li * sizeof(Real));
#else
  for (Indice i = 0; i < li; i++)
    v[i] = mat.v[i];
#endif
}

//!destructeur de la classe MatrixDiag
/*!
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag::~MatrixDiag()
//-----------------------------------------------------------------------------
{
  desallocate();
}

//-----------------------------------------------------------------------------
void MatrixDiag::allocate(const Indice li_, const Indice co_)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (li_ != co_)
  {
    fatalError("MatrixDiag::allocate()",
               "tried to declare a non SQUARE matDiag with size [%d,%d]",
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
    internalFatalErrorLine("MatrixDiag::allocate()",
                           "new Real[%d] Error\n"
                           "Seems to have an overflow memory error\n"
                           "Check your memory size, and memory consumption first",
                           li);
  }
#endif
}

//-----------------------------------------------------------------------------
void MatrixDiag::desallocate()
//-----------------------------------------------------------------------------
{
  // destruction du tableau de pointeurs de vecteurs
  // en meme temps, les vecteurs sont detruits
  li = 0;
  co = 0;

  //  if (sz>0) delete[] v;
  delete[] v;

#ifdef VERIF_assert
// else assert(v==NULL);
#endif
}

//!redimensionne la matrice
/*!
  Cette m�thode est utilis�e pour sp�cifier une nouvelle dimension de matrice de celle donn�e lors de l'initialisation par le constructeur
  \param li_ nombre de lignes
  \param co_ nombre de colonnes 
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void MatrixDiag::redim(const Indice li_, const Indice co_)
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

//!affichage du contenu d'une matrice
/*!
  Cette m�thode est une surd�finition de << pour les flux de sortie, son utilisation est donn�e comme suit
  
  Exemple
  \code
  MatrixDiag t;
  cout << t << endl;
  \endcode
  \param os flux de sortie
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//!affichage du contenu d'une matrice
/*!
  Cette m�thode permet d'afficher le contenu d'une matrice sur la sortie ostream
  \param os flux de sortie
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void MatrixDiag::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  register Indice i, j;

  os << "diagonal matrix " << li << "x" << co << " ={{";
  for (i = 0; i < li; i++)
  {
    for (j = 0; j < co; j++)
    {
      if (j != 0)
        os << ",";
      os << (i == j ? v[i] : 0.);
    }
    if (i != li - 1)
      os << "},{";
  }
  os << "}}";
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement le remplissage des valeurs d'une matrice par un scalaire
  
  Exemple :
  \code
  MatrixDiag t1;
  t1=fill(1.); // affecte 1 � toutes les composantes de la matrice
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void MatrixDiag::fill(Real val)
//-----------------------------------------------------------------------------
{
  Indice i;
  for (i = 0; i < li; i++)
    v[i] = val;
}

//!renvoie une matrice identit� � partir d'une matrice quelconque
/*!
  Cette m�thode prend en argument une matrice quelconque et renvoie une matrice identit�. Par d�finition une matrice diagonale identit� est enti�rement remplie de 1.
  
  Exemple :
  \code
  MatrixDiag t1;
  t1.Identity(); // renvoie identit�
  \endcode
  \warning Cette m�thode modifie son argument
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void MatrixDiag::Identity()
//-----------------------------------------------------------------------------
{
  Indice i;
  for (i = 0; i < li; i++)
    v[i] = 1.;
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement le remplissage des valeurs d'une matrice par un scalaire
  
  Exemple :
  \code
  MatrixDiag t1;
  t1=1.; // affecte 1 � toutes les composantes de la matrice
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag &MatrixDiag::operator=(const Real &val)
//-----------------------------------------------------------------------------
{
  fill(val);
  return *this;
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement l'affectation sous la forme donn�e en exemple
  
  Exemple :
  \code
  MatrixDiag t1,t2;
  t1=t2; // egalit� de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag &MatrixDiag::operator=(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  // redimension de la matrice resultat
  redim(mat.li, mat.co);

  // recopie des valeurs
#ifdef MEM_funct
  memcpy(v, mat.v, li * sizeof(Real));
#else
  for (Indice i = 0; i < li; i++)
    v[i] = mat.v[i];
#endif

  return *this;
}

//!addition de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration d'addition des matrices et d'�crire simplement la somme de deux matrices sous la forme donn�e en exemple
  
  Exemple :
  \code
  MatrixDiag t1,t2,t3;
  t3=t1+t2; // somme de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag
MatrixDiag::operator+(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  // test si egalite de dimensions
#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
  {
    fatalError("MatrixDiag::operator +",
               "matrix sizes incompatible\n"
               "You're about to add a [%d,%d] and a [%d,%d] matrix",
               li,
               co, mat.li, mat.co);
  }
#endif

  // creation d'une nouvelle matrice
  MatrixDiag resu(mat.li, mat.co);

  // calcul de la somme
  for (Indice i = 0; i < li; i++)
    resu.v[i] = v[i] + mat.v[i];

  // renvoi de l'objet
  return resu;
}

//!soustraction de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration de soustraction des matrices et d'�crire simplement la soustraction de deux matrices sous la forme donn�e en exemple
  
  Exemple :
  \code
  MatrixDiag t1,t2,t3;
  t3=t1-t2; // soustraction de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag
MatrixDiag::operator-(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
  {
    fatalError("MatrixDiag::operator -",
               "matrix sizes incompatible\n"
               "You're about to substract [%d,%d] and [%d,%d] matrix",
               li,
               co, mat.li, mat.co);
  }
#endif

  // creation d'une nouvelle matrice
  MatrixDiag resu(li, co);

  // calcul de la difference
  for (Indice i = 0; i < li; i++)
    resu.v[i] = v[i] - mat.v[i];

  // renvoi de l'objet
  return resu;
}

//!addition de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration d'addition des matrices et d'�crire simplement la somme de deux matrices sous la forme donn�e en exemple
  
  Exemple :
  \code
  MatrixDiag t1,t2;
  t2+=t1; // somme de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void MatrixDiag::operator+=(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  // test si egalite de dimensions
#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
  {
    fatalError("MatrixDiag::operator +=",
               "matrix sizes incompatible\n"
               "You're about to add a [%d,%d] and a [%d,%d] matrix",
               li,
               co, mat.li, mat.co);
  }
#endif

  // calcul de la somme
  for (Indice i = 0; i < li; i++)
    v[i] += mat.v[i];
}

//!soustraction de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration de soustraction des matrices et d'�crire simplement la soustraction de deux matrices sous la forme donn�e en exemple
  
  Exemple :
  \code
  MatrixDiag t1,t2;
  t2-=t1; // soustraction de deux matrices
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void MatrixDiag::operator-=(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
  {
    fatalError("MatrixDiag::operator -=",
               "matrix sizes incompatible\n"
               "You're about to substract [%d,%d] and [%d,%d] matrix",
               li,
               co, mat.li, mat.co);
  }

#endif

  // calcul de la difference
  for (Indice i = 0; i < li; i++)
    v[i] -= mat.v[i];
}

//!multiplication d'une matrice par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des matrices et d'�crire simplement la multiplication d'une matrice par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  MatrixDiag t1,t2;
  Real l;
  t2=t1*l; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag
    MatrixDiag::operator*(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  MatrixDiag resu(li, co);

  for (Indice i = 0; i < li; i++)
    resu.v[i] = lambda * v[i];
  return resu;
}

//!division d'une matrice par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de division des matrices et d'�crire simplement la division d'une matrice par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  MatrixDiag t1;
  Real l;
  t1/=l; // division par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag
MatrixDiag::operator/(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  MatrixDiag resu(li, co);

  for (Indice i = 0; i < li; i++)
    resu.v[i] = v[i] / lambda;
  return resu;
}

//!multiplication d'une matrice par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des matrices et d'�crire simplement la multiplication d'une matrice par un scalaire sous la forme donn�e en exemple. Elle est identique � la forme pr�c�dente (et commutative).
  
  Exemple :
  \code
  MatrixDiag t1,t2;
  Real l;
  t2=l*t1; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag
operator*(const Real &lambda, const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  MatrixDiag resu(mat.li, mat.co);

  for (Indice i = 0; i < mat.li; i++)
    resu.v[i] = lambda * mat.v[i];
  return resu;
}

//!multiplication de deux matrices
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des matrices et d'�crire simplement la multiplication de deux matrice sous la forme donn�e en exemple. Cette op�ration correspond � la notion de produit contract� de deux matrices.
  
  Exemple :
  \code
  MatrixDiag t1,t2,t3;
  t3=t1*t2; // produit contract�
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag
    MatrixDiag::operator*(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (co != mat.li)
  {
    fatalError("MatrixDiag::operator *",
               "matrix sizes incompatible\n"
               "You're about to multiply a [%d,%d] and a [%d,%d] matrix",
               li, co, mat.li, mat.co);
  }
#endif

  MatrixDiag resu(li, mat.co);

  for (Indice i = 0; i < li; i++)
    resu.v[i] = v[i] * mat.v[i];

  return resu;
}

//!multiplication d'une matrice par un vecteur
/*!
  Cette m�thode calcule le produit d'une matrice A de taille N-x-N par un vecteur x de taille N. ceci g�n�re un vecteur y de taille N.
  Cette m�thode retourne un vecteur
  Exemple :
  \code
  MatrixDiag t1;
  Vector v1,v2;
  v2=t1*v1; // produit
  \endcode
  \param vec vecteur du second membre
  \return vecteur r�sultant de l'op�ration de multiplication
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
    MatrixDiag::operator*(const Vector &vec) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (co != vec.size())
  {
    fatalError("MatrixDiag::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               li, co, vec.size());
  }
#endif

  Vector resu(li);
  Real *Res = resu.v;
  Real *Mat = v;
  Real *Vec = vec.v;

  for (Indice i = 0; i < li; i++)
    *Res++ = *Mat++ * *Vec++;
  //     resu.v[i] = v[i] * vec.v[i];

  return resu;
}

//-----------------------------------------------------------------------------
void MatrixDiag::productBy(Vector &vec) const
//-----------------------------------------------------------------------------
{

#ifdef VERIF_math
  if (co != vec.size())
  {
    fatalError("MatrixDiag::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               li, co, vec.size());
  }
#endif

  Real *Vec = vec.v;
  Real *Mat = v;
  for (Indice i = li - 1; i >= 0; i--)
    *Vec++ *= *Mat++;
}

//-----------------------------------------------------------------------------
void MatrixDiag::divideBy(Vector &vec) const
//-----------------------------------------------------------------------------
{

#ifdef VERIF_math
  if (co != vec.size())
  {
    fatalError("MatrixDiag::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               li, co, vec.size());
  }
#endif

  Real *Vec = vec.v;
  Real *Mat = v;
  for (Indice i = li - 1; i >= 0; i--)
    *Vec++ /= *Mat++;
}

//!calcule la trace d'une matrice
/*!
  Cette m�thode calcule la trace d'une matrice carr�e
  \return valeur de la trace de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Real MatrixDiag::Trace()
//-----------------------------------------------------------------------------
{
  Real trace = 0.;
  for (Indice i = 0; i < li; i++)
    trace += v[i];
  return trace;
}

//!transpos�e d'une matrice
/*!
  Cette m�thode renvoie la transpos�e d'une matrice
  \return transpos�e de la matrice
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
MatrixDiag
MatrixDiag::Transpose()
//-----------------------------------------------------------------------------
{
  return *this;
}

//!r�solution d'un syst�me lin�aire
/*!
  Cette m�thode calcule la solution du syst�me lin�aire  \f[ A x = b \f] avec A matrice N-x-N et x et y vecteurs de taille N.

  \param x vecteur du second membre
  \return vecteur solution du syst�me lin�aire
  \author Olivier PANTALE
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
Vector
MatrixDiag::Solve(Vector &vec)
//-----------------------------------------------------------------------------
{
  Vector resu(li);

#ifdef VERIF_math
  if (li != vec.size())
  {
    fatalError("MatrixDiag::Solve",
               "matrix and vector sizes incompatible\n"
               "You're about to Solve a linear system with a [%d,%d]"
               " matrix and a [%d] vector",
               li, co, vec.size());
  }
#endif

  for (Indice i = 0; i < li; i++)
    resu(i) = vec(i) / v[i];

  return resu;
}

//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::getInverse() const
//-----------------------------------------------------------------------------
{
  MatrixDiag res(li, li);

  Real *Res = res.v;
  Real *Loc = v;

  for (Indice i = li; i > 0; i--)
    *Res++ = 1.0 / (*Loc++);

  return res;
}

//!egalit� de deux matrices
/*!
  Cette m�thode teste l'�galit� de deux matrices
  \return True si les deux matrices sont identiques, False dans la cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Boolean
MatrixDiag::operator==(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if ((li != mat.li) && (co != mat.co))
  {
    fatalError("MatrixDiag::operator ==",
               "The dimensions of the two matrix are'nt corresponding\n"
               "they are respectively a [%d,%d] and a [%d,%d] matrix",
               li, co, mat.li, mat.co);
  }
#endif

  for (Indice i = 0; i < li; i++)
    if (v[i] != mat.v[i])
      return False;
  return True;
}

//!in�galit� de deux matrices
/*!
  Cette m�thode teste l'in�galit� de deux matrices
  \return True si les deux matrices sont differentes, False dans la cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Boolean
MatrixDiag::operator!=(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  return !(*this == mat);
}

//!sortie sur flux ofstream
/*!
  Cette m�thode permet d'�crire une matrice dans un fichier (notament) binaire
  
  Exemple :
  \code
  ofstream pfile("fichier");
  Matrix t;
  t.write(pfile);
  t.close();
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void MatrixDiag::write(ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)&li, sizeof(Indice));
  ofs.write((char *)&co, sizeof(Indice));
  ofs.write((char *)v, li * sizeof(Real));
}

//!lecture sur flux ifstream
/*!
  Cette m�thode permet de lire une matrice depuis un fichier (notament) binaire
  
  Exemple :
  \code
  ifstream pfile("fichier");
  Matrix t;
  t.read(pfile);
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void MatrixDiag::read(ifstream &ifs)
//-----------------------------------------------------------------------------
{
  Indice li_, co_;

  ifs.read((char *)&li_, sizeof(Indice));
  ifs.read((char *)&co_, sizeof(Indice));
  redim(li_, co_);
  ifs.read((char *)v, li * sizeof(Real));
}

//!sortie sur flux ofstream
/*!
  Cette m�thode permet d'�crire une matrice dans un fichier (notament) binaire
  
  Exemple :
  \code
  ofstream pfile("fichier");
  Matrix t;
  pfile << t;
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  mat.write(os);
  return os;
}

//!lecture sur flux ifstream
/*!
  Cette m�thode permet de lire une matrice depuis un fichier (notament) binaire
  
  Exemple :
  \code
  ifstream pfile("fichier");
  Matrix t;
  pfile >> t;
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  mat.read(is);
  return is;
}

//-----------------------------------------------------------------------------
long MatrixDiag::Memory() const
//-----------------------------------------------------------------------------
{
  return (0);
}
