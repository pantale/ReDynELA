/***************************************************/
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
	Class ExportDynELA implementation
*/

#include <InputDyn.h>
#include <ExportDynELA.h>

extern ObjectsList objectList;

//-----------------------------------------------------------------------------
ExportDynELA::ExportDynELA()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ExportDynELA::ExportDynELA(const ExportDynELA &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ExportDynELA::~ExportDynELA()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ExportDynELA::headingWrite()
//-----------------------------------------------------------------------------
{
	Unix os;

	// write the header of the file
	fprintf(pfile, "/***************************************************/\n");
	fprintf(pfile, "/*                                                 */\n");
	fprintf(pfile, "/*  DynELA Project                                 */\n");
	fprintf(pfile, "/*                                                 */\n");
	fprintf(pfile, "/*  (c) Copyright 1997-2004                        */\n");
	fprintf(pfile, "/*                                                 */\n");
	fprintf(pfile, "/*     Equipe C.M.A.O                              */\n");
	fprintf(pfile, "/*     Laboratoire Genie de production             */\n");
	fprintf(pfile, "/*     Ecole Nationale d'Ingenieurs de Tarbes      */\n");
	fprintf(pfile, "/*     BP 1629 - 65016 TARBES cedex                */\n");
	fprintf(pfile, "/*                                                 */\n");
	fprintf(pfile, "/*                                                 */\n");
	fprintf(pfile, "/*  Main Author: Olivier PANTALE                   */\n");
	fprintf(pfile, "/*                                                 */\n");
	fprintf(pfile, "/***************************************************/\n");
	fprintf(pfile, "\n/*\n");
	fprintf(pfile, "  DynELA v. %d.%d.%d-%d\n\n", VERSION, RELEASE, UNDER_RELEASE, ALPHA_VERSION);
	fprintf(pfile, "  %s\n", os.getCurrDate().chars());
	fprintf(pfile, "  Job created by user %s\n", os.getCurrEnvValue("LOGNAME").chars());
	fprintf(pfile, "  Job executed on a %s system\n", os.getCurrHosttype().chars());
	fprintf(pfile, "  Name of the host : %s\n", os.getCurrHostname().chars());
	fprintf(pfile, "  Host ID : %0x\n*/\n\n", os.getCurrHostId());
}

//-----------------------------------------------------------------------------
void ExportDynELA::memoryVariablesWrite()
//-----------------------------------------------------------------------------
{
	sepWrite("Internal variables list output for information");
	fprintf(pfile, "/*\n");
	for (Indice i = 0; i < objectList.list.size(); i++)
	{
		fprintf(pfile, "%s %s;\n", objectList.getObject(i)->getObjectClass().chars(), objectList.getObject(i)->getObjectName().chars());
	}
	fprintf(pfile, "*/\n");
}

//-----------------------------------------------------------------------------
void ExportDynELA::sepWrite(String str)
//-----------------------------------------------------------------------------
{
	fprintf(pfile, "\n/*\n  %s\n*/\n", str.chars());
}

//-----------------------------------------------------------------------------
void ExportDynELA::structureWrite()
//-----------------------------------------------------------------------------
{
	Indice j;
	// Physic* physic;
	Domain *domain;
	// Grid* grid;

	sepWrite("Data structure write");
	fprintf(pfile, "Structure %s;\n", pstructure->name.chars());
	//   for (i=0; i<pstructure->physics.size();i++)
	//     {
	//       physic=pstructure->physics(i);
	//       if (physic->name!="defaultPhysic")
	// 	fprintf(pfile,"Physic %s;\n",physic->name.chars());
	for (j = 0; j < pstructure->domains.size(); j++)
	{
		domain = pstructure->domains(j);
		if (domain->name != "defaultDomain")
			fprintf(pfile, "Domain %s;\n", domain->name.chars());
		// 	  for (k=0; k<domain->grids.size();k++)
		// 	    {
		// 	      grid=domain->grids(k);
		// 	 //     fprintf(pfile,"Grid %s;\n",grid->name.chars());
		// 	    }
	}
	//    }
}

//-----------------------------------------------------------------------------
void ExportDynELA::nodesWrite()
//-----------------------------------------------------------------------------
{
	Indice j, l;
	// Physic* physic;
	Domain *domain;
	//  Grid* grid;
	Node *pnd;

	sepWrite("Nodes write");
	//   for (i=0; i<pstructure->physics.size();i++)
	//     {
	//       physic=pstructure->physics(i);
	//       if (physic->name!="defaultPhysic")
	// 	fprintf(pfile,"%s.setPhysic(%s);\n",pstructure->name.chars(),physic->name.chars());
	for (j = 0; j < pstructure->domains.size(); j++)
	{
		domain = pstructure->domains(j);
		if (domain->name != "defaultDomain")
			fprintf(pfile, "%s.setDomain(%s);\n", /*physic*/ pstructure->name.chars(), domain->name.chars());

		// 	  for (k=0; k<domain->grids.size();k++)
		// 	    {
		// 	      grid=domain->grids(k);
		//      fprintf(pfile,"%s.setGrid(%s);\n",domain->name.chars(),grid->name.chars());

		for (l = 0; l < domain->nodes.size(); l++)
		{
			pnd = domain->nodes(l);
			fprintf(pfile, "%s.createNode(%ld, %14.7E, %14.7E, %14.7E);\n", pstructure->name.chars(), pnd->number, pnd->coords(0), pnd->coords(1), pnd->coords(2));
			//	}
			//	    }
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::elementsWrite()
//-----------------------------------------------------------------------------
{
	Indice i, j, l, m;
	// Physic* physic;
	Domain *domain;
	// Grid* grid;
	Element *pel;
	String elType[ElementsNumber + 1];
	int eelType;

	sepWrite("Elements write");

	if (pstructure->getElementsNumber() == 0)
		return;

	for (i = 0; i < ElementsNumber + 1; i++)
		elType[i] = "";

	//   for (i=0; i<pstructure->physics.size();i++)
	//     {
	//       physic=pstructure->physics(i);
	for (j = 0; j < pstructure->domains.size(); j++)
	{
		domain = pstructure->domains(j);

		// 	  for (k=0; k<domain->grids.size();k++)
		// 	    {
		// 	      grid=domain->grids(k);

		for (l = 0; l < domain->elements.size(); l++)
		{
			pel = domain->elements(l);

			elType[pel->getType()] = pel->getName();

			//		}
			//	    }
		}
	}

	for (i = 0; i < ElementsNumber + 1; i++)
	{
		if (elType[i] != "")
		{
			fprintf(pfile, "%s pel_%s;\n", elType[i].chars(), elType[i].chars());
		}
	}

	eelType = pstructure->domains(0)->elements(0)->getType();
	fprintf(pfile, "\n%s.setDefaultElement(pel_%s);\n", pstructure->name.chars(), elType[eelType].chars());
	//   for (i=0; i<pstructure->physics.size();i++)
	//     {
	//       physic=pstructure->physics(i);
	//       if (physic->name!="defaultPhysic")
	//       fprintf(pfile,"%s.setPhysic(%s);\n",pstructure->name.chars(),physic->name.chars());
	for (j = 0; j < pstructure->domains.size(); j++)
	{
		domain = pstructure->domains(j);
		if (domain->name != "defaultDomain")
			fprintf(pfile, "%s.setDomain(%s);\n", /*physic*/ pstructure->name.chars(), domain->name.chars());

		// 	  for (k=0; k<domain->grids.size();k++)
		// 	    {
		// 	      grid=domain->grids(k);
		//	      fprintf(pfile,"%s.setGrid(%s);\n",domain->name.chars(),grid->name.chars());

		for (l = 0; l < domain->elements.size(); l++)
		{
			pel = domain->elements(l);
			if (pel->getType() != eelType)
			{
				eelType = pel->getType();
				fprintf(pfile, "%s.setDefaultElement(pel_%s);\n", pstructure->name.chars(), elType[eelType].chars());
			}

			fprintf(pfile, "%s.createElement(%ld", pstructure->name.chars(), pel->number);
			for (m = 0; m < pel->nodes.size(); m++)
			{
				fprintf(pfile, ", %ld", pel->nodes(m)->number);
			}
			fprintf(pfile, ");\n");
			//		}
			//	    }
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::nodeAndElementSetWrite()
//-----------------------------------------------------------------------------
{
	ElementSet *eset;
	NodeSet *nset;
	Indice j, l, m;
	//  Physic* physic;
	Domain *domain;
	//  Grid* grid;
	Element *pel;
	Node *pnd;
	Indice previous, first;
	Object *ob;

	sepWrite("Node sets write");
	ob = objectList.getFirstObject(DYG_NodeSet);
	while (ob != NULL)
	//  for (ob = 0; ob < objectList.list.size (); ob++)
	{
		//      if (objectList.getObject (ob)->getObjectType() == DYG_NodeSet)
		//	{
		nset = (NodeSet *)ob->object;
		fprintf(pfile, "\n%s %s;\n", ob->getObjectClass().chars(), nset->name.chars());

		//       for (i=0; i<pstructure->physics.size();i++)
		// 	{
		// 	  physic=pstructure->physics(i);
		// 	  if (physic->name!="defaultPhysic")
		// 	    fprintf(pfile,"%s.setPhysic(%s);\n",pstructure->name.chars(),physic->name.chars());
		for (j = 0; j < pstructure->domains.size(); j++)
		{
			domain = pstructure->domains(j);
			if (domain->name != "defaultDomain")
				fprintf(pfile, "%s.setDomain(%s);\n", pstructure->name.chars(), domain->name.chars());

			// 	      for (k=0; k<domain->grids.size();k++)
			// 		{
			first = -1;
			// 		  grid=domain->grids(k);
			for (l = 0; l < domain->nodes.size(); l++)
			{
				pnd = domain->nodes(l);
				//  if (nset->nodes.AppN(pnd->Id)!=NULL)
				for (m = 0; m < nset->size(); m++)
				{
					if (pnd == nset->getNode(m))
					{
						if (first == -1)
						{
							first = previous = pnd->number;
						}
						else
						{
							if (previous == pnd->number - 1)
							{
								previous = pnd->number;
							}
							else
							{
								if (first != previous)
									fprintf(pfile, "%s.add(%ld,%ld);\n", nset->name.chars(), first, previous);
								else
									fprintf(pfile, "%s.add(%ld);\n", nset->name.chars(), first);
								first = previous = pnd->number;
							}
						}
						m = nset->size();
					}
				}
			}
			if (first != -1)
				if (first != previous)
					fprintf(pfile, "%s.add(%ld,%ld);\n", nset->name.chars(), first, previous);
				else
					fprintf(pfile, "%s.add(%ld);\n", nset->name.chars(), first);
			//		}
			//	    }
		}
		ob = objectList.getNextObject();
	}

	sepWrite("Element sets write");
	ob = objectList.getFirstObject(DYG_ElementSet);
	while (ob != NULL)
	//  for (ob = 0; ob < objectList.list.size (); ob++)
	{
		//     if (objectList.getObject (ob)->getObjectType() == DYG_ElementSet)
		//	{
		eset = (ElementSet *)ob->object;
		fprintf(pfile, "\n%s %s;\n", ob->getObjectClass().chars(), eset->name.chars());

		//      for (i=0; i<pstructure->physics.size();i++)
		// 	{
		// 	  physic=pstructure->physics(i);
		// 	  if (physic->name!="defaultPhysic")
		// 	    fprintf(pfile,"%s.setPhysic(%s);\n",pstructure->name.chars(),physic->name.chars());
		for (j = 0; j < pstructure->domains.size(); j++)
		{
			domain = pstructure->domains(j);
			if (domain->name != "defaultDomain")
				fprintf(pfile, "%s.setDomain(%s);\n", pstructure->name.chars(), domain->name.chars());

			// 	      for (k=0; k<domain->grids.size();k++)
			// 		{
			first = -1;
			// 		  grid=domain->grids(k);
			for (l = 0; l < domain->elements.size(); l++)
			{
				pel = domain->elements(l);
				//  if (eset->elements.AppN(pel->Id)!=NULL)
				for (m = 0; m < eset->size(); m++)
				{
					if (pel == eset->getElement(m))
					{
						if (first == -1)
						{
							first = previous = pel->number;
						}
						else
						{
							if (previous == pel->number - 1)
							{
								previous = pel->number;
							}
							else
							{
								if (first != previous)
									fprintf(pfile, "%s.add(%ld,%ld);\n", eset->name.chars(), first, previous);
								else
									fprintf(pfile, "%s.add(%ld);\n", eset->name.chars(), first);
								first = previous = pel->number;
							}
						}
						m = eset->size();
					}
				}
			}
			if (first != -1)
				if (first != previous)
					fprintf(pfile, "%s.add(%ld,%ld);\n", eset->name.chars(), first, previous);
				else
					fprintf(pfile, "%s.add(%ld);\n", eset->name.chars(), first);
			//		}
			//	    }
		}
		ob = objectList.getNextObject();
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::materialsWrite()
//-----------------------------------------------------------------------------
{
	Indice i, j;
	Material *pmat;
	ElementSet *eset;
	String name;

	sepWrite("Materials write");

	// isotropic hardening definition
	Object *ob = objectList.getFirstObjectDerivatedFrom(DYG_IsotropicHardening);
	while (ob != NULL)
	{
		IsotropicHardening *is = (IsotropicHardening *)ob->object;
		fprintf(pfile, "%s %s;\n", is->Name(), is->name.chars());
		fprintf(pfile, "%s\n", is->convertToDynELASourceFile(is->name).chars());
		ob = objectList.getNextObject();
	}

	for (i = 0; i < pstructure->materials.size(); i++)
	{
		pmat = pstructure->materials(i);

		name = objectList.getObjectName((Object *)pmat);
		if (name == "")
		{
			char nm[50];
			sprintf(nm, "generated_material_%ld", i);
		}

		// declaration
		fprintf(pfile, "Material %s;\n", name.chars());

		// parametres standard
		fprintf(pfile, "%s", pmat->convertToDynELASourceFile(name).chars());

		// groupes d'�l�ments associ� au mat�riau
		for (j = 0; j < pmat->elementsSet.size(); j++)
		{
			eset = pmat->elementsSet(j);
			fprintf(pfile, "%s.attachMaterialToElements(%s,%s);\n", pstructure->name.chars(), name.chars(), eset->name.chars());
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::solversWrite()
//-----------------------------------------------------------------------------
{
	Indice j, k;
	//  Physic* physic;
	Domain *domain;
	Solver *psolver;
	String name;

	sepWrite("FEM Solvers");
	//   for (i=0; i<pstructure->physics.size();i++)
	//     {
	//       physic=pstructure->physics(i);
	//          if (physic->name!="defaultPhysic")
	//    fprintf(pfile,"%s.setPhysic(%s);\n",pstructure->name.chars(),physic->name.chars());
	for (j = 0; j < pstructure->domains.size(); j++)
	{
		domain = pstructure->domains(j);
		if (domain->name != "defaultDomain")
			fprintf(pfile, "%s.setDomain(%s);\n", pstructure->name.chars(), domain->name.chars());

		for (k = 0; k < domain->solvers.size(); k++)
		{
			psolver = domain->solvers(k);

			name = objectList.getObjectName((Object *)psolver);

			fprintf(pfile, "%s %s;\n", psolver->Name(), name.chars());
			fprintf(pfile, "%s.setTimes(%12.7E, %12.7E);\n", name.chars(), psolver->getStartTime(), psolver->getEndTime());
			if ((psolver->startIncrement != 0) || (psolver->endIncrement != 0))
				fprintf(pfile, "%s.setIncrements(%ld, %ld);\n", name.chars(), psolver->startIncrement, psolver->endIncrement);
			fprintf(pfile, "%s.addSolver(%s);\n", pstructure->name.chars(), name.chars());
		}
		//	}
	}

	fprintf(pfile, "%s.setSaveTimes(%12.7E, %12.7E, %12.7E);\n", pstructure->name.chars(), pstructure->startSaveTime, pstructure->endSaveTime, pstructure->saveTime);
}

//-----------------------------------------------------------------------------
void ExportDynELA::historyFilesWrite()
//-----------------------------------------------------------------------------
{
	Indice j, k, l;
	//  Physic* physic;
	Domain *domain;
	HistoryFile *phist;
	String name;

	sepWrite("History Files");
	//   for (i=0; i<pstructure->physics.size();i++)
	//     {
	//       physic=pstructure->physics(i);
	//       if (physic->name!="defaultPhysic")
	// 	fprintf(pfile,"%s.setPhysic(%s);\n",pstructure->name.chars(),physic->name.chars());
	for (j = 0; j < pstructure->domains.size(); j++)
	{
		domain = pstructure->domains(j);
		if (domain->name != "defaultDomain")
			fprintf(pfile, "%s.setDomain(%s);\n", pstructure->name.chars(), domain->name.chars());
		for (k = 0; k < domain->historyFiles.size(); k++)
		{
			phist = domain->historyFiles(k);

			name = objectList.getObjectName((Object *)phist);
			fprintf(pfile, "HistoryFile %s;\n", name.chars());
			fprintf(pfile, "%s.addHistoryFile(%s);\n", pstructure->name.chars(),
					name.chars());
			fprintf(pfile, "%s.setName(\"%s\");\n", name.chars(),
					phist->getName().chars());
			fprintf(pfile, "%s.setSaveTime(%10.7E,%10.7E,%10.7E);\n", name.chars(),
					phist->getStartTime(),
					phist->getStopTime(),
					phist->getSaveTime());
			for (l = 0; l < phist->getFieldSize(); l++)
			{
				fprintf(pfile, "%s.append(%s,\"%s\",%ld);\n", name.chars(),
						phist->getNodeSet(l)->name.chars(),
						phist->getField(l).chars(),
						phist->getComponent(l));
			}
		}
		//	}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::functionsWrite()
//-----------------------------------------------------------------------------
{
	//  Indice i;
	Function *func;

	sepWrite("Functions section");
	Object *ob = objectList.getFirstObjectDerivatedFrom(DYG_Function);
	while (ob != NULL)
	{
		func = (Function *)ob->object;
		fprintf(pfile, "%s", func->convertToDynELASourceFile().chars());
		ob = objectList.getNextObject();
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::boundaryWrite()
//-----------------------------------------------------------------------------
{
	boundaryRestrainWrite();
	boundaryDisplacementWrite();
	boundarySpeedWrite();
	boundaryAcceleration();
	boundaryForceWrite();
	boundaryTemperatureWrite();
	boundaryFluxWrite();
	boundaryConvectionWrite();
}

//-----------------------------------------------------------------------------
void ExportDynELA::boundarySpeedWrite()
//-----------------------------------------------------------------------------
{
	Indice ob, i;
	BoundarySpeed *pbo;
	String name;
	Vec3D vec;

	sepWrite("Boundary Speed conditions");
	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_BoundarySpeed)
		{
			pbo = (BoundarySpeed *)objectList.getObject(ob)->object;
			name = objectList.getObjectName((Object *)pbo);

			fprintf(pfile, "BoundarySpeed %s;\n", name.chars());

			vec = pbo->get();
			fprintf(pfile, "%s.set(%12.7E, %12.7E, %12.7E);\n", name.chars(), vec(0), vec(1), vec(2));
			if (pbo->getFunction() != NULL)
				fprintf(pfile, "%s.setFunction(%s);\n", name.chars(), pbo->getFunction()->name.chars());
			for (i = 0; i < pbo->nodeSet.size(); i++)
			{
				if (pbo->typeSet(i) == 0)
					fprintf(pfile, "%s.attachInitialBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
				else
					fprintf(pfile, "%s.attachConstantBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::boundaryDisplacementWrite()
//-----------------------------------------------------------------------------
{
	Indice ob, i;
	BoundaryDisplacement *pbo;
	String name;
	Vec3D vec;

	sepWrite("Boundary Displacement conditions");
	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_BoundaryDisplacement)
		{
			pbo = (BoundaryDisplacement *)objectList.getObject(ob)->object;
			name = objectList.getObjectName((Object *)pbo);

			fprintf(pfile, "BoundaryDisplacement %s;\n", name.chars());

			vec = pbo->get();
			fprintf(pfile, "%s.set(%d, %d, %d);\n", name.chars(), (int)vec(0), (int)vec(1), (int)vec(2));
			if (pbo->getFunction() != NULL)
				fprintf(pfile, "%s.setFunction(%s);\n", name.chars(), pbo->getFunction()->name.chars());
			for (i = 0; i < pbo->nodeSet.size(); i++)
			{
				if (pbo->typeSet(i) == 0)
					fprintf(pfile, "%s.attachInitialBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
				else
					fprintf(pfile, "%s.attachConstantBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::boundaryRestrainWrite()
//-----------------------------------------------------------------------------
{
	Indice ob, i;
	BoundaryRestrain *pbo;
	String name;
	Vec3D vec;

	sepWrite("Boundary Restrain conditions");
	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_BoundaryRestrain)
		{
			pbo = (BoundaryRestrain *)objectList.getObject(ob)->object;
			name = objectList.getObjectName((Object *)pbo);

			fprintf(pfile, "BoundaryRestrain %s;\n", name.chars());

			vec = pbo->get();
			fprintf(pfile, "%s.set(%d, %d, %d);\n", name.chars(), (int)vec(0), (int)vec(1), (int)vec(2));
			for (i = 0; i < pbo->nodeSet.size(); i++)
			{
				if (pbo->typeSet(i) == 0)
					fprintf(pfile, "%s.attachInitialBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
				else
					fprintf(pfile, "%s.attachConstantBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::boundaryAcceleration()
//-----------------------------------------------------------------------------
{
	Indice ob, i;
	BoundaryAcceleration *pbo;
	String name;
	Vec3D vec;

	sepWrite("Boundary Acceleration conditions");
	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_BoundaryAcceleration)
		{
			pbo = (BoundaryAcceleration *)objectList.getObject(ob)->object;
			name = objectList.getObjectName((Object *)pbo);

			fprintf(pfile, "BoundaryAcceleration %s;\n", name.chars());

			vec = pbo->get();
			fprintf(pfile, "%s.set(%12.7E, %12.7E, %12.7E);\n", name.chars(), vec(0), vec(1), vec(2));
			if (pbo->getFunction() != NULL)
				fprintf(pfile, "%s.setFunction(%s);\n", name.chars(), pbo->getFunction()->name.chars());
			for (i = 0; i < pbo->nodeSet.size(); i++)
			{
				if (pbo->typeSet(i) == 0)
					fprintf(pfile, "%s.attachInitialBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
				else
					fprintf(pfile, "%s.attachConstantBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::boundaryForceWrite()
//-----------------------------------------------------------------------------
{
	Indice ob, i;
	BoundaryForce *pbo;
	String name;
	Vec3D vec;

	sepWrite("Boundary Force conditions");
	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_BoundaryForce)
		{
			pbo = (BoundaryForce *)objectList.getObject(ob)->object;
			name = objectList.getObjectName((Object *)pbo);

			fprintf(pfile, "BoundaryForce %s;\n", name.chars());

			vec = pbo->get();
			fprintf(pfile, "%s.set(%12.7E, %12.7E, %12.7E);\n", name.chars(), vec(0), vec(1), vec(2));
			if (pbo->getFunction() != NULL)
				fprintf(pfile, "%s.setFunction(%s);\n", name.chars(), pbo->getFunction()->name.chars());
			for (i = 0; i < pbo->nodeSet.size(); i++)
			{
				if (pbo->typeSet(i) == 0)
					fprintf(pfile, "%s.attachInitialBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
				else
					fprintf(pfile, "%s.attachConstantBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::boundaryTemperatureWrite()
//-----------------------------------------------------------------------------
{
	Indice ob, i;
	BoundaryTemperature *pbo;
	String name;
	Real vec;

	sepWrite("Boundary Temperature conditions");
	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_BoundaryTemperature)
		{
			pbo = (BoundaryTemperature *)objectList.getObject(ob)->object;
			name = objectList.getObjectName((Object *)pbo);

			fprintf(pfile, "BoundaryTemperature %s;\n", name.chars());

			vec = pbo->get();
			fprintf(pfile, "%s.set(%12.7E);\n", name.chars(), vec);
			if (pbo->getFunction() != NULL)
				fprintf(pfile, "%s.setFunction(%s);\n", name.chars(), pbo->getFunction()->name.chars());
			for (i = 0; i < pbo->nodeSet.size(); i++)
			{
				if (pbo->typeSet(i) == 0)
					fprintf(pfile, "%s.attachInitialBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
				else
					fprintf(pfile, "%s.attachConstantBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::boundaryFluxWrite()
//-----------------------------------------------------------------------------
{
	Indice ob, i;
	BoundaryFlux *pbo;
	String name;
	Real vec;

	sepWrite("Boundary Flux conditions");
	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_BoundaryFlux)
		{
			pbo = (BoundaryFlux *)objectList.getObject(ob)->object;
			name = objectList.getObjectName((Object *)pbo);

			fprintf(pfile, "BoundaryFlux %s;\n", name.chars());

			vec = pbo->get();
			fprintf(pfile, "%s.set(%12.7E);\n", name.chars(), vec);
			if (pbo->getFunction() != NULL)
				fprintf(pfile, "%s.setFunction(%s);\n", name.chars(), pbo->getFunction()->name.chars());
			for (i = 0; i < pbo->nodeSet.size(); i++)
			{
				if (pbo->typeSet(i) == 0)
					fprintf(pfile, "%s.attachInitialBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
				else
					fprintf(pfile, "%s.attachConstantBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::boundaryConvectionWrite()
//-----------------------------------------------------------------------------
{
	Indice ob, i;
	BoundaryConvection *pbo;
	String name;
	Vec3D vec;

	sepWrite("Boundary Convection conditions");
	for (ob = 0; ob < objectList.list.size(); ob++)
	{
		if (objectList.getObject(ob)->getObjectType() == DYG_BoundaryConvection)
		{
			pbo = (BoundaryConvection *)objectList.getObject(ob)->object;
			name = objectList.getObjectName((Object *)pbo);

			fprintf(pfile, "BoundaryConvection %s;\n", name.chars());

			vec = pbo->get();
			fprintf(pfile, "%s.set(%12.7E, %12.7E);\n", name.chars(), vec(0), vec(1));
			if (pbo->getFunction() != NULL)
				fprintf(pfile, "%s.setFunction(%s);\n", name.chars(), pbo->getFunction()->name.chars());
			for (i = 0; i < pbo->nodeSet.size(); i++)
			{
				if (pbo->typeSet(i) == 0)
					fprintf(pfile, "%s.attachInitialBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
				else
					fprintf(pfile, "%s.attachConstantBC(%s,%s);\n", pstructure->name.chars(), name.chars(), pbo->nodeSet(i)->name.chars());
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::structure_read()
//-----------------------------------------------------------------------------
{
	fatalError("ExportDynELA::structure_read", "Method not available");
}

//-----------------------------------------------------------------------------
void ExportDynELA::renumerateNodesAndElements()
//-----------------------------------------------------------------------------
{
	Indice j, l;
	//  Physic* physic;
	Domain *domain;
	//  Grid* grid;

	//   for (i=0; i<pstructure->physics.size();i++)
	//     {
	//       physic=pstructure->physics(i);
	for (j = 0; j < pstructure->domains.size(); j++)
	{
		domain = pstructure->domains(j);

		// 	  for (k=0; k<domain->grids.size();k++)
		// 	    {
		// 	      grid=domain->grids(k);
		for (l = 0; l < domain->nodes.size(); l++)
		{
			domain->nodes(l)->number = l + 1;
		}
		//	      grid->nodes.compact();
		for (l = 0; l < domain->elements.size(); l++)
		{
			domain->elements(l)->number = l + 1;
		}
		//	      grid->elements.compact();
		// 	    }
		// 	}
	}
}

//-----------------------------------------------------------------------------
void ExportDynELA::structure_write()
//-----------------------------------------------------------------------------
{
	if ((pfile = fopen(filename.chars(), "w")) == NULL)
	{
		fatalError("void ExportDynELA::structure_write()",
				   "Unable to write data file");
		exit(-1);
	}

	//  pstructure->sortElementsAndNodes();
	// renumerotation
	// renumerateNodesAndElements();

	// entete de fichier
	//cout << "heading\n";
	headingWrite();

	// m�moire
	//cout << "memory\n";
	memoryVariablesWrite();

	// structure
	//cout << "structure\n";
	structureWrite();

	// noeuds
	//cout << "nodes\n";
	nodesWrite();

	// noeuds
	//cout << "elements\n";
	elementsWrite();

	// nodes et elements set
	//cout << "sets\n";
	nodeAndElementSetWrite();

	// fonctions
	//cout << "functions\n";
	functionsWrite();

	// mat�riaux
	//cout << "materials\n";
	materialsWrite();

	// boundary conditions
	//cout << "boundaries\n";
	boundaryWrite();

	// mat�riaux
	//cout << "solvers\n";
	solversWrite();

	historyFilesWrite();

	sepWrite("End of file");
	fprintf(pfile, "END;\n");

	fclose(pfile);
}
