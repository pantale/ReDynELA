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

/*!
  \file Configuration.C
  \brief Definition of the configuration file classes used to store informations about the DynELA FEM code on user disk.
  
  This file contains the definitions of both the configurationItem and configurationList classes.
  \ingroup basicTools
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#include <Const.h>
#include <Boolean.h>
#include <Configuration.h>
#include <Error.h>
#include <iostream>

//!configurationList class default constructor
/*!
  This constructor downloads automaticaly the configuration fiole of the DynELA Finite Element Code from the home directory.
  The name of the configuration file is defined through the variable \ref configurationFileName defined in the \ref Const.h file.
  If the configuration file doen't exist or if it's not readable, defaults values defined in the \ref Const.h files are initialized.
*/
//-----------------------------------------------------------------------------
configurationList::configurationList()
//-----------------------------------------------------------------------------
{
    // Test the presence of the Home variable
    if (getenv("HOME") == NULL)
    {
        printf("Unable to find environment variable $HOME\nNo preferences file read\n");
        return;
    }

    // construct the filename of the configuration file
    filename = getenv("HOME");
    filename += "/" configurationFileName;

    // read the configuration file
    readConfiguration();
}

//!configurationList class destructor
/*!
  Nothing has to be done here.
*/
//-----------------------------------------------------------------------------
configurationList::~configurationList()
//-----------------------------------------------------------------------------
{
}

//!Download of the configuration file
/*!
  This method reads the configuration file and fill the list of configuration parameters.
  This method is usually never directly called by the operator as it's impliciely called by the constructor.
  Therefore, when the class is instanciated, this method is automatically called and the parameters are filled from the parameter file.
  \return Boolean value Success if configuration file has been read or Failed if configuration file has not been read or error occured.
*/
//-----------------------------------------------------------------------------
Boolean
configurationList::readConfiguration()
//-----------------------------------------------------------------------------
{
    FILE *pfile;
    String scanline;

    // flush the old list of configuration items
    items.flush();

    // print message if verbose is on
#ifdef verboseMessages
    printf("reading preference file\n");
#endif
    pfile = fopen(filename.chars(), "r");
    if (pfile == NULL)
    {
#ifdef verboseMessages
        printf("No preferences file read (using default values)\n");
#endif
        return Fail;
    }

    // read a string line
    scanline.scanFileLine(pfile);

    // scan if the file is up to date (ie, good version or not)
    if (scanline != magicConfigurationString)
    {
        fclose(pfile);
        cerr << "Reading preferences file\nCorrupted file or too old preferences file format\ndefault values will be used\n";
        return Fail;
    }

    // scan a line
    while (scanline.scanFileLine(pfile))
    {
        configurationItem *item = new configurationItem();
        item->parameter = scanline.before(" ");
        item->value = scanline.after(" ");
        items << item;
    }

    // close the file
    fclose(pfile);

    // return Success, all ok, parameters loaded
    return (Success);
}

//!Save all parameters into the configuration file
/*!
  This method writes the configuration file with the list of the configuration parameters.
  \return Boolean value Success if configuration file has been written or Failed if configuration file has not been written or error occured.
*/
//-----------------------------------------------------------------------------
Boolean
configurationList::writeConfiguration()
//-----------------------------------------------------------------------------
{
    Indice i;
    configurationItem *item;
    FILE *pfile;
    String scanline;

#ifdef verboseMessages
    printf("writing preference file\n");
#endif
    pfile = fopen(filename.chars(), "w");
    if (pfile == NULL)
    {
        warning("configurationList::writeConfiguration",
                "No preferences file write\nthere's a problem somewhere\n");
        return Fail;
    }

    // print the magic line
    fprintf(pfile, magicConfigurationString "\n");

    // store all parameters
    for (i = 0; i < items.size(); i++)
    {
        item = items(i);
        fprintf(pfile, "%s %s\n", item->parameter.chars(),
                item->value.chars());
    }

    // close the configuration file
    fclose(pfile);

    // return Success, all ok
    return (Success);
}

//!Returns the value associated to a parameter
/*!
  This method returns the value associated to a parameter. The returned value is a String.
  If no value is associated to the given parameter, or if this parameter is not defined, and empty String ("") is returned.
  \param param Name of the parameter we want to recover the associated value
  \return String defining the associated value of the given parameter
*/
//-----------------------------------------------------------------------------
String
configurationList::getParameter(String param)
//-----------------------------------------------------------------------------
{
    Indice i;
    configurationItem *item;

    for (i = 0; i < items.size(); i++)
    {
        item = items(i);
        if (item->parameter == param)
        {
            return (item->value);
        }
    }
    return "";
}

//!Associates a value to a parameter
/*!
  This method store a value for a given parameter in the configuration list. All values are of String type.
  If the parameter is not present, a new parameter is created, if it's already present, the associated value is changed to the new one.
  As this is a volatile list in memory, the use must call the writeConfiguration() method to store the list of parameters in the configuration file of teh DynELA Finite Element Code.
  \param param Name of the parameter to create/modify
  \param val Associated value of the parameter to create/modify
*/
//-----------------------------------------------------------------------------
void configurationList::setParameter(String param, String val)
//-----------------------------------------------------------------------------
{
    Indice i;
    configurationItem *item;

    for (i = 0; i < items.size(); i++)
    {

        // get the item
        item = items(i);

        // if this parameter is already in the list
        if (item->parameter == param)
        {

            // change the associated value
            item->value = val;

            // return
            return;
        }
    }

    // creates a new parameter and store it
    item = new configurationItem();
    item->parameter = param;
    item->value = val;
    items << item;
}
