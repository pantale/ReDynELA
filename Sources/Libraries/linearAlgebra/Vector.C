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

// begin date : 05/03/1997
// revision date : 02/09/1997

/*
  Class Vector definition
*/

#include <Vector.h>

/*!
  \file Vector.C
  \brief fichier .C de d�finition des vecteurs
  \ingroup linearAlgebra

  Ce fichier sert � la d�finition de la classe de vecteurs

  \author &copy; Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/

//!constructeur de la classe vecteur
/*!
  Cette m�thode construit un vecteur. Par d�faut, le contenu du vecteur est mis � z�ro et la taille est �galement � z�ro.
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector::Vector()
//-----------------------------------------------------------------------------
{
  nb = 0;
  v = NULL;
}

//!constructeur de la classe vecteur
/*!
  Cette m�thode construit un vecteur d'une taille donn�e. Par d�faut, le contenu du vecteur est mis � z�ro.
  \param num taille du vecteur
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector::Vector(Indice num)
//-----------------------------------------------------------------------------
{
  // allocation dynamique de memoire
  nb = num;
  v = new Real[num];

#ifdef VERIF_alloc
  if (!v)
  {
    fatalErrorLine("Vector::Vector(Indice num)",
                   "memory allocation error for function new");
  }
#endif

  // remplissage de 0
  for (Indice i = 0; i < nb; i++)
    v[i] = 0.;
}

//!constructeur par recopie d'un vecteur
/*!
  Ce constructeur recopie les valeurs contenues dans un vecteur. Selon la valeur du flag \ref MEM_funct, la copie est faite soit terme � terme (flag non d�fini) ou par appel � memcopy() (flag d�fini).
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector::Vector(const Vector &vect)
//-----------------------------------------------------------------------------
{
  // allocation memoire;
  nb = vect.nb;
  v = new Real[nb];

#ifdef VERIF_alloc
  if (!v)
  {
    fatalErrorLine("Vector::Vector(const Vector & vect)",
                   "memory allocation error for function new");
  }
#endif

#ifdef MEM_funct
  memcpy(v, vect.v, nb * sizeof(Real));
#else
  for (Indice i = 0; i < nb; i++)
    v[i] = vect.v[i];
#endif
}

//-----------------------------------------------------------------------------
Vector::Vector(Indice siz, Real val0, ...)
//-----------------------------------------------------------------------------
{
  // allocation dynamique de memoire
  nb = siz;
  v = new Real[siz];

  // A place to store the list of arguments
  va_list arguments;

  // Store arguments values
  va_start(arguments, val0);

  // traitement de la premi�re valeur
  v[0] = val0;

  for (Indice i = 1; i < siz; i++)
  {
    v[i] = va_arg(arguments, Real);
  }
}

//!destructeur de la classe Vector
/*!
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector::~Vector()
//-----------------------------------------------------------------------------
{
  if (nb > 0)
    delete[] v;

#ifdef VERIF_assert
  else
    assert(v == NULL);
#endif

  nb = 0;
}

//-----------------------------------------------------------------------------
void Vector::swapWith(Vector &vec)
//-----------------------------------------------------------------------------
{
  Real *tmp;

  // swap dimensions
#ifdef VERIF_math
  if (vec.nb != nb)
    fatalError("Vector::swapWith", "Only for same size vectors\n");
#endif

  // swap storages
  tmp = v;
  v = vec.v;
  vec.v = tmp;
}

//!redimensionne le vecteur
/*!
  Cette m�thode est utilis�e pour sp�cifier une nouvelle dimension de vecteur de celle donn�e lors de l'initialisation par le constructeur
  \param nb_ taille du vecteur
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Vector::redim(const Indice nb_)
//-----------------------------------------------------------------------------
{
  Real *v2;

#ifdef VERIF_math
  if (nb_ < 1)
  {
    fatalErrorLine("Vector::redim",
                   "function called with value nb=%d <1", nb_);
  }
#endif

  if (v == NULL)
  {

    // nouvelle allocation memoire
    v = new Real[nb_];

    // verification de l'allocation memoire
#ifdef VERIF_alloc
    if (!v)
    {
      fatalErrorLine("void Vector::redim(const Indice nb_)",
                     "memory allocation error for function new");
    }
#endif

    // remplissage par defaut
    for (Indice i = 0; i < nb; i++)
      v[i] = 0.0;
  }
  else
  {

    // nouvelle allocation memoire
    v2 = new Real[nb_];

    // verification de l'allocation memoire
#ifdef VERIF_alloc
    if (!v2)
    {
      fatalErrorLine("void Vector::redim(const Indice nb_)",
                     "memory allocation error for function new");
    }
#endif

    if (v2 != v)
    {
#ifdef MEM_funct
      memcpy(v2, v, nb * sizeof(Real));
#else
      for (Indice i = 0; i < nb; i++)
        v2[i] = v[i];
#endif

      // initialisation
      for (Indice i = nb; i < nb_; i++)
        v2[i] = 0.;
      delete[] v;
      v = v2;
    }
  }

  // allocation de la taille
  this->nb = nb_;
}

//!affichage du contenu d'un vecteur
/*!
  Cette m�thode est une surd�finition de << pour les flux de sortie, son utilisation est donn�e comme suit
  
  Exemple
  \code
  Vector t;
  cout << t << endl;
  \endcode
  \param os flux de sortie
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const Vector &vector)
//-----------------------------------------------------------------------------
{
  vector.print(os);
  return os;
}

//!affichage du contenu d'un vecteur
/*!
  Cette m�thode permet d'afficher le contenu d'un vecteur sur la sortie ostream
  \param os flux de sortie
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Vector::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  Indice i;

  os << "vector n=" << nb << " coords={";
  if (nb != 0)
    os << v[0];
  for (i = 1; i < nb; i++)
    os << "," << v[i];
  os << "}";
}

//!affichage � l'�cran suivant format pr�d�fini
/*!
  Cette m�thode affiche le contenu du vecteur � l'�cran selon un format pr�d�fini. La s�lection du format d'affichage est faite en utilisant la m�thode \ref setOutType(). Le type de sortie est d�finit par les variable enum�r�es dans \ref OutVector.
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void Vector::printOut()
//-----------------------------------------------------------------------------
{
  switch (outType)
  {
  case outVectorTypeNormal:
  {
    Indice i;
    cout << "vector " << nb << " coords={";
    if (nb != 0)
      cout << v[0];
    for (i = 1; i < nb; i++)
      cout << "," << v[i];
    cout << "}";
  }
  break;
  case outVectorTypeMupad:
  {
    Indice i;
    cout << "matrix([[";
    if (nb != 0)
      cout << v[0];
    for (i = 1; i < nb; i++)
      cout << "],[" << v[i];
    cout << "]])\n";
  }
  break;
  default:
    fatalError("Vector::printOut()", "unknown type %d\n", outType);
  }
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement le remplissage des valeurs d'un vecteur par un scalaire
  
  Exemple :
  \code
  Vector t1;
  t1=fill(1.); // affecte 1 � toutes les composantes du vecteur
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Vector::fill(Real val)
//-----------------------------------------------------------------------------
{
  Indice i;

  for (i = 0; i < nb; i++)
    v[i] = val;
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement le remplissage des valeurs d'un vecteur par un scalaire
  
  Exemple :
  \code
  Vector t1;
  t1=1.; // affecte 1 � toutes les composantes du vecteur
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector &Vector::operator=(Real val)
//-----------------------------------------------------------------------------
{
  fill(val);
  return *this;
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement le remplissage des valeurs d'un vecteur par un ensemble de scalaires (tableau)
  
  Exemple :
  \code
  Vector t1(10);
  Real t2[10];
  t1=t2; // recopie des valeurs
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector &Vector::operator=(const Real *vals)
//-----------------------------------------------------------------------------
{
#ifdef MEM_funct
  memcpy(v, vals, nb * sizeof(Real));
#else
  Indice
      i;
  for (i = 0; i < nb; i++)
    v[i] = vals[i];
#endif

  return *this;
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement l'affectation sous la forme donn�e en exemple
  
  Exemple :
  \code
  Vector t1,t2;
  t1=t2; // egalit� de deux vecteurs
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector &Vector::operator=(const Vector &vect)
//-----------------------------------------------------------------------------
{
  // redimension du vecteur resultat
  if (vect.nb != nb)
    redim(vect.nb);

    // recopie des valeurs
#ifdef MEM_funct
  memcpy(v, vect.v, nb * sizeof(Real));
#else
  Indice i;
  for (i = 0; i < nb; i++)
    v[i] = vect.v[i];
#endif

  return *this;
}

//!addition de deux vecteurs
/*!
  Cette methode permet de surd�finir l'op�ration d'addition des vecteurs et d'�crire simplement la somme de deux vecteurs sous la forme donn�e en exemple
  
  Exemple :
  \code
  Vector t1,t2,t3;
  t3=t1+t2; // somme de deux vecteurs
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Vector::operator+(const Vector &vect) const
//-----------------------------------------------------------------------------
{
  Indice i;
  Vector resu(nb);

#ifdef VERIF_math
  if (nb != vect.nb)
  {
    fatalErrorLine("Vector::operator +",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", nb,
                   vect.nb);
  }
#endif

  // calcul de la somme
  for (i = 0; i < nb; i++)
    resu.v[i] = v[i] + vect.v[i];
  return resu;
}

//!addition de deux vecteurs
/*!
  Cette methode permet de surd�finir l'op�ration d'addition des vecteurs et d'�crire simplement la somme de deux vecteurs sous la forme donn�e en exemple
  
  Exemple :
  \code
  Vector t1,t2;
  t2+=t1; // somme de deux vecteurs
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Vector::operator+=(const Vector &vect)
//-----------------------------------------------------------------------------
{
  Indice i;

#ifdef VERIF_math
  if (nb != vect.nb)
  {
    fatalErrorLine("Vector::operator +=",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", nb,
                   vect.nb);
  }
#endif

  // calcul de la somme
  for (i = 0; i < nb; i++)
    v[i] += vect.v[i];
}

//!soustraction de deux vecteurs
/*!
  Cette methode permet de surd�finir l'op�ration de soustraction des vecteurs et d'�crire simplement la soustraction de deux vecteurs sous la forme donn�e en exemple
  
  Exemple :
  \code
  Vector t1,t2,t3;
  t3=t1-t2; // soustraction de deux vecteurs
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Vector::operator-(const Vector &vect) const
//-----------------------------------------------------------------------------
{
  Indice i;
  Vector resu(nb);

#ifdef VERIF_math
  if (nb != vect.nb)
  {
    fatalErrorLine("Vector::operator -",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", nb,
                   vect.nb);
  }
#endif

  for (i = 0; i < nb; i++)
    resu.v[i] = v[i] - vect.v[i];
  return resu;
}

//!soustraction de deux vecteurs
/*!
  Cette methode permet de surd�finir l'op�ration de soustraction des vecteurs et d'�crire simplement la soustraction de deux vecteurs sous la forme donn�e en exemple
  
  Exemple :
  \code
  Vector t1,t2;
  t2-=t1; // soustraction de deux vecteurs
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Vector::operator-=(const Vector &vect)
//-----------------------------------------------------------------------------
{
  Indice i;

#ifdef VERIF_math
  if (nb != vect.nb)
  {
    fatalErrorLine("Vector::operator -=",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", nb,
                   vect.nb);
  }
#endif

  for (i = 0; i < nb; i++)
    v[i] -= vect.v[i];
}

//!multiplication d'un vecteur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des vecteurs et d'�crire simplement la multiplication d'un vecteur par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Vector t1,t2;
  Real l;
  t2=t1*l; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
    Vector::operator*(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Indice i;
  Vector resu(nb);

  for (i = 0; i < nb; i++)
    resu.v[i] = lambda * v[i];
  return resu;
}

//!multiplication d'un vecteur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des vecteurs et d'�crire simplement la multiplication d'un vecteur par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Vector t1;
  Real l;
  t1*=l; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Vector::operator*=(const Real lambda)
//-----------------------------------------------------------------------------
{
  Indice i;
  for (i = 0; i < nb; i++)
    v[i] *= lambda;
}

//!division d'un vecteur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de division des vecteurs et d'�crire simplement la division d'un vecteur par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Vector t1,t2;
  Real l;
  t1=t2/l; // division par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Vector::operator/(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Indice i;
  Vector resu(nb);
  Real unSurLambda = 1.0 / lambda;

  for (i = 0; i < nb; i++)
    resu.v[i] = v[i] * unSurLambda;
  return resu;
}

//!division d'un vecteur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de division des vecteurs et d'�crire simplement la division d'un vecteur par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Vector t1;
  Real l;
  t1/=l; // division par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Vector::operator/=(const Real lambda)
//-----------------------------------------------------------------------------
{
  Indice i;
  Real unSurLambda = 1.0 / lambda;
  for (i = 0; i < nb; i++)
    v[i] *= unSurLambda;
}

//!multiplication d'un vecteur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des vecteurs et d'�crire simplement la multiplication d'un vecteur par un scalaire sous la forme donn�e en exemple. Elle est identique � la forme pr�c�dente (et commutative).
  
  Exemple :
  \code
  Vector t1,t2;
  Real l;
  t2=l*t1; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
operator*(const Real lambda, const Vector &vect)
//-----------------------------------------------------------------------------
{
  Indice i;
  Vector resu(vect.nb);

  for (i = 0; i < vect.nb; i++)
    resu.v[i] = lambda * vect.v[i];
  return resu;
}

//!norme d'un vecteur
/*!
  Cette m�thode calcule la norme d'un vecteur et la renvoie
  \return norme du vecteur
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Real Vector::norm()
//-----------------------------------------------------------------------------
{
  Indice i;
  Real norme = 0.;

  for (i = 0; i < nb; i++)
    norme += SQ(v[i]);
  return sqrt(norme);
}

//!carr� de la norme d'un vecteur
/*!
  Cette m�thode calcule le carr� de la norme d'un vecteur et la renvoie
  \return carr� de la norme du vecteur
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Real Vector::norm2()
//-----------------------------------------------------------------------------
{
  Indice i;
  Real norme = 0.;

  for (i = 0; i < nb; i++)
    norme += SQ(v[i]);
  return norme;
}

//!normalisation d'un vecteur
/*!
  Cette m�thode modifie le vecteur en le rendant norm�, c'est � dire avec une norme �gale � 1.
*/
//-----------------------------------------------------------------------------
void Vector::normalize()
//-----------------------------------------------------------------------------
{
  // calcul de la norme
  Real norme = norm();

  // recalcul des composantes
  for (Indice i = 0; i < nb; i++)
    v[i] /= norme;
}

//!valeur maximale dans un vecteur
/*!
  Cette m�thode extrait la valeur maximale d'un vecteur
*/
//-----------------------------------------------------------------------------
Real Vector::maxValue()
//-----------------------------------------------------------------------------
{
  Real val = v[0];

  for (Indice i = 1; i < nb; i++)
    if (v[i] > val)
      val = v[i];

  return val;
}

//!valeur minimale dans un vecteur
/*!
  Cette m�thode extrait la valeur minimale d'un vecteur
*/
//-----------------------------------------------------------------------------
Real Vector::minValue()
//-----------------------------------------------------------------------------
{
  Real val = v[0];

  for (Indice i = 1; i < nb; i++)
    if (v[i] < val)
      val = v[i];

  return val;
}

//!valeur maximale dans un vecteur
/*!
  Cette m�thode extrait la valeur maximale d'un vecteur
*/
//-----------------------------------------------------------------------------
Real Vector::maxAbsoluteValue()
//-----------------------------------------------------------------------------
{
  Real val = Abs(v[0]);

  for (Indice i = 1; i < nb; i++)
    if (Abs(v[i]) > val)
      val = Abs(v[i]);

  return val;
}

//!valeur minimale dans un vecteur
/*!
  Cette m�thode extrait la valeur minimale d'un vecteur
*/
//-----------------------------------------------------------------------------
Real Vector::minAbsoluteValue()
//-----------------------------------------------------------------------------
{
  Real val = Abs(v[0]);

  for (Indice i = 1; i < nb; i++)
    if (Abs(v[i]) < val)
      val = Abs(v[i]);

  return val;
}

//!Assemblage d'un vecteur dans un autre
/*!
  Cette m�thode effectue la phase d'assemblage d'ue vecteur dans un autre conform�ment � ce qui se fait classiquement en Elements Finis.
  
  \param V Vecteur �lementaire � assembler dans le grand vecteur
  \param int tableau d'Indice correspondant au nombre de NOEUDS de l'�l�ment et contenant les Id des noeuds que l'on utilise
  \param numberOfDim nombre de dimensions du probl�me
  \code
  Vector Ve;
  Indice numberOfDim=...; // nombre de dimensions du probl�me
  Indice *ind=new Indice[loop_I]; // allocation m�moire pour ind
  for (Indice I=0; I < loop_I; I++) ind[I]=pel->nodes (I)->Id; // affectation des indices
  K.gatherFrom(Ve, ind, numberOfDim); 
  delete [] ind;
  \endcode
  \author Olivier PANTALE
  \version 1.1.0
*/
//-----------------------------------------------------------------------------
void Vector::gatherFrom(const Vector &V, Indice *ind0, int numberOfDim)
//-----------------------------------------------------------------------------
{
  Indice loop_I = V.nb / numberOfDim;
  Indice *ind = new Indice[loop_I];

  // preparation
  Indice *pind = ind;
  Indice *pind0 = ind0;
  for (Indice I = loop_I; I > 0; I--)
    *pind++ = *pind0++ * numberOfDim;

  // boucle principale
  switch (numberOfDim)
  {
  case 1:
  {
    for (Indice I = loop_I - 1; I >= 0; I--)
    {
      v[ind[I]] += V.v[I];
    }
  }
  break;
  case 2:
  {
    for (Indice I = loop_I - 1; I >= 0; I--)
    {
      Real *gl = &v[ind[I]];
      Real *lo = &V.v[I * numberOfDim];
      *gl++ += *lo++;
      *gl += *lo;
    }
  }
  break;
  case 3:
  {
    for (Indice I = loop_I - 1; I >= 0; I--)
    {
      Real *gl = &v[ind[I]];
      Real *lo = &V.v[I * numberOfDim];
      *gl++ += *lo++;
      *gl++ += *lo++;
      *gl += *lo;
    }
  }
  break;

    // cas g�n�ral
  default:
  {
    for (Indice I = loop_I - 1; I >= 0; I--)
    {
      // boucle sur les ddl
      Indice i = I * numberOfDim;
      Indice globI = ind[I];
      for (Indice Ii = 0; Ii < numberOfDim; Ii++)
        (*this)(globI + Ii) += V(i + Ii);
    }
  }
  }
  delete[] ind;
}

//!Assemblage d'un vecteur dans un autre
/*!
  Cette m�thode effectue la phase d'assemblage d'ue vecteur dans un autre conform�ment � ce qui se fait classiquement en Elements Finis.
  
  \param V Vecteur �lementaire � assembler dans le grand vecteur
  \param int tableau d'Indice correspondant au nombre de NOEUDS de l'�l�ment et contenant les Id des noeuds que l'on utilise
  \param numberOfDim nombre de dimensions du probl�me
  \code
  Vector Ve;
  Indice numberOfDim=...; // nombre de dimensions du probl�me
  Indice *ind=new Indice[loop_I]; // allocation m�moire pour ind
  for (Indice I=0; I < loop_I; I++) ind[I]=pel->nodes (I)->Id; // affectation des indices
  K.scatterFrom(Ve, ind, numberOfDim); 
  delete [] ind;
  \endcode
  \author Olivier PANTALE
  \version 1.1.0
*/
//-----------------------------------------------------------------------------
void Vector::scatterFrom(const Vector &V, Indice *ind0, int numberOfDim)
//-----------------------------------------------------------------------------
{
  // Indice mr=nb;
  Indice loop_I = nb / numberOfDim;
  Indice *ind = new Indice[loop_I];

  // preparation
  Indice *pind = ind;
  Indice *pind0 = ind0;
  for (Indice I = loop_I; I > 0; I--)
    *pind++ = *pind0++ * numberOfDim;

  // boucle principale
  switch (numberOfDim)
  {
  case 1:
  {
    for (Indice I = loop_I - 1; I >= 0; I--)
    {
      v[I] += V.v[ind[I]];
    }
  }
  break;
  case 2:
  {
    for (Indice I = loop_I - 1; I >= 0; I--)
    {
      Real *lo = &V.v[ind[I]];
      Real *gl = &v[I * numberOfDim];
      *gl++ += *lo++;
      *gl += *lo;
    }
  }
  break;
  case 3:
  {
    for (Indice I = loop_I - 1; I >= 0; I--)
    {
      Real *lo = &V.v[ind[I]];
      Real *gl = &v[I * numberOfDim];
      *gl++ += *lo++;
      *gl++ += *lo++;
      *gl += *lo;
    }
  }
  break;

    // cas g�n�ral
  default:
  {
    // boucle sur les ddl
    for (Indice I = loop_I - 1; I >= 0; I--)
    {
      Indice i = I * numberOfDim;
      Indice globI = ind[I];
      for (Indice Ii = 0; Ii < numberOfDim; Ii++)
        (*this)(i + Ii) += V(globI + Ii);
    }
  }
  }
  delete[] ind;
}

//!egalit� de deux vecteurs
/*!
  Cette m�thode teste l'�galit� de deux vecteurs
  \return True si les deux vecteurs sont identiques, False dans la cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Boolean
Vector::operator==(const Vector &vect) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (nb != vect.nb)
  {
    fatalErrorLine("Vector::operator ==",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", nb,
                   vect.nb);
  }
#endif

  for (Indice i = 0; i < nb; i++)
    if (v[i] != vect.v[i])
      return False;
  return True;
}

//!in�galit� de deux vecteurs
/*!
  Cette m�thode teste l'in�galit� de deux vecteurs
  \return True si les deux vecteurs sont differents, False dans la cas contraire
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Boolean
Vector::operator!=(const Vector &vec) const
//-----------------------------------------------------------------------------
{
  return !(*this == vec);
}

//!distance entre deux vecteurs
/*!
  Cette m�thode calcule la distance avec une norme Euclidienne entre deux points d�finis par deux vecteurs.
  \param w second vecteur
  \return valeur de la distance
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Real Vector::distance(const Vector &w) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (nb != w.nb)
  {
    fatalErrorLine("Vector::distance",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", nb,
                   w.nb);
  }
#endif

  Vector x = w - *this;
  return x.norm();
}

//!carr� de la distance entre deux vecteurs
/*!
  Cette m�thode calcule le carr� de la distance avec une norme Euclidienne entre deux points d�finis par deux vecteurs.
  \param w second vecteur
  \return valeur carr� de la distance
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Real Vector::distance2(const Vector &w) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (nb != w.nb)
  {
    fatalErrorLine("Vector::distance2",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", nb,
                   w.nb);
  }
#endif

  Vector x = w - *this;
  return x.norm2();
}

//!produit scalaire
/*!
  Cette m�thode calcule le produit scalaire de deux vecteurs
  \param w second vecteur
  \return valeur du produit scalaire des deux vecteurs
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Real Vector::dot(const Vector &w) const
//-----------------------------------------------------------------------------
{
  Real prod = 0;
  Indice i;

#ifdef VERIF_math
  if (nb != w.nb)
  {
    fatalErrorLine("Vector::dot",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", nb,
                   w.nb);
  }
#endif

  for (i = 0; i < nb; i++)
    prod += v[i] * w.v[i];
  return prod;
}

//!produit vectoriel
/*!
  Cette m�thode calcule le produit vectoriel de deux vecteurs
  \warning cette m�thode ne fonctionne que si la dimension des vecteurs est de 3.
  \param w second vecteur
  \return valeur du produit vectoriel des deux vecteurs
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector
Vector::vectorial(const Vector &w) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (nb != 3)
  {
    fatalErrorLine("Vector::vectorial",
                   "size of v1(%d) not allowed for this operation", nb);
  }
  if (w.nb != 3)
  {
    fatalErrorLine("Vector::vectorial",
                   "size of v2(%d) not allowed for this operation", w.nb);
  }
#endif

  Vector resu(nb);

  resu.v[0] = v[1] * w.v[2] - v[2] * w.v[1];
  resu.v[1] = v[2] * w.v[0] - v[0] * w.v[2];
  resu.v[2] = v[0] * w.v[1] - v[1] * w.v[0];

  return resu;
}

//!sortie sur flux ofstream
/*!
  Cette m�thode permet d'�crire un vecteur dans un fichier (notament) binaire
  
  Exemple :
  \code
  ofstream pfile("fichier");
  Vector t;
  t.write(pfile);
  t.close();
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
void Vector::write(ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)&nb, sizeof(Indice));
  ofs.write((char *)v, nb * sizeof(Real));
}

//!lecture sur flux ifstream
/*!
  Cette m�thode permet de lire un vecteur depuis un fichier (notament) binaire
  
  Exemple :
  \code
  ifstream pfile("fichier");
  Vector t;
  t.read(pfile);
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
Vector &Vector::read(ifstream &ifs)
//-----------------------------------------------------------------------------
{
  Indice
      nb_;

  ifs.read((char *)&nb_, sizeof(Indice));
  redim(nb_);
  ifs.read((char *)v, nb * sizeof(Real));
  return *this;
}

//!sortie sur flux ofstream
/*!
  Cette m�thode permet d'�crire un vecteur dans un fichier (notament) binaire
  
  Exemple :
  \code
  ofstream pfile("fichier");
  Vector t;
  pfile << t;
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const Vector &vect)
//-----------------------------------------------------------------------------
{
  vect.write(os);
  return os;
}

//!lecture sur flux ifstream
/*!
  Cette m�thode permet de lire un vecteur depuis un fichier (notament) binaire
  
  Exemple :
  \code
  ifstream pfile("fichier");
  Vector t;
  pfile >> t;
  \endcode
  \author Olivier PANTALE
  \version 0.9.4
*/
//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, Vector &vect)
//-----------------------------------------------------------------------------
{
  vect.read(is);
  return is;
}
