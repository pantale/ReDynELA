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
	Class Picture implementation
*/

/*!
  \file Picture.C
  \brief fichier .C de d�finition des fichiers et manipulations d'images graphiques
  \ingroup basicTools

  Dans ce fichier on regroupe les d�finition li�es aux fichiers graphiques et � la manipulation des images graphiques en m�moire. On trouvera les m�thodes de manipulation d'image utilis�es par imageAnalyser, ainsi que des interfaces de lecture et �criture de fichiers graphiques.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <Picture.h>
#include <Error.h>
#include <Boolean.h>
#include <Const.h>

//!constructeur par d�faut de la classe Picture
/*!
  Ce constructeur n'alloue pas la m�moire pour les stockage de l'image. On cr�e juste une instance de la classe Picture. ce sont g�n�ralement les m�thodes associ�es � la cr�ation d'une image qui se chargent d'allouer la m�moire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Picture::Picture()
//-----------------------------------------------------------------------------
{
  buffer = NULL;
}

//!constructeur par recopie de la classe Picture
/*!
  Ce constructeur est charg� de faire une copie de l'image en m�moire. Il alloue la m�moire n�cessaire par l'interm�diaire de la m�thode allocate(), puis recopie le contenu d'une image graphique dans l'autre.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Picture::Picture(const Picture &X)
//-----------------------------------------------------------------------------
{
  allocate(X.width, X.height);

  // recopie
  memcpy(buffer, X.buffer, width * height * sizeof(BUFFER_TYPE));
}

//!destructeur de la classe Picture
/*!
  Ce destructeur lib�re la m�moire graphique si necessaire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Picture::~Picture()
//-----------------------------------------------------------------------------
{
  if (buffer != NULL)
    delete[] buffer;
}

//!allocation m�moire pour une image graphique
/*!
  Cette m�thode alloue la m�moire pour une image graphique.
  \param w largeur de l'image (pixels)
  \param h hauteur de l'image (pixels)
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Picture::allocate(Indice w, Indice h)
//-----------------------------------------------------------------------------
{
  width = w;
  height = h;

  if (buffer != NULL)
    delete[] buffer;

  buffer = new BUFFER_TYPE[w * h];

  //  for (Indice i=0;i<w*h;i++) buffer[i]=0;

  // if an error occured
  if (!buffer)
  {
    fatalError("memory allocation",
               "unable to allocate memory for image storage");
  }
}

//!flip horizontal d'une image graphique
/*!
  Cette m�thode effectue un flip horizontal d'une image graphique en m�moire. L'image courante est modifi�e.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Picture::horizontalFlip()
//-----------------------------------------------------------------------------
{
  Indice i, j;
  BUFFER_TYPE *pdebut;
  BUFFER_TYPE *pfin;
  BUFFER_TYPE *pbuffer = new BUFFER_TYPE;

  // si pas d'image, retour
  if (buffer == NULL)
    return;

  for (i = 0; i < height; i++)
  {
    pdebut = buffer + (width * i);
    pfin = pdebut + width - 1;
    for (j = 0; j < (width / 2); j++)
    {
      *pbuffer = *pdebut;
      *pdebut++ = *pfin;
      *pfin-- = *pbuffer;
    }
  }
  delete pbuffer;
}

//!flip vertical d'une image graphique
/*!
  Cette m�thode effectue un flip vertical d'une image graphique en m�moire. L'image courante est modifi�e.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Picture::verticalFlip()
//-----------------------------------------------------------------------------
{
  Indice i;
  BUFFER_TYPE *tmpBuffer; // allocation memoire pour une ligne
  BUFFER_TYPE *pdebut;
  BUFFER_TYPE *pfin;

  // si pas d'image, retour
  if (buffer == NULL)
    return;

  tmpBuffer = new BUFFER_TYPE[width];
  pdebut = buffer;
  pfin = buffer + (width * height - width);

  // copie physique
  for (i = 0; i < height / 2; i++)
  {
    memcpy(tmpBuffer, pdebut, sizeof(BUFFER_TYPE) * width);
    memcpy(pdebut, pfin, sizeof(BUFFER_TYPE) * width);
    memcpy(pfin, tmpBuffer, sizeof(BUFFER_TYPE) * width);
    pdebut += width;
    pfin -= width;
  }

  delete[] tmpBuffer;
}

//!rotation � droite d'une image
/*!
  Cette m�thode effectue une rotation � droite d'une image graphique. L'angle de rotation est de -90 degr�s.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Picture::rightRotate()
//-----------------------------------------------------------------------------
{
  BUFFER_TYPE *tmpBuffer; // allocation memoire pour une image
  BUFFER_TYPE *pbuff, *pdebut;
  tmpBuffer = new BUFFER_TYPE[width * height];
  int i, j;

  pbuff = tmpBuffer;
  tmpBuffer += width * height;

  for (i = width - 1; i >= 0; i--)
  {
    pdebut = buffer + i;
    for (j = 0; j < height; j++)
    {
      *tmpBuffer-- = *pdebut;
      pdebut += width;
    }
  }

  int tmp = height;
  height = width;
  width = tmp;

  delete[] buffer;
  buffer = pbuff;
}

//!rotation � gauche d'une image
/*!
  Cette m�thode effectue une rotation � gauche d'une image graphique. L'angle de rotation est de +90 degr�s.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Picture::leftRotate()
//-----------------------------------------------------------------------------
{
  BUFFER_TYPE *tmpBuffer; // allocation memoire pour une image
  BUFFER_TYPE *pbuff, *pdebut;
  tmpBuffer = new BUFFER_TYPE[width * height];
  int i, j;

  pbuff = tmpBuffer;

  for (i = width - 1; i >= 0; i--)
  {
    pdebut = buffer + i;
    for (j = 0; j < height; j++)
    {
      *tmpBuffer++ = *pdebut;
      pdebut += width;
    }
  }

  int tmp = height;
  height = width;
  width = tmp;

  delete[] buffer;
  buffer = pbuff;
}

//!inversion vid�o d'une image
/*!
  Cette m�thode effectue l'inversion video d'une image graphique. On cr�e ainsi un n�gatif de l'image.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Picture::videoInverse()
//-----------------------------------------------------------------------------
{
  BUFFER_TYPE *pbuffer;

  // initialisation du buffer
  pbuffer = buffer;

  // balayage et inversion video
  for (Indice i = 0; i < height * width; i++)
  {
    (*pbuffer++) = ((*pbuffer) ^ (0xFFFFFF));
  }
}

//!echange de 2 couleurs
/*!
  Cette m�thode �change 2 couleurs sur l'image active. Cette m�thode permet donc d'obtenir par exemple une image sur fond blanc au lieu de fond noir, en �changeant les couleurs 0,0,0 et 1,1,1. Tous les points noirs de l'image d'origine deviennent blancs et vice-versa.
  \param red1, green1, blue1 composantes RGB de la premi�re couleur
  \param red2, green2, blue2 composantes RGB de la deucxi�me couleur
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Picture::swapColors(unsigned char red1, unsigned char green1,
                         unsigned char blue1, unsigned char red2,
                         unsigned char green2, unsigned char blue2)
//-----------------------------------------------------------------------------
{
  BUFFER_TYPE color1, color2, color;
  BUFFER_TYPE *pbuffer;
  Indice i;

  // creation des couleurs
  color1 = ((blue1 << 16) + (green1 << 8) + red1) & (0xFFFFFF);
  color2 = ((blue2 << 16) + (green2 << 8) + red2) & (0xFFFFFF);

  // initialisation du buffer
  pbuffer = buffer;

  // balayage et inversion video
  for (i = 0; i < height * width; i++)
  {
    color = (*pbuffer) & (0xFFFFFF);
    if (color == color1)
      *pbuffer = color2;
    if (color == color2)
      *pbuffer = color1;
    pbuffer++;
  }
}

//!changement de taille d'une image
/*!
  Cette m�thode permet de d�finir une nouvelle taille pour l'image, aussi bien en agrandissement qu'en diminution de taille. Il convient quand m�me de faire attention � des probl�mes de moir� lors du changement de taille car l'algorithme utilis� est relativement simpliste.
  \param wd nouvelle largeur de l'image
  \param ht nouvelle hauteur de l'image
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Picture::resize(int wd, int ht)
//-----------------------------------------------------------------------------
{
  Indice i, j, ii, jj;
  Real hx, vx;

  // allocation
  BUFFER_TYPE *buffer2 = new BUFFER_TYPE[wd * ht];
  if (!buffer2)
  {
    fatalError("memory allocation",
               "unable to allocate memory for image storage");
  }

  hx = ((Real)width) / ((Real)wd);
  vx = ((Real)height) / ((Real)ht);

  for (i = 0; i < ht; i++)
  {
    for (j = 0; j < wd; j++)
    {
      ii = (Indice)(i * vx);
      jj = (Indice)(j * hx);
      buffer2[j + (i * wd)] = buffer[(Indice)(jj + (ii * width))];
    }
  }

  // destruction
  delete[] buffer;

  buffer = buffer2;
  width = wd;
  height = ht;
}

//!change la taille d'une image sans effectuer de zoom
/*!
  Cette m�thode change la taille d'une image sans effectuer de zoom. Elle ajoute un bord autour de l'image. 
  \warning La nouvelle taille de l'image doit �tre sup�rieure � l'ancienne.
  \param wd nouvelle largeur de l'image
  \param ht nouvelle hauteur de l'image
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Picture::resample(int wd, int ht)
//-----------------------------------------------------------------------------
{
  Indice i, j;
  Indice hx, vx;

  if (wd < width)
    fatalError("Not implemented", "resample image with width < old width");
  if (ht < height)
    fatalError("Not implemented", "resample image with height < old height");

  // allocation
  BUFFER_TYPE *buffer2 = new BUFFER_TYPE[wd * ht];
  if (!buffer2)
  {
    fatalError("memory allocation",
               "unable to allocate memory for image storage");
  }

  hx = (wd - width) / 2;
  vx = (ht - height) / 2;

  // mise au noir
  for (i = 0; i < ht * wd; i++)
    buffer2[i] = 0;

  // recopie image
  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width; j++)
    {
      buffer2[(j + hx) + ((i + vx) * wd)] = buffer[j + (i * width)];
    }
  }

  // destruction
  delete[] buffer;

  buffer = buffer2;
  width = wd;
  height = ht;
}

//-----------------------------------------------------------------------------
int Picture::GetByte(FILE *fp)
//-----------------------------------------------------------------------------
{
  int c;

  if ((c = getc(fp)) == EOF)
    fatalError("get byte", "unexpected end of file");

  return c;
}

//-----------------------------------------------------------------------------
int Picture::GetWord(FILE *fp)
//-----------------------------------------------------------------------------
{
  int c;

  c = getc(fp);
  c |= (getc(fp) << 8);
  return c;
}

//-----------------------------------------------------------------------------
void Picture::PutByte(FILE *pfile, unsigned char ch)
//-----------------------------------------------------------------------------
{
  putc(ch, pfile);
}

//-----------------------------------------------------------------------------
void Picture::PutWord(FILE *pfile, unsigned int ch)
//-----------------------------------------------------------------------------
{
  putc((ch & 0xff), pfile);
  putc((ch & 0xff00) >> 8, pfile);
}

//!constructeur par d�faut pour la classe targaPicture
/*!
  Ce constructeur n'alloue pas la m�moire pour les stockage de l'image. On cr�e juste une instance de la classe Picture. ce sont g�n�ralement les m�thodes associ�es � la cr�ation d'une image qui se chargent d'allouer la m�moire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
targaPicture::targaPicture()
//-----------------------------------------------------------------------------
{
  compress = False;
}

//!constructeur par recopie de la classe targaPicture
/*!
  Ce constructeur est charg� de faire une copie de l'image en m�moire. Il alloue la m�moire n�cessaire par l'interm�diaire de la m�thode allocate(), puis recopie le contenu d'une image graphique dans l'autre.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
targaPicture::targaPicture(const targaPicture &X)
//-----------------------------------------------------------------------------
{
  allocate(X.width, X.height);
  compress = False;

  // recopie
  memcpy(buffer, X.buffer, width * height * sizeof(BUFFER_TYPE));
}

//!destructeur de la classe targaPicture
/*!
  Ce destructeur lib�re la m�moire graphique si necessaire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
targaPicture::~targaPicture()
//-----------------------------------------------------------------------------
{
}

//!sauvegarde l'image sur disque
/*!
  Cette m�thode sauvegarde l'image en m�moire sur disque au format Targa.
  \param filename nom du fichier de sauvegarde
  \return nombre d'octets �crits sur disque
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Indice targaPicture::toFile(String filename)
//-----------------------------------------------------------------------------
{
  FILE *
      pfile;
  unsigned long *
      pbuff,
      *pbuff2;
  Indice
      total = 0;
  Indice
      i,
      j,
      nbr;
  int
      flag;

  if ((pfile = fopen(filename.chars(), "wb")) == NULL)
    fatalError("open file", "file open error");

  /* sauvegarde de l'entete */
  PutByte(pfile, 0); /* Symbole du targa */
  PutByte(pfile, 0); /* Pas de Map true colors */
  switch (compress)
  {
  case True:
    PutByte(pfile, TGA_RLERGB);
    break;
  case False:
    PutByte(pfile, TGA_RGB);
    break;
  default:
    fatalError("saving targa picture", "unknown compression type");
  }
  PutWord(pfile, 0); /* Pas de Map */
  PutWord(pfile, 0); /* Pas de Map */
  PutByte(pfile, 0); /* Pas de Map */
  PutWord(pfile, 0); /* Origine en 0,0 */
  PutWord(pfile, 0); /* Origine en 0,0 */
  PutWord(pfile, width);
  PutWord(pfile, height);
  PutByte(pfile, 24); /* taille 24 bits */
  PutByte(pfile, 32); /* no-interleave 00 upper left 1 reserved 0 0000 */

  /* sauvegarde fichier */
  switch (compress)
  {
  case False:
    pbuff = buffer;
    for (i = 0; i < height * width; i++)
    {
      PutByte(pfile, ((*pbuff) & 0xff0000) >> 16);
      PutByte(pfile, ((*pbuff) & 0x00ff00) >> 8);
      PutByte(pfile, (*pbuff) & 0x0000ff);
      pbuff++;
    }
    total = 3 * height * width;
    break;
  case True:
    total = 0;
    for (i = 0; i < height; i++)
    {
      j = 0;
      pbuff = (buffer) + (width * i);
      while (j < width)
      {
        pbuff2 = pbuff + 1;
        nbr = 1;
        flag = 0;
        while (nbr < 127 && flag == 0)
        {
          if (*pbuff2 == *pbuff)
          {
            nbr++;
            pbuff2++;
          }
          else
            flag = 1;
        }
        if (j + nbr > width)
          nbr = width - j;
        PutByte(pfile, (nbr - 1) | 0x80);
        PutByte(pfile, ((*pbuff) & 0xff0000) >> 16);
        PutByte(pfile, ((*pbuff) & 0x00ff00) >> 8);
        PutByte(pfile, (*pbuff) & 0x0000ff);
        j += nbr;
        pbuff = pbuff2;
        total += 4;
      }
    }
    break;
  }

  /* fermeture fichier */
  fclose(pfile);
  return total;
}

//!lecture d'une image � partir du disque
/*!
  Cette m�thode lit une image graphique au format Targa sur disque.
  \param filename nom du fichier graphique
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void targaPicture::fromFile(String filename)
//-----------------------------------------------------------------------------
{
  //  unsigned char IDLength;
  //  unsigned char CoMapType;
  unsigned char ImgType;
  //  unsigned int Index;
  //  unsigned int Length;
  //  unsigned char CoSize;
  //  unsigned int X_org;
  //  unsigned int Y_org;
  unsigned int Largeur;
  unsigned int Hauteur;
  unsigned char PixelSize;
  //  unsigned char AttBits;
  //  unsigned char Rsrvd;
  unsigned char OrgBit;
  //  unsigned char IntrLve;
  FILE *pfile;
  unsigned long *pbuff;
  char flags;
  int /*mapped, */ rlencoded;
  unsigned int hor, ver /*,realhor */;
  int RLE_flag = 0, RLE_count = 0;
  char Red, Grn, Blu;
  Red = Grn = Blu = 0;

  /* ouverture du fichier */
  pfile = fopen(filename.chars(), "rb");
  if (pfile == NULL)
    fatalError("impossible de lire", "le fichier %s\n", filename.chars());

  /* Lecture de l'entete */
  /*IDLength = */ GetByte(pfile);
  /*CoMapType = */ GetByte(pfile);
  ImgType = GetByte(pfile);
  /*Index = */ GetWord(pfile);
  /*Length = */ GetWord(pfile);
  /*CoSize = */ GetByte(pfile);
  /*X_org = */ GetWord(pfile);
  /*Y_org = */ GetWord(pfile);
  Largeur = GetWord(pfile);
  Hauteur = GetWord(pfile);
  PixelSize = GetByte(pfile);
  flags = GetByte(pfile);
  //AttBits = flags & 0xf;
  //Rsrvd = (flags & 0x10) >> 4;
  OrgBit = (flags & 0x20) >> 5;
  //IntrLve = (flags & 0xc0) >> 6;

  allocate(Largeur, Hauteur);

  switch (ImgType)
  {
  case TGA_RGB:
  case TGA_RLERGB:
    break;

  default:
    fatalError("Type d'image TARGA inconnu", "lecture");
  }

  /* Recherche si run length encoded */
  if (ImgType == TGA_RLEMap || ImgType == TGA_RLERGB || ImgType == TGA_RLEMono)
    rlencoded = 1;
  else
    rlencoded = 0;

  /* lecture de l'image */
  pbuff = buffer;

  for (hor = 0; hor < Hauteur; hor++)
  {
    for (ver = 0; ver < Largeur; ver++)
    {
      if (rlencoded)
      {
        if (RLE_count == 0)
        {
          unsigned char i;
          i = GetByte(pfile);
          RLE_flag = (i & 0x80) >> 7;
          if (RLE_flag == 0)
            RLE_count = i + 1;
          else
            RLE_count = i - 127;
          RLE_count--;
        }
        else
        {
          RLE_count--;
          if (RLE_flag != 0)
            goto PixEncode;
        }
      }
      switch (PixelSize)
      {

      case 8: /* Niveaux de gris */
        Red = Grn = Blu = GetByte(pfile);
        break;

      case 16:
      case 15:
      {
        char j, k;
        j = GetByte(pfile);
        k = GetByte(pfile);
        Red = (k & 0x7c) >> 2;
        Grn = ((k & 0x03) << 3) + ((j & 0xe0) >> 5);
        Blu = j & 0x1f;
      }
      break;

      case 32:
      case 24:
        Blu = GetByte(pfile);
        Grn = GetByte(pfile);
        Red = GetByte(pfile);
        if (PixelSize == 32)
          (void)GetByte(pfile);
      }
    PixEncode:
      *pbuff++ = (Blu << 16) + (Grn << 8) + Red;
    }
  }

  fclose(pfile);

  if (OrgBit != 0)
    verticalFlip();
}

//!constructeur par d�faut de la classe ppmPicture
/*!
  Ce constructeur n'alloue pas la m�moire pour les stockage de l'image. On cr�e juste une instance de la classe Picture. ce sont g�n�ralement les m�thodes associ�es � la cr�ation d'une image qui se chargent d'allouer la m�moire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
ppmPicture::ppmPicture()
//-----------------------------------------------------------------------------
{
}

//!constructeur par recopie de la classe ppmPicture
/*!
  Ce constructeur est charg� de faire une copie de l'image en m�moire. Il alloue la m�moire n�cessaire par l'interm�diaire de la m�thode allocate(), puis recopie le contenu d'une image graphique dans l'autre.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
ppmPicture::ppmPicture(const ppmPicture &X)
//-----------------------------------------------------------------------------
{
  allocate(X.width, X.height);

  // recopie
  memcpy(buffer, X.buffer, width * height * sizeof(BUFFER_TYPE));
}

//!destructeur de la classe ppmPicture
/*  Ce destructeur lib�re la m�moire graphique si necessaire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
ppmPicture::~ppmPicture()
//-----------------------------------------------------------------------------
{
}

//!sauvegarde l'image sur disque
/*!
  Cette m�thode sauvegarde l'image en m�moire sur disque au format PPM.
  \param filename nom du fichier de sauvegarde
  \return nombre d'octets �crits sur disque
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Indice ppmPicture::toFile(String filename)
//-----------------------------------------------------------------------------
{
  FILE *
      pfile;
  unsigned long *
      pbuff;
  Indice
      i;
  char *
      tampon;
  Indice
      ptampon = 0;
  tampon = new char[height * width * 3];

  // ouverture du flux de sortie
  if ((pfile = fopen(filename.chars(), "w")) == NULL)
    fatalError("open file", "file open error");

  // ecriture entete
  fprintf(pfile, "%c%c\n%d %d\n%d\n", PPM_MAGIC1, RPPM_MAGIC2, width, height,
          255);

  pbuff = buffer;
  for (i = 0; i < height * width; i++)
  {
    tampon[ptampon++] = (char)((*pbuff) & 0x0000ff);
    tampon[ptampon++] = (char)(((*pbuff) & 0x00ff00) >> 8);
    tampon[ptampon++] = (char)(((*pbuff) & 0xff0000) >> 16);
    pbuff++;
  }
  fwrite(tampon, 1, 3 * height * width, pfile);
  fclose(pfile);
  delete[] tampon;
  return 0;
}

//!lecture d'une image � partir du disque
/*!
  Cette m�thode lit une image graphique au format PPM sur disque.
  \param filename nom du fichier graphique
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void ppmPicture::fromFile(String filename)
//-----------------------------------------------------------------------------
{
  int magic1, magic2;
  int incols, inrows, maxval;
  char Red, Grn, Blu;
  Indice i;
  FILE *pfile;
  char inpstring[256];
  unsigned long *pbuff;

  /* ouverture du fichier */
  pfile = fopen(filename.chars(), "r");
  if (pfile == NULL)
    fatalError("impossible de lire", "le fichier %s\n", filename.chars());

  // lecture en tete
  magic1 = fgetc(pfile);
  magic2 = fgetc(pfile);
  fgetc(pfile);

  // sometimes, graphics programs include comments in files
  String str = "#";
  fgets(inpstring, 256, pfile);
  while (str[0] == '#')
  {
    fgets(inpstring, 256, pfile);
    str = inpstring;
    str.strip();
  }

  // now, we have the right line
  sscanf(inpstring, "%d %d", &inrows, &incols);
  fgets(inpstring, 256, pfile);
  sscanf(inpstring, "%d", &maxval);

  // tests d'image
  if (maxval > 255)
  {
    printf("Only handles maximum value of 255 < (found)%d\n", maxval);
    return;
  }
  if (magic1 != PPM_MAGIC1)
  {
    printf("Bad magic number 1; not ppm file?\n");
    return;
  }
  if (magic2 != RPPM_MAGIC2)
  {
    printf("Bad magic number 2; not raw ppm file?\n");
    return;
  }

  // allocation memoire
  allocate(inrows, incols);

  pbuff = buffer;
  for (i = 0; i < height * width; i++)
  {
    Red = GetByte(pfile);
    Grn = GetByte(pfile);
    Blu = GetByte(pfile);
    *pbuff++ = (Blu << 16) + (Grn << 8) + Red;
  }

  fclose(pfile);
}

//!constructeur par d�faut de la classe cuyvPicture
/*!
  Ce constructeur n'alloue pas la m�moire pour les stockage de l'image. On cr�e juste une instance de la classe Picture. ce sont g�n�ralement les m�thodes associ�es � la cr�ation d'une image qui se chargent d'allouer la m�moire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
cuyvPicture::cuyvPicture()
//-----------------------------------------------------------------------------
{
}

//!constructeur par recopie de la classe cuyvPicture
/*!
  Ce constructeur est charg� de faire une copie de l'image en m�moire. Il alloue la m�moire n�cessaire par l'interm�diaire de la m�thode allocate(), puis recopie le contenu d'une image graphique dans l'autre.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
cuyvPicture::cuyvPicture(const cuyvPicture &X)
//-----------------------------------------------------------------------------
{
  allocate(X.width, X.height);

  // recopie
  memcpy(buffer, X.buffer, width * height * sizeof(BUFFER_TYPE));
}

//!destructeur de la classe cuyvPicture
/*!
  Ce destructeur lib�re la m�moire graphique si necessaire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
cuyvPicture::~cuyvPicture()
//-----------------------------------------------------------------------------
{
}

#define FIXNUM 16

#define FIX(a, b) ((int)((a) * (1 << (b))))
#define UNFIX(a, b) ((a + (1 << (b - 1))) >> (b))
#define CCIRUV(x) (((((x)-128) * 224) >> 8) + 128)
#define CCIRY(x) (((((x)) * 219) >> 8) + 16)

#define CLIP(t) (((t) > 255) ? 255 : (((t) < 0) ? 0 : (t)))
#define GETY(r, g, b) UNFIX((FIX(0.299, FIXNUM) * (r) + \
                             FIX(0.587, FIXNUM) * (g) + \
                             FIX(0.144, FIXNUM) * (b)), \
                            FIXNUM)
#define GETU(r, g, b) UNFIX((FIX(-0.1687, FIXNUM) * (r) + \
                             FIX(-0.3313, FIXNUM) * (g) + \
                             FIX(0.5, FIXNUM) * (b)),     \
                            FIXNUM)
#define GETV(r, g, b) UNFIX((FIX(0.5, FIXNUM) * (r) +     \
                             FIX(-0.4187, FIXNUM) * (g) + \
                             FIX(-0.0813, FIXNUM) * (b)), \
                            FIXNUM)

//!sauvegarde l'image sur disque
/*!
  Cette m�thode sauvegarde l'image en m�moire sur disque au format Cuyv.
  \param filename nom du fichier de sauvegarde
  \return nombre d'octets �crits sur disque
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Indice cuyvPicture::toFile(String filename)
//-----------------------------------------------------------------------------
{
  FILE *
      outy,
      *outu, *outv;
  String
      name;
  int
      outcols,
      outrows;
  Indice
      ptampon = 0;
  unsigned long *
      pbuff;
  pbuff = buffer;
  unsigned char *
      temprgb1,
      *temprgb2, *tempy1, *tempy2, *tempu, *tempv;
  int
      r1,
      r2,
      r3,
      r4,
      g1,
      g2,
      g3,
      g4,
      b1,
      b2,
      b3,
      b4;
  int
      i,
      j,
      k;
  unsigned char *
      rgb1p,
      *rgb2p, *y1p, *y2p, *up, *vp;
  int
      temp;

  name = filename + ".Y";
  if (!(outy = fopen(name.chars(), "w")))
  {
    printf("cannot open file: %s\n", name.chars());
    exit(-1);
  }
  name = filename + ".U";
  if (!(outu = fopen(name.chars(), "w")))
  {
    printf("cannot open file: %s\n", name.chars());
    exit(-1);
  }
  name = filename + ".V";
  if (!(outv = fopen(name.chars(), "w")))
  {
    printf("cannot open file: %s\n", name.chars());
    exit(-1);
  }

  if (height & 1)
    outrows = height - 1;
  else
    outrows = height;
  if (width & 1)
    outcols = width - 1;
  else
    outcols = width;

  temprgb1 = (unsigned char *)calloc(width * 3, sizeof(char));
  temprgb2 = (unsigned char *)calloc(width * 3, sizeof(char));
  tempy1 = (unsigned char *)calloc(outcols, sizeof(char));
  tempy2 = (unsigned char *)calloc(outcols, sizeof(char));
  tempu = (unsigned char *)calloc(outcols >> 1, sizeof(char));
  tempv = (unsigned char *)calloc(outcols >> 1, sizeof(char));

  for (i = 0; i < (outrows >> 1); i++)
  {

    ptampon = 0;
    for (k = 0; k < width; k++)
    {
      temprgb1[ptampon++] = (char)((*pbuff) & 0x0000ff);
      temprgb1[ptampon++] = (char)(((*pbuff) & 0x00ff00) >> 8);
      temprgb1[ptampon++] = (char)(((*pbuff) & 0xff0000) >> 16);
      pbuff++;
    }
    ptampon = 0;
    for (k = 0; k < width; k++)
    {
      temprgb2[ptampon++] = (char)((*pbuff) & 0x0000ff);
      temprgb2[ptampon++] = (char)(((*pbuff) & 0x00ff00) >> 8);
      temprgb2[ptampon++] = (char)(((*pbuff) & 0xff0000) >> 16);
      pbuff++;
    }

    rgb1p = temprgb1;
    rgb2p = temprgb2;

    y1p = tempy1;
    y2p = tempy2;
    up = tempu;
    vp = tempv;

    for (j = 0; j < (outcols >> 1); j++)
    {
      r1 = *(rgb1p++);
      g1 = *(rgb1p++);
      b1 = *(rgb1p++);
      r2 = *(rgb1p++);
      g2 = *(rgb1p++);
      b2 = *(rgb1p++);

      r3 = *(rgb2p++);
      g3 = *(rgb2p++);
      b3 = *(rgb2p++);
      r4 = *(rgb2p++);
      g4 = *(rgb2p++);
      b4 = *(rgb2p++);

      temp = GETY(r1, g1, b1);
      *(y1p++) = CLIP(temp);
      temp = GETY(r2, g2, b2);
      *(y1p++) = CLIP(temp);
      temp = GETY(r3, g3, b3);
      *(y2p++) = CLIP(temp);
      temp = GETY(r4, g4, b4);
      *(y2p++) = CLIP(temp);

      /* We use a box filter for decimation */

      temp =
          ((GETU(r1 + r2 + r3 + r4, g1 + g2 + g3 + g4, b1 + b2 + b3 + b4) +
            (1 << 9) + (1 << 1)) >>
           2);
      *(up++) = CLIP(temp);
      temp =
          ((GETV(r1 + r2 + r3 + r4, g1 + g2 + g3 + g4, b1 + b2 + b3 + b4) +
            (1 << 9) + (1 << 1)) >>
           2);
      *(vp++) = CLIP(temp);
    }
    fwrite(tempy1, 1, outcols * sizeof(char), outy);
    fwrite(tempy2, 1, outcols * sizeof(char), outy);
    fwrite(tempu, 1, (outcols >> 1) * sizeof(char), outu);
    fwrite(tempv, 1, (outcols >> 1) * sizeof(char), outv);
  }

  fclose(outy);
  fclose(outu);
  fclose(outv);

  free(temprgb1);
  free(temprgb2);
  free(tempy1);
  free(tempy2);
  free(tempu);
  free(tempv);

  return (0);
}

//!lecture d'une image � partir du disque
/*!
  Cette m�thode lit une image graphique au format Targa sur disque.
  \warning Cette m�thode n'est pas impl�ment�e. Le format Cuyv n'est disponible qu'en �criture. Il sert � sauvegarder les images pour l'application de cr�ation des fichier animation Mpeg.
  \param filename nom du fichier graphique
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void cuyvPicture::fromFile(String filename)
//-----------------------------------------------------------------------------
{
  fatalError("cuyv from file ", "not implemented yet\n");
}
