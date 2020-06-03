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
// revision date : 16/03/2000

/*
 * Class List definition
 */

#ifndef __List_h__
#define __List_h__

#include <Const.h>
#include <Boolean.h>
#include <String.h>
#include <Error.h>

/*!
  \file List.h
  \brief fichier .h de d�finition de la liste standard pour tout type d'objet
  \ingroup basicTools

  Ce fichier d�crit les listes pour tous types d'objet.

  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/

#define DEFAULT_stack_size 10 //!< taille par d�faut de la liste
#define DEFAULT_stack_inc 10  //!< incr�ment par d�faut pour la liste

template <class Type>
class ListId;

/*!
  \class List ListId.h
  \brief liste standard pour tout type d'objet
  \ingroup basicTools

  Cette classe permet de g�rer tout type d'objet en m�moire sous forme de liste de type vectorielle par l'interm�diaire g�n�ralement des pointeurs sur les objets. Cette classe est un peu comme une classe de vecteurs que l'on utiliserait pour g�rer les objets en m�moire. Elle peut par exemple etre utilis�e pour g�rer une liste de noeuds, une liste d'�l�ments...

  Ce n'est pas une liste chain�e, mais une liste bas�e sur une notion de vecteur avec une dimension dynamique. La taille de la liste est adapt�e pour contenir le bon nombre d'�lements en m�moire. L'utilisateur � acc�s � chacun des �l�ments de la liste � travers l'index de cet �lement dans la liste ou � travers certaines m�thode particuli�res de recherche d'�lement selon divers crit�res. La partie de gestion des �l�ments en m�moire est totalement transparente pour l'utilisateur m�me s'il peut "garder" un oeil sur les param�tres ifluant cette gestion en m�moire.

  Exemple:
  \code
  void test()
  {
  List <object*> listOfObjects;
  object* obj1=new object;
  listOfObjects << object;
  }
  \endcode
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
/** @dia:pos 128,4 */
template <class Type>
class List
{

    friend class ListId<Type>;

private:
    Indice sz;
    Indice s_size;
    Indice s_inc;
    Indice pcurrent;
    Type *ptr;

public:
    List(const Indice stack = DEFAULT_stack_size);
    virtual ~List();

    // member methods
public:
    void redim(const Indice taille);
    void close();
    Type &operator()(const Indice i);
    Type operator()(const Indice i) const;
    Type next();
    Type first();
    Type last();
    Type previous();
    Type currentUp();
    Type currentDown();
    Type current();
    Indice size() const;
    Indice stackSize() const;
    Indice &stackIncrement();
    List<Type> operator<<(const Type objet);
    void sort(Boolean (*funct)(const Type objet1, const Type objet2));
    Type dichotomySearch(Indice (*funct)(const Type objet1, const Indice i), const Indice i) const;
    void inverse();
    void del(Indice ind);
    void del(Indice start, Indice stop);
    void delBefore(Indice ind);
    void delAfter(Indice ind);
    Boolean contains(const Type objet) const;

    // basic search method
    Indice getIndex(const Type objet) const;

    // comparison methods
    Boolean operator==(const List<Type> &objet) const;
    Boolean operator!=(const List<Type> &objet) const;
    Indice objectSize()
    {
        return (sizeof(*this) + s_size * sizeof(ptr));
    }

    // io methods
    void print(ostream &os) const;

    // virtual methods
    virtual void flush();
    virtual void add(const Type objet);
    virtual void insert(const Type objet, Indice ind);
};

/*!
  \class ListId List.h
  \brief classe de liste d'objets avec index.
  \ingroup basicTools
  \author &copy; Olivier PANTALE

  Cette classe permet de g�rer tout type d'objet en m�moire sous forme de liste de type vectorielle par l'interm�diaire g�n�ralement des pointeurs sur les objets. Cette classe est un peu comme une classe de vecteurs que l'on utiliserait pour g�rer les objets en m�moire. Elle peut par exemple etre utilis�e pour g�rer une liste de noeuds, une liste d'�l�ments...

  Ce n'est pas une liste chain�e, mais une liste bas�e sur une notion de vecteur avec une dimension dynamique. La taille de la liste est adapt�e pour contenir le bon nombre d'�lements en m�moire. L'utilisateur � acc�s � chacun des �l�ments de la liste � travers l'index de cet �lement dans la liste ou � travers certaines m�thode particuli�res de recherche d'�lement selon divers crit�res. La partie de gestion des �l�ments en m�moire est totalement transparente pour l'utilisateur m�me s'il peut "garder" un oeil sur les param�tres ifluant cette gestion en m�moire.

  Cette classe ListId est une extension de la classe List. Elle inclu plus de possibilit�s bas�es sur la pr�sence d'un indice � l'int�rieur de chaque objet (\b nomm� \b Id \b obligatoirement). A l'aide de cet index, on peut alors effecuer des op�rations de tri sur la liste et optimiser la recherche d'�lements dans la liste.

  \warning L'utilisation de ce type de liste pour une classe oblige � inclure une variable \b Id de type Indice dans la classe.

  Exemple:
  \code
  void test()
  {
  ListId <object*> listOfObjects;
  object* obj1=new object;
  listOfObjects << object;
  }
  \endcode
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/

/** @dia:pos 124,46 */
/** @dia:route List;v,139.8,36.2,40,135.8,46 */
template <class Type>
class ListId : public List<Type>
{

    Boolean sorted;
    Boolean comp;

public:
    ListId(const Indice stack = DEFAULT_stack_size);
    ~ListId();

    // member methods
    Type AppN(const Indice i) const;
    Indice IAppN(const Indice i) const;
    Boolean isSorted() const;
    Boolean isCompacted() const;
    void flush();
    void add(const Type objet);
    void sort();
    void forceSort();
    void sort(Boolean (*funct)(const Type objet1, const Type objet2));
    void compact();
    void del(const Type ind);
    void del(const Type start, const Type stop);
    void delBefore(const Type ind);
    void delAfter(const Type ind);
    void del(Indice ind)
    {
        List<Type>::del(ind);
    }
    void insert(const Type objet, Indice ind);
    void del(Indice start, Indice stop)
    {
        List<Type>::del(start, stop);
    }
    void delBefore(Indice ind)
    {
        List<Type>::delBefore(ind);
    }
    void delAfter(Indice ind)
    {
        List<Type>::delAfter(ind);
    }
};

//!constructeur par d�faut de la classe List
/*!
  Ce constructeur alloue la m�moire par d�faut pour une instance de la classe List. Si la taille de la liste n'est pas pr�cis�e, la taille par d�faut est prise en compte, celle ci est d�finie par la valeur de DEFAULT_stack_size.
  \param stack d�finit la taille initiale de la liste en nombre d'objets
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
List<Type>::List(const Indice stack)
//-----------------------------------------------------------------------------
{
    // allocation des constantes
    s_size = stack;
    s_inc = DEFAULT_stack_inc;
    sz = 0;
    pcurrent = 0;

    // allocation m�moire pour la liste
    ptr = new Type[s_size];

#ifdef VERIF_alloc
    if (ptr == NULL)
        fatalError("List<Type>::List",
                   "Memory allocation error\n"
                   "May be out of memory on this system\n");
#endif
}

//!extension de la taille d'une liste
/*!
  Cette m�thode est utilis�e pour allonger ou r�duire la taille d'une liste. Si la nouvelle taille propos�e est inf�rieure � la taille minimale necessaire pour stocker les �l�ments actuels de la liste, une erreur est g�n�r�e. Cette m�thode ne doit g�n�ralement pas �tre appel�e par l'utilisateur (sauf s'il a une totale maitreise du truc ;-0 ). Cette m�thode est fortement utilis�e en interne par les autres m�thodes de la classe. Dans le cas o� l'utilisateur n'a pas la maitrise suffisante pour ce genre d'op�ration, il vaut mieux laisser la classe g�rer elle m�me ses allocations m�moire.

  Une utilisation possible de cette m�thode se situe dans la pr�-allocation m�moire, quand on connait � l'avance le nombre d'objets qui seront stock�s dans la liste. On ajuste alors la taille de la liste � cette valeur ce qui �vite les op�rations d'ajustement dynamique de taille co�teuses en temps CPU. Bien entendu, les m�canismes d'allocation dynamique existent et on peut d�passer cette valeur.

  Exemple:
  \code
  void test()
  {
  List <object*> listOfObjects;
  listOfObjects.redim(1280); // 1280 objets � stocker
  for (i=0;i<1280);i++)
  {
  object* obj1=new object;
  listOfObjects << obj1; // stockage sans aucune r�allocation dynamique
  }
  object* obj2=new object;
  listOfObjects << obj2; // et Oups, un 1281 �me
  }
  \endcode
  \param size_ d�finit la nouvelle taille de la liste
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::redim(const Indice size_)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (size_ < sz)
        fatalError("template <class Type> void List<Type>::redim(const Indice)\n",
                   "new size < actual size means to truncate list\n"
                   "not allowed with redim method\n"
                   "Must use a 'del' method");
#endif

    // affectation de la size_
    s_size = size_;

    // allocation de la zone memoire
    Type *ptr2 = new Type[s_size];

#ifdef VERIF_alloc
    if (ptr2 == NULL)
        fatalError("List<Type>::redim",
                   "memory allocation error\nMay be out of memory on this system\n");
#endif

    // recopie de la zone memoire
    memcpy(ptr2, ptr, sz * sizeof(Type));

    // destruction de l'ancien pointeur
    delete[] ptr;

    // reaffectation du pointeur
    ptr = ptr2;
}

//!ajuste la taille courante d'une liste � la taille r�elle
/*!
  Cette m�thode est utilis�e pour ajuster la taille de la liste en fonction du nombre d'objets r�els contenus dans la liste. Cette m�thode permet alors de r�cup�rer de l'espace m�moire, principalement pour les petites listes.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::close()
//-----------------------------------------------------------------------------
{
    // on fait un redim a la size_ reelle de la pile
    redim(sz);
}

//!accesseur aux �l�ments de la liste
/*!
  Cette m�thode est utilis�e pour acc�der aux �l�ments de la liste. Cet acc�s est � la fois en lecture et en �criture. Cette m�thode retourne l'�lement [i] de la liste. La base de r�f�rence est 0 (permier �l�ment d'indice 0) comme habituellement en C et C++.
  \param ind num�ro de l'�lement dans la liste
  \return un �lement de la liste
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type &List<Type>::operator()(const Indice ind)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if ((ind < 0) || (ind >= sz))
    {
        cerr << "Fatal Error in template <class Type> Type& List<Type>::operator ()\n";
        cerr << "Indice " << ind << " out of allowd range {0-" << sz - 1 << "}\n";
        exit(-1);
    }
#endif
    return ptr[pcurrent = ind];
}

//!accesseur aux �l�ments de la liste
/*!
  Cette m�thode est utilis�e pour acc�der aux �l�ments de la liste. Cet acc�s est en lecture seule. Cette m�thode retourne l'�lement [i] de la liste. La base de r�f�rence est 0 (permier �l�ment d'indice 0) comme habituellement en C et C++.
  \param ind num�ro de l'�lement dans la liste
  \return un �lement de la liste
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::operator()(const Indice ind)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if ((ind < 0) || (ind >= sz))
    {
        cerr << "Fatal Error in template <class Type> Type& List<Type>::operator ()\n";
        cerr << "Indice " << ind << " out of allowd range {0-" << sz - 1 << "}\n";
        exit(-1);
    }
#endif
    return ptr[ind];
}

//!renvoie l'�l�ment suivant dans la liste
/*!
  Cette m�thode utilise un m�canisme de rep�rage interne dans la liste pour renvoyer l'�l�ment suivant le pr�c�dent appel dans la liste. Pour utiliser cette m�thode, il convient de bien cerner les bornes de la liste, et d'avoir bien r�f�renc� le d�part par les m�thodes first(), last() ou les accesseurs ().
  \return �l�ment suivant dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::next()
//-----------------------------------------------------------------------------
{
    if (pcurrent >= sz - 1)
    {
        return NULL;
    }
    return ptr[++pcurrent];
}

//!renvoie l'�l�ment suivant dans la liste
/*!
  Cette m�thode utilise un m�canisme de rep�rage interne dans la liste pour renvoyer l'�l�ment suivant le pr�c�dent appel dans la liste. Pour utiliser cette m�thode, il convient de bien cerner les bornes de la liste, et d'avoir bien r�f�renc� le d�part par les m�thodes first(), last() ou les accesseurs ().
  \return �l�ment suivant dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::currentUp()
//-----------------------------------------------------------------------------
{
    if (pcurrent >= sz)
    {
        return NULL;
    }

    return ptr[pcurrent++];
}

//!renvoie l'�l�ment suivant dans la liste
/*!
  Cette m�thode utilise un m�canisme de rep�rage interne dans la liste pour renvoyer l'�l�ment suivant le pr�c�dent appel dans la liste. Pour utiliser cette m�thode, il convient de bien cerner les bornes de la liste, et d'avoir bien r�f�renc� le d�part par les m�thodes first(), last() ou les accesseurs ().
  \return �l�ment suivant dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::currentDown()
//-----------------------------------------------------------------------------
{
    if (pcurrent < 0)
    {
        return NULL;
    }

    return ptr[pcurrent--];
}

//!renvoie le premier �lement de la liste
/*!
  Cette m�thode retourne le premier �lement de la liste.
  \return premier �lement dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::first()
//-----------------------------------------------------------------------------
{
    if (sz == 0)
    {
        return NULL;
    }
    return ptr[pcurrent = 0];
}

//!renvoie le dernier �lement de la liste
/*!
  Cette m�thode retourne le dernier �lement de la liste.
  \return dernier �lement dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::last()
//-----------------------------------------------------------------------------
{
    if (sz == 0)
    {
        return NULL;
    }
    return ptr[pcurrent = sz - 1];
}

//!renvoie l'�lement pr�c�dent dans la liste
/*!
  Cette m�thode utilise un m�canisme de rep�rage interne dans la liste pour renvoyer l'�l�ment pr�c�dent du "pr�c�dent appel" dans la liste. Pour utiliser cette m�thode, il convient de bien cerner les bornes de la liste, et d'avoir bien r�f�renc� le d�part par les m�thodes first(), last() ou les accesseurs ().
  \return �l�ment pr�c�dent dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::previous()
//-----------------------------------------------------------------------------
{
    if (pcurrent == 0)
    {
        return NULL;
    }
    return ptr[--pcurrent];
}

//!�l�ment courant dans la liste
/*!
  Cette m�thode utilise un m�canisme de rep�rage interne dans la liste pour renvoyer l'�l�ment courant du "pr�c�dent appel" dans la liste. Pour utiliser cette m�thode, il convient de bien cerner les bornes de la liste, et d'avoir bien r�f�renc� le d�part par les m�thodes first(), last() ou les accesseurs ().
  \return �l�ment courant dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::current()
//-----------------------------------------------------------------------------
{
    if (sz == 0)
    {
        return NULL;
    }
    return ptr[pcurrent];
}

//!taille de la liste
/*!
  Cette m�thode retourne la taille de la liste, c'est � dire le nombre exact d'�lements dans la liste. Les indices varient dans l'intervalle [0:sz-1]
  \return taille de la liste
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Indice List<Type>::size() const
//-----------------------------------------------------------------------------
{
    return sz;
}

//!taille de la pile
/*!
  Cette m�thode retourne la taille de la pile, c'est � dire le nombre maxi d'�lements dans la liste avant la prochaine r�allocation dynamique.
  \return taille de la pile
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Indice List<Type>::stackSize() const
//-----------------------------------------------------------------------------
{
    return s_size;
}

//!valeur de l'incr�ment de pile
/*!
  Cette m�thode est utilis�e pour contr�ler la valeur de l'incr�ment de pile. A la prochaine allocation m�moire pour la pile, cette valeur sera utilis�e pour d�terminer de quelle taille doit etre augment�e la pile. Une grande valeur permet d'obtenir un code plus rapide (car elle diminue la fr�quence des allocations m�moire), mais elle est �galement plus co�teuse en terme de m�moire allou�e.
  \return valeur de l'incr�ment de pile
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Indice &List<Type>::stackIncrement()
//-----------------------------------------------------------------------------
{
    return s_inc;
}

//!vide la contenu de la pile
/*!
  Cette m�thode vide le contenu de la pile et ram�ne sa taille r�elle � z�ro et sa taille de pile � DEFAULT_stack_size. La pile est comme neuve !!! (c'est une pile rechargeable ;-] )
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::flush()
//-----------------------------------------------------------------------------
{
    s_size = DEFAULT_stack_size;
    sz = 0;
    delete[] ptr;
    pcurrent = 0;

    ptr = new Type[s_size];
#ifdef VERIF_alloc
    if (ptr == NULL)
        fatalError("template <class Type> void List<Type>::flush()",
                   "fonction flush de template <class Type> List<Type>\n");
#endif
}

//!ajoute un objet � la liste
/*!
  Cette m�thode ajoute un objet � la liste. L'objet est ajout� � la fin de la liste, et la taille de liste est automatiquement incr�ment�e si besoin.
  \param objet � rajouter � la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
List<Type> List<Type>::operator<<(const Type object)
//-----------------------------------------------------------------------------
{
    add(object);
    return *this;
}

//!ins�re un �lement dans la liste
/*!
  Cette m�thode ajoute un objet � la liste. L'objet est ins�r� au milieu de la liste, et la taille de liste est automatiquement incr�ment�e si besoin.
  \param ind d�finit la position de l'insertion dans la liste
  \param objet � rajouter � la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::insert(const Type objet,
                        Indice ind)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (ind > sz)
        fatalError("List<Type>::insert(Indice ind)",
                   "position indice (%d) out of bounds (%d)\n", ind, sz);
#endif

    // add the last object at the end
    add(ptr[sz - 1]);

    // move from the end to the insertion point
    if (sz - 2 >= ind)
    {
        memmove(ptr + ind + 1, ptr + ind, (sz - ind - 2) * sizeof(Type));
    }

    // insert the object
    ptr[ind] = objet;
}

//!ins�re un �lement dans la liste
/*!
  Cette m�thode ajoute un objet � la liste. L'objet est ins�r� au milieu de la liste, et la taille de liste est automatiquement incr�ment�e si besoin.
  \param ind d�finit la position de l'insertion dans la liste
  \param objet � rajouter � la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::insert(const Type objet,
                          Indice ind)
//-----------------------------------------------------------------------------
{
    List<Type>::insert(objet, ind);
    sorted = No;
    comp = No;
}

//!ajoute un objet � la liste
/*!
  Cette m�thode ajoute un objet � la liste. L'objet est ajout� � la fin de la liste, et la taille de liste est automatiquement incr�ment�e si besoin.
  \param objet � rajouter � la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::add(const Type object)
//-----------------------------------------------------------------------------
{
    // test de reallocation memoire
    if (sz >= s_size)
    {
        redim(s_size + s_inc);
    }

    // stockage du courrant
    pcurrent = sz;

    // ajout de l'objet
    ptr[sz++] = object;
}

//!inverse la liste
/*!
  Cette m�thode inverse l'ordre des �lements dans la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::inverse()
//-----------------------------------------------------------------------------
{
    Type v;
    for (Indice i = 0; i < sz / 2; i++)
    {
        v = ptr[i];
        ptr[i] = ptr[sz - i - 1];
        ptr[sz - i - 1] = v;
    }
}

//!supprime un ensemble d'�l�ments dans la liste
/*!
  Cette m�thode supprime un ensemble d'�l�ments dans la liste. Cette m�thode est utilis�e pour supprimer tout un segment de la liste, en d�finissant les indices de d�part et d'arriv�e du segment dans la liste. Si les param�tres start et stop sont �gaux, un seul �lement est supprim�.
  \param start premier �lement � supprimer
  \param stop dernier �lement � supprimer
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::del(Indice start, Indice stop)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (start > stop)
        fatalError("List<Type>::del(Indice start,Indice stop)",
                   "start indice %d > stop indice %d\n", start, stop);
    if (stop >= sz)
        fatalError("List<Type>::del(Indice start,Indice stop)",
                   "stop indice (%d) out of bounds (%d)\n", stop, sz);
#endif

    // recouvrement
    if (sz - stop - 1 > 0)
    {
        memmove(ptr + start, ptr + (stop + 1), (sz - stop - 1) * sizeof(Type));
    }

    // recalcul de la size_
    sz -= (stop - start + 1);
}

//!supprime un �lement de la liste
/*!
  Cette m�thode supprime un �l�ment dans la liste. Cette m�thode est utilis�e pour supprimer un seul �lement de la liste, en d�finissant l'indice de l'�l�ment � supprimer
  \param ind �l�ment � supprimer
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::del(Indice ind)
//-----------------------------------------------------------------------------
{
    List<Type>::del(ind, ind);
}

//!supprime les �lements avant un indice donn�
/*!
  Cette m�thode supprime tous les �lements de la liste compris entre le d�but de la liste et la valeur donn�e en argument de cette m�thode. Cette m�thode est �quivalente � del(0,ind-1).
  \param ind d�finit le premier �lement de la liste � garder
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::delBefore(Indice ind)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (ind - 1 >= sz)
        fatalError("List<Type>::delBefore(Indice ind)",
                   "ind indice (%d) out of bounds (%d)\n", ind, sz);
#endif

    List<Type>::del(0, ind - 1);
}

//!supprime les �lements apr�s un indice donn�
/*!
  Cette m�thode supprime tous les �lements de la liste compris entre la valeur donn�e en argument de cette m�thode et la fin de la liste. Cette m�thode est �quivalente � del(ind+1,last()).
  \param ind d�finit le dernier �lement de la liste � garder
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::delAfter(Indice ind)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (ind + 1 >= sz)
        fatalError("List<Type>::delAfter(Indice ind)",
                   "ind indice (%d) out of bounds (%d)\n", ind, sz);
#endif

    List<Type>::del(ind + 1, sz - 1);
}

//!comparaison de deux listes
/*!
  Cette m�thode est utilis�e pour comparer deux listes entre elles. Elle teste l'�galit�.
  \param liste de comparaison
  \return True si les deux listes sont �gales, False dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Boolean List<Type>::operator==(const List<
                               Type>
                                   &liste) const
//-----------------------------------------------------------------------------
{
    // la comparaison porte sur la size_
    if (sz != liste.sz)
    {
        return (False);
    }

    // et sur le contenu
    for (Indice i = 0; i < sz; i++)
        if (ptr[i] != liste.ptr[i])
        {
            return (False);
        }

    return (True);
}

//!comparaison de deux listes
/*!
  Cette m�thode est utilis�e pour comparer deux listes entre elles. Elle teste la non �galit�.
  \param liste de comparaison
  \return False si les deux listes sont �gales, True dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Boolean List<Type>::operator!=(const List<
                               Type>
                                   &liste) const
//-----------------------------------------------------------------------------
{
    return !(*this == liste);
}

//!affichage des �l�ments de la liste
/*!
  Cette m�thode affiche les �l�ments de la liste. Elle est utilis�e � des fins de d�bogage.
  \warning Les objets g�r�s par la liste doivent avoir une m�thode \b << permettant l'affichage de leur contenu.
  \param os flux \c ostream de sortie
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::print(ostream &os) const
//-----------------------------------------------------------------------------
{
    os << "list " << sz << "/" << s_size << "={";
    for (Indice i = 0; i < sz; i++)
    {
        if (i != 0)
        {
            cout << ",";
        }
        os << *ptr[i];
    }
    os << "}";
}

//!m�thode de recherche dans la liste
/*!
  Cette m�thode effectue une recherche simple d'un �l�ment dans la liste et renvoie un Index indiquant la place de l'objet dans la liste. Si l'objet n'est pas trouv�, elle retourne la valeur -1.
  \param objet objet � rechercher dans la liste
  \return Index de l'objet dans la liste
  \author &copy; Olivier PANTALE
  \version 1.0.0
  \date 2002
*/
//-----------------------------------------------------------------------------
template <class Type>
Indice List<Type>::getIndex(const Type objet) const
//-----------------------------------------------------------------------------
{
    // recherche bourrin de base
    for (Indice i = 0; i < sz; i++)
    {
        if (ptr[i] == objet)
        {
            //	  pcurrent=i;
            return i;
        }
    }

    // bourrin pas trouv� alors on retourne -1
    return -1;
}

//!m�thode de recherche dans la liste
/*!
  Cette m�thode effectue une recherche simple d'un �l�ment dans la liste et renvoie un bool�en selon la pr�sence ou non de cet objet dans la liste.
  \param objet objet � rechercher dans la liste
  \return True si l'objet est dans la liste, False dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Boolean List<Type>::contains(const Type objet) const
//-----------------------------------------------------------------------------
{
    for (Indice i = 0; i < sz; i++)
    {
        if (ptr[i] == objet)
        {
            return True;
        }
    }
    return False;
}

//!constructeur par d�faut de la classe ListId
/*!
  Ce constructeur alloue la m�moire par d�faut pour une instance de la classe List. Si la taille de la liste n'est pas pr�cis�e, la taille par d�faut est prise en compte, celle ci est d�finie par la valeur de DEFAULT_stack_size.
  \param stack d�finit la taille initiale de la liste en nombre d'objets
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
ListId<Type>::ListId(const Indice stack) : List<Type>(stack)
//-----------------------------------------------------------------------------
{
    // allocation des constantes
    sorted = Yes;
    comp = Yes;
}

//!destructeur de la classe List
//-----------------------------------------------------------------------------
template <class Type>
List<Type>::~List()
//-----------------------------------------------------------------------------
{
    // vide, ici il semble que l'on ait rien a faire
}

//!destructeur de la classe ListId
//-----------------------------------------------------------------------------
template <class Type>
ListId<Type>::~ListId()
//-----------------------------------------------------------------------------
{
    // vide, ici il semble que l'on ait rien a faire
}

//!recherche d'une �lement dans la liste
/*!
  Cette m�thode est utilis�e pour rechercher un �l�ment dans la liste en utilisant un algorithme dichotomique. Cette m�thode retourne l'�l�ment correspondant dans la liste ou la valeur NULL si l'�l�ment n'est pas dans la liste.
  Exemple:
  \code
  class truc
  {
    public:
    Indice z; // une valeur
  };
  ListId <truc*> listeTrucs; // la liste
  Indice compare(truc* p1, Indice in) // la fonction de comparaison
  {
    return (p1->z - in); // comparaison
  }
  ...
  {
  ...
  listeTrucs.sort(compare,10); // cherche la valeur 10
  }
  \endcode
  \warning Cette m�thode est uniquement valable si la liste est tri�e sur le param�tre de recherche.
  \param funct fonction d�finissant la m�thode de comparaison � utiliser
  \param ind valeur particuli�re de l'�l�ment � rechercher dans la liste
  \return l'�l�ment correspondant dans la liste ou la valeur NULL si l'�l�ment n'est pas dans la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::dichotomySearch(Indice (*funct)(const Type objet1, const Indice in), const Indice ind) const
//-----------------------------------------------------------------------------
{
    Indice i;

    // si la size_ est nulle, on retourne NULL
    if (this->sz == 0)
    {
        return NULL;
    }

    // tri dichotomique
    Indice g, d, dx;
    g = 0;
    d = this->sz - 1;
    do
    {
        i = (g + d) / 2;
        dx = funct(this->ptr[i], ind);
        if (dx == 0)
        {
            return this->ptr[i];
        }
        if (dx > 0)
        {
            d = i - 1; // a droite de i
        }
        else
        {
            g = i + 1; // a gauche de i
        }
    } while (g <= d);

    return NULL;
}

//!recherche d'une �lement dans la liste
/*!
  Cette m�thode est utilis�e pour rechercher un �l�ment dans la liste. L'algorithme de recherche est bas� sur le param�tre \b Id d�clar� dans les objets de la liste. Cette m�thode retourne l'�l�ment correspondant dans la liste ou la valeur NULL si l'�l�ment n'est pas dans la liste.
  \param ind index de l'�l�ment � rechercher dans la liste
  \return l'�l�ment correspondant dans la liste ou la valeur NULL si l'�l�ment n'est pas dans la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type ListId<Type>::AppN(const Indice ind) const
//-----------------------------------------------------------------------------
{
    Indice i;

    // si la size_ est nulle, on retourne NULL
    if (this->sz == 0)
    {
        return NULL;
    }

    // si elle est triee et compactee
    if (comp == Yes)
        if ((ind >= 0) && (ind < this->sz))
        {
            return this->ptr[ind];
        }

    // recherche selon le cas
    if (sorted == Yes)
    {
        // tri dichotomique
        Indice g, d;
        g = 0;
        d = this->sz - 1;
        do
        {
            i = (g + d) / 2;
            if (this->ptr[i]->Id == ind)
            {
                return this->ptr[i];
            }
            if (ind < this->ptr[i]->Id)
            {
                d = i - 1; // a droite de i
            }
            else
            {
                g = i + 1; // a gauche de i
            }
        } while (g <= d);
    }
    else
    {
        // tri bete mais terriblement efficace non !!
        for (i = 0; i < this->sz; i++)
            if (this->ptr[i]->Id == ind)
            {
                return this->ptr[i];
            }
    }
    return NULL;
}

//!recherche d'une �lement dans la liste
/*!
  Cette m�thode est utilis�e pour rechercher un �l�ment dans la liste. L'algorithme de recherche est bas� sur le param�tre \b Id d�clar� dans les objets de la liste. Cette m�thode retourne l'indice de l'�l�ment correspondant dans la liste ou la valeur 0 si l'�l�ment n'est pas dans la liste.
  \warning La valeur de retour 0 peut etre confondue avec la premi�re valeur de la liste ??? BUG ???
  \param ind index de l'�l�ment � rechercher dans la liste
  \return indice de l'�l�ment correspondant dans la liste ou la valeur 0 si l'�l�ment n'est pas dans la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Indice ListId<Type>::IAppN(const Indice ind) const
//-----------------------------------------------------------------------------
{
    Indice i;

    // si la size_ est nulle, on retourne NULL
    if (this->sz == 0)
    {
        return 0;
    }

    // si elle est triee et compactee
    if (comp == Yes)
        if ((ind >= 0) && (ind < this->sz))
        {
            return (ind);
        }

    // recherche selon le cas
    if (sorted == Yes)
    {
        // tri dichotomique
        Indice g, d;
        g = 0;
        d = this->sz - 1;
        do
        {
            i = (g + d) / 2;
            if (this->ptr[i]->Id == ind)
            {
                return (i);
            }
            if (ind < this->ptr[i]->Id)
            {
                d = i - 1; // a droite de i
            }
            else
            {
                g = i + 1; // a gauche de i
            }
        } while (g <= d);
    }
    else
    {
        // tri bete
        for (i = 0; i < this->sz; i++)
            if (this->ptr[i]->Id == ind)
            {
                return (i);
            }
    }
    return 0;
}

//!teste si la liste est tri�e
/*!
  Cette m�thode teste si la liste est actuellement tri�e.
  \return True si la liste est tri�e False dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Boolean ListId<Type>::isSorted() const
//-----------------------------------------------------------------------------
{
    return sorted;
}

//!teste si la liste est compact�e
/*!
  Cette m�thode teste si la liste est actuellement compact�e. La notion de compaction est li�e au fait que les indices de la liste sont contigus les uns aux autres en ordre croissant sans aucun "trou".
  \return True si la liste est compact�e False dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Boolean ListId<Type>::isCompacted() const
//-----------------------------------------------------------------------------
{
    return comp;
}

//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::flush()
//-----------------------------------------------------------------------------
{
    this->s_size = DEFAULT_stack_size;
    this->sz = 0;
    sorted = Yes;
    comp = Yes;
    delete[] this->ptr;
    this->pcurrent = 0;

    this->ptr = new Type[this->s_size];
#ifdef VERIF_alloc
    if (this->ptr == NULL)
        fatalError("template <class Type> void ListId<Type>::flush()",
                   "fonction flush de template <class Type> ListId<Type>\n");
#endif
}

//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::add(const Type object)
//-----------------------------------------------------------------------------
{
    // test de reallocation memoire
    if (this->sz >= this->s_size)
    {
        this->redim(this->s_size + this->s_inc);
    }

    // test de tri
    if (sorted == Yes)
    {
        if (this->sz != 0)
        {
            if (this->ptr[this->sz - 1]->Id > object->Id)
            {
                sorted = No;
                comp = No;
            }

            // test de comp
            if (object->Id - this->ptr[this->sz - 1]->Id != 1)
            {
                comp = No;
            }
        }
        else
        {
            if (object->Id != 0)
            {
                comp = No;
            }
        }
    }
    // stockage du courrant
    this->pcurrent = this->sz;

    // ajout de l'objet
    this->ptr[this->sz++] = object;
}

//!trie la liste
/*!
  Cette m�thode trie les �lements de la liste en fonction de l'index \b Id contenu dans chaque �l�ment de la liste. Cette m�thode force le tri de la pile, meme ci celle-ci semble d�ja tri�e.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::forceSort()
//-----------------------------------------------------------------------------
{
    sorted = No;
    sort();
}

//!trie la liste
/*!
  Cette m�thode trie les �lements de la liste en fonction de l'index \b Id contenu dans chaque �l�ment de la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::sort()
//-----------------------------------------------------------------------------
{
    if (sorted == Yes)
    {
        return;
    }

    register Type v;
    register Indice i, j;
    register Indice h = 1;
    while (h <= this->sz)
    {
        h = 3 * h + 1;
    }
    while (h != 1)
    {
        h = (Indice)(h / 3);
        for (i = h + 1; i <= this->sz; i++)
        {
            v = this->ptr[i - 1];
            j = i;
            while (this->ptr[j - h - 1]->Id > v->Id)
            {
                this->ptr[j - 1] = this->ptr[j - h - 1];
                j = j - h;
                if (j <= h)
                {
                    break;
                }
            }
            this->ptr[j - 1] = v;
        }
    }

    // maintenant elle est triee
    sorted = Yes;
}

//!tri de la liste � partir d'une fonction de comparaison
/*!
  Cette m�thode trie les �lements de la pile en utilisant une fonction de comparaison d�finie par l'utilisateur. Cette m�thode est tr�s performante pour trier une liste et tr�s souple d'utilisation. L'utilisation peut sembler complexe, mais elle est d�finie dans l'exemple ci-dessous.

  Exemple:
  \code
  class truc
  {
    public:
    Real z; // une valeur
  };
  ListId <truc*> listeTrucs; // la liste
  Boolean compare(truc* p1,truc* p2) // la fonction de comparaison
  {
    return (p1->z < p2->z); // comparaison
  }
  ...
  {
  ...
  listeTrucs.sort(compare); // tri selon la fonction de comparaison
  }
  \endcode
  \param funct fonction d�finissant la m�thode de comparaison � utiliser
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<
    Type>::sort(Boolean (*funct)(const Type objet1, const Type objet2))
//-----------------------------------------------------------------------------
{
    register Type v;
    register Indice i, j;
    register Indice h = 1;
    while (h <= this->sz)
    {
        h = 3 * h + 1;
    }
    while (h != 1)
    {
        h = (Indice)(h / 3);
        for (i = h + 1; i <= this->sz; i++)
        {
            v = this->ptr[i - 1];
            j = i;
            while (funct(this->ptr[j - h - 1], v))
            {
                this->ptr[j - 1] = this->ptr[j - h - 1];
                j -= h;
                if (j <= h)
                {
                    break;
                }
            }
            this->ptr[j - 1] = v;
        }
    }
}

//!compactage de la liste
/*!
  Cette m�thode compacte la liste. La notion de compaction est li�e au fait que les indices de la liste sont contigus les uns aux autres en ordre croissant sans aucun "trou".
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::compact()
//-----------------------------------------------------------------------------
{
    for (Indice i = 0; i < this->sz; i++)
    {
        this->ptr[i]->Id = i;
    }

    // maintenant elle est triee et compactee
    comp = Yes;
    sorted = Yes;
}

//!supprime un ensemble d'�l�ments dans la liste
/*!
  Cette m�thode supprime un ensemble d'�l�ments dans la liste. Cette m�thode est utilis�e pour supprimer tout un segment de la liste, en d�finissant les indices de d�part et d'arriv�e du segment dans la liste. Si les param�tres start et stop sont �gaux, un seul �lement est supprim�.
  \param start premier �lement � supprimer
  \param stop dernier �lement � supprimer
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::del(const Type start,
                       const Type stop)
//-----------------------------------------------------------------------------
{
    List<Type>::del(IAppN(start->Id), IAppN(stop->Id));
}

//!supprime un �lement de la liste
/*!
  Cette m�thode supprime un �l�ment dans la liste. Cette m�thode est utilis�e pour supprimer un seul �lement de la liste, en d�finissant l'indice de l'�l�ment � supprimer
  \param ind �l�ment � supprimer
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::del(const Type ind)
//-----------------------------------------------------------------------------
{
    Indice i = IAppN(ind->Id);
    List<Type>::del(i, i);
}

//!supprime les �lements avant un indice donn�
/*!
  Cette m�thode supprime tous les �lements de la liste compris entre le d�but de la liste et la valeur donn�e en argument de cette m�thode. Cette m�thode est �quivalente � del(0,ind-1).
  \param ind d�finit le premier �lement de la liste � garder
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::delBefore(const Type ind)
//-----------------------------------------------------------------------------
{
    List<Type>::del(0, IAppN(ind->Id) - 1);
}

//!supprime les �lements apr�s un indice donn�
/*!
  Cette m�thode supprime tous les �lements de la liste compris entre la valeur donn�e en argument de cette m�thode et la fin de la liste. Cette m�thode est �quivalente � del(ind+1,last()).
  \param ind d�finit le dernier �lement de la liste � garder
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListId<Type>::delAfter(const Type ind)
//-----------------------------------------------------------------------------
{
    List<Type>::del(IAppN(ind->Id) + 1, this->sz - 1);
}

//-----------------------------------------------------------------------------
//void sort (Boolean (*funct) (const Type objet1, const Type objet2))
template <class Type>
void ListId<
    Type>::sort(Boolean (*funct)(const Type objet1, const Type objet2))
//-----------------------------------------------------------------------------
{
    List<Type>::sort(*funct);
    sorted = No;
}

#endif
