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
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
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
