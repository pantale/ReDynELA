/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2006                                                *
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

/*
  Class io_Structure definition
*/

#include <Domain.h>
#include <io_Structure.h>

//-----------------------------------------------------------------------------
io_Structure::io_Structure(void)
//-----------------------------------------------------------------------------
{
  pstructure = NULL;
  action = None;
  incremental = None;
  binary = False;
  currentSave = 1;
  filename = "";
}

//-----------------------------------------------------------------------------
io_Structure::~io_Structure()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void io_Structure::transfert()
//-----------------------------------------------------------------------------
{
  if (name == "")
  {
    cerr << "Fatal Error\nMust use link function to attach a filename before call to transfert";
    exit(-1);
  }

  if (pstructure == NULL)
  {
    cerr << "Fatal Error\nMust use link function to attach a structure before call to transfert";
    exit(-1);
  }

  // test de sauvegarde incrementale
  if (incremental == True)
  {
    char tmp[5];

    sprintf(tmp, ".%03ld", currentSave);
    filename = name + tmp;
    currentSave++;
  }
  else
  {
    filename = name;
  }

  switch (action)
  {
  case Read:
  {
    structure_read();
  }
  break;
  case Write:
  {
    cout << "Saving result file " << filename << " ..." << endl;
    structure_write();
  }
  break;
  default:
    cerr << "Fatal Error\nMust define mode before call to transfert";
    exit(-1);
  }
}

//-----------------------------------------------------------------------------
void io_Structure::link(String st)
//-----------------------------------------------------------------------------
{
  name = st;
}

//-----------------------------------------------------------------------------
String &io_Structure::getFileName()
//-----------------------------------------------------------------------------
{
  return name;
}

//-----------------------------------------------------------------------------
void io_Structure::link(Structure *pd)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(pstructure == NULL);
#endif

  pstructure = pd;
}

//-----------------------------------------------------------------------------
char io_Structure::getMode()
//-----------------------------------------------------------------------------
{
  return (action);
}

//-----------------------------------------------------------------------------
void io_Structure::setMode(char mode)
//-----------------------------------------------------------------------------
{
  action = mode;
}

//-----------------------------------------------------------------------------
void io_Structure::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  if (filename != "")
    os << "filename = " << filename;
  else
    os << "filename = UNKNOWN";
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, io_Structure &structure)
//-----------------------------------------------------------------------------
{
  structure.print(os);
  return os;
}
