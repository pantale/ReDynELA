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

// begin date : 04/03/1997
// revision date : 18/07/1997

#include <SampleArchive.h>

//-----------------------------------------------------------------------------
void SampleFile::fromSource(String nm)
//-----------------------------------------------------------------------------
{
  struct stat bufferStat;
  name = nm;

  // get status of file
  stat(name.chars(), &bufferStat);
  size = bufferStat.st_size;
  mode = bufferStat.st_mode;

  FILE *pfile = fopen(name.chars(), "r");
  if (pfile == NULL)
    fatalError("SampleFileData::fromFile", "Unable to read file\n");
  data = new char[size + 1];
  fread(data, sizeof(char), size, pfile);
  fclose(pfile);
}

//-----------------------------------------------------------------------------
void SampleFile::toSource()
//-----------------------------------------------------------------------------
{
  FILE *pfile = fopen(name.chars(), "w");
  if (pfile == NULL)
    fatalError("SampleFileData::fromFile", "Unable to write file\n");
  fwrite(data, sizeof(char), size, pfile);
  fclose(pfile);
}

//-----------------------------------------------------------------------------
void SampleFile::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  name.toFile(pfile);
  fwrite((void *)(&size), sizeof(Indice), 1, pfile);
  fwrite((void *)(&mode), sizeof(mode_t), 1, pfile);
  fwrite(data, sizeof(char), size, pfile);
}

//-----------------------------------------------------------------------------
void SampleFile::fromFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  name.fromFile(pfile);
  fread((void *)(&size), sizeof(Indice), 1, pfile);
  fread((void *)(&mode), sizeof(mode_t), 1, pfile);

  if (data != NULL)
    delete[] data;
  data = new char[size + 1];

  fread(data, sizeof(char), size, pfile);
}

//-----------------------------------------------------------------------------
void SampleArchive::toFile(String nm)
//-----------------------------------------------------------------------------
{
  FILE *pfile;
  Indice number = files.size();

  //  String nm = name;
  nm += ".archive";
  pfile = fopen(nm.chars(), "w");

  if (pfile == NULL)
    fatalError("SampleFileData::fromFile", "Unable to write file\n");

  name.toFile(pfile);
  description.toFile(pfile);
  readme.toFile(pfile);
  fwrite((void *)(&number), sizeof(Indice), 1, pfile);
  fwrite((void *)(&size), sizeof(Indice), 1, pfile);

  for (Indice i = 0; i < number; i++)
  {
    files(i)->toFile(pfile);
    files(i)->toSource();
  }

  fclose(pfile);
}

//-----------------------------------------------------------------------------
void SampleArchive::fromFile(String nm, Boolean load)
//-----------------------------------------------------------------------------
{
  FILE *pfile;
  Indice number = 0;

  nm += ".archive";

  pfile = fopen(nm.chars(), "r");
  if (pfile == NULL)
    fatalError("SampleFileData::getSample", "Unable to read file\n");

  name.fromFile(pfile);
  description.fromFile(pfile);
  readme.fromFile(pfile);

  fread((void *)(&number), sizeof(Indice), 1, pfile);
  fread((void *)(&size), sizeof(Indice), 1, pfile);

  if (load)
    for (Indice i = 0; i < number; i++)
    {
      SampleFile *pf = new SampleFile;
      files << pf;
      pf->fromFile(pfile);
    }

  fclose(pfile);
}

//-----------------------------------------------------------------------------
void SampleArchive::toSource()
//-----------------------------------------------------------------------------
{
  for (Indice i = 0; i < files.size(); i++)
    files(i)->toSource();
}
