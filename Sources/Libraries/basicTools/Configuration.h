/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2016                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Configuration.h
  \brief Declaration of the configuration file classes used to store informations about the DynELA FEM code on user disk.

  This file contains the declarations of both the configurationItem and configurationList classes.
  \ingroup basicTools
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/

#ifndef __Configuration_h__
#define __Configuration_h__

#include <Const.h>
#include <List.h>

#define magicConfigurationString "#DynELA configuration file v. 2.0"                                                                  //!< Defines the magic header of the configuration file
#define getConfParameter(CONF, PAR) (CONF.getParameter(PAR##Param) == "" ? String(PAR##DefaultValue) : CONF.getParameter(PAR##Param)) //!< Returns the value (a String) of the stored parameter in the configuration file or the default value if not present in the configuration file.
#define getpConfParameter(CONF, PAR) (CONF->getParameter(PAR##Param) == "" ? PAR##DefaultValue : CONF->getParameter(PAR##Param))      //!< Returns the value of the stored parameter in the configuration file or the default value if not present in the configuration file.
#define setConfParameter(CONF, PAR, VAL) (CONF.setParameter(PAR##Param, VAL))                                                         //!< Set the value of a parameter (a String) to be stored in the configuration file.
#define setpConfParameter(CONF, PAR, VAL) (CONF->setParameter(PAR##Param, VAL))                                                       //!< Set the value of a parameter to be stored in the configuration file.

/*!
  \class configurationItem
  \brief Private class to manage the items of a configuration file.

  No need to access members and operators of this class.
  \ingroup basicTools
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/
class configurationItem
{
private:
  friend class configurationList;
  friend class List<configurationItem *>;
  String parameter;
  String value;

  // constructors and destructor
  configurationItem(){};
  ~configurationItem(){};
};

/*!
  \class configurationList Configuration.h
  \brief Class to manage configurations parameters from/to the DynELA configuration file.

  This class is used to store all the data concerning the configuration file of the DynELA finite element code. 
  This class automatically load the configuration parameters from the configuration file defined by the reference name \ref configurationFileName.
  Parameters are obtained through the method \ref getParameter() and are stored through the method \ref setParameter().
  Modified parameters are then stored on the disk with the \ref writeConfiguration() method.

  Example:
  This example illustrates the use of the configurationList class
  \code
  configurationList configuration;
  String str=getConfParameter(configuration,manualsPath); // We get th value associated to the parameter "manualsPath"
  str="/"; // We change the value of the parameter
  setConfParameter(configuration,manualsPath,str); // We store the new value into the database
  configuration.writeConfiguration(); // And write the database on the disk
  \endcode
  \ingroup basicTools
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2016
*/
class configurationList
{
private:
  List<configurationItem *> items;
  String filename;

public:
  configurationList();
  ~configurationList();
  Boolean writeConfiguration();
  Boolean readConfiguration();
  String getParameter(String param);
  void setParameter(String param, String val);
};

#endif
