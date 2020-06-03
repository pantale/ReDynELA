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

// begin date : 20/03/1997

/*
  Class IntegrationPoint definition
*/

#include <IntegrationPoint.h>

//-----------------------------------------------------------------------------
IntegrationPointBase::IntegrationPointBase(Indice dim, Indice nonodes)
//-----------------------------------------------------------------------------
{
  // fonctions d'interpolation
  //  N.redim (nonodes);

  // derivees des fonctions d'interpolation
  dN.redim(nonodes, dim);

  // Jacobien et inverse
  JxW.redim(dim, dim);
  InvJxW.redim(dim, dim);

  R.Identity();
}

//-----------------------------------------------------------------------------
IntegrationPointBase::~IntegrationPointBase()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void IntegrationPointBase::baseFlush()
//-----------------------------------------------------------------------------
{
  // fonctions d'interpolation
  //  N = 0.;
  dN = 0.;

  // Jacobien et inverse
  JxW = 0.;
  InvJxW = 0.;
  detJ = 0.;
  detJ0 = 0.;

  // poids
  //  weight = 0.;
  //  coords = 0.;
}

//-----------------------------------------------------------------------------
UnderIntegrationPoint::UnderIntegrationPoint(Indice dim, Indice nonodes) : IntegrationPointBase(dim, nonodes)
//-----------------------------------------------------------------------------
{
  // initialisations
  flush();
}

//-----------------------------------------------------------------------------
UnderIntegrationPoint::~UnderIntegrationPoint()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void UnderIntegrationPoint::flush()
//-----------------------------------------------------------------------------
{
  // initialisations
}

//-----------------------------------------------------------------------------
IntegrationPoint::IntegrationPoint(Indice dim, Indice nonodes) : IntegrationPointBase(dim, nonodes)
//-----------------------------------------------------------------------------
{
  // initialisations
  flush();
}

//-----------------------------------------------------------------------------
IntegrationPoint::~IntegrationPoint()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void IntegrationPoint::flush()
//-----------------------------------------------------------------------------
{
  // initialisations
  S = 0.;
  //  dS = 0.;
  p = 0.;
  //  dp = 0.;
  Sig = 0.;
  Eps = 0.;
  dEps = 0.;
  //  EpsEqv = 0.;
  evp = 0.;
  EpsPlas = 0.;
  evp = 1e-10;
  //  evp = 0.0;
}

//-----------------------------------------------------------------------------
void IntegrationPoint::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "p=" << p << ", S=" << S << endl;
  //  os << "dp=" << dp << ", dS=" << dS << endl;
  os << "Sig=" << Sig << endl;
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, IntegrationPoint &field)
//-----------------------------------------------------------------------------
{
  field.print(os);
  return os;
}

//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const IntegrationPoint &intPt)
//-----------------------------------------------------------------------------
{
  intPt.write(os);
  return os;
}

//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, IntegrationPoint &intPt)
//-----------------------------------------------------------------------------
{
  intPt.read(is);
  return is;
}

//-----------------------------------------------------------------------------
void IntegrationPoint::write(ofstream &pfile) const
//-----------------------------------------------------------------------------
{
  pfile.write((char *)&p, sizeof(Real));
  //  pfile.write ((char *) &dp, sizeof (Real));
  pfile.write((char *)&evp, sizeof(Real));

  pfile << S;
  pfile << Sig;

  pfile << Eps;
  pfile << dEps;
  pfile << EpsPlas;
}

//-----------------------------------------------------------------------------
IntegrationPoint &IntegrationPoint::read(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  pfile.read((char *)&p, sizeof(Real));
  //  pfile.read ((char *) &dp, sizeof (Real));
  pfile.read((char *)&evp, sizeof(Real));

  pfile >> S;
  pfile >> Sig;

  pfile >> Eps;
  pfile >> dEps;
  pfile >> EpsPlas;

  return *this;
}
