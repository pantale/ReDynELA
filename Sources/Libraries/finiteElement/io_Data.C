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

// begin date : 19/05/1998

/*
	Class io_Data definition
*/

#include <Structure.h>
#include <Domain.h>
#include <io_Data.h>

#define VECTORadr(val) &(val(0)), &(val(1)), &(val(2))
#define VECTOR(val) (val(0)), (val(1)), (val(2))
#define TENSORadr(val) &(val(0, 0)), &(val(0, 1)), &(val(0, 2)), &(val(1, 0)), &(val(1, 1)), &(val(1, 2)), &(val(2, 0)), &(val(2, 1)), &(val(2, 2))
#define TENSOR(val) (val(0, 0)), (val(0, 1)), (val(0, 2)), (val(1, 0)), (val(1, 1)), (val(1, 2)), (val(2, 0)), (val(2, 1)), (val(2, 2))

//-----------------------------------------------------------------------------
io_Data::io_Data()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
io_Data::~io_Data()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void io_Data::nodesRead(FILE *pfile)
//-----------------------------------------------------------------------------
{
  /*  ListId < Node * >lnd;
  Indice number;
  Vec3D x;
  char str[20];

  lnd = pdomain->nodes;

  fscanf (pfile, "%s %ld", str, &number);

  for (Indice i = 0; i < number; i++)
    {
      fscanf (pfile, "%lf,%lf,%lf", &x (0), &x (1), &x (2));
      lnd (i)->coords = x;
    }*/
}

//-----------------------------------------------------------------------------
void io_Data::nodesWrite(FILE *pfile)
//-----------------------------------------------------------------------------
{
  /*  Node *pnd;
  ListId < Node * >lnd;

  lnd = pdomain->nodes;

  fprintf (pfile, "COORDINATES %ld\n", lnd.size ());

  for (Indice i = 0; i < lnd.size (); i++)
    {
      pnd = lnd (i);
      fprintf (pfile, "%.6E,%.6E,%.6E\n", pnd->coords (0), pnd->coords (1),
	       pnd->coords (2));
    }*/
}

//-----------------------------------------------------------------------------
void io_Data::elements_read(FILE *pfile)
//-----------------------------------------------------------------------------
{
  /*
//  Node* pnd;
  ListId < Node * >*plnd_dom;
  ListId < Element * >*plel_dom;
  Indice num, mat;
  int rep, TheEnd = 0;
  Indice line = 1;
  int taille;
  Indice n[10];
  Indice i;
  Element *pel;

  plnd_dom = &(pdomain->nodes);
  plel_dom = &(pdomain->elements);

  fscanf (pfile, "%d", &taille);
  line++;
  switch (taille)
    {
    case 4:
      break;
    default:
      fatalError ("void io_Data::elements_read(FILE* pfile)",
		  "ne sait pas lire les structures autres que 4 noeuds");
    }

  while (TheEnd == 0)
    {

      switch (taille)
	{
	case 4:
	  rep = fscanf (pfile, "%ld,%ld,%ld,%ld,%ld,%ld",
			&num, &mat, &n[0], &n[1], &n[2], &n[3]);
	  if (rep != -1)
	    {

	      // creer un nouveau element s'il n'existe pas
	      if ((pel = plel_dom->AppN (num)) == NULL)
		{
		  pel = new ElQua4n2D (num);
		  //domain->Add(pel);
		  pdomain->elements << pel;
		}
	      else
		{
		  cout << "Warning: element " << num << " redefined line " <<
		    line << endl;
		  //    pel->nodes->flush();
		}

	      // injecter la liste des noeuds de l'element
	      for (i = 0; i < 4; i++)
		{
		  if ((plnd_dom->AppN (n[i])) == NULL)
		    {
		      cerr << "Fatal Error: node " << n[i] <<
			" not existing line " << line << endl;
		      exit (-1);
		    }
		  pel->addNode (plnd_dom->AppN (n[i]));
		}
	    }
	  else
	    TheEnd = 1;
	  break;
	default:
	  fatalError ("void io_Data::elements_read(FILE* pfile)",
		      "ne sait pas lire les structures autres que 4 noeuds");
	}
      line++;
    }
  // fermeture de la liste
  pdomain->elements.close ();

  cout << pdomain->elements.size () << " elements read\n";*/
}

//-----------------------------------------------------------------------------
void io_Data::elements_write(FILE *pfile)
//-----------------------------------------------------------------------------
{
  /*  Element *pel;
  ListId < Element * >lel;
  Indice i, j;

  lel = pdomain->elements;

  fprintf (pfile, "%d\n", lel (1)->getNumberOfNodes());

  for (i = 0; i < lel.size (); i++)
    {
      pel = lel (i);

      fprintf (pfile, "%ld,%d", pel->number, (int) pel->material->Id);

      for (j = 0; j < pel->getNumberOfNodes(); j++)
	fprintf (pfile, ",%ld", pel->nodes (j)->number);

      fprintf (pfile, "\n");
    }*/
}

static const char *fields[] =
    {"materialSpeed", "gridSpeed", "DmatSpeed", "forces", "delta_disp",
     "density", "Ddensity", "energy", "Denergy", "temp", "press", "dpress", "sigma", "S", "dS", "Eps",
     "dEps", "EpsPlas", "alpha"};

//-----------------------------------------------------------------------------
void io_Data::datasRead(FILE *pfile)
//-----------------------------------------------------------------------------
{
  /*//  Node *pnd;
  Element *pel;
  IntegrationPoint *pint;
  ListId < Node * >lnd;
  ListId < Element * >lel;
  Indice number, number2, i, j;
  char str1[40];
  char str2[40];
  Vec3D vec;
  //  Real val;
  Indice curfield = 0;

  lnd = pdomain->nodes;
  lel = pdomain->elements;

#define in_NODAL_VECTOR(VALUE) fscanf(pfile,"%s %s %ld",str1,str2,&number);\
  if (strcmp(str2,fields[curfield++])!=0) fatalError("","Unknown variable %s when %s is expected\n",str2,fields[curfield-1]);\
  for (i=0;i<number;i++) {\
    fscanf(pfile,"%lf,%lf,%lf",\
	   VECTORadr(lnd(i)->Current->VALUE));\
  }

#define in_NODAL_VALUE(VALUE) fscanf(pfile,"%s %s %ld",str1,str2,&number);\
  if (strcmp(str2,fields[curfield++])!=0) fatalError("","Unknown variable %s when %s is expected\n",str2,fields[curfield-1]);\
  for (i=0;i<number;i++) {\
    fscanf(pfile,"%lf",&(lnd(i)->Current->VALUE));\
  }\

#define in_INTEGRATION_VALUE(VALUE) fscanf(pfile,"%s %s %ld",str1,str2,&number);\
  if (strcmp(str2,fields[curfield++])!=0) fatalError("","Unknown variable %s when %s is expected\n",str2,fields[curfield-1]);\
  for (i=0;i<number;i++) {\
    pel=lel(i);\
    fscanf(pfile,"%ld",&number2);\
    for (j=0;j<number2;j++) {\
      fscanf(pfile,"%lf",&(pel->intPoint(j)->VALUE));\
    }\
  }

#define in_INTEGRATION_TENSOR(VALUE) fscanf(pfile,"%s %s %ld",str1,str2,&number);\
  if (strcmp(str2,fields[curfield++])!=0) fatalError("","Unknown variable %s when %s is expected\n",str2,fields[curfield-1]);\
  for (i=0;i<number;i++) {\
    pel=lel(i);\
    fscanf(pfile,"%ld",&number2);\
    for (j=0;j<number2;j++) {\
      pint=pel->intPoint(j);\
      fscanf(pfile,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",TENSORadr(pint->VALUE));\
    }\
  }

  // material speed
  in_NODAL_VECTOR (mat_speed);

  // grid speed
//  in_NODAL_VECTOR(grid_speed);

  // material acceleration
  in_NODAL_VECTOR (dmat_speed);

  //nodal force
  in_NODAL_VECTOR (fe);

  // displacement
  in_NODAL_VECTOR (delta_disp);

  // density
  in_NODAL_VALUE (ro);

  // inc density
  in_NODAL_VALUE (dro);

  // energy
  in_NODAL_VALUE (e);

  // inc energy
  in_NODAL_VALUE (de);

  // temperature
  in_NODAL_VALUE (T);

  // p
  in_INTEGRATION_VALUE (p);

  // dp
  in_INTEGRATION_VALUE (dp);

  // stress
  in_INTEGRATION_TENSOR (Sig);

  // S
  in_INTEGRATION_TENSOR (S);

  // dS
  in_INTEGRATION_TENSOR (dS);

  // Strain
  in_INTEGRATION_TENSOR (Eps);

  // dStrain
  in_INTEGRATION_TENSOR (dEps);

  // deformation plastique
  in_INTEGRATION_TENSOR (EpsPlas);
//  cout << EpsPlas<<endl;

  // alpha
  in_INTEGRATION_VALUE (alpha);*/
}

//-----------------------------------------------------------------------------
void io_Data::datasWrite(FILE *pfile)
//-----------------------------------------------------------------------------
{
  /*  Node *pnd;
  Element *pel;
  //  IntegrationPoint* pint;
  ListId < Node * >lnd;
  ListId < Element * >lel;
  ListId < IntegrationPoint * >lint;
  Indice i, j;
  Indice curfield = 0;

  lnd = pdomain->nodes;
  lel = pdomain->elements;


#define out_NODAL_VECTOR(VALUE) fprintf(pfile,"VECTOR %s %ld\n",fields[curfield++],lnd.size());\
  for (i=0;i<lnd.size();i++) {\
    pnd=lnd(i);\
    fprintf(pfile,"%.6E,%.6E,%.6E\n",VECTOR(pnd->Current->VALUE));\
  }

#define out_NODAL_VALUE(VALUE) fprintf(pfile,"VECTOR %s %ld\n",fields[curfield++],lnd.size());\
  for (i=0;i<lnd.size();i++) {\
    pnd=lnd(i);\
    fprintf(pfile,"%.6E\n",pnd->Current->VALUE);\
  }

#define out_INTEGRATION_VALUE(VALUE) fprintf(pfile,"SCALAR %s %ld\n",fields[curfield++],lel.size());\
  for (i=0;i<lel.size();i++) {\
    pel=lel(i);\
    fprintf(pfile,"%ld\n",pel->intPoint.size());\
    for (j=0;j<pel->intPoint.size();j++) {\
      fprintf(pfile,"%.6E\n",pel->intPoint(j)->VALUE);\
    }\
  }

#define out_INTEGRATION_TENSOR(VALUE) fprintf(pfile,"TENSOR %s %ld\n",fields[curfield++],lel.size());\
  for (i=0;i<lel.size();i++) {\
    pel=lel(i);\
    fprintf(pfile,"%ld\n",pel->intPoint.size());\
    for (j=0;j<pel->intPoint.size();j++) {\
      fprintf(pfile,"%.6E,%.6E,%.6E,%.6E,%.6E,%.6E,%.6E,%.6E,%.6E\n",TENSOR(pel->intPoint(j)->VALUE));\
    }\
  }

  // vitesse materielle
  out_NODAL_VECTOR (mat_speed);

  // Vitesse de grille
//  out_NODAL_VECTOR(grid_speed);

  // acceleration materielle
  out_NODAL_VECTOR (dmat_speed);

  //nodal force
  out_NODAL_VECTOR (fe);

  // displacement
  out_NODAL_VECTOR (delta_disp);

  // densite nodale
  out_NODAL_VALUE (ro);

  // increment de densite nodale
  out_NODAL_VALUE (dro);

  // energie
  out_NODAL_VALUE (e);

  // increment d'energie
  out_NODAL_VALUE (de);

  // temperature
  out_NODAL_VALUE (T);

  // press
  out_INTEGRATION_VALUE (p);

  // increment de press
  out_INTEGRATION_VALUE (dp);

  // contrainte
  out_INTEGRATION_TENSOR (Sig);

  // deviateur
  out_INTEGRATION_TENSOR (S);

  // increment de deviateur
  out_INTEGRATION_TENSOR (dS);

  // deformations
  out_INTEGRATION_TENSOR (Eps);

  // increment de deformations
  out_INTEGRATION_TENSOR (dEps);

  // deformation plastique
  out_INTEGRATION_TENSOR (EpsPlas);
//  cout << EpsPlas<<endl;

  // alpha
  out_INTEGRATION_VALUE (alpha);*/
}

//-----------------------------------------------------------------------------
Real io_Data::getReadTimeData()
//-----------------------------------------------------------------------------
{
  Real tim = 0.;

  if (binary)
  {
    ifstream pfile(name.chars());

    if (!pfile)
    {
      fatalError("ioData::getReadTimeData", "Unable to read file '%s'",
                 name.chars());
    }

    tim = pstructure->getReadTimeData(pfile);

    pfile.close();
  }

  return tim;
}

//-----------------------------------------------------------------------------
void io_Data::structure_read()
//-----------------------------------------------------------------------------
{
  if (binary)
  {
    //    cout << "Reading BINARY file " <<filename<< endl;

    //   String name;
    //   name=filename;
    String name;
    if (filename.contains(".bin"))
      name = filename;
    else
      name = filename + ".bin";

    ifstream pfile(name.chars());

    if (!pfile)
    {
      fatalError("io_Data::structure_read", "Unable to read file %s",
                 name.chars());
    }

    pstructure->readData(pfile);

    pfile.close();
  }
  else
  {
    //    cout << "Reading ASCII file " <<filename<< endl;

    FILE *pfile;
    //    String name;
    //  name=filename+".data";

    if ((pfile = fopen(filename.chars(), "r")) == NULL)
    {
      fatalError("void io_Data::domain_read()", "Unable to read file");
    }

    nodesRead(pfile);
    datasRead(pfile);

    fclose(pfile);
  }
}

//-----------------------------------------------------------------------------
void io_Data::structure_write()
//-----------------------------------------------------------------------------
{
  if (binary)
  {
    //    cout << "Writing BINARY file " <<filename<< endl;

    String name;
    name = filename + ".bin";
    ofstream pfile(name.chars());

    if (!pfile)
    {
      fatalError("ioData::domain_write", "Unable to write file %s",
                 name.chars());
    }

    pstructure->writeData(pfile);

    pfile.close();
  }
  else
  {
    //    cout << "Writing ASCII file " <<filename<< endl;

    FILE *pfile;
    String name;
    name = filename + ".data";

    if ((pfile = fopen(name.chars(), "w")) == NULL)
    {
      fatalError("void io_Data::domain_write()", "Unable to write file");
    }

    nodesWrite(pfile);
    datasWrite(pfile);

    fclose(pfile);
  }
}
