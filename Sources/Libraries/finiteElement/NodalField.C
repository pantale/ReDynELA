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
	Class NodalField definition
*/

#include <NodalField.h>

//-----------------------------------------------------------------------------
NodalField::NodalField()
//-----------------------------------------------------------------------------
{
  // initialisations
  Clear();
}

//-----------------------------------------------------------------------------
NodalField::NodalField(const NodalField &X)
//-----------------------------------------------------------------------------
{
  cout << "Copie de NodalField" << X.fe << endl;
}

//-----------------------------------------------------------------------------
NodalField::~NodalField()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void NodalField::Clear()
//-----------------------------------------------------------------------------
{
  // initialisations
  ro = 0.;
  dro = 0.;
  //    new_ro=0.; new_dro=0.;
  e = 0.;
  de = 0.;
  //new_e=0.;
  //    de=0.; new_de=0.;
  dmat_speed = 0.;
  mat_speed = 0.;
  //  grid_speed=0.;
  fe = 0.;
  T = 0.;
  // T0=0.;
  delta_disp = 0.;
  disp = 0.;
}

//-----------------------------------------------------------------------------
void NodalField::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "ro=" << ro << ", e=" << e << ", T=" << T << endl;
  os << "acc=" << dmat_speed << endl;
  os << "delta_depl=" << delta_disp << endl;
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, NodalField &field)
//-----------------------------------------------------------------------------
{
  field.print(os);
  return os;
}

//-----------------------------------------------------------------------------
ofstream &operator<<(ofstream &os, const NodalField &intPt)
//-----------------------------------------------------------------------------
{
  intPt.write(os);
  return os;
}

//-----------------------------------------------------------------------------
ifstream &operator>>(ifstream &is, NodalField &intPt)
//-----------------------------------------------------------------------------
{
  intPt.read(is);
  return is;
}

//-----------------------------------------------------------------------------
void NodalField::write(ofstream &pfile) const
//-----------------------------------------------------------------------------
{
  pfile.write((char *)&ro, sizeof(Real));
  pfile.write((char *)&dro, sizeof(Real));
  pfile.write((char *)&e, sizeof(Real));
  pfile.write((char *)&de, sizeof(Real));
  pfile.write((char *)&T, sizeof(Real));

  pfile << mat_speed;
  pfile << dmat_speed;
  //  pfile << grid_speed;
  pfile << fe;
  pfile << delta_disp;
  pfile << disp;
}

//-----------------------------------------------------------------------------
NodalField &NodalField::read(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  pfile.read((char *)&ro, sizeof(Real));
  pfile.read((char *)&dro, sizeof(Real));
  pfile.read((char *)&e, sizeof(Real));
  pfile.read((char *)&de, sizeof(Real));
  pfile.read((char *)&T, sizeof(Real));

  pfile >> mat_speed;
  pfile >> dmat_speed;
  //  pfile >> grid_speed;
  pfile >> fe;
  pfile >> delta_disp;
  pfile >> disp;

  return *this;
}

//-----------------------------------------------------------------------------
Indice
NodalField::objectSize()
//-----------------------------------------------------------------------------
{
  Indice sz;
  sz = sizeof(*this);
  return sz;
}
