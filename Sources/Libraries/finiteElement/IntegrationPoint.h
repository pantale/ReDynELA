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

// begin date : 20/03/1997

/*
  Class IntegrationPoint definition
*/

#ifndef __IntegrationPoint_h__
#define __IntegrationPoint_h__

#include <upFemSolver.h>

struct ElementIntegrationPoint;

/**Classe de gestion des donn�es aux points d'int�gration.
Cette classe sert � d�finir les donn�es scalaires et tensorielles que l'on a au niveau de chaque point d'int�gration d'un �l�ment. Cette classe ne comporte pas de m�canismes de gestion ou de calcul, elle ne sert que de stockage de donn�es (la classe "Element" et ses classes d�riv�es dont elle d�pend g�rent tous les calculs relatifs au domaine d'int�gration).
see Element
version DynELA v.0.9.0
author Olivier PANTALE
*/
class IntegrationPointBase
{

public:
  Tensor2 R;     //!< Rotation au point d'integration
  Matrix JxW;    //!< Jacobien de l'�l�ment au point d'int�gration
  Matrix InvJxW; //!< Inverse du Jacobien de l'�l�ment au point d'int�gration
  Real detJ;     //!< Determinant du Jacobien
  Real detJ0;    //!< Determinant du Jacobien de la configuration de r�f�rence
  Matrix dN;     //!< Matrice des d�riv�es des fonctions d'interpolation par rapport aux coordonn�es physiques
  const ElementIntegrationPoint *data;

public:
  IntegrationPointBase(Indice dim, Indice nodes);
  virtual ~IntegrationPointBase();
  virtual void flush() = 0;
  void baseFlush();
};

class UnderIntegrationPoint : public IntegrationPointBase
{

public:
  UnderIntegrationPoint(Indice dim, Indice nodes);
  ~UnderIntegrationPoint();
  void flush();
};

class IntegrationPoint : public IntegrationPointBase
{

public:
  Tensor2 S;        //!< Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$
  Real p;           //!< partie sph�rique du tenseur des contraintes
  Tensor2 Sig;      //!< tenseur des contraintes
  Tensor2 Eps;      //!< tenseur des d�formations
  Tensor2 dEps;     //!< incr�ment du tenseur des d�formations
  Tensor2 EpsPlas;  //!< tenseur des d�formations plastiques
  Tensor2 dEpsPlas; //!< incr�ment du tenseur des d�formations plastiques
  Real evp;         //!< parametre d'�crouissage

public:
  IntegrationPoint(Indice dim, Indice nodes);
  ~IntegrationPoint();

  void flush();

  // gestion des flux entree et sortie
  friend ostream &operator<<(ostream &os, IntegrationPoint &field);
  friend ofstream &operator<<(ofstream &, const IntegrationPoint &);
  friend ifstream &operator>>(ifstream &, IntegrationPoint &);
  void print(ostream &os) const;
  void write(ofstream &pfile) const;
  IntegrationPoint &read(ifstream &pfile);
};

#endif
