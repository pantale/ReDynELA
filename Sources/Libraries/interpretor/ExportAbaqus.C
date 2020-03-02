/*                                                 */
/*  DynELA Project                                 */
/*                                                 */
/*  (c) Copyright 1997-2004                        */
/*                                                 */
/*	Equipe C.M.A.O                             */
/*	Laboratoire Genie de production            */
/*	Ecole Nationale d'Ingenieurs de Tarbes     */
/*	BP 1629 - 65016 TARBES cedex               */
/*                                                 */
/*                                                 */
/*  Main Author: Olivier PANTALE                   */
/*                                                 */
/***************************************************/

// begin date :
// revision date :

/*
	Class ExportAbaqus implementation
*/

#include <InputDyn.h>
#include <ExportAbaqus.h>
extern ObjectsList objectList;

//-----------------------------------------------------------------------------
ExportAbaqus::ExportAbaqus()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ExportAbaqus::ExportAbaqus(const ExportAbaqus &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ExportAbaqus::~ExportAbaqus()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ExportAbaqus::structure_read()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ExportAbaqus::headingWrite()
//-----------------------------------------------------------------------------
{
	Unix sys;
	fprintf(pfile, "*HEADING\n");
	fprintf(pfile, "DYNELA to ABAQUS FILE TRANSLATOR %s",
			sys.getCurrDate().chars());
}

//-----------------------------------------------------------------------------
void ExportAbaqus::nodesWrite()
//-----------------------------------------------------------------------------
{
	Node *pnd;
	Indice i;

	fprintf(pfile, "*NODE, SYSTEM=R\n");
	for (i = 0; i < Global_Structure->domains.current()->nodes.size(); i++)
	{
		pnd = Global_Structure->domains.current()->nodes(i);
		fprintf(pfile, "%11ld,%14.7E,%14.7E,%14.7E\n", pnd->number,
				pnd->coords(0), pnd->coords(1), pnd->coords(2));
	}
}

//-----------------------------------------------------------------------------
void ExportAbaqus::elementsWrite()
//-----------------------------------------------------------------------------
{
	Element *pel;
	Indice i, j;
	Indice nb;

	// type ElQua4n2D
	nb = 0;
	for (i = 0; i < Global_Structure->domains.current()->elements.size(); i++)
		if (Global_Structure->domains.current()->elements(i)->getType() == Element::ElQua4n2D)
		{
			nb = 1;
			i = Global_Structure->domains.current()->elements.size();
		}
	if (nb != 0)
	{
		fprintf(pfile, "*ELEMENT,TYPE=CPE4R\n");
		for (i = 0; i < Global_Structure->domains.current()->elements.size(); i++)
		{
			pel = Global_Structure->domains.current()->elements(i);
			if (pel->getType() == Element::ElQua4n2D)
			{
				fprintf(pfile, "  %ld", pel->number);
				for (j = 0; j < pel->nodes.size(); j++)
					fprintf(pfile, ", %ld", pel->nodes(j)->number);
				fprintf(pfile, "\n");
			}
		}
	}

	// type ElQua4nAx
	nb = 0;
	for (i = 0; i < Global_Structure->domains.current()->elements.size(); i++)
		if (Global_Structure->domains.current()->elements(i)->getType() == Element::ElQua4nAx)
		{
			nb = 1;
			i = Global_Structure->domains.current()->elements.size();
		}
	if (nb != 0)
	{
		fprintf(pfile, "*ELEMENT,TYPE=CAX4R\n");
		for (i = 0; i < Global_Structure->domains.current()->elements.size(); i++)
		{
			pel = Global_Structure->domains.current()->elements(i);
			if (pel->getType() == Element::ElQua4nAx)
			{
				fprintf(pfile, "  %ld", pel->number);
				for (j = 0; j < pel->nodes.size(); j++)
					fprintf(pfile, ", %ld", pel->nodes(j)->number);
				fprintf(pfile, "\n");
			}
		}
	}

	// type ElTet4n3D
	nb = 0;
	for (i = 0; i < Global_Structure->domains.current()->elements.size(); i++)
		if (Global_Structure->domains.current()->elements(i)->getType() == Element::ElTet4n3D)
		{
			nb = 1;
			i = Global_Structure->domains.current()->elements.size();
		}
	if (nb != 0)
	{
		fprintf(pfile, "*ELEMENT,TYPE=C3D4\n");
		for (i = 0; i < Global_Structure->domains.current()->elements.size(); i++)
		{
			pel = Global_Structure->domains.current()->elements(i);
			if (pel->getType() == Element::ElTet4n3D)
			{
				fprintf(pfile, "  %ld", pel->number);
				for (j = 0; j < pel->nodes.size(); j++)
					fprintf(pfile, ", %ld", pel->nodes(j)->number);
				fprintf(pfile, "\n");
			}
		}
	}

	// type ElHex8n3D
	nb = 0;
	for (i = 0; i < Global_Structure->domains.current()->elements.size(); i++)
		if (Global_Structure->domains.current()->elements(i)->getType() == Element::ElHex8n3D)
		{
			nb = 1;
			i = Global_Structure->domains.current()->elements.size();
		}
	if (nb != 0)
	{
		fprintf(pfile, "*ELEMENT,TYPE=C3D8\n");
		for (i = 0; i < Global_Structure->domains.current()->elements.size(); i++)
		{
			pel = Global_Structure->domains.current()->elements(i);
			if (pel->getType() == Element::ElHex8n3D)
			{
				fprintf(pfile, "  %ld", pel->number);
				for (j = 0; j < pel->nodes.size(); j++)
					fprintf(pfile, ", %ld", pel->nodes(j)->number);
				fprintf(pfile, "\n");
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportAbaqus::materialsWrite()
//-----------------------------------------------------------------------------
{
	Material *pmat;
	Indice i;

	for (i = 0; i < Global_Structure->materials.size(); i++)
	{
		pmat = Global_Structure->materials(i);

		fprintf(pfile, "*MATERIAL,NAME=%s\n", pmat->name.chars());

		fprintf(pfile, "*ELASTIC,TYPE=ISOTROPIC\n");
		fprintf(pfile, "%9.4E, %9.4E\n", pmat->young(), pmat->nu());
		fprintf(pfile, "*DENSITY\n%9.4E\n", pmat->density());
		fprintf(pfile, "*EXPANSION,TYPE=ISO,ZERO=21.85\n%9.4E\n",
				pmat->dilat());
		fprintf(pfile, "*SPECIFIC HEAT\n%9.4E\n", pmat->heatCoeff());

		if (pmat->getIsotropicHardening() != NULL)
		{
			switch (pmat->getIsotropicHardening()->getType())
			{
			case IsotropicHardening::Elastic:
				break;
			case IsotropicHardening::ElastoPlastic:
			{
				IsoHardElastoplastic *hard = (IsoHardElastoplastic *)pmat->getIsotropicHardening();
				fprintf(pfile, "*INELASTIC HEAT FRACTION\n1.0\n");
				fprintf(pfile, "**  ELASTOPLASTIC {\n");
				fprintf(pfile, "**    A: %9.4E;\n", hard->Yield_Stress());
				fprintf(pfile, "**    B: %9.4E;\n", hard->Hard_Param());
				fprintf(pfile, "**    n: %9.4E;\n", hard->Hard_Exponent());
				//		fprintf (pfile, "**    EPSMAX: %9.4E;\n", hard->Fail_Strain ());
				//		fprintf (pfile, "**    SIGMAX: %9.4E;\n", hard->Max_Stress ());
				fprintf(pfile, "**  };\n");
				fprintf(pfile, "*PLASTIC\n");
				for (Real eps = 0; eps < 10; eps += 0.1)
				{
					fprintf(pfile, "%9.4E,%9.4E\n",
							hard->Yield_Stress() +
								hard->Hard_Param() * pow(eps,
														 hard->Hard_Exponent()),
							eps);
				}
			}
			break;
			case IsotropicHardening::ElastoPlasticTabular:
			{
				IsoHardElastoplasticTabular *hard = (IsoHardElastoplasticTabular *)pmat->getIsotropicHardening();
				;
				fprintf(pfile, "*INELASTIC HEAT FRACTION\n1.0\n");
				fprintf(pfile, "*PLASTIC\n");
				DiscreteFunction *df = hard->getFunction();
				for (Indice k = 0; k < df->getNumberOfPoints(); k++)
				{
					fprintf(pfile, "%9.4E,%9.4E\n", df->getYPoint(k), df->getXPoint(k));
				}
			}
			break;
			default:
				warning("Material write",
						"Write of material type %ld not implemented\n",
						pmat->getIsotropicHardening()->getType());
				break;
			}
		}
		/*NMAT      switch (pmat->getType ())
	{
	case Material::Elastic:
//      fprintf(pfile,"  ELASTIC;\n");
	  break;
	case Material::ElastoPlastic:
	  {
	    MaterialElastoplastic *pmat2;
	    pmat2 = (MaterialElastoplastic *) (Global_Structure->materials (i));
	    fprintf (pfile, "*INELASTIC HEAT FRACTION\n1.0\n");
	    fprintf (pfile, "**  ELASTOPLASTIC {\n");
	    fprintf (pfile, "**    A: %9.4E;\n", pmat2->Yield_Stress ());
	    fprintf (pfile, "**    B: %9.4E;\n", pmat2->Hard_Param ());
	    fprintf (pfile, "**    n: %9.4E;\n", pmat2->Hard_Exponent ());
	    fprintf (pfile, "**    EPSMAX: %9.4E;\n", pmat2->Fail_Strain ());
	    fprintf (pfile, "**    SIGMAX: %9.4E;\n", pmat2->Max_Stress ());
	    fprintf (pfile, "**  };\n");
	    fprintf (pfile, "*PLASTIC\n");
	    for (Real eps = 0; eps < 10; eps += 0.1)
	      {
		fprintf (pfile, "%9.4E,%9.4E\n",
			 pmat2->Yield_Stress () +
			 pmat2->Hard_Param () * pow (eps,
						     pmat2->Hard_Exponent ()),
			 eps);
	      }
	  }
	  break;
	case Material::ElastoPlasticTabular:
	  {
	    MaterialElastoplasticTabular *pmat2;
//      coupleReal* pt;
	    pmat2 = (MaterialElastoplasticTabular *) (Global_Structure->materials (i));
	    fprintf (pfile, "*INELASTIC HEAT FRACTION\n1.0\n");
	    fprintf (pfile, "*PLASTIC\n");
	    DiscreteFunction *df = pmat2->getFunction ();
	    for (Indice k = 0; k < df->getNumberOfPoints (); k++)
	      {
//        pt=df->getPoint(k);
		fprintf (pfile, "%9.4E,%9.4E\n", df->getYPoint (k),
			 df->getXPoint (k) );
	      }
	    //  ELASTOPLASTIC TABULAR { DISCRETE FUNCTION %s;};\n",pmat2->getFunctionName().chars()); 
	  }
	  break;

	default:
	  warning ("Material write",
		   "Write of material type %ld not implemented\n",
		   pmat->getType ());
	  break;
	}
*/
	}
}

//-----------------------------------------------------------------------------
void ExportAbaqus::groupsWrite()
//-----------------------------------------------------------------------------
{
	NodeSet *nodesSet;
	ElementSet *elementsSet;
	Indice ob, j;

	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_NodeSet)
		{
			nodesSet = (NodeSet *)objectList.getObject(ob)->object;
			fprintf(pfile, "*NSET,NSET=%s\n", nodesSet->name.chars());
			for (j = 0; j < nodesSet->nodes.size(); j++)
			{
				if (j % 8 == 0)
					fprintf(pfile, "%ld", nodesSet->nodes(j)->number);
				else
					fprintf(pfile, ", %ld", nodesSet->nodes(j)->number);
				if ((j != 0) && ((j + 1) % 8 == 0))
					fprintf(pfile, "\n");
			}
			if ((j) % 8 != 0)
				fprintf(pfile, "\n");
		}
	}

	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_ElementSet)
		{
			elementsSet = (ElementSet *)objectList.getObject(ob)->object;
			fprintf(pfile, "*ELSET,ELSET=%s\n", elementsSet->name.chars());
			for (j = 0; j < elementsSet->elements.size(); j++)
			{
				if (j % 8 == 0)
					fprintf(pfile, "%ld", elementsSet->elements(j)->number);
				else
					fprintf(pfile, ", %ld", elementsSet->elements(j)->number);
				if ((j != 0) && ((j + 1) % 8 == 0))
					fprintf(pfile, "\n");
			}
			if ((j) % 8 != 0)
				fprintf(pfile, "\n");
		}
	}
}

//-----------------------------------------------------------------------------
void ExportAbaqus::solidsWrite()
//-----------------------------------------------------------------------------
{
	Material *pmat;
	Indice i, j;

	//  fprintf(pfile,"SOLID {\n");

	for (i = 0; i < Global_Structure->materials.size(); i++)
	{
		pmat = Global_Structure->materials(i);
		for (j = 0; j < pmat->elementsSet.size(); j++)
			fprintf(pfile, "*SOLID SECTION,ELSET=%s,MATERIAL=%s\n",
					pmat->elementsSet(j)->name.chars(), pmat->name.chars());
	}
}

//-----------------------------------------------------------------------------
void ExportAbaqus::boundariesWrite()
//-----------------------------------------------------------------------------
{
	Node *pnd;
	BoundarySpeed *bsp;
	BoundaryRestrain *bdi;

	Indice i, j;
	BoundaryCondition *pbound;
	Boundary *pb;
	//  Indice counter;
	Boolean first;

	first = True;
	for (i = 0; i < Global_Structure->domains.current()->nodes.size(); i++)
	{
		pnd = Global_Structure->domains.current()->nodes(i);
		pbound = pnd->boundary;

		if (pbound != NULL)
		{
			if (pbound->isConstant())
			{
				for (j = 0; j < pbound->getNumberOfConstants(); j++)
				{
					pb = pbound->getConstant(j);
					if (pb->Type() == Boundary::Restrain)
					{
						if (first)
							fprintf(pfile, "*BOUNDARY,TYPE=DISPLACEMENT\n");
						first = False;
						bdi = (BoundaryRestrain *)pb;
						Vec3D disp;
						disp = bdi->get();
						if ((disp(0) == 1) && (disp(1) == 1) && (disp(2) == 1))
							fprintf(pfile, "%ld,ENCASTRE\n", pnd->number);
						if ((disp(0) == 1) && (disp(1) == 1) && (disp(2) == 0))
							fprintf(pfile, "%ld,1,2\n", pnd->number);
						if ((disp(0) == 1) && (disp(1) == 0) && (disp(2) == 1))
							fprintf(pfile, "%ld,1\n", pnd->number);
						if ((disp(0) == 1) && (disp(1) == 0) && (disp(2) == 0))
							fprintf(pfile, "%ld,1\n", pnd->number);
						if ((disp(0) == 0) && (disp(1) == 1) && (disp(2) == 1))
							fprintf(pfile, "%ld,2,3\n", pnd->number);
						if ((disp(0) == 0) && (disp(1) == 1) && (disp(2) == 0))
							fprintf(pfile, "%ld,2,2\n", pnd->number);
						if ((disp(0) == 0) && (disp(1) == 0) && (disp(2) == 1))
							fprintf(pfile, "%ld,3,3\n", pnd->number);
					}
				}
			}
		}
	}

	first = True;
	for (i = 0; i < Global_Structure->domains.current()->nodes.size(); i++)
	{
		pnd = Global_Structure->domains.current()->nodes(i);
		pbound = pnd->boundary;
		if (pbound != NULL)
		{
			if (pbound->isInitial())
			{
				for (j = 0; j < pbound->getNumberOfInitials(); j++)
				{
					pb = pbound->getInitial(j);
					if (pb->Type() == Boundary::Speed)
					{
						if (first)
							fprintf(pfile, "*INITIAL CONDITIONS, TYPE=VELOCITY\n");
						first = False;
						bsp = (BoundarySpeed *)pb;
						Vec3D speed;
						speed = bsp->get();
						if (speed(0) != 0)
							fprintf(pfile, "%ld,1,%lf\n", pnd->number,
									speed(0));
						if (speed(1) != 0)
							fprintf(pfile, "%ld,2,%lf\n", pnd->number,
									speed(1));
						if (speed(2) != 0)
							fprintf(pfile, "%ld,3,%lf\n", pnd->number,
									speed(2));
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportAbaqus::boudariesStepWrite()
//-----------------------------------------------------------------------------
{
	Node *pnd;
	BoundarySpeed *bsp;

	Indice i, j;
	BoundaryCondition *pbound;
	Boundary *pb;
	Boolean first;

	first = True;
	for (i = 0; i < Global_Structure->domains.current()->nodes.size(); i++)
	{
		pnd = Global_Structure->domains.current()->nodes(i);
		pbound = pnd->boundary;
		if (pbound != NULL)
		{
			if (pbound->isConstant())
			{
				for (j = 0; j < pbound->getNumberOfConstants(); j++)
				{
					pb = pbound->getConstant(j);
					if (pb->Type() == Boundary::Speed)
					{
						if (first)
							fprintf(pfile, "*BOUNDARY,TYPE=VELOCITY\n");
						first = False;
						bsp = (BoundarySpeed *)pb;
						if (first)
							fprintf(pfile, "*BOUNDARY,TYPE=VELOCITY\n");
						Vec3D speed;
						speed = bsp->get();
						if (speed(0) != 0)
							fprintf(pfile, "%ld,1,1,%lf\n", pnd->number,
									speed(0));
						if (speed(1) != 0)
							fprintf(pfile, "%ld,2,2,%lf\n", pnd->number,
									speed(1));
						if (speed(2) != 0)
							fprintf(pfile, "%ld,3,3,%lf\n", pnd->number,
									speed(2));
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportAbaqus::interfacesWrite()
//-----------------------------------------------------------------------------
{
	Interface *pint;
	SideFace *psd;
	Indice i, j;
	ContactLaw *plaw;

	for (i = 0; i < Global_Structure->domains.current()->interfaces.size(); i++)
	{
		pint = Global_Structure->domains.current()->interfaces(i);

		// Master part
		fprintf(pfile, "*SURFACE, NAME=%sM,type=element\n", pint->name.chars());
		//      fprintf (pfile, "*SURFACE DEFINITION,NAME=%sM\n", pint->name.chars ());
		for (j = 0; j < pint->master->sides.size(); j++)
		{
			psd = pint->master->sides(j);
			fprintf(pfile, "%ld,", psd->element->number);
			if (psd->nodes(0) == psd->element->nodes(0))
				fprintf(pfile, "S4\n");
			if (psd->nodes(0) == psd->element->nodes(1))
				fprintf(pfile, "S1\n");
			if (psd->nodes(0) == psd->element->nodes(2))
				fprintf(pfile, "S2\n");
			if (psd->nodes(0) == psd->element->nodes(3))
				fprintf(pfile, "S3\n");
		}

		// Slave part
		fprintf(pfile, "*SURFACE, NAME=%sS,type=element\n", pint->name.chars());
		//      fprintf (pfile, "*SURFACE DEFINITION,NAME=%sS\n", pint->name.chars ());
		for (j = 0; j < pint->slave->sides.size(); j++)
		{
			psd = pint->slave->sides(j);
			fprintf(pfile, "%ld,", psd->element->number);
			if (psd->nodes(0) == psd->element->nodes(0))
				fprintf(pfile, "S4\n");
			if (psd->nodes(0) == psd->element->nodes(1))
				fprintf(pfile, "S1\n");
			if (psd->nodes(0) == psd->element->nodes(2))
				fprintf(pfile, "S2\n");
			if (psd->nodes(0) == psd->element->nodes(3))
				fprintf(pfile, "S3\n");
		}

		// Contact Law
		fprintf(pfile, "*SURFACE INTERACTION,NAME=%s\n", pint->name.chars());
		plaw = pint->contactLaw;
		switch (plaw->Type())
		{
		case coulombContactLaw:
		{
			CoulombLaw *pcoul = (CoulombLaw *)pint->contactLaw;
			fprintf(pfile, "*FRICTION\n");
			fprintf(pfile, "%9.4E\n", pcoul->friction);
		}
		break;
		default:
			break;
		}
		fprintf(pfile, "*CONTACT PAIR,INTERACTION=%s\n", pint->name.chars());
		fprintf(pfile, "%sM,%sS\n", pint->name.chars(), pint->name.chars());
	}
}

//-----------------------------------------------------------------------------
void ExportAbaqus::stepWrite()
//-----------------------------------------------------------------------------
{
	//  Physic* physic;
	Domain *domain;
	Solver *psolver;

	//  physic=pstructure->physics.current();
	domain = pstructure->domains.current();
	psolver = domain->solvers.current();

	fprintf(pfile, "*STEP\n");
	fprintf(pfile, "*DYNAMIC, EXPLICIT, ADIABATIC\n");
	//  fprintf(pfile,"  START TIME: %9.4E;\n",Global_Structure->domains.current()->times.startTime);
	fprintf(pfile, ",%9.4E\n", psolver->getEndTime());

	// conditions aux limites dans le step
	boudariesStepWrite();

	//   fprintf(pfile,"  START TIME: %9.4E;\n",Global_Structure->domains.current()->times.startTime);
	//   fprintf(pfile,"  STOP TIME: %9.4E;\n",Global_Structure->domains.current()->times.endTime);

	//   fprintf(pfile,"  START SAVE TIME: %9.4E;\n",Global_Structure->domains.current()->times.startSaveTime);
	//   fprintf(pfile,"  STOP SAVE TIME: %9.4E;\n",Global_Structure->domains.current()->times.endSaveTime);

	//   if (Global_Structure->domains.current()->times.saveTime != 0)
	//     {
	//       fprintf (pfile, "*RESTART,WRITE,NUMBER INTERVAL=%d\n",
	// 	       (int) (1 +
	// 		      Global_Structure->domains.current()->times.endTime /
	// 		      Global_Structure->domains.current()->times.saveTime));
	//     }

	//   fprintf(pfile,"  START INCREMENT: %ld;\n",Global_Structure->domains.current()->times.startIncrement);
	//   fprintf(pfile,"  STOP INCREMENT: %ld;\n",Global_Structure->domains.current()->times.endIncrement);
	//   fprintf(pfile,"  TIMESTEP FACTOR: %9.4E;\n",Global_Structure->domains.current()->times.timeStepFactor);

	// ecriture des interfaces
	interfacesWrite();

	//   if (Global_Structure->domains.current()->times.saveTime != 0)
	//     {
	//       fprintf (pfile, "*FILE OUTPUT,TIMEMARKS=YES,NUM=%d\n",
	// 	       (int) (1 +
	// 		      Global_Structure->domains.current()->times.endTime /
	// 		      Global_Structure->domains.current()->times.saveTime));
	//     }

	// comment� ici

	//   fprintf (pfile, "*NODE FILE\n");
	//   fprintf (pfile, "U,V,RF \n");
	//   fprintf (pfile, "*EL FILE\n");
	//   fprintf (pfile, "S,PEEQ,MISES,PE,LE,NE\n");

	if (Global_Structure->saveTime != 0)
	{
		fprintf(pfile, "*OUTPUT,FIELD,NUMBER INTERVAL=%d,VARIABLE=ALL\n",
				(int)(1 +
					  Global_Structure->endSaveTime /
						  Global_Structure->saveTime));
	}
	//   fprintf (pfile, "*NODE OUTPUT\n");
	//   fprintf (pfile, "U,V\n");
	//   fprintf (pfile, "*ELEMENT OUTPUT\n");
	//   fprintf (pfile, "PEEQ,TEMP\n");

	fprintf(pfile, "*END STEP\n");
}

//-----------------------------------------------------------------------------
void ExportAbaqus::structure_write()
//-----------------------------------------------------------------------------
{
	if ((pfile = fopen(filename.chars(), "w")) == NULL)
	{
		fatalError("void InputData::domain_write()",
				   "Unable to write data file");
		exit(-1);
	}

	headingWrite();
	nodesWrite();
	elementsWrite();
	groupsWrite();
	materialsWrite();
	solidsWrite();
	boundariesWrite();
	stepWrite();

	fclose(pfile);
}
