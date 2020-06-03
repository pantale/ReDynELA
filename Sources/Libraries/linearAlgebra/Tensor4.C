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

/*
  Class Tensor4 implementation
*/

#include <Tensor4.h>

/*!
  \file Tensor4.C
  \brief fichier .C de d�finition et de manipulation tenseurs d'ordre 4
  \ingroup linearAlgebra

  Ce fichier d�finit les m�thodes permettant de g�rer les tenseurs d'ordre 4.

  \author Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004  
*/

//!constructeur de la classe Tensor4
/*!
  Cette m�thode construit un tenseur d'ordre 4. Par d�faut, le contenu du tenseur est mis � z�ro
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4::Tensor4()
//-----------------------------------------------------------------------------
{
  // initialisation
  fill(0.);
}

//!destructeur de la classe Tensor4
//-----------------------------------------------------------------------------
Tensor4::~Tensor4()
//-----------------------------------------------------------------------------
{
}

//!affichage du contenu d'un tenseur
/*!
  Cette m�thode est une surd�finition de << pour les flux de sortie, son utilisation est donn�e comme suit
  
  Exemple
  \code
  Tensor4 t;
  cout << t << endl;
  \endcode
  \param os flux de sortie
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.print(os);
  return os;
}

//!affichage du contenu d'un tenseur
/*!
  Cette m�thode permet d'afficher le contenu d'un tenseur sur la sortie ostream
  \param os flux de sortie
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Tensor4::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  Indice i, j, k, l;
  os << "tensor 3x3x3\n";
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      for (k = 0; k < 3; k++)
      {
        for (l = 0; l < 3; l++)
        {
          os << "T[" << i << "," << j << "," << k << "," << l << "]="
             << v[tensorInd4(i, j, k, l, 3)];
        }
      }
    }
  }
}

//!renvoie un tenseur identit� � partir d'un tenseur quelconque
/*!
  Cette m�thode prend en argument un tenseur quelconque et renvoie un tenseur identit�
  
  Exemple :
  \code
  Tensor4 t1;
  t1.Identity(); // renvoie identit�
  \endcode
  \warning Cette m�thode modifie son argument
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Tensor4::Identity()
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < 3; i++)
    for (Indice j = 0; j < 3; j++)
      for (Indice k = 0; k < 3; k++)
        for (Indice l = 0; l < 3; l++)
          v[tensorInd4(i, j, k, l, 3)] =
              (kronecker(i, k) * kronecker(j, l) +
               kronecker(i, l) * kronecker(j, k)) /
              2.;
}

//!affectation d'�galit�
/*! 
  Cette m�thode est une surd�finition de la methode d'�galit� permettant d'�crire simplement le remplissage des valeurs d'un tenseur par un scalaire
  
  Exemple :
  \code
  Tensor4 t1;
  t1=1.; // affecte 1 � toutes les composantes du tenseur
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4 &Tensor4::operator=(const Real &val)
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
  Tensor4 t1,t2;
  t1=t2; // egalit� de deux tenseurs
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4 &Tensor4::operator=(const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
#ifdef MEM_funct
  memcpy(v, t1.v, 81 * sizeof(Real));
#else
  for (Indice i = 0; i < 27; i++)
    v[i] = t1.v[i];
#endif
  return *this;
}

//!addition de deux tenseurs du deuxi�me ordre
/*!
  Cette methode permet de surd�finir l'op�ration d'addition des tenseurs et d'�crire simplement la somme de deux tenseurs sous la forme donn�e en exemple
  
  Exemple :
  \code
  Tensor4 t1,t2,t3;
  t3=t1+t2; // somme de deux tenseurs
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
Tensor4::operator+(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor4 t2;

  // calcul de la somme
  for (Indice i = 0; i < 81; i++)
    t2.v[i] = v[i] + t1.v[i];

  // renvoi de l'objet
  return t2;
}

//!soustraction de deux tenseurs du deuxi�me ordre
/*!
  Cette methode permet de surd�finir l'op�ration de soustraction des tenseurs et d'�crire simplement la soustraction de deux tenseurs sous la forme donn�e en exemple
  
  Exemple :
  \code
  Tensor4 t1,t2,t3;
  t3=t1-t2; // soustraction de deux tenseurs
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
Tensor4::operator-(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor4 t2;

  // calcul de la somme
  for (Indice i = 0; i < 81; i++)
    t2.v[i] = v[i] - t1.v[i];

  // renvoi de l'objet
  return t2;
}

//!multiplication d'un tenseur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des tenseurs et d'�crire simplement la multiplication d'un tenseur par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Tensor4 t1,t2;
  Real l;
  t2=t1*l; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
    Tensor4::operator*(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (Indice i = 0; i < 81; i++)
    t2.v[i] = lambda * v[i];

  return t2;
}

//!division d'un tenseur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de division des tenseurs et d'�crire simplement la division d'un tenseur par un scalaire sous la forme donn�e en exemple
  
  Exemple :
  \code
  Tensor4 t1,t2;
  Real l;
  t2=t1/l; // division par un scalaire
  \endcode
  \warning Cette op�ration n'est bien sur pas commutative
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
Tensor4::operator/(const Real &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (Indice i = 0; i < 81; i++)
    t2.v[i] = v[i] / lambda;

  return t2;
}

//!multiplication d'un tenseur par un scalaire
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des tenseurs et d'�crire simplement la multiplication d'un tenseur par un scalaire sous la forme donn�e en exemple. Elle est identique � la forme pr�c�dente (et commutative).
  
  Exemple :
  \code
  Tensor4 t1,t2;
  Real l;
  t2=l*t1; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
operator*(const Real &lambda, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (Indice i = 0; i < 81; i++)
    t2.v[i] = lambda * t1.v[i];

  return t2;
}

//!multiplication d'un tenseur par un vecteur
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des tenseurs et d'�crire simplement la multiplication d'un tenseur par un vecteur sous la forme donn�e en exemple. Cette op�ration correspond � la notion de produit contract� d'un tenseur et d'un vecteur, elle renvoie un vecteur \c Vec3D.
  
  Exemple :
  \code
  Tensor4 t1;
  Vec3D v1;
  Tensor3 t2;
  t2=t1*v1; // produit contract�
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor3
    Tensor4::operator*(const Vec3D &v1) const
//-----------------------------------------------------------------------------
{
  Tensor3 t3;

  for (Indice i = 0; i < 3; i++)
    for (Indice j = 0; j < 3; j++)
      for (Indice k = 0; k < 3; k++)
        for (Indice l = 0; l < 3; l++)
          t3(i, j, k) += v[tensorInd4(i, j, k, l, 3)] * v1(l);

  return t3;
}

//!multiplication de deux tenseurs
/*!
  Cette methode permet de surd�finir l'op�ration de multiplication des tenseurs et d'�crire simplement la multiplication de deux tenseur sous la forme donn�e en exemple. Cette op�ration correspond � la notion de double produit contract� de deux tenseurs.
  
  \param t1 deuxi�me tenseur dans le double produit contract�
  
  Exemple :
  \code
  Tensor4 t1;
  Tensor2 t2,t3;
  t3=t1*t2; // double produit contract�
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor2
    Tensor4::operator*(const Tensor2 &t2) const
//-----------------------------------------------------------------------------
{
  Tensor2 t3;

  for (Indice i = 0; i < 3; i++)
    for (Indice j = 0; j < 3; j++)
      for (Indice k = 0; k < 3; k++)
        for (Indice l = 0; l < 3; l++)
          t3(i, j) += v[tensorInd4(i, j, k, l, 3)] * t2(k, l);

  return t3;
}

//!egalit� de deux tenseurs
/*!
  Cette m�thode teste l'�galit� de deux tenseurs
  \return True si les deux tenseurs sont identiques, False dans la cas contraire
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Boolean
Tensor4::operator==(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  Indice i;

  for (i = 0; i < 81; i++)
    if (v[i] != t1.v[i])
      return False;
  return True;
}

//!in�galit� de deux tenseurs
/*!
  Cette m�thode teste l'in�galit� de deux tenseurs
  \return True si les deux tenseurs sont differents, False dans la cas contraire
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Boolean
Tensor4::operator!=(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  return !(*this == t1);
}

//!sortie sur flux ofstream
/*!
  Cette m�thode permet d'�crire un tenseur dans un fichier (notament) binaire
  
  Exemple :
  \code
  ofstream pfile("fichier");
  Tensor4 t;
  t.write(pfile);
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Tensor4::write(ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)v, 81 * sizeof(Real));
}

//!lecture sur flux ifstream
/*!
  Cette m�thode permet de lire un tenseur depuis un fichier (notament) binaire
  
  Exemple :
  \code
  ifstream pfile("fichier");
  Tensor4 t;
  t.read(pfile);
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Tensor4::read(ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)v, 81 * sizeof(Real));
}

//!sortie sur flux ofstream
/*!
  Cette m�thode permet d'�crire un tenseur dans un fichier (notament) binaire
  
  Exemple :
  \code
  ofstream pfile("fichier");
  Tensor4 t;
  pfile << t;
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.write(os);
  return os;
}

//!lecture sur flux ifstream
/*!
  Cette m�thode permet de lire un tenseur depuis un fichier (notament) binaire
  
  Exemple :
  \code
  ifstream pfile("fichier");
  Tensor4 t;
  pfile >> t;
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.read(is);
  return is;
}
