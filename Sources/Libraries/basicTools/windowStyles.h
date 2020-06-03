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

// begin date : 04/03/1997
// revision date : 17/07/1997

/*
  Const definitions of DynELA
*/

/*!
  \file windowStyles.h
  \brief fichier .h de d�finition des style graphiques associ�s � QT
  \ingroup basicTools

  Ce fichier regroupe les d�finitions li�es au style des fen�tres graphiques sur DynELA conform�ment � l'environnement graphique QT.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
  \date 1997-2004
*/

#ifndef __WindowStyles_h__
#define __WindowStyles_h__

#include <basicTools.h>
#include <QtGui>
#include <QStyleFactory>

enum
{
  style_CDE = 0,    // CDE look and feel
  style_Cleanlooks, // Widget style similar to the Clearlooks style available in GNOME
  style_Gtk,        // Widget style rendered by GTK+
  style_Motif,      // Motif look and feel
  style_Plastique,  // Widget style similar to the Plastik style available in KDE
  style_Windows,    // Microsoft Windows-like look and feel
};

/*!
  \brief applique le style courant � l'application graphique

  Cette fonction applique le style courant � l'application graphique dans l'environnement graphique QT. Voir l'exemple ci-dessous pour l'illustration de l'utilisation de cette fonction.

  Exemple :
  \code
  // dans le constructeur de la fenetre principale de l'application on �crit les deux lignes suivantes:
  configurationList configuration; // charge et initialise les param�tres de configuration
  applyWindowStyle(configuration); // applique le style graphique � la fenetre en cours
  \endcode
  \param configuration liste des variables de configuration qui contient les param�tres graphiques du style � utiliser. voir classe \ref configurationList
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
inline void
applyWindowStyle(Indice parameter)
{
  // apply the style
  switch (parameter)
  {
  case style_CDE: // CDE look and feel
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    break;
  case style_Cleanlooks: //Widget style similar to the Clearlooks style available in GNOME
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    break;
  case style_Gtk: //Widget style rendered by GTK+
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    break;
  case style_Motif: //Motif look and feel
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    break;
  case style_Plastique: //Widget style similar to the Plastik style available in KDE
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    break;
  case style_Windows: //Microsoft Windows-like look and feel
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    break;
  default:
    cout << "Unknown style\n";
  }
}

inline void
applyWindowStyle(configurationList configuration)
{
  // select the default font for the application
  String fontDisplay = getConfParameter(configuration, defaultFontForGUI);
  QFont dispFont;
  dispFont.fromString(fontDisplay.chars());
  QApplication::setFont(dispFont);

  // apply the style
  applyWindowStyle(getConfParameter(configuration, currentWindowStyle).getInt());
}

#endif
