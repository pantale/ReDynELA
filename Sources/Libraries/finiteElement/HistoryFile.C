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
// revision date :

/*
	Class HistoryFile implementation
*/

#include <HistoryFile.h>
#include <Node.h>
#include <Select.h>

/*!
  \file HistoryFile.C
  \brief fichier .C de d�finition des fichiers d'historique sur DynELA
  \ingroup femLibrary

  Ce fichier sert � la d�finition des des fichiers d'historique sur DynELA

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/

//!default constructor of the class HistoryFileItem
/*!

*/
//-----------------------------------------------------------------------------
HistoryFileItem::HistoryFileItem()
//-----------------------------------------------------------------------------
{
}

//!copy constructor of the class HistoryFileItem
/*!

*/
//-----------------------------------------------------------------------------
HistoryFileItem::HistoryFileItem(const HistoryFileItem &X)
//-----------------------------------------------------------------------------
{
}

//!destructor of the class HistoryFileItem
/*!

*/
//-----------------------------------------------------------------------------
HistoryFileItem::~HistoryFileItem()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Real HistoryFileItem::get()
//-----------------------------------------------------------------------------
{
  return node->getNodalValue(field, component);
}

//!default constructor of the class HistoryFile
/*!

*/
//-----------------------------------------------------------------------------
HistoryFile::HistoryFile()
//-----------------------------------------------------------------------------
{
  pfile = NULL;
  stopTime = 0;
  startTime = 0;
  nextTime = 0;
  saveTime = 0;
}

//!copy constructor of the class HistoryFile
/*!

*/
//-----------------------------------------------------------------------------
HistoryFile::HistoryFile(const HistoryFile &X)
//-----------------------------------------------------------------------------
{
}

//!destructor of the class HistoryFile
/*!

*/
//-----------------------------------------------------------------------------
HistoryFile::~HistoryFile()
//-----------------------------------------------------------------------------
{
  close();
}

//-----------------------------------------------------------------------------
void HistoryFile::save(Real tm)
//-----------------------------------------------------------------------------
{
  // if end of save time
  if ((stopTime > 0) && (tm >= stopTime + saveTime))
    return;

  if (pfile == NULL)
  {
    open();
    headerWrite();
  }

  if (tm < nextTime)
    return;

  nextTime += saveTime;

  fprintf(pfile, "%10.7E ", tm);

  for (Indice i = 0; i < list.size(); i++)
  {
    fprintf(pfile, "%10.7E ", list(i)->get());
  }
  fprintf(pfile, "\n");

  fflush(pfile);
}

//-----------------------------------------------------------------------------
void HistoryFile::headerWrite()
//-----------------------------------------------------------------------------
{
  if (pfile == NULL)
    internalFatalError("HistoryFile::headerWrite", "File not open\n");
  fprintf(pfile, "#DynELA_plot history file\n");
  fprintf(pfile, "#plotted : ");
  for (Indice i = 0; i < list.size(); i++)
  {
    fprintf(pfile, "%s ", list(i)->name.chars());
  }
  fprintf(pfile, "\n");
}

//-----------------------------------------------------------------------------
void HistoryFile::setName(String _name)
//-----------------------------------------------------------------------------
{
  fileName = _name;
}

//-----------------------------------------------------------------------------
void HistoryFile::open()
//-----------------------------------------------------------------------------
{
  if (fileName == "")
    fatalError("HistoryFile::open", "No file name specified for object : %s\n", name.chars());
  if (pfile != NULL)
    internalFatalError("HistoryFile::open", "File already open as %s\n", fileName.chars());
  pfile = fopen(fileName.chars(), "w");
}

//-----------------------------------------------------------------------------
void HistoryFile::close()
//-----------------------------------------------------------------------------
{
  fclose(pfile);
  pfile = NULL;
}

//-----------------------------------------------------------------------------
void HistoryFile::append(HistoryFileItem *item)
//-----------------------------------------------------------------------------
{
  list << item;
}

//-----------------------------------------------------------------------------
void HistoryFile::append(NodeSet *ns, String str, Indice comp)
//-----------------------------------------------------------------------------
{
  String nn;
  String ff;
  ff = str;

  HistoryFileAtom *atom = new HistoryFileAtom;
  atom->comp = comp;
  atom->str = str;
  atom->ns = ns;
  historyFileAtom << atom;

  nn.convert(comp);
  if (comp != 0)
  {
    str = str + "_" + nn;
  }

  for (Indice i = 0; i < ns->nodes.size(); i++)
  {
    HistoryFileItem *h = new HistoryFileItem;

    // affect the name
    if (ns->nodes.size() > 1)
    {
      nn.convert(ns->nodes(i)->number);
      nn = str + "_n" + nn;
      h->name = nn;
    }
    else
    {
      h->name = str;
    }

    // affect the node
    h->node = ns->nodes(i);

    // affect the component
    if (comp < 0)
      fatalError("HistoryFileItem::get", "No sense for negative component in object : %s", name.chars());
    h->component = comp;
    h->field = ff;

    append(h);
  }
}

//-----------------------------------------------------------------------------
void HistoryFile::setSaveTime(Real st)
//-----------------------------------------------------------------------------
{
  startTime = st;
  nextTime = st;
  saveTime = st;
  stopTime = 0;
}

//-----------------------------------------------------------------------------
void HistoryFile::setSaveTime(Real sta, Real end, Real st)
//-----------------------------------------------------------------------------
{
  startTime = sta;
  nextTime = sta;
  saveTime = st;
  stopTime = end;
}

//-----------------------------------------------------------------------------
String HistoryFile::getName()
//-----------------------------------------------------------------------------
{
  return fileName;
}

//-----------------------------------------------------------------------------
Real HistoryFile::getSaveTime()
//-----------------------------------------------------------------------------
{
  return saveTime;
}

//-----------------------------------------------------------------------------
Real HistoryFile::getStartTime()
//-----------------------------------------------------------------------------
{
  return startTime;
}

//-----------------------------------------------------------------------------
Real HistoryFile::getStopTime()
//-----------------------------------------------------------------------------
{
  return stopTime;
}

//-----------------------------------------------------------------------------
Indice HistoryFile::getFieldSize()
//-----------------------------------------------------------------------------
{
  return historyFileAtom.size();
}

//-----------------------------------------------------------------------------
NodeSet *HistoryFile::getNodeSet(Indice i)
//-----------------------------------------------------------------------------
{
  return historyFileAtom(i)->ns;
}

//-----------------------------------------------------------------------------
String HistoryFile::getField(Indice i)
//-----------------------------------------------------------------------------
{
  return historyFileAtom(i)->str;
}

//-----------------------------------------------------------------------------
Indice HistoryFile::getComponent(Indice i)
//-----------------------------------------------------------------------------
{
  return historyFileAtom(i)->comp;
}
