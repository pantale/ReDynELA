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

// begin date : 22/05/1997

/*
  Class Interface definition
*/

#include <Interface.h>
#include <Side.h>
#include <ContactLaw.h>

//-----------------------------------------------------------------------------
Interface::Interface(Indice _Id)
//-----------------------------------------------------------------------------
{
  // numero de l'interface
  Id = _Id;

  master = NULL;
  slave = NULL;
}

//-----------------------------------------------------------------------------
Interface::Interface(const Interface &X)
//-----------------------------------------------------------------------------
{
  cerr << "erreur Interface copy not implemented" << X.Id << endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
Interface::~Interface()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Boolean
Interface::setMaster(Side *pside)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(master == NULL);
#endif

  // initer le side
  master = pside;

  // switch du flag de master
  master->isMaster() = True;

  master->interface = this;

  // boolean Success
  return Success;
}

//-----------------------------------------------------------------------------
Boolean
Interface::setSlave(Side *pside)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(slave == NULL);
#endif

  // attacher le side
  slave = pside;

  // switch du flag de master
  slave->isMaster() = False;

  slave->interface = this;

  // boolean Success
  return Success;
}

//-----------------------------------------------------------------------------
void Interface::Init()
//-----------------------------------------------------------------------------
{
  slave->Init();
  master->Init();
}

//-----------------------------------------------------------------------------
void Interface::ScanIncludeNodes()
//-----------------------------------------------------------------------------
{
  //  Indice total;

  // initialisation
  //  total=0;

  // initialisations et calculs sur master et slave
  master->computeScanVolume();
  slave->computeScanVolume();

  // calcul des normales
  master->computeSideFacesNormals();
  slave->computeSideFacesNormals();

  // scan des inclusions
  //total+=master->scanIncludeNodes(slave);
  master->scanIncludeNodes(slave);
  //  total+=slave->scanIncludeNodes(master);
  //  slave->scanIncludeNodes(master);

  //  cout << total << " new penetrating nodes\n";
  // if (total!=0) exit (0);
}

/*//-----------------------------------------------------------------------------
void Interface::computeSpeeds(Real timeStep)
//-----------------------------------------------------------------------------
{
  // calcul des contacts
  slave->computeSpeeds(timeStep);
}*/

//-----------------------------------------------------------------------------
void Interface::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  // sortie de la loi de contact
  contactLaw->toFile(pfile);

  fprintf(pfile, "\nMASTER SURFACE DEFINITION\n");
  fprintf(pfile, "ELEMENT     NODES ON SURFACE \n");
  master->toFile(pfile);

  fprintf(pfile, "\nSLAVE SURFACE DEFINITION\n");
  fprintf(pfile, "ELEMENT     NODES ON SURFACE \n");
  slave->toFile(pfile);
}

//-----------------------------------------------------------------------------
Boolean
Interface::check()
//-----------------------------------------------------------------------------
{
  if (master->check() == False)
    return False;
  if (slave->check() == False)
    return False;
  return True;
}

//-----------------------------------------------------------------------------
void Interface::setContactLaw(ContactLaw *law)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(law != NULL);
#endif
  contactLaw = law;
}
