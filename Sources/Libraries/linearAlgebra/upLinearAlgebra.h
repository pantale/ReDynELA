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

// begin date : 10/09/1997
// revision date :

#ifndef __upLinearAlgebra_h__
#define __upLinearAlgebra_h__

/*!
  \file upLinearAlgebra.h
  \brief fichier .h de d�finition des includes de base
  \ingroup linearAlgebra

  Ce fichier regroupe l'ensemble des includes de base pour la librairie linearAlgebra de DynELA. C'est le premier des fichiers de d�claration de la librairie linearAlgebra, inclu par tous les autres fichiers de d�claration de DynELA dans la librairie linearAlgebra.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <basicTools.h>

#define Ind(i, j, li, co) ((j) * (li) + (i))

#endif
