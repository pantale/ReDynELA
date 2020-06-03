/***************************************************/
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
  Class ContactLaw definition
*/

#ifndef __ContactLaw_h__
#define __ContactLaw_h__

#include <upFemSolver.h>

enum
{
  coulombContactLaw
};

/*<ContactLaw:*/
/** @dia:pos 294.8,42.8 */
class ContactLaw
{

public:
  // constructeurs
  ContactLaw();
  // ContactLaw (const ContactLaw& X);
  virtual ~ContactLaw();

  // fonctions membres
  virtual int Type() = 0;
  virtual String Name() = 0;
  virtual void toFile(FILE *pfile) = 0;
};
/*>ContactLaw:*/

#endif
