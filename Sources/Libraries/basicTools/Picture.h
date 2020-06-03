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
	Class Picture definition
*/

/*!
  \file Picture.h
  \brief fichier .h de d�finition des fichiers et manipulations d'images graphiques
  \ingroup basicTools

  Dans ce fichier on regroupe les d�finition li�es aux fichiers graphiques et � la manipulation des images graphiques en m�moire. On trouvera les m�thodes de manipulation d'image utilis�es par imageAnalyser, ainsi que des interfaces de lecture et �criture de fichiers graphiques.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#ifndef __Picture_h__
#define __Picture_h__

#include <String.h>

#define BUFFER_TYPE unsigned long

//enum {pictureTypeRGBA};
enum
{
  targaFormat,
  ppmFormat,
  cuyvFormat
};

class targaPicture;
class ppmPicture;
class cuyvPicture;

/*!
  \brief classe de base de manipulaton des images
  \ingroup basicTools

  Cette classe sert � stocker et � manipuler les images graphiques en m�moire. Cette classe inclue les m�canismes de stockage interne de l'image graphique, et dispose de diverses m�thodes publiques permettant de manipuler ces images graphiques en m�moire. Cette classe est une classe de base, et n'est g�n�ralement jamais instanci�e par l'utilisateur, elle ne peut d'ailleurs pas l'�tre vu qu'elle comprend des m�thodes purement virtuelles.

  Pour son utilisation, l'utilisateur doit instancier une des classes d�riv�es de celle-ci. Ces classes d�riv�es incluent toutes les m�thodes de cette classe de base (par h�ritage) ainsi que des m�thodes sp�cialis�es pour faire la lecture ou �criture de fichiers dans un format ou un autre. On se repportera � l'�xemple ci-dessous qui illistre ce m�canisme.

  Example:
  On d�crit ci-dessous une m�thode simple pour convertir un fichier d'un format � un autre. Afin de r�aliser cette op�ration, on lit un premier fichier de type targa � partir d'une instance de la classe targaPicture (un fichier .tga), puis on cr�e une instance de la classe ppmPicture par recopie de la premi�re. Enfin, on sauvegarde l'instance de la classe ppmPicture sur dique (un fichier .ppm).
  \code
  targaPicture picture; // cr�e une instance targaPicture
  picture.fromFile("image.tga"); // charge le fichier
  ppmPicture picture2((ppmPicture&)picture); // convertit le fichier
  picture2.toFile("image2.ppm"); // sauvegarde le fichier
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/
/** @dia:pos 88,24 */
class Picture
{

  friend class targaPicture;
  friend class ppmPicture;
  friend class cuyvPicture;

public:
  int width;           //!< largeur de l'image en pixels
  int height;          //!< hauteur de l'image en pixels
  BUFFER_TYPE *buffer; //!< acc�s au buffer de stockage de l'image (dangereux pour les novices)

private:
  int GetByte(FILE *fp);
  int GetWord(FILE *fp);
  void PutByte(FILE *fp, unsigned char);
  void PutWord(FILE *fp, unsigned int);

public:
  // constructeurs
  Picture();
  Picture(const Picture &X);
  virtual ~Picture();
  void allocate(Indice w, Indice h);

  // fonctions membres
  void verticalFlip();
  void horizontalFlip();
  void videoInverse();
  void rightRotate();
  void leftRotate();
  void swapColors(unsigned char red1, unsigned char green1,
                  unsigned char blue1, unsigned char red2,
                  unsigned char green2, unsigned char blue2);
  void resize(int wd, int ht);
  void resample(int wd, int ht);
  virtual Indice toFile(String filename) = 0;
  virtual void fromFile(String nom_fichier) = 0;
};

/* Definitions for image types. */
#define TGA_Null 0
#define TGA_Map 1
#define TGA_RGB 2
#define TGA_Mono 3
#define TGA_RLEMap 9
#define TGA_RLERGB 10
#define TGA_RLEMono 11
#define TGA_CompMap 32
#define TGA_CompMap4 33

/* Definitions for interleave flag. */
#define TGA_IL_None 0
#define TGA_IL_Two 1
#define TGA_IL_Four 2

/*!
  \class targaPicture Picture.h
  \brief classe de manipulation des images au format Targa
  \ingroup basicTools

  Cette classe est destin�e � servir d'interface avec le format de fichier graphique Targa.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

/** @dia:pos 104,54 */
/** @dia:route Picture;v,95.775,42.6,48,110.55,54 */
class targaPicture : public Picture
{
protected:
  Boolean compress;

public:
  // constructeurs
  targaPicture();
  targaPicture(const targaPicture &X);
  ~targaPicture();

  // fonctions membres
  Indice toFile(String filename);
  void fromFile(String nom_fichier);
  Boolean &compression()
  {
    return compress;
  } //!< switch on or off the compression algorithm
};

#define PPM_MAGIC1 'P'
#define PPM_MAGIC2 '3'
#define RPPM_MAGIC2 '6'

/*!
  \class ppmPicture Picture.h
  \brief classe de manipulation des images au format Ppm
  \ingroup basicTools

  Cette classe est destin�e � servir d'interface avec le format de fichier graphique Ppm.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

/** @dia:pos 86,54 */
/** @dia:route Picture;v,95.775,42.6,48,92.375,54 */
class ppmPicture : public Picture
{
public:
  // constructeurs
  ppmPicture();
  ppmPicture(const ppmPicture &X);
  ~ppmPicture();

  // fonctions membres
  Indice toFile(String filename);
  void fromFile(String nom_fichier);
};

/*!
  \class cuyvPicture Picture.h
  \brief classe de manipulation des images au format Cuyv
  \ingroup basicTools

  Cette classe est destin�e � servir d'interface avec le format de fichier graphique Cuyv.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

/** @dia:pos 94,64 */
/** @dia:route Picture;v,95.775,42.6,48,100.375,64 */
class cuyvPicture : public Picture
{
public:
  // constructeurs
  cuyvPicture();
  cuyvPicture(const cuyvPicture &X);
  ~cuyvPicture();

  // fonctions membres
  Indice toFile(String filename);
  void fromFile(String nom_fichier);
};
#endif
