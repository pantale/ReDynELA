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

// begin date : 14/03/1997

/*
  Class Viscosity definition
*/

#ifndef __Viscosity_h__
#define __Viscosity_h__

#include <upFemSolver.h>

class ElementSet;

/*!
  \file Viscosity.h
  \brief fichier .h de d�finition de la classe g�nerique des mat�riaux
  \ingroup femLibrary

  Ce fichier sert � la d�finition la classe g�nerique des mat�riaux.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

#define lawName_Viscosity "generic viscosity"
#define Name_Viscosity "Viscosity"

class IntegrationPoint;
class Element;

/*!
  \brief Classe virtuelle de gestion des mat�riaux
  \ingroup femLibrary

  Classe virtuelle de gestion des mat�riaux pour DynELA. Cette classe regroupe les param�tres de base communs � tous les types de mat�riaux sur DynELA, comme le module de Young, le coefficient de Poisson, ou la densit� d'un mat�riau. Pour pouvoir utiliser un mat�riau sur DynELA, on utilise alors une des classes d�riv�es de celle-ci, ce qui permet alors de d�finir un comportement �lastique, �lasto-plastique, ou tout autre type de comportement.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
/** @dia:pos 4,4 */
class Viscosity
{
public:
  enum
  {
    Virtual = 0,
    Perzina
  };

public:
  /** @dia:route 0,2;h,-120.6,-172.85,78,-67.4794,-133.165 */
  String name;

public:
  Viscosity();
  Viscosity(const Viscosity &X);
  virtual ~Viscosity();

  //  void plotToFile(String name, Real epsMax = 5);

  /* fonctions virtuelles */
  virtual Indice getType() = 0;
  virtual const char *lawName() { return lawName_Viscosity; }
  virtual const char *Name() { return Name_Viscosity; }
  virtual Indice getNumberOfConstitutiveParameters() = 0;
  virtual const char *getConstitutiveParameterName(Indice ind) = 0;
  virtual Real &getConstitutiveParameter(Indice ind) = 0;
  virtual Real getViscousStress(Element *, Real shift = 0.0) = 0;
  //  virtual Boolean isYield () = 0;
  //  virtual void plot(FILE* pfile, Real epsMax = 10) = 0;
  /*   Boolean isLinear () */
  /*   { */
  /*     return is_Linear; */
  /*   } */
  virtual void print(ostream &os) const = 0;
  //  virtual void toFile (FILE * pfile) = 0;

  /*   void compute_C (); */
  /*   void checkValues (); */
  /*   Matrix getHookeMatrix(short type); */
  virtual String convertToDynELASourceFile(String name);

  /* fonctions entree sortie */
  /*   friend ostream & operator << (ostream & os, Viscosity & mat); */
  /*   virtual void toFile (FILE * pfile) = 0; */
};

#endif
