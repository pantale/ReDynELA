/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2016                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Const.h
  \brief Declaration of the various constants of the DynELA FEM code.

  This file contains the declarations of all the constants that are used in the DynELA FEM code.
  This is the main configuration file of the DynELA application. This file will be included in all the subsequent files of the DynELA project.
  We define here everything concerning the options of compilation of the code (optimized or not, verifications and asserts, verbosity of the code during execution,...)
  All the configuration parameters are also defined in this file. All basic type of the DynELA code are also defined : \b Real, \b Indice, \b Boolean...

  Briefly speaking this file is THE FILE !
  \ingroup basicTools
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __Const_h__
#define __Const_h__

#include <upBasicTools.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

#define PROGRAM_NAME "DynELA" //!< Generic name of the application : Of course here it's DynELA
#define VERSION 3             //!< Main version number of the DynELA FEM code
#define RELEASE 1             //!< Release version of the DynELA FEM code
#define UNDER_RELEASE 0       //!< Under release version of the DynELA FEM code
#define ALPHA_VERSION 0       //!< Incrementing alpha version of the DynELA FEM code
#define G2TR_STUDY            //!< If this is defined, this option allow to include the specific parts of teh code developped to make it buggy for tutorial sessions about large deformations. Never use it unless you know exactly what you do !

//defined constants
#define maxWarnings 20 //!< Maximum number of warnings allowed before stopping the FEM code
//#define verboseMessages		        //!< Define this parameter if you want verbose analysis of the code. Used during development.

#define localCout cout

// Compilation constant
#define OPTIMIZE     //!< This option activates optimized algorithms of the DynELA code. This also desactivate the following options of the code \ref MEM_funct, \ref VERIF_bounds, \ref VERIF_math, \ref VERIF_alloc and \ref VERIF_assert.
#define MEM_funct    //!< Allow the use of the memcopy function to copy objects (for example in case of vectors copy).
#define VERIF_bounds //!< Verify the boundaries (indices of matrices and vectors). If the indices are out of bounds, this generates an error.
#define VERIF_math   //!< Verify the coherence of mathematical functions such as divisions by zero.
#define VERIF_alloc  //!< Verify the memory allocations during the execution of the program. For example after each call to the new or malloc function.
#define VERIF_assert //!< Activates the mechanism of \b assert() verifications

// Defines used for applications
#ifdef bool
#define Boolean bool
#else
#define Boolean unsigned char //!< Definition of the boolean type if not defined by default. If boolean type is existing, this one is used, if not, an unsigned char is used.
#endif
//#define Short   unsigned char	        //!< Definition of type Short (unsigned char)
//#define int int		        //!< Definition of type int, corresponding to an int
#define Indice long //!< Definition of type Indice (in all applications, we use this type instead of the classic \c int)
#define Real double //!< Definition of type Real (in all applications, we use this type instead of the classic \c float)

#define sourceFileExtension ".dyn"           //!< Definition of the extension associated to the source file of DynELA
#define resultFileExtension ".bin"           //!< Definition of the extension associated to the binary results file of DynELA
#define statusSingleFileExtension ".dyn.sta" //!< Definition of the extension associated to the status result file of DynELA
#define resultAsciiFileExtension ".data"     //!< Definition of the extension associated to the data file of DynELA
#define statusFileExtension ".sta"           //!< Definition of the extension associated to the status file of DynELA
#define logFileExtension ".log"              //!< Definition of the extension associated to the log file of DynELA
#define plotFileExtension ".plot"            //!< Definition of the extension associated to the graphic plots file of DynELA
#define gnuplotFileExtension ".gp"           //!< Definition of the extension associated to the gnuplot command file of DynELA
#define viewFileExtension ".view"            //!< Definition of the extension associated to the visualisation file of DynELA
#define progressFileExtension ".progress"    //!< Definition of the extension associated to the progress file of DynELA
#define pidFileExtension ".pid"              //!< Definition of the extension associated to the pid identification file of DynELA
#define poscriptFileExtension ".ps"          //!< Definition of the extension associated to the postscript file of DynELA
#define configurationFileName ".DynELA"      //!< Definition of the configuration file of DynELA
#define onlinePdfManualName "user.pdf"       //!< Definition of the online pdf manual of DynELA
/*
 *  Definitions of the keywords in the configuration file of DynELA
 */
#define textEditorParam "textEditor"  //!< Keyword associated to the definition of the default text editor in the configuration file of DynELA FEM code.
#define textEditorDefaultValue "kate" //!< Definition of the default text editor in the configuration file of DynELA FEM code.

#define mailUserParam "mailUser" //!< Keyword associated to the definition of the mailbox in the configuration file of DynELA FEM code.
#define mailUserDefaultValue ""  //!< Definition of the mailbox in the configuration file of DynELA FEM code.

#define refreshPeriodGUIParam "refreshPeriodGUI" //!< Keyword associated to the definition of refresh period of the graphical interface in the configuration file of DynELA FEM code.
#define refreshPeriodGUIDefaultValue "10"        //!<Definition of refresh period of the graphical interface in the configuration file of DynELA FEM code.

#define movieDurationGUIParam "movieDurationGUI" //!< Keyword associated to the definition of the duration of the movie in the configuration file of DynELA FEM code.
#define movieDurationGUIDefaultValue "120"       //!< Definition of the duration of the movie in the configuration file of DynELA FEM code.

#define maxNumberPolyForMotionFilledParam "maxNumberPolyForMotionFilled" //!< Keyword associated to the definition of the maximum number of filled polygons when dynamic motion is activated in the configuration file of DynELA FEM code.
#define maxNumberPolyForMotionFilledDefaultValue "10000"                 //!< Definition of the maximum number of filled polygons when dynamic motion is activated in the configuration file of DynELA FEM code.

#define maxNumberPolyForMotionPointParam "maxNumberPolyForMotionPoint" //!< Keyword associated to the definition of the maximum number of polygons when dynamic motion is activated in the configuration file of DynELA FEM code.
#define maxNumberPolyForMotionPointDefaultValue "100000"               //!< Definition of the maximum number of polygons when dynamic motion is activated in the configuration file of DynELA FEM code.

#define materialColorParam "materialColor"            //!< Keyword associated to the definition of the material color in the postprocessor in the configuration file of DynELA FEM code.
#define materialColorDefaultValue "0.36 0.84 1.0 1.0" //!< Definition of the material color in the postprocessor in the configuration file of DynELA FEM code.

#define shininessParam "shininess" //!< Keyword associated to the definition of the shininess in the postprocessor in the configuration file of DynELA FEM code.
#define shininessDefaultValue "50" //!< Definition of the shininess in the postprocessor in the configuration file of DynELA FEM code.

#define lineWidthParam "lineWidth" //!< Keyword associated to the definition of the line width in the postprocessor in the configuration file of DynELA FEM code.
#define lineWidthDefaultValue "2"  //!< Definition of the line width in the postprocessor in the configuration file of DynELA FEM code.

#define axisColorParam "axisColor"          //!< Keyword associated to the definition of the axis color in the postprocessor in the configuration file of DynELA FEM code.
#define axisColorDefaultValue "1.0 1.0 0.0" //!< Definition of the axis color in the postprocessor in the configuration file of DynELA FEM code.

#define displacementsColorParam "displacementsColor"    //!< Keyword associated to the definition of the displacement color in the postprocessor in the configuration file of DynELA FEM code.
#define displacementsColorDefaultValue "0.33 0.66 0.00" //!< Definition of the displacement color in the postprocessor in the configuration file of DynELA FEM code.

#define speedsColorParam "speedsColor"           //!< Keyword associated to the definition of the speed color in the postprocessor in the configuration file of DynELA FEM code.
#define speedsColorDefaultValue "1.00 0.66 0.00" //!< Definition of the speed color in the postprocessor in the configuration file of DynELA FEM code.

#define forcesColorParam "forcesColor"           //!< Keyword associated to the definition of the forces color in the postprocessor in the configuration file of DynELA FEM code.
#define forcesColorDefaultValue "1.00 0.33 0.00" //!< Definition of the forces color in the postprocessor in the configuration file of DynELA FEM code.

#define groupsColorParam "groupsColor"        //!< Keyword associated to the definition of the groups color in the postprocessor in the configuration file of DynELA FEM code.
#define groupsColorDefaultValue "0.0 1.0 0.0" //!< Definition of the groups color in the postprocessor in the configuration file of DynELA FEM code.

#define normalsColorParam "normalsColor"       //!< Keyword associated to the definition of the normals color in the postprocessor in the configuration file of DynELA FEM code.
#define normalsColorDefaultValue "0.0 0.0 0.1" //!< Definition of the normals color in the postprocessor in the configuration file of DynELA FEM code.

#define nodesNumberColorParam "nodesNumberColor"      //!< Keyword associated to the definition of the nodes color in the postprocessor in the configuration file of DynELA FEM code.
#define nodesNumberColorDefaultValue "1.00 1.00 0.50" //!< Definition of the nodes color in the postprocessor in the configuration file of DynELA FEM code.

#define elementsNumberColorParam "elementsNumberColor"   //!< Keyword associated to the definition of the elements color in the postprocessor in the configuration file of DynELA FEM code.
#define elementsNumberColorDefaultValue "0.33 1.00 1.00" //!< Definition of the elements color in the postprocessor in the configuration file of DynELA FEM code.

#define videoImageInverseParam "videoImageInverse" //!< Keyword associated to the definition of the video inverse in the postprocessor in the configuration file of DynELA FEM code.
#define videoImageInverseDefaultValue "0"          //!< Definition of the video inverse in the postprocessor in the configuration file of DynELA FEM code.

#define colorsImageSwapParam "colorsImageSwap"                  //!< Keyword associated to the definition of the color swap in the postprocessor in the configuration file of DynELA FEM code.
#define colorsImageSwapDefaultValue "0 0.0 0.0 0.0 1.0 1.0 1.0" //!< Definition of the color swap in the postprocessor in the configuration file of DynELA FEM code.

#define clipPlaneColorParam "clipPlaneColor"     //!< Keyword associated to the definition of the clip-plane color in the postprocessor in the configuration file of DynELA FEM code.
#define clipPlaneColorDefaultValue "0.0 1.0 1.0" //!< Definition of the clip-plane color in the postprocessor in the configuration file of DynELA FEM code.

#define meshDisplayParam "meshDisplay" //!< Keyword associated to the definition of the mesh display in the postprocessor in the configuration file of DynELA FEM code.
#define meshDisplayDefaultValue "1"    //!< Definition of the mesh display in the postprocessor in the configuration file of DynELA FEM code.

#define startupDisplayParam "startupDisplay" //!< Keyword associated to the definition of the splash screen display in the postprocessor in the configuration file of DynELA FEM code.
#define startupDisplayDefaultValue "3"       //!< Definition of the splash screen display in the postprocessor in the configuration file of DynELA FEM code.

#define currentWindowStyleParam "windowStyle" //!< Keyword associated to the definition of the windows style in the postprocessor in the configuration file of DynELA FEM code.
#define currentWindowStyleDefaultValue "2"    //!< Definition of the windows style in the postprocessor in the configuration file of DynELA FEM code.

#define imageFormatParam "imageFormat" //!< Keyword associated to the definition of the image format in the postprocessor in the configuration file of DynELA FEM code.
#define imageFormatDefaultValue "1"    //!< Definition of the image format in the postprocessor in the configuration file of DynELA FEM code.

#define psViewerParam "psViewer"  //!< Keyword associated to the definition of the postscript viewer in the configuration file of DynELA FEM code.
#define psViewerDefaultValue "gv" //!< Definition of the postscript viewer in the configuration file of DynELA FEM code.

#define pdfViewerParam "pdfViewer"       //!< Keyword associated to the definition of the pdf viewer in the configuration file of DynELA FEM code.
#define pdfViewerDefaultValue "acroread" //!< Definition of the pdf viewer in the configuration file of DynELA FEM code.

#define postscriptDisplayParam "postscriptDisplay" //!< Keyword associated to the definition of the postscript auto display in the configuration file of DynELA FEM code.
#define postscriptDisplayDefaultValue "1"          //!< Definition of the postscript auto display in the configuration file of DynELA FEM code.

#define manualsPathParam "manualsPath"                      //!< Keyword associated to the definition of the manuals path in the configuration file of DynELA FEM code.
#define manualsPathDefaultValue "/usr/share/DynELA/manuals" //!< Definition of the manuals path in the configuration file of DynELA FEM code.

#define binariesPathParam "binariesPath"                 //!< Keyword associated to the definition of the binaries path in the configuration file of DynELA FEM code.
#define binariesPathDefaultValue "/usr/share/DynELA/bin" //!< Definition of the binaries path in the configuration file of DynELA FEM code.

#define samplesPathParam "samplesPath"                      //!< Keyword associated to the definition of the samples path in the configuration file of DynELA FEM code.
#define samplesPathDefaultValue "/usr/share/DynELA/samples" //!< Definition of the samples path in the configuration file of DynELA FEM code.

#define defaultFontForGUIParam "defaultFontForGUI"                     //!< Keyword associated to the definition of the default font in the configuration file of DynELA FEM code.
#define defaultFontForGUIDefaultValue "Helvetica,12,-1,5,50,0,0,0,0,0" //!< Definition of the default font in the configuration file of DynELA FEM code.

/*!
  \enum Format_type
  \brief Definitions of the formats associated to files

  Here we can use two kinds of files: \c Binary and \c Ascii files.
*/
enum Format_type
{
  Ascii, //!< ASCII file type
  Binary //!< Binary file type
};

/*!
  \enum Action_io
  \brief Definition of the actions associated to files

  Here we can have three actions on a file : \c Write mode, \c Read mode et \c Null mode.
*/
enum Action_io
{
  None, //!< \c mode Null : nothing is done or the action is unknown
  Read, //!< \c mode Read : read mode for a file
  Write //!< \c mode Write : write mode for a file
};

// turn off all verifs if OPTIMIZE is on
#ifdef OPTIMIZE

// undef verif bounds
#ifdef VERIF_bounds
#undef VERIF_bounds
#endif

// undef verif allocations
#ifdef VERIF_alloc
#undef VERIF_alloc
#endif

// undef verif maths
#ifdef VERIF_math
#undef VERIF_math
#endif

// undef verif asserts
#ifdef VERIF_assert
#undef VERIF_assert
#endif

#endif

// Le define qui permet de mettre des coucou et de les virer ensuite facilement
#define COUT cout

#endif
