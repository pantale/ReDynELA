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
// revision date :

/*
	Class Exception implementation
*/

#include <Exception.h>

/*!
  \class Exception Exception.C
  \brief Class 
  \ingroup basicTools
  \version 1.0.0
  \date 1997-2002
  \author Olivier PANTALE


*/
//-----------------------------------------------------------------------------
Exception::Exception()
    : file(""), line(0), function(""), cond(""), exc("")
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Exception::Exception(const char *f, const int l, const char *func, const char *c, const char *e)
    : file(f), line(l), function(func), cond(c), exc(e)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Exception::SetInfo(const char *f, const int l, const char *func, const char *c, const char *e)
//-----------------------------------------------------------------------------
{
  file = f;
  line = l;
  function = func;
  cond = c;
  exc = e;
}

//-----------------------------------------------------------------------------
void Exception::print()
//-----------------------------------------------------------------------------
{
  cerr << "The exception: " << exc << "\noccurred in line <" << line
       << "> of file <" << file
       << "> in method:" << std::endl
       << "    " << function << std::endl
       << "The violated condition was: " << std::endl
       << "    " << cond << std::endl;
}
