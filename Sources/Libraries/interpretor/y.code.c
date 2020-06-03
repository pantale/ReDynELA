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

// begin date :
// revision date :

#include <linearAlgebra.h>

//-----------------------------------------------------------------------------
int InputDynerror(const char *msg)
//-----------------------------------------------------------------------------
{
	cerr << msg << " on line " << scanned_line[current_scanned_file] << " file " << scanned_file[current_scanned_file] << endl;
	exit(-1);
	return Fail;
}

//-----------------------------------------------------------------------------
FILE *parsingFileDataLevel0(String name)
//-----------------------------------------------------------------------------
{
	FILE *pfile[150];
	String currentName[150];
	int currentLine[150];
	int currentFile = 0;
	FILE *pfileOut;
	int ch;
	String st;
	Boolean treat;
	treat = True;

	if ((pfile[currentFile] = fopen(name.chars(), "r")) == NULL)
	{
		fatalError("Unable to read data file", "%s\n", name.chars());
		exit(-1);
	}

	if ((pfileOut = fopen(".DynELA_input", "w")) == NULL)
	{
		fatalError("Unable to write temporary data file", "%s\n", ".DynELA_input");
		exit(-1);
	}

	currentName[currentFile] = name;
	currentLine[currentFile] = 0;

	// copie et traitement niveau 0
	while (!(((ch = getc(pfile[currentFile])) == EOF) && (currentFile == 0)))
	{
		switch (ch)
		{

		case '\n':
		{
			currentLine[currentFile]++;
			treat = True;
			putc(ch, pfileOut);
		}
		break;

		case EOF:
		{
			fclose(pfile[currentFile]);
			currentFile--;
			fprintf(pfileOut, "DYNELA_END_INCLUDE ;\n");
		}
		break;

		case '#':
		{
			if (treat == False)
			{
				putc(ch, pfileOut);
				break;
			}
			st.scanFileLine(pfile[currentFile]);
			currentLine[currentFile]++;
			st.strip();

			if (st.before(7) == "include")
			{
				// traitement d'un include
				String st2;
				st2 = st.after(6);
				st2.strip();
				if (st2.contains("<") == False)
					fatalError("Syntax Error in #include",
							   "Preprocessing phase 0 aborded\nfile : %s\nline : %d\n",
							   currentName[currentFile].chars(), currentLine[currentFile]);
				st2 = st2.after("<");
				if (st2.contains(">") == False)
					fatalError("Syntax Error in #include",
							   "Preprocessing phase 0 aborded\nfile : %s\nline : %d\n",
							   currentName[currentFile].chars(), currentLine[currentFile]);
				st2 = st2.before(">");
				currentFile++;
				currentName[currentFile] = st2;
				currentLine[currentFile] = 0;
				if (currentFile >= 150)
					fatalError("Too many include files encountered",
							   "Program stopped because %d includes files where scanned\nMay be there's a recursive inclusion of files\nCheck our sources\n",
							   currentFile);
				scanned_line[currentFile] = 0;
				fprintf(pfileOut, "DYNELA_BEGIN_INCLUDE \"%s\";\n", st2.chars());
				if ((pfile[currentFile] = fopen(st2.chars(), "r")) == NULL)
				{
					fatalError("Unable to read data file",
							   "file : %s\nincluded from %s at line %d",
							   st2.chars(), currentName[currentFile - 1].chars(), currentLine[currentFile - 1]);
					exit(-1);
				}
			}
			else
			{
				fprintf(pfileOut, "#%s\n", st.chars());
			}
		}
		break;

		default:
			putc(ch, pfileOut);
			if ((ch != '\t') || (ch != ' '))
				treat = False;

			break;
		}
	}
	fclose(pfileOut);

	// reouverture fichier
	pfileOut = fopen(".DynELA_input", "r");
	return pfileOut;
}

//-----------------------------------------------------------------------------
void parseInputDyn(/*String filename, */ char typeOfPreprocessing)
//-----------------------------------------------------------------------------
{
	FILE *pfile;
	Indice command;
	FilesFluxes filesFluxes;
	FILE *currentOutFile;
	Indice for0[10], for1[10], for2[10], for3[10];
	int levelFor = 0;

	// start level 0 parsing of file for macros and includes
	pfile = parsingFileDataLevel0(parsedFileName);

	scanned_file[current_scanned_file] = parsedFileName;

	// associate file
	InputDynin = pfile;
	currentOutFile = NULL;

	endOfFile = False;
	cout << "Start parsing source file\n";

	parenLevel = 0;

	while (endOfFile == False && yyparse() == Success)
	{

		levelFor = 0;
		// start the read buffer
		inputDyn->currentBufferIndice = 0;
		while ((command = inputDyn->bufferGetCommand()) != CMDyn_END)
		{
			switch (command)
			{

			case CMDyn_PAREN_BEGIN:
			{
				// increase the level
				parenLevel++;
			}
			break;

			case CMDyn_PAREN_END:
			{
				// decrease the level
				parenLevel--;

				// delete all objects with level up
				objectList.deleteUpLevel(parenLevel);
			}
			break;

			case REAL:
			{
				Class_Real *obj = new Class_Real;
				(*(Real *)obj->object) = inputDyn->bufferGetReal();
				inputDyn->stackObjects.push(obj);
			}
			break;

			case STRING_CREATE:
			{
				Class_String *obj = new Class_String;
				(*(String *)obj->object) = inputDyn->bufferGetString();
				inputDyn->stackObjects.push(obj);
			}
			break;

			case INTEGER:
			{
				Class_Real *obj = new Class_Real;
				(*(Real *)obj->object) = (Real)inputDyn->bufferGetInteger();
				inputDyn->stackObjects.push(obj);
			}
			break;

			case CMDyn_FOPEN:
			{
				String fluxName, fluxFile;
				FILE *outFile;
				fluxName = inputDyn->bufferGetString();
				fluxFile = inputDyn->bufferGetString();
				if (!(outFile = fopen(fluxFile.chars(), "w")))
					fatalError("Cannot write on file", "%s", fluxFile.chars());
				filesFluxes.addFlux(fluxName, outFile);
			}
			break;

			case CMDyn_FOPEN_APPEND:
			{
				String fluxName, fluxFile;
				FILE *outFile;
				fluxName = inputDyn->bufferGetString();
				fluxFile = inputDyn->bufferGetString();
				if (!(outFile = fopen(fluxFile.chars(), "a")))
					fatalError("Cannot append on file", "%s", fluxFile.chars());
				filesFluxes.addFlux(fluxName, outFile);
			}
			break;

			case CMDyn_FCLOSE:
				fclose(filesFluxes.getFlux(inputDyn->bufferGetString()));
				break;

			case CMDyn_OUTPUT:
			{
				String fluxName = inputDyn->bufferGetString();
				if (fluxName == "cout")
					currentOutFile = stdout;
				else
					currentOutFile = filesFluxes.getFlux(fluxName);
			}
			break;

			case CMDyn_OUTPUT_VAL:
			{
				Object *obj = inputDyn->stackObjects.pop();
				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					fprintf(currentOutFile, "%10.5E", *(Real *)obj->object);
				}
				break;

				case DYG_String:
				{
					String st = *(String *)obj->object;
					while (st.contains("\\n"))
					{
						int ind;
						ind = st.search("\\n");
						st = st.before(ind) + '\n' + st.after(ind + 1);
					}
					fprintf(currentOutFile, "%s", st.chars());
				}
				break;

				default:
				{
					fatalError("CMDyn_OUTPUT", "Don't know how to operate << on %s\n", obj->getObjectClass().chars());
				}
				}
			}
			break;

			case CMDyn_OUTPUT_END:
			{
				currentOutFile = NULL;
			}
			break;

			case CMDyn_IF_START:
			{
				// condition d'execution
				Object *obj;
				obj = inputDyn->stackObjects.pop();
				if (obj->getObjectType() != DYG_Real)
				{
					fatalError("CMDyn_IF_START", "Object %s not allowed\n",
							   obj->getObjectClass().chars());
				}

				// get the boolean value
				Boolean ex = (Boolean)(*(Real *)(obj->object));

				// skip to next
				if (!ex)
				{
					inputDyn->bufferNextCommand(CMDyn_IF);
				}
			}
			break;

			case CMDyn_IF:
			{
			}
			break;

			case CMDyn_FOR_START:
			{
				levelFor++;
				for0[levelFor] = inputDyn->currentBufferIndice;
				for1[levelFor] = for2[levelFor] = for3[levelFor] = 0;
			}
			break;

			case CMDyn_FOR_START1:
			{
				for1[levelFor] = inputDyn->currentBufferIndice;

				// condition d'execution
				Object *obj;
				obj = inputDyn->stackObjects.pop();
				if (obj->getObjectType() != DYG_Real)
				{
					fatalError("CMDyn_FOR_START", "Object %s not allowed\n",
							   obj->getObjectClass().chars());
				}

				// get the boolean value
				Boolean ex = (Boolean)(*(Real *)obj->object);

				if (ex)
				{
					inputDyn->bufferNextCommand(CMDyn_FOR_START2);
				}
				else
				{
					inputDyn->bufferNextCommand(CMDyn_FOR_END);
					levelFor--;
				}
			}
			break;

			case CMDyn_FOR_START2:
			{
				for2[levelFor] = inputDyn->currentBufferIndice;
				inputDyn->currentBufferIndice = for0[levelFor];
			}
			break;

			case CMDyn_FOR_END:
			{
				for3[levelFor] = inputDyn->currentBufferIndice;
				inputDyn->currentBufferIndice = for1[levelFor];
			}
			break;

			case STACK_FLUSH:
			{
				inputDyn->stackObjects.flush();
			}
			break;

			case CMDyn_PRAGMA:
			{
				String arg = inputDyn->bufferGetString();
				Boolean ok = False;
				if (arg == "radians")
				{
					inputDyn->angleDef = InputDyn::Radians;
					ok = True;
				}
				if (arg == "degrees")
				{
					inputDyn->angleDef = InputDyn::Degrees;
					ok = True;
				}
				if (arg == "debugLanguage on")
				{
					inputDyn->traceLanguage = True;
					if (inputDyn->pfileTraceLanguage == NULL)
					{
						inputDyn->pfileTraceLanguage = fopen("interpretor.trace", "w");
					}
					ok = True;
				}

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> #pragma %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							arg.chars());

				if (arg == "debugLanguage off")
				{
					inputDyn->traceLanguage = False;
					ok = True;
				}

				if (!ok)
				{
					warning("Unknown #pragma directive", "argument is \"%s\" at line %ld of file\n", arg.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars());
				}
			}
			break;

			case CMDyn_QUIT:
				endOfFile = True;
				break;

			case CMDyn_QUIT_PREPROCESSOR:
				if (typeOfPreprocessing == InputDyn::Solve)
				{
					endOfFile = True;
				}
				// si on est en mode extraction de r�sultats,
				// il faut appliquer un initSolve
				if (typeOfPreprocessing == InputDyn::Extract)
				{
					Global_Structure->initSolve();
				}
				break;

			case CMDyn_QUIT_EXTRACT:
				if (typeOfPreprocessing == InputDyn::Extract)
				{
					endOfFile = True;
				}
				break;

			case UNARY_OPERATOR:
			{
				// operator to use
				String op = inputDyn->bufferGetString();

				// get the arguments
				Object *obj = inputDyn->stackObjects.pop();

				if (op == "ABS")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = Abs(*(Real *)obj->object);
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply ABS operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "INT")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = Indice(*(Real *)obj->object);
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply INT operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "ROUND")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = Indice(*(Real *)obj->object + 0.5);
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply ROUND operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "SQRT")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = sqrt(*(Real *)(obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply SQRT operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "SIN")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = sin((inputDyn->angleDef == InputDyn::Degrees ? DegToRad : 1) * (*(Real *)obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply SIN operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "COS")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = cos((inputDyn->angleDef == InputDyn::Degrees ? DegToRad : 1) * (*(Real *)obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply COS operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "TAN")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = tan((inputDyn->angleDef == InputDyn::Degrees ? DegToRad : 1) * (*(Real *)obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply TAN operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "ASIN")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (inputDyn->angleDef == InputDyn::Degrees ? RadToDeg : 1) * asin((*(Real *)obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply ASIN operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "ACOS")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (inputDyn->angleDef == InputDyn::Degrees ? RadToDeg : 1) * acos((*(Real *)obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply ACOS operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "ATAN")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (inputDyn->angleDef == InputDyn::Degrees ? RadToDeg : 1) * atan((*(Real *)obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply ATAN operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "SINH")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = sinh((inputDyn->angleDef == InputDyn::Degrees ? DegToRad : 1) * (*(Real *)obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply SINH operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "COSH")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = cosh((inputDyn->angleDef == InputDyn::Degrees ? DegToRad : 1) * (*(Real *)obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply COSH operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "TANH")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = tanh((inputDyn->angleDef == InputDyn::Degrees ? DegToRad : 1) * (*(Real *)obj->object));
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply TANH operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "LN")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = log(*(Real *)obj->object);
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply LN operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "LOG")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = log10(*(Real *)obj->object);
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply LOG operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "EXP")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = exp(*(Real *)obj->object);
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply EXP operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "ERF")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = erf(*(Real *)obj->object);
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply ERF operator on %s\n", obj->getObjectClass().chars());
					}
				}

				if (op == "!")
				{
					if (obj->getObjectType() == DYG_Real)
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = !(*(Real *)obj->object);
						inputDyn->stackObjects.push(sol);
					}
					else
					{
						fatalError("UNARY_OPERATOR", "Don't know how to apply ! operator on %s\n", obj->getObjectClass().chars());
					}
				}

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s : operator %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj->getObjectName().chars(),
							op.chars());
			}
			break;

			case NEG_OPERATOR:
			{
				// get the arguments
				Object *obj = inputDyn->stackObjects.pop();

				if (obj->getObjectType() == DYG_Real)
				{
					Class_Real *sol = new Class_Real;
					(*(Real *)sol->object) = -(*(Real *)obj->object);
					inputDyn->stackObjects.push(sol);
				}
				else
				{
					fatalError("NEG_OPERATOR", "Don't know how to apply - operator on %s\n", obj->getObjectClass().chars());
				}
			}
			break;

			case VARIABLE_PLUSEGAL:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("Object not existing", "%s at line %d", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// get the arguments
				Object *arg = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s : operator += applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());

				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					int typ = arg->getObjectType();
					if (typ == DYG_Real)
					{
						*(Real *)obj->object += *(Real *)arg->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_PLUSEGAL", "unable to convert from %s to %s",
								   arg->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				default:
					fatalError("VARIABLE_PLUSEGAL", "unable to operate on %s\n", obj->getObjectClass().chars());
				}
			}
			break;

			case VARIABLE_DIVEGAL:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("Object not existing", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// get the arguments
				Object *arg = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s : operator /= applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());

				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					int typ = arg->getObjectType();
					if (typ == DYG_Real)
					{
						if (*(Real *)arg->object == 0)
							InputDynerror("divide by zero");
						*(Real *)obj->object /= *(Real *)arg->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_DIVEGAL", "unable to convert from %s to %s",
								   arg->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				default:
					fatalError("VARIABLE_DIVEGAL", "unable to operate on %s\n", obj->getObjectClass().chars());
				}
			}
			break;

			case VARIABLE_MOINSEGAL:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("Object not existing", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// get the arguments
				Object *arg = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s : operator -= applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());

				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					int typ = arg->getObjectType();
					if (typ == DYG_Real)
					{
						*(Real *)obj->object -= *(Real *)arg->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_MOINSEGAL", "unable to convert from %s to %s",
								   arg->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				default:
					fatalError("VARIABLE_MOINSEGAL", "unable to operate on %s\n", obj->getObjectClass().chars());
				}
			}
			break;

			case VARIABLE_MULTEGAL:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("Object not existing", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// get the arguments
				Object *arg = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s : operator *= applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());

				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					int typ = arg->getObjectType();
					if (typ == DYG_Real)
					{
						*(Real *)obj->object *= *(Real *)arg->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_MULTEGAL", "unable to convert from %s to %s",
								   arg->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				default:
					fatalError("VARIABLE_MULTEGAL", "unable to operate on %s\n", obj->getObjectClass().chars());
				}
			}
			break;

			case VARIABLE_POSTINCREMENT:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("Object not existing", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s : operator ++ applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());

				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					Real val;
					val = *(Real *)obj->object;
					Class_Real *n = new Class_Real;
					(*(Real *)n->object) = val;
					inputDyn->stackObjects.push(n);
					(*(Real *)obj->object)++;
				}
				}
			}
			break;

			case VARIABLE_POSTDECREMENT:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("Object not existing", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s : operator -- applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());

				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					Real val;
					val = (*(Real *)obj->object);
					Class_Real *n = new Class_Real;
					(*(Real *)n->object) = val;
					inputDyn->stackObjects.push(n);
					(*(Real *)obj->object)--;
				}
				}
			}
			break;

			case VARIABLE_PREINCREMENT:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("Object not existing", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s : operator ++ applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());
				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					Real val;
					val = (*(Real *)obj->object);
					Class_Real *n = new Class_Real;
					(*(Real *)n->object) = val + 1;
					inputDyn->stackObjects.push(n);
					(*(Real *)obj->object) = (*(Real *)n->object);
				}
				}
			}
			break;

			case VARIABLE_PREDECREMENT:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("Object not existing", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s : operator -- applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());

				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					Real val;
					val = (*(Real *)obj->object);
					Class_Real *n = new Class_Real;
					(*(Real *)n->object) = val - 1;
					inputDyn->stackObjects.push(n);
					(*(Real *)obj->object) = (*(Real *)n->object);
				}
				}
			}
			break;

				/* Auto include part starts here */
			case CMDyn_DYG_Object_Real_Constructor:
			{

				// create the object
				Class_Real *tmpObject = new Class_Real;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Real::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Real_Pointer:
			{

				// point the object
				Class_Real *tmpObject = new Class_Real;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Real::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Boolean_Constructor:
			{

				// create the object
				Class_Boolean *tmpObject = new Class_Boolean;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Boolean::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Boolean_Pointer:
			{

				// point the object
				Class_Boolean *tmpObject = new Class_Boolean;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Boolean::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_String_Constructor:
			{

				// create the object
				Class_String *tmpObject = new Class_String;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object String::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_String_Pointer:
			{

				// point the object
				Class_String *tmpObject = new Class_String;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object String::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Node_Constructor:
			{

				// create the object
				Class_Node *tmpObject = new Class_Node;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Node::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Node_Pointer:
			{

				// point the object
				Class_Node *tmpObject = new Class_Node;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Node::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Domain_Constructor:
			{

				// create the object
				Class_Domain *tmpObject = new Class_Domain;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Domain::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Domain_Pointer:
			{

				// point the object
				Class_Domain *tmpObject = new Class_Domain;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Domain::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElQua4n2D_Constructor:
			{

				// create the object
				Class_ElQua4n2D *tmpObject = new Class_ElQua4n2D;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElQua4n2D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElQua4n2D_Pointer:
			{

				// point the object
				Class_ElQua4n2D *tmpObject = new Class_ElQua4n2D;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElQua4n2D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElHex8n3D_Constructor:
			{

				// create the object
				Class_ElHex8n3D *tmpObject = new Class_ElHex8n3D;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElHex8n3D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElHex8n3D_Pointer:
			{

				// point the object
				Class_ElHex8n3D *tmpObject = new Class_ElHex8n3D;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElHex8n3D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElTri3n2D_Constructor:
			{

				// create the object
				Class_ElTri3n2D *tmpObject = new Class_ElTri3n2D;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElTri3n2D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElTri3n2D_Pointer:
			{

				// point the object
				Class_ElTri3n2D *tmpObject = new Class_ElTri3n2D;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElTri3n2D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElQua4nAx_Constructor:
			{

				// create the object
				Class_ElQua4nAx *tmpObject = new Class_ElQua4nAx;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElQua4nAx::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElQua4nAx_Pointer:
			{

				// point the object
				Class_ElQua4nAx *tmpObject = new Class_ElQua4nAx;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElQua4nAx::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElTet4n3D_Constructor:
			{

				// create the object
				Class_ElTet4n3D *tmpObject = new Class_ElTet4n3D;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElTet4n3D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElTet4n3D_Pointer:
			{

				// point the object
				Class_ElTet4n3D *tmpObject = new Class_ElTet4n3D;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElTet4n3D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElTet10n3D_Constructor:
			{

				// create the object
				Class_ElTet10n3D *tmpObject = new Class_ElTet10n3D;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElTet10n3D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElTet10n3D_Pointer:
			{

				// point the object
				Class_ElTet10n3D *tmpObject = new Class_ElTet10n3D;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElTet10n3D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Vec3D_Constructor:
			{

				// create the object
				Class_Vec3D *tmpObject = new Class_Vec3D;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Vec3D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Vec3D_Pointer:
			{

				// point the object
				Class_Vec3D *tmpObject = new Class_Vec3D;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Vec3D::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Vector_Constructor:
			{

				// create the object
				Class_Vector *tmpObject = new Class_Vector;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Vector::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Vector_Pointer:
			{

				// point the object
				Class_Vector *tmpObject = new Class_Vector;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Vector::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Matrix_Constructor:
			{

				// create the object
				Class_Matrix *tmpObject = new Class_Matrix;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Matrix::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Matrix_Pointer:
			{

				// point the object
				Class_Matrix *tmpObject = new Class_Matrix;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Matrix::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Tensor2_Constructor:
			{

				// create the object
				Class_Tensor2 *tmpObject = new Class_Tensor2;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Tensor2::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Tensor2_Pointer:
			{

				// point the object
				Class_Tensor2 *tmpObject = new Class_Tensor2;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Tensor2::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Tensor3_Constructor:
			{

				// create the object
				Class_Tensor3 *tmpObject = new Class_Tensor3;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Tensor3::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Tensor3_Pointer:
			{

				// point the object
				Class_Tensor3 *tmpObject = new Class_Tensor3;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Tensor3::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Tensor4_Constructor:
			{

				// create the object
				Class_Tensor4 *tmpObject = new Class_Tensor4;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Tensor4::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Tensor4_Pointer:
			{

				// point the object
				Class_Tensor4 *tmpObject = new Class_Tensor4;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Tensor4::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Select_Constructor:
			{

				// create the object
				Class_Select *tmpObject = new Class_Select;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Select::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Select_Pointer:
			{

				// point the object
				Class_Select *tmpObject = new Class_Select;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Select::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_NodeSet_Constructor:
			{

				// create the object
				Class_NodeSet *tmpObject = new Class_NodeSet;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object NodeSet::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_NodeSet_Pointer:
			{

				// point the object
				Class_NodeSet *tmpObject = new Class_NodeSet;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object NodeSet::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElementSet_Constructor:
			{

				// create the object
				Class_ElementSet *tmpObject = new Class_ElementSet;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElementSet::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ElementSet_Pointer:
			{

				// point the object
				Class_ElementSet *tmpObject = new Class_ElementSet;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ElementSet::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Structure_Constructor:
			{

				// create the object
				Class_Structure *tmpObject = new Class_Structure;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Structure::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Structure_Pointer:
			{

				// point the object
				Class_Structure *tmpObject = new Class_Structure;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Structure::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_DiscreteFunction_Constructor:
			{

				// create the object
				Class_DiscreteFunction *tmpObject = new Class_DiscreteFunction;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object DiscreteFunction::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_DiscreteFunction_Pointer:
			{

				// point the object
				Class_DiscreteFunction *tmpObject = new Class_DiscreteFunction;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object DiscreteFunction::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_PolynomialFunction_Constructor:
			{

				// create the object
				Class_PolynomialFunction *tmpObject = new Class_PolynomialFunction;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object PolynomialFunction::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_PolynomialFunction_Pointer:
			{

				// point the object
				Class_PolynomialFunction *tmpObject = new Class_PolynomialFunction;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object PolynomialFunction::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_SinusFunction_Constructor:
			{

				// create the object
				Class_SinusFunction *tmpObject = new Class_SinusFunction;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object SinusFunction::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_SinusFunction_Pointer:
			{

				// point the object
				Class_SinusFunction *tmpObject = new Class_SinusFunction;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object SinusFunction::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_DiscreteFunctionSet_Constructor:
			{

				// create the object
				Class_DiscreteFunctionSet *tmpObject = new Class_DiscreteFunctionSet;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object DiscreteFunctionSet::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_DiscreteFunctionSet_Pointer:
			{

				// point the object
				Class_DiscreteFunctionSet *tmpObject = new Class_DiscreteFunctionSet;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object DiscreteFunctionSet::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_RampFunction_Constructor:
			{

				// create the object
				Class_RampFunction *tmpObject = new Class_RampFunction;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object RampFunction::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_RampFunction_Pointer:
			{

				// point the object
				Class_RampFunction *tmpObject = new Class_RampFunction;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object RampFunction::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Material_Constructor:
			{

				// create the object
				Class_Material *tmpObject = new Class_Material;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Material::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Material_Pointer:
			{

				// point the object
				Class_Material *tmpObject = new Class_Material;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Material::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardElastic_Constructor:
			{

				// create the object
				Class_IsoHardElastic *tmpObject = new Class_IsoHardElastic;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardElastic::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardElastic_Pointer:
			{

				// point the object
				Class_IsoHardElastic *tmpObject = new Class_IsoHardElastic;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardElastic::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardElastoplasticTabular_Constructor:
			{

				// create the object
				Class_IsoHardElastoplasticTabular *tmpObject = new Class_IsoHardElastoplasticTabular;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardElastoplasticTabular::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardElastoplasticTabular_Pointer:
			{

				// point the object
				Class_IsoHardElastoplasticTabular *tmpObject = new Class_IsoHardElastoplasticTabular;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardElastoplasticTabular::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardElastoplastic_Constructor:
			{

				// create the object
				Class_IsoHardElastoplastic *tmpObject = new Class_IsoHardElastoplastic;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardElastoplastic::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardElastoplastic_Pointer:
			{

				// point the object
				Class_IsoHardElastoplastic *tmpObject = new Class_IsoHardElastoplastic;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardElastoplastic::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardMetafor_Constructor:
			{

				// create the object
				Class_IsoHardMetafor *tmpObject = new Class_IsoHardMetafor;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardMetafor::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardMetafor_Pointer:
			{

				// point the object
				Class_IsoHardMetafor *tmpObject = new Class_IsoHardMetafor;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardMetafor::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardPower_Constructor:
			{

				// create the object
				Class_IsoHardPower *tmpObject = new Class_IsoHardPower;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardPower::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardPower_Pointer:
			{

				// point the object
				Class_IsoHardPower *tmpObject = new Class_IsoHardPower;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardPower::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardJohnsonCook_Constructor:
			{

				// create the object
				Class_IsoHardJohnsonCook *tmpObject = new Class_IsoHardJohnsonCook;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardJohnsonCook::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_IsoHardJohnsonCook_Pointer:
			{

				// point the object
				Class_IsoHardJohnsonCook *tmpObject = new Class_IsoHardJohnsonCook;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object IsoHardJohnsonCook::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Interface_Constructor:
			{

				// create the object
				Class_Interface *tmpObject = new Class_Interface;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Interface::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Interface_Pointer:
			{

				// point the object
				Class_Interface *tmpObject = new Class_Interface;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Interface::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryCondition_Constructor:
			{

				// create the object
				Class_BoundaryCondition *tmpObject = new Class_BoundaryCondition;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryCondition::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryCondition_Pointer:
			{

				// point the object
				Class_BoundaryCondition *tmpObject = new Class_BoundaryCondition;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryCondition::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundarySpeed_Constructor:
			{

				// create the object
				Class_BoundarySpeed *tmpObject = new Class_BoundarySpeed;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundarySpeed::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundarySpeed_Pointer:
			{

				// point the object
				Class_BoundarySpeed *tmpObject = new Class_BoundarySpeed;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundarySpeed::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryAcceleration_Constructor:
			{

				// create the object
				Class_BoundaryAcceleration *tmpObject = new Class_BoundaryAcceleration;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryAcceleration::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryAcceleration_Pointer:
			{

				// point the object
				Class_BoundaryAcceleration *tmpObject = new Class_BoundaryAcceleration;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryAcceleration::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryDisplacement_Constructor:
			{

				// create the object
				Class_BoundaryDisplacement *tmpObject = new Class_BoundaryDisplacement;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryDisplacement::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryDisplacement_Pointer:
			{

				// point the object
				Class_BoundaryDisplacement *tmpObject = new Class_BoundaryDisplacement;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryDisplacement::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryRestrain_Constructor:
			{

				// create the object
				Class_BoundaryRestrain *tmpObject = new Class_BoundaryRestrain;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryRestrain::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryRestrain_Pointer:
			{

				// point the object
				Class_BoundaryRestrain *tmpObject = new Class_BoundaryRestrain;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryRestrain::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryForce_Constructor:
			{

				// create the object
				Class_BoundaryForce *tmpObject = new Class_BoundaryForce;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryForce::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryForce_Pointer:
			{

				// point the object
				Class_BoundaryForce *tmpObject = new Class_BoundaryForce;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryForce::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryTemperature_Constructor:
			{

				// create the object
				Class_BoundaryTemperature *tmpObject = new Class_BoundaryTemperature;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryTemperature::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryTemperature_Pointer:
			{

				// point the object
				Class_BoundaryTemperature *tmpObject = new Class_BoundaryTemperature;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryTemperature::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryFlux_Constructor:
			{

				// create the object
				Class_BoundaryFlux *tmpObject = new Class_BoundaryFlux;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryFlux::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryFlux_Pointer:
			{

				// point the object
				Class_BoundaryFlux *tmpObject = new Class_BoundaryFlux;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryFlux::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryConvection_Constructor:
			{

				// create the object
				Class_BoundaryConvection *tmpObject = new Class_BoundaryConvection;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryConvection::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoundaryConvection_Pointer:
			{

				// point the object
				Class_BoundaryConvection *tmpObject = new Class_BoundaryConvection;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoundaryConvection::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_HistoryFile_Constructor:
			{

				// create the object
				Class_HistoryFile *tmpObject = new Class_HistoryFile;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object HistoryFile::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_HistoryFile_Pointer:
			{

				// point the object
				Class_HistoryFile *tmpObject = new Class_HistoryFile;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object HistoryFile::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ExplicitSolver_Constructor:
			{

				// create the object
				Class_ExplicitSolver *tmpObject = new Class_ExplicitSolver;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ExplicitSolver::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ExplicitSolver_Pointer:
			{

				// point the object
				Class_ExplicitSolver *tmpObject = new Class_ExplicitSolver;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ExplicitSolver::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ExplicitSolverCH_Constructor:
			{

				// create the object
				Class_ExplicitSolverCH *tmpObject = new Class_ExplicitSolverCH;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ExplicitSolverCH::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_ExplicitSolverCH_Pointer:
			{

				// point the object
				Class_ExplicitSolverCH *tmpObject = new Class_ExplicitSolverCH;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object ExplicitSolverCH::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Side_Constructor:
			{

				// create the object
				Class_Side *tmpObject = new Class_Side;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Side::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_Side_Pointer:
			{

				// point the object
				Class_Side *tmpObject = new Class_Side;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object Side::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_CoulombLaw_Constructor:
			{

				// create the object
				Class_CoulombLaw *tmpObject = new Class_CoulombLaw;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object CoulombLaw::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_CoulombLaw_Pointer:
			{

				// point the object
				Class_CoulombLaw *tmpObject = new Class_CoulombLaw;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object CoulombLaw::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_io_Data_Constructor:
			{

				// create the object
				Class_io_Data *tmpObject = new Class_io_Data;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object io_Data::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_io_Data_Pointer:
			{

				// point the object
				Class_io_Data *tmpObject = new Class_io_Data;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object io_Data::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoxMesher_Constructor:
			{

				// create the object
				Class_BoxMesher *tmpObject = new Class_BoxMesher;

				// set the level of the object
				tmpObject->setObjectLevel(parenLevel);

				// create the associated object
				tmpObject->allocate();

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoxMesher::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case CMDyn_DYG_Object_BoxMesher_Pointer:
			{

				// point the object
				Class_BoxMesher *tmpObject = new Class_BoxMesher;

				// name the object
				tmpObject->setObjectName(inputDyn->bufferGetString());

				// push in stack
				objectList.addObject(tmpObject);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object BoxMesher::%s constructed\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							tmpObject->getObjectName().chars());
			}
			break;

			case VARIABLE_SET:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("VARIABLE_SET", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s operator = applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					int typ = inputDyn->stackObjects.top()->getObjectType();
					if (typ == DYG_Real)
					{
						Real v = (*(Real *)(inputDyn->stackObjects.pop())->object);
						(*(Real *)(obj)->object) = v;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_SET", "unable to convert from %s to %s\n",
								   inputDyn->stackObjects.top()->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				case DYG_String:
				{
					int typ = inputDyn->stackObjects.top()->getObjectType();
					if (typ == DYG_String)
					{
						(obj)->object = (inputDyn->stackObjects.pop())->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_SET", "unable to convert from %s to %s\n",
								   inputDyn->stackObjects.top()->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				case DYG_Vec3D:
				{
					int typ = inputDyn->stackObjects.top()->getObjectType();
					if (typ == DYG_Vec3D)
					{
						(obj)->object = (inputDyn->stackObjects.pop())->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_SET", "unable to convert from %s to %s\n",
								   inputDyn->stackObjects.top()->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				case DYG_Vector:
				{
					int typ = inputDyn->stackObjects.top()->getObjectType();
					if (typ == DYG_Vector)
					{
						(obj)->object = (inputDyn->stackObjects.pop())->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_SET", "unable to convert from %s to %s\n",
								   inputDyn->stackObjects.top()->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				case DYG_Matrix:
				{
					int typ = inputDyn->stackObjects.top()->getObjectType();
					if (typ == DYG_Matrix)
					{
						(obj)->object = (inputDyn->stackObjects.pop())->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_SET", "unable to convert from %s to %s\n",
								   inputDyn->stackObjects.top()->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				case DYG_Tensor2:
				{
					int typ = inputDyn->stackObjects.top()->getObjectType();
					if (typ == DYG_Tensor2)
					{
						(obj)->object = (inputDyn->stackObjects.pop())->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_SET", "unable to convert from %s to %s\n",
								   inputDyn->stackObjects.top()->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				case DYG_DiscreteFunction:
				{
					int typ = inputDyn->stackObjects.top()->getObjectType();
					if (typ == DYG_DiscreteFunction)
					{
						(obj)->object = (inputDyn->stackObjects.pop())->object;
						inputDyn->stackObjects.push(obj);
					}
					else
					{
						fatalError("VARIABLE_SET", "unable to convert from %s to %s\n",
								   inputDyn->stackObjects.top()->getObjectClass().chars(),
								   obj->getObjectClass().chars());
					}
				}
				break;
				default:
				{
					internalFatalError("VARIABLE_SET Action unknown", "Object %s not in list", obj->getObjectClass().chars());
				}
				}
			}
			break;
			case VARIABLE_GET:
			{
				String obj_name;
				obj_name = inputDyn->bufferGetString();
				if (!objectList.existObject(obj_name))
				{
					fatalError("VARIABLE_SET", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s poped from stack\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars());

				// now apply method on object
				inputDyn->stackObjects.push(obj);
			}
			break;

			case CMDyn_DYG_Object_Method_0:
			{
				String obj_name;
				String meth_name;
				obj_name = inputDyn->bufferGetString();
				meth_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("case CMDyn_DYG_Object_Method", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s method %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							meth_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_String:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "empty"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Boolean|
							Class_Boolean *ob = new Class_Boolean;
							ob->allocate();
							*((Boolean *)ob->object) = (((String *)obj->object)->empty());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getReal"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((String *)obj->object)->getReal());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "length"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((String *)obj->object)->length());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "ok"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Boolean|
							Class_Boolean *ob = new Class_Boolean;
							ob->allocate();
							*((Boolean *)ob->object) = (((String *)obj->object)->ok());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "strip"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((String *)obj->object)->strip();
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "String::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class String with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "String::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class String with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Vec3D:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "norm"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vec3D *)obj->object)->norm());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "norm2"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vec3D *)obj->object)->norm2());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "normalize"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((Vec3D *)obj->object)->normalize();
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "size"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vec3D *)obj->object)->size());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Vec3D::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class Vec3D with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Vec3D::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Vec3D with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Vector:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "norm"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vector *)obj->object)->norm());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "norm2"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vector *)obj->object)->norm2());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "normalize"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((Vector *)obj->object)->normalize();
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "size"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vector *)obj->object)->size());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Vector::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class Vector with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Vector::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Vector with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Matrix:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "Identity"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->Identity();
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "Trace"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Matrix *)obj->object)->Trace());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "Transpose"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Matrix|
							Class_Matrix *ob = new Class_Matrix;
							ob->allocate();
							*((Matrix *)ob->object) = (((Matrix *)obj->object)->Transpose());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "colSum"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vector|
							Class_Vector *ob = new Class_Vector;
							ob->allocate();
							*((Vector *)ob->object) = (((Matrix *)obj->object)->colSum());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "det"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Matrix *)obj->object)->det());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "factLU"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Matrix|
							Class_Matrix *ob = new Class_Matrix;
							ob->allocate();
							*((Matrix *)ob->object) = (((Matrix *)obj->object)->factLU());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getNullSpace"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Matrix|
							Class_Matrix *ob = new Class_Matrix;
							ob->allocate();
							*((Matrix *)ob->object) = (((Matrix *)obj->object)->getNullSpace());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "inverse"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Matrix|
							Class_Matrix *ob = new Class_Matrix;
							ob->allocate();
							*((Matrix *)ob->object) = (((Matrix *)obj->object)->inverse());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "moyTrace"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Matrix *)obj->object)->moyTrace());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "outStdout"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->outStdout();
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "rowSum"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vector|
							Class_Vector *ob = new Class_Vector;
							ob->allocate();
							*((Vector *)ob->object) = (((Matrix *)obj->object)->rowSum());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Matrix::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class Matrix with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Matrix::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Matrix with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Tensor2:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "Identity"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor2 *)obj->object)->Identity();
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "Trace"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Tensor2 *)obj->object)->Trace());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "Transpose"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Tensor2|
							Class_Tensor2 *ob = new Class_Tensor2;
							ob->allocate();
							*((Tensor2 *)ob->object) = (((Tensor2 *)obj->object)->Transpose());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "colSum"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((Tensor2 *)obj->object)->colSum());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "det"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Tensor2 *)obj->object)->det());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "deviator"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Tensor2|
							Class_Tensor2 *ob = new Class_Tensor2;
							ob->allocate();
							*((Tensor2 *)ob->object) = (((Tensor2 *)obj->object)->deviator());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "equivalent"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Tensor2 *)obj->object)->equivalent());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "inverse"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Tensor2|
							Class_Tensor2 *ob = new Class_Tensor2;
							ob->allocate();
							*((Tensor2 *)ob->object) = (((Tensor2 *)obj->object)->inverse());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "misesEquivalent"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Tensor2 *)obj->object)->misesEquivalent());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "moyTrace"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Tensor2 *)obj->object)->moyTrace());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "norme"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Tensor2 *)obj->object)->norme());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "rowSum"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((Tensor2 *)obj->object)->rowSum());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Tensor2::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class Tensor2 with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Tensor2::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Tensor2 with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Tensor3:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "Identity"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor3 *)obj->object)->Identity();
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Tensor3::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class Tensor3 with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Tensor3::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Tensor3 with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Tensor4:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "Identity"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor4 *)obj->object)->Identity();
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Tensor4::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class Tensor4 with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Tensor4::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Tensor4 with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_NodeSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "clear"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((NodeSet *)obj->object)->clear();
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "size"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((NodeSet *)obj->object)->size());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "NodeSet::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class NodeSet with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "NodeSet::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class NodeSet with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_ElementSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "clear"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((ElementSet *)obj->object)->clear();
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "ElementSet::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class ElementSet with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "ElementSet::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class ElementSet with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Structure:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "compact"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->compact();
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getCurrentTime"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Structure *)obj->object)->getCurrentTime());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getElementsNumber"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Structure *)obj->object)->getElementsNumber());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getFirstDataFileNumber"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Structure *)obj->object)->getFirstDataFileNumber());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getLastDataFileNumber"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Structure *)obj->object)->getLastDataFileNumber());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getNodesNumber"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Structure *)obj->object)->getNodesNumber());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Structure::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class Structure with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Structure::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Structure with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_DiscreteFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "CentralDifferenceDerivate"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->CentralDifferenceDerivate());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "EulerBackwardDerivate"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->EulerBackwardDerivate());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "EulerForwardDerivate"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->EulerForwardDerivate());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "ZeroForcing1Derivate"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->ZeroForcing1Derivate());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "ZeroForcing2Derivate"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->ZeroForcing2Derivate());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "absoluteValueCurve"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->absoluteValueCurve());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "average"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->average());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "derivate"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->derivate());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "flush"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunction *)obj->object)->flush();
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getName"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |String|
							Class_String *ob = new Class_String;
							ob->allocate();
							*((String *)ob->object) = (((DiscreteFunction *)obj->object)->getName());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getNumberOfPoints"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->getNumberOfPoints());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "integrate"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->integrate());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "maxX"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->maxX());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "maxY"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->maxY());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "minX"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->minX());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "minY"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->minY());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "print"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunction *)obj->object)->print();
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "size"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->size());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "DiscreteFunction::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class DiscreteFunction with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "DiscreteFunction::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class DiscreteFunction with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_PolynomialFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getName"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |String|
							Class_String *ob = new Class_String;
							ob->allocate();
							*((String *)ob->object) = (((PolynomialFunction *)obj->object)->getName());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "print"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((PolynomialFunction *)obj->object)->print();
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "PolynomialFunction::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class PolynomialFunction with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "PolynomialFunction::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class PolynomialFunction with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_SinusFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getName"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |String|
							Class_String *ob = new Class_String;
							ob->allocate();
							*((String *)ob->object) = (((SinusFunction *)obj->object)->getName());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "print"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((SinusFunction *)obj->object)->print();
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "SinusFunction::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class SinusFunction with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "SinusFunction::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class SinusFunction with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_DiscreteFunctionSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "size"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunctionSet *)obj->object)->size());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "DiscreteFunctionSet::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class DiscreteFunctionSet with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "DiscreteFunctionSet::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class DiscreteFunctionSet with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_RampFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getName"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |String|
							Class_String *ob = new Class_String;
							ob->allocate();
							*((String *)ob->object) = (((RampFunction *)obj->object)->getName());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "print"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((RampFunction *)obj->object)->print();
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "RampFunction::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class RampFunction with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "RampFunction::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class RampFunction with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsoHardElastoplasticTabular:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((IsoHardElastoplasticTabular *)obj->object)->getFunction());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardElastoplasticTabular::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardElastoplasticTabular with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardElastoplasticTabular::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardElastoplasticTabular with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Boundary:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Boundary::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class Boundary with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Boundary::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Boundary with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundarySpeed:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "get"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((BoundarySpeed *)obj->object)->get());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundarySpeed::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class BoundarySpeed with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundarySpeed::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundarySpeed with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryAcceleration:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryAcceleration::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryAcceleration with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryAcceleration::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryAcceleration with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryDisplacement:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "get"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((BoundaryDisplacement *)obj->object)->get());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryDisplacement::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryDisplacement with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryDisplacement::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryDisplacement with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryRestrain:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "get"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((BoundaryRestrain *)obj->object)->get());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryRestrain::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryRestrain with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryRestrain::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryRestrain with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryForce:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "get"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((BoundaryForce *)obj->object)->get());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryForce::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryForce with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryForce::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryForce with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryTemperature:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "get"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((BoundaryTemperature *)obj->object)->get());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryTemperature::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryTemperature with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryTemperature::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryTemperature with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryFlux:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "get"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((BoundaryFlux *)obj->object)->get());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryFlux::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryFlux with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryFlux::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryFlux with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryConvection:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryConvection::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryConvection with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryConvection::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryConvection with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Function:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getName"))
					{
						DYG_existMethod = True;
						Boolean run = True;
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |String|
							Class_String *ob = new Class_String;
							ob->allocate();
							*((String *)ob->object) = (((Function *)obj->object)->getName());

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "print"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// ok, now apply the method on the object
						if (run)
						{
							((Function *)obj->object)->print();
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Function::%s(0 args);\nAround line %d in file %s\nMethod %s not exist for class Function with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Function::%s(0 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Function with 0 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 0 argument(s)\nWith object %s", meth_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Method_1:
			{
				String obj_name;
				String meth_name;
				obj_name = inputDyn->bufferGetString();
				meth_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("case CMDyn_DYG_Object_Method", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters
				Object *objParam_0 = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s method %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							meth_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_String:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "convert"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |String|
							Class_String *ob = new Class_String;
							ob->allocate();
							*((String *)ob->object) = (((String *)obj->object)->convert((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "String::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class String with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "String::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class String with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Domain:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "addInterface"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Interface))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Domain *)obj->object)->addInterface(((Interface *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getNodeByNumber"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Node*|
							Class_Node *ob = new Class_Node;
							ob->object = (((Domain *)obj->object)->getNodeByNumber((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Domain::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Domain with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Domain::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Domain with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Vec3D:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "distance"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vec3D *)obj->object)->distance((*(Vec3D *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "distance2"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vec3D *)obj->object)->distance2((*(Vec3D *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "dot"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vec3D *)obj->object)->dot((*(Vec3D *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "fill"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Vec3D *)obj->object)->fill((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "vectorial"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((Vec3D *)obj->object)->vectorial((*(Vec3D *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Vec3D::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Vec3D with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Vec3D::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Vec3D with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Vector:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "distance"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vector *)obj->object)->distance((*(Vector *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "distance2"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vector *)obj->object)->distance2((*(Vector *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "dot"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Vector *)obj->object)->dot((*(Vector *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "fill"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Vector *)obj->object)->fill((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "indexOK"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Boolean|
							Class_Boolean *ob = new Class_Boolean;
							ob->allocate();
							*((Boolean *)ob->object) = (((Vector *)obj->object)->indexOK((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "redim"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Vector *)obj->object)->redim((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "vectorial"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vector|
							Class_Vector *ob = new Class_Vector;
							ob->allocate();
							*((Vector *)ob->object) = (((Vector *)obj->object)->vectorial((*(Vector *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Vector::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Vector with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Vector::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Vector with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Matrix:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "fill"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->fill((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "forwBackLU"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vector|
							Class_Vector *ob = new Class_Vector;
							ob->allocate();
							*((Vector *)ob->object) = (((Matrix *)obj->object)->forwBackLU((*(Vector *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getAntiSymetricPart"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Matrix))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->getAntiSymetricPart((*(Matrix *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getCol"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vector|
							Class_Vector *ob = new Class_Vector;
							ob->allocate();
							*((Vector *)ob->object) = (((Matrix *)obj->object)->getCol((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getRow"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vector|
							Class_Vector *ob = new Class_Vector;
							ob->allocate();
							*((Vector *)ob->object) = (((Matrix *)obj->object)->getRow((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getSolve"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vector|
							Class_Vector *ob = new Class_Vector;
							ob->allocate();
							*((Vector *)ob->object) = (((Matrix *)obj->object)->getSolve((*(Vector *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getSymetricPart"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Matrix))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->getSymetricPart((*(Matrix *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "solve"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->solve((*(Vector *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "trans_mult"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vector|
							Class_Vector *ob = new Class_Vector;
							ob->allocate();
							*((Vector *)ob->object) = (((Matrix *)obj->object)->trans_mult((*(Vector *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Matrix::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Matrix with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Matrix::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Matrix with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Tensor2:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "fill"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor2 *)obj->object)->fill((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getAntiSymetricPart"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Tensor2))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor2 *)obj->object)->getAntiSymetricPart((*(Tensor2 *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getCol"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((Tensor2 *)obj->object)->getCol((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getRow"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((Tensor2 *)obj->object)->getRow((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getSymetricPart"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Tensor2))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor2 *)obj->object)->getSymetricPart((*(Tensor2 *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "prod1"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Tensor2))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Tensor2|
							Class_Tensor2 *ob = new Class_Tensor2;
							ob->allocate();
							*((Tensor2 *)ob->object) = (((Tensor2 *)obj->object)->prod1((*(Tensor2 *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "prod2"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Tensor2))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Tensor2 *)obj->object)->prod2((*(Tensor2 *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Tensor2::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Tensor2 with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Tensor2::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Tensor2 with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Tensor3:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "fill"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor3 *)obj->object)->fill((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Tensor3::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Tensor3 with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Tensor3::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Tensor3 with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Tensor4:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "fill"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor4 *)obj->object)->fill((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Tensor4::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Tensor4 with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Tensor4::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Tensor4 with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_NodeSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((NodeSet *)obj->object)->add((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((NodeSet *)obj->object)->add(((NodeSet *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getNodeNumber"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Indice|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((NodeSet *)obj->object)->getNodeNumber((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "intersect"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((NodeSet *)obj->object)->intersect(((NodeSet *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "substract"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((NodeSet *)obj->object)->substract(((NodeSet *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "surface"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_ElementSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((NodeSet *)obj->object)->surface(((ElementSet *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "NodeSet::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class NodeSet with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "NodeSet::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class NodeSet with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_ElementSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ElementSet *)obj->object)->add((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_ElementSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ElementSet *)obj->object)->add(((ElementSet *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "intersect"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_ElementSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ElementSet *)obj->object)->intersect(((ElementSet *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "substract"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_ElementSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ElementSet *)obj->object)->substract(((ElementSet *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "ElementSet::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class ElementSet with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "ElementSet::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class ElementSet with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Structure:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "addHistoryFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_HistoryFile))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->addHistoryFile(((HistoryFile *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "addInterface"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Interface))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->addInterface(((Interface *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "addMaterial"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Material))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->addMaterial(((Material *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "addSolver"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Solver) && (objParam_0->getObjectType() != DYG_ExplicitSolver) && (objParam_0->getObjectType() != DYG_ExplicitSolverCH))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->addSolver(((Solver *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "readResultFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->readResultFile((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "scale"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->scale((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "scale"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->scale((*(Vec3D *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setDefaultElement"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Element) && (objParam_0->getObjectType() != DYG_ElQua4n2D) && (objParam_0->getObjectType() != DYG_ElHex8n3D) && (objParam_0->getObjectType() != DYG_ElTri3n2D) && (objParam_0->getObjectType() != DYG_ElQua4nAx) && (objParam_0->getObjectType() != DYG_ElTet4n3D) && (objParam_0->getObjectType() != DYG_ElTet10n3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->setDefaultElement(((Element *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setDomain"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Domain))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->setDomain(((Domain *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setResultFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->setResultFile((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "translate"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->translate((*(Vec3D *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Structure::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Structure with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Structure::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Structure with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_DiscreteFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "autoSort"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Boolean))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunction *)obj->object)->autoSort((*(Boolean *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getInverseValue"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->getInverseValue((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getSlope"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->getSlope((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getValue"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->getValue((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getXPoint"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->getXPoint((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getYPoint"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((DiscreteFunction *)obj->object)->getYPoint((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "intoBoundsX"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Boolean|
							Class_Boolean *ob = new Class_Boolean;
							ob->allocate();
							*((Boolean *)ob->object) = (((DiscreteFunction *)obj->object)->intoBoundsX((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "offsetCurve"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->offsetCurve((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "scaleCurve"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->scaleCurve((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setName"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunction *)obj->object)->setName((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "toGnuplot"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunction *)obj->object)->toGnuplot((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "DiscreteFunction::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class DiscreteFunction with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "DiscreteFunction::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class DiscreteFunction with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_PolynomialFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getSlope"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((PolynomialFunction *)obj->object)->getSlope((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getValue"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((PolynomialFunction *)obj->object)->getValue((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setName"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((PolynomialFunction *)obj->object)->setName((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "PolynomialFunction::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class PolynomialFunction with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "PolynomialFunction::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class PolynomialFunction with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_SinusFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getSlope"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((SinusFunction *)obj->object)->getSlope((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getValue"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((SinusFunction *)obj->object)->getValue((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setConstant"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((SinusFunction *)obj->object)->setConstant((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setName"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((SinusFunction *)obj->object)->setName((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "SinusFunction::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class SinusFunction with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "SinusFunction::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class SinusFunction with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_DiscreteFunctionSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunctionSet *)obj->object)->add(((DiscreteFunction *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "del"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunctionSet *)obj->object)->del((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "del"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunctionSet *)obj->object)->del((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "get"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunctionSet *)obj->object)->get((Indice)(*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "get"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunctionSet *)obj->object)->get((*(String *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "read"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunctionSet *)obj->object)->read((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "write"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunctionSet *)obj->object)->write((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "DiscreteFunctionSet::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class DiscreteFunctionSet with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "DiscreteFunctionSet::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class DiscreteFunctionSet with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_RampFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getSlope"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((RampFunction *)obj->object)->getSlope((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getValue"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((RampFunction *)obj->object)->getValue((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setName"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((RampFunction *)obj->object)->setName((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "toGnuplot"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((RampFunction *)obj->object)->toGnuplot((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "RampFunction::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class RampFunction with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "RampFunction::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class RampFunction with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Material:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setColor"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setColor((*(Vec3D *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setConductivity"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setConductivity((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setDensity"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setDensity((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setDilatation"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setDilatation((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setHardening"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_IsotropicHardening) && (objParam_0->getObjectType() != DYG_IsoHardElastic) && (objParam_0->getObjectType() != DYG_IsoHardElastoplasticTabular) && (objParam_0->getObjectType() != DYG_IsoHardElastoplastic) && (objParam_0->getObjectType() != DYG_IsoHardMetafor) && (objParam_0->getObjectType() != DYG_IsoHardPower) && (objParam_0->getObjectType() != DYG_IsoHardJohnsonCook))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setHardening(((IsotropicHardening *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setHeatCoefficient"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setHeatCoefficient((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setInitTemperature"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setInitTemperature((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setPoisson"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setPoisson((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setYoung"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setYoung((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Material::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Material with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Material::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Material with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsotropicHardening:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "plotToFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsotropicHardening *)obj->object)->plotToFile((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsotropicHardening::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class IsotropicHardening with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsotropicHardening::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsotropicHardening with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsoHardElastic:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "plotToFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardElastic *)obj->object)->plotToFile((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardElastic::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardElastic with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardElastic::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardElastic with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsoHardElastoplasticTabular:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "plotToFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardElastoplasticTabular *)obj->object)->plotToFile((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardElastoplasticTabular *)obj->object)->setFunction(((DiscreteFunction *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardElastoplasticTabular::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardElastoplasticTabular with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardElastoplasticTabular::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardElastoplasticTabular with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsoHardElastoplastic:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "plotToFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardElastoplastic *)obj->object)->plotToFile((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setHardExponent"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardElastoplastic *)obj->object)->setHardExponent((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setHardParameter"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardElastoplastic *)obj->object)->setHardParameter((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setYieldStress"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardElastoplastic *)obj->object)->setYieldStress((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardElastoplastic::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardElastoplastic with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardElastoplastic::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardElastoplastic with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsoHardMetafor:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "plotToFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardMetafor *)obj->object)->plotToFile((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardMetafor::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardMetafor with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardMetafor::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardMetafor with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsoHardPower:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "plotToFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardPower *)obj->object)->plotToFile((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardPower::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardPower with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardPower::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardPower with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsoHardJohnsonCook:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "plotToFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardJohnsonCook *)obj->object)->plotToFile((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardJohnsonCook::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardJohnsonCook with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardJohnsonCook::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardJohnsonCook with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Interface:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setContactLaw"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_ContactLaw) && (objParam_0->getObjectType() != DYG_CoulombLaw))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Interface *)obj->object)->setContactLaw(((ContactLaw *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setMaster"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Side))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Interface *)obj->object)->setMaster(((Side *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setSlave"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Side))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Interface *)obj->object)->setSlave(((Side *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Interface::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Interface with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Interface::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Interface with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Boundary:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Function) && (objParam_0->getObjectType() != DYG_PolynomialFunction) && (objParam_0->getObjectType() != DYG_SinusFunction) && (objParam_0->getObjectType() != DYG_RampFunction) && (objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Boundary *)obj->object)->setFunction(((Function *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Boundary::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Boundary with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Boundary::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Boundary with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryCondition:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "addConstant"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Boundary) && (objParam_0->getObjectType() != DYG_BoundaryDisplacement) && (objParam_0->getObjectType() != DYG_BoundaryRestrain) && (objParam_0->getObjectType() != DYG_BoundaryAcceleration) && (objParam_0->getObjectType() != DYG_BoundaryForce) && (objParam_0->getObjectType() != DYG_BoundarySpeed) && (objParam_0->getObjectType() != DYG_BoundaryTemperature) && (objParam_0->getObjectType() != DYG_BoundaryFlux) && (objParam_0->getObjectType() != DYG_BoundaryConvection))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryCondition *)obj->object)->addConstant(((Boundary *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "addInitial"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Boundary) && (objParam_0->getObjectType() != DYG_BoundaryDisplacement) && (objParam_0->getObjectType() != DYG_BoundaryRestrain) && (objParam_0->getObjectType() != DYG_BoundaryAcceleration) && (objParam_0->getObjectType() != DYG_BoundaryForce) && (objParam_0->getObjectType() != DYG_BoundarySpeed) && (objParam_0->getObjectType() != DYG_BoundaryTemperature) && (objParam_0->getObjectType() != DYG_BoundaryFlux) && (objParam_0->getObjectType() != DYG_BoundaryConvection))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryCondition *)obj->object)->addInitial(((Boundary *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryCondition::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryCondition with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryCondition::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryCondition with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundarySpeed:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundarySpeed *)obj->object)->set((*(Vec3D *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Function) && (objParam_0->getObjectType() != DYG_PolynomialFunction) && (objParam_0->getObjectType() != DYG_SinusFunction) && (objParam_0->getObjectType() != DYG_RampFunction) && (objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundarySpeed *)obj->object)->setFunction(((Function *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundarySpeed::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoundarySpeed with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundarySpeed::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundarySpeed with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryAcceleration:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Function) && (objParam_0->getObjectType() != DYG_PolynomialFunction) && (objParam_0->getObjectType() != DYG_SinusFunction) && (objParam_0->getObjectType() != DYG_RampFunction) && (objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryAcceleration *)obj->object)->setFunction(((Function *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryAcceleration::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryAcceleration with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryAcceleration::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryAcceleration with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryDisplacement:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryDisplacement *)obj->object)->set((*(Vec3D *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Function) && (objParam_0->getObjectType() != DYG_PolynomialFunction) && (objParam_0->getObjectType() != DYG_SinusFunction) && (objParam_0->getObjectType() != DYG_RampFunction) && (objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryDisplacement *)obj->object)->setFunction(((Function *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryDisplacement::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryDisplacement with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryDisplacement::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryDisplacement with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryRestrain:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryRestrain *)obj->object)->set((*(Vec3D *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Function) && (objParam_0->getObjectType() != DYG_PolynomialFunction) && (objParam_0->getObjectType() != DYG_SinusFunction) && (objParam_0->getObjectType() != DYG_RampFunction) && (objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryRestrain *)obj->object)->setFunction(((Function *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryRestrain::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryRestrain with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryRestrain::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryRestrain with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryForce:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryForce *)obj->object)->set((*(Vec3D *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Function) && (objParam_0->getObjectType() != DYG_PolynomialFunction) && (objParam_0->getObjectType() != DYG_SinusFunction) && (objParam_0->getObjectType() != DYG_RampFunction) && (objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryForce *)obj->object)->setFunction(((Function *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryForce::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryForce with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryForce::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryForce with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryTemperature:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryTemperature *)obj->object)->set((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Function) && (objParam_0->getObjectType() != DYG_PolynomialFunction) && (objParam_0->getObjectType() != DYG_SinusFunction) && (objParam_0->getObjectType() != DYG_RampFunction) && (objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryTemperature *)obj->object)->setFunction(((Function *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryTemperature::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryTemperature with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryTemperature::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryTemperature with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryFlux:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryFlux *)obj->object)->set((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Function) && (objParam_0->getObjectType() != DYG_PolynomialFunction) && (objParam_0->getObjectType() != DYG_SinusFunction) && (objParam_0->getObjectType() != DYG_RampFunction) && (objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryFlux *)obj->object)->setFunction(((Function *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryFlux::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryFlux with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryFlux::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryFlux with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryConvection:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setFunction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Function) && (objParam_0->getObjectType() != DYG_PolynomialFunction) && (objParam_0->getObjectType() != DYG_SinusFunction) && (objParam_0->getObjectType() != DYG_RampFunction) && (objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryConvection *)obj->object)->setFunction(((Function *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryConvection::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryConvection with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryConvection::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryConvection with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_HistoryFile:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setName"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((HistoryFile *)obj->object)->setName((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setSaveTime"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((HistoryFile *)obj->object)->setSaveTime((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "HistoryFile::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class HistoryFile with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "HistoryFile::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class HistoryFile with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Solver:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setComputeTimeStepFrequency"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Solver *)obj->object)->setComputeTimeStepFrequency((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setRFI"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Solver *)obj->object)->setRFI((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setTimeStepMethod"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Solver *)obj->object)->setTimeStepMethod((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setTimeStepSafetyFactor"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Solver *)obj->object)->setTimeStepSafetyFactor((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setUnderIntegratePressure"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Solver *)obj->object)->setUnderIntegratePressure((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Solver::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Solver with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Solver::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Solver with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_ExplicitSolver:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setComputeTimeStepFrequency"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolver *)obj->object)->setComputeTimeStepFrequency((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setRFI"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolver *)obj->object)->setRFI((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setTimeStepMethod"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolver *)obj->object)->setTimeStepMethod((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setTimeStepSafetyFactor"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolver *)obj->object)->setTimeStepSafetyFactor((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setUnderIntegratePressure"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolver *)obj->object)->setUnderIntegratePressure((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "ExplicitSolver::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class ExplicitSolver with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "ExplicitSolver::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class ExplicitSolver with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_ExplicitSolverCH:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setComputeTimeStepFrequency"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolverCH *)obj->object)->setComputeTimeStepFrequency((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setDissipation"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolverCH *)obj->object)->setDissipation((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setRFI"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolverCH *)obj->object)->setRFI((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setTimeStepMethod"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolverCH *)obj->object)->setTimeStepMethod((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setTimeStepSafetyFactor"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolverCH *)obj->object)->setTimeStepSafetyFactor((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setUnderIntegratePressure"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolverCH *)obj->object)->setUnderIntegratePressure((Indice)(*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "ExplicitSolverCH::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class ExplicitSolverCH with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "ExplicitSolverCH::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class ExplicitSolverCH with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Function:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getSlope"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Function *)obj->object)->getSlope((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getValue"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Function *)obj->object)->getValue((*(Real *)objParam_0->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setName"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Function *)obj->object)->setName((*(String *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Function::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Function with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Function::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Function with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Side:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "addNodeSet"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Side *)obj->object)->addNodeSet(((NodeSet *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Side::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Side with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Side::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Side with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_CoulombLaw:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setFriction"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((CoulombLaw *)obj->object)->setFriction((*(Real *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "CoulombLaw::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class CoulombLaw with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "CoulombLaw::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class CoulombLaw with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Mesher:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "link"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Structure))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Mesher *)obj->object)->link(((Structure *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setElement"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Element) && (objParam_0->getObjectType() != DYG_ElQua4n2D) && (objParam_0->getObjectType() != DYG_ElHex8n3D) && (objParam_0->getObjectType() != DYG_ElTri3n2D) && (objParam_0->getObjectType() != DYG_ElQua4nAx) && (objParam_0->getObjectType() != DYG_ElTet4n3D) && (objParam_0->getObjectType() != DYG_ElTet10n3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Mesher *)obj->object)->setElement(((Element *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Mesher::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class Mesher with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Mesher::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Mesher with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoxMesher:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "link"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Structure))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoxMesher *)obj->object)->link(((Structure *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setElement"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Element) && (objParam_0->getObjectType() != DYG_ElQua4n2D) && (objParam_0->getObjectType() != DYG_ElHex8n3D) && (objParam_0->getObjectType() != DYG_ElTri3n2D) && (objParam_0->getObjectType() != DYG_ElQua4nAx) && (objParam_0->getObjectType() != DYG_ElTet4n3D) && (objParam_0->getObjectType() != DYG_ElTet10n3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoxMesher *)obj->object)->setElement(((Element *)objParam_0->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoxMesher::%s(1 args);\nAround line %d in file %s\nMethod %s not exist for class BoxMesher with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoxMesher::%s(1 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoxMesher with 1 argument", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 1 argument(s)\nWith object %s", meth_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Method_2:
			{
				String obj_name;
				String meth_name;
				obj_name = inputDyn->bufferGetString();
				meth_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("case CMDyn_DYG_Object_Method", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters
				Object *objParam_1 = inputDyn->stackObjects.pop();
				Object *objParam_0 = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s method %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							meth_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_Matrix:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "computeLeftEigenVectors"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Matrix))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->computeLeftEigenVectors((*(Vector *)objParam_0->object), (*(Matrix *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "computeRightEigenVectors"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Matrix))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->computeRightEigenVectors((*(Vector *)objParam_0->object), (*(Matrix *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "factLU"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Vector))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Matrix|
							Class_Matrix *ob = new Class_Matrix;
							ob->allocate();
							*((Matrix *)ob->object) = (((Matrix *)obj->object)->factLU((*(Vector *)objParam_0->object), (*(Vector *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getPseudoInverse"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Boolean))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Matrix|
							Class_Matrix *ob = new Class_Matrix;
							ob->allocate();
							*((Matrix *)ob->object) = (((Matrix *)obj->object)->getPseudoInverse((*(Boolean *)objParam_0->object), (*(Real *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "indexOK"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if (objParam_1->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Boolean|
							Class_Boolean *ob = new Class_Boolean;
							ob->allocate();
							*((Boolean *)ob->object) = (((Matrix *)obj->object)->indexOK((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "redim"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->redim((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Matrix::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class Matrix with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Matrix::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Matrix with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Tensor2:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "extProd"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Tensor2|
							Class_Tensor2 *ob = new Class_Tensor2;
							ob->allocate();
							*((Tensor2 *)ob->object) = (((Tensor2 *)obj->object)->extProd((*(Vec3D *)objParam_0->object), (*(Vec3D *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "polarDecompose"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Tensor2))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Tensor2))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor2 *)obj->object)->polarDecompose((*(Tensor2 *)objParam_0->object), (*(Tensor2 *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "polarDecomposeLnU"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Tensor2))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Tensor2))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Tensor2 *)obj->object)->polarDecomposeLnU((*(Tensor2 *)objParam_0->object), (*(Tensor2 *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Tensor2::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class Tensor2 with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Tensor2::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Tensor2 with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_NodeSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((NodeSet *)obj->object)->add((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "NodeSet::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class NodeSet with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "NodeSet::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class NodeSet with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_ElementSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ElementSet *)obj->object)->add((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "ElementSet::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class ElementSet with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "ElementSet::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class ElementSet with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Structure:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "attachConstantBC"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Boundary) && (objParam_0->getObjectType() != DYG_BoundaryDisplacement) && (objParam_0->getObjectType() != DYG_BoundaryRestrain) && (objParam_0->getObjectType() != DYG_BoundaryAcceleration) && (objParam_0->getObjectType() != DYG_BoundaryForce) && (objParam_0->getObjectType() != DYG_BoundarySpeed) && (objParam_0->getObjectType() != DYG_BoundaryTemperature) && (objParam_0->getObjectType() != DYG_BoundaryFlux) && (objParam_0->getObjectType() != DYG_BoundaryConvection))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->attachConstantBC(((Boundary *)objParam_0->object), ((NodeSet *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "attachInitialBC"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Boundary) && (objParam_0->getObjectType() != DYG_BoundaryDisplacement) && (objParam_0->getObjectType() != DYG_BoundaryRestrain) && (objParam_0->getObjectType() != DYG_BoundaryAcceleration) && (objParam_0->getObjectType() != DYG_BoundaryForce) && (objParam_0->getObjectType() != DYG_BoundarySpeed) && (objParam_0->getObjectType() != DYG_BoundaryTemperature) && (objParam_0->getObjectType() != DYG_BoundaryFlux) && (objParam_0->getObjectType() != DYG_BoundaryConvection))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->attachInitialBC(((Boundary *)objParam_0->object), ((NodeSet *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "attachMaterialToElements"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_ElementSet))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Material))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->attachMaterialToElements(((Material *)objParam_0->object), ((ElementSet *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getNodalTensor"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_String))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Tensor2|
							Class_Tensor2 *ob = new Class_Tensor2;
							ob->allocate();
							*((Tensor2 *)ob->object) = (((Structure *)obj->object)->getNodalTensor((Indice)(*(Real *)objParam_0->object), (*(String *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getNodalValue"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_String))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Structure *)obj->object)->getNodalValue((Indice)(*(Real *)objParam_0->object), (*(String *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "getNodalVector"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_String))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Vec3D|
							Class_Vec3D *ob = new Class_Vec3D;
							ob->allocate();
							*((Vec3D *)ob->object) = (((Structure *)obj->object)->getNodalVector((Indice)(*(Real *)objParam_0->object), (*(String *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "rotate"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->rotate((*(String *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "rotate"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->rotate((*(Vec3D *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "scale"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->scale((*(Real *)objParam_0->object), ((NodeSet *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "scale"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->scale((*(Vec3D *)objParam_0->object), ((NodeSet *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "translate"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->translate((*(Vec3D *)objParam_0->object), ((NodeSet *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Structure::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class Structure with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Structure::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Structure with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_DiscreteFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunction *)obj->object)->add((*(Real *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "addCurves"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if (objParam_1->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->addCurves(((DiscreteFunction *)objParam_0->object), (Indice)(*(Real *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "divideCurves"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if (objParam_1->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->divideCurves(((DiscreteFunction *)objParam_0->object), (Indice)(*(Real *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "errorCurves"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if (objParam_1->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->errorCurves(((DiscreteFunction *)objParam_0->object), (Indice)(*(Real *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "movingAverage"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if (objParam_1->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->movingAverage((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "multiplyCurves"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if (objParam_1->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->multiplyCurves(((DiscreteFunction *)objParam_0->object), (Indice)(*(Real *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "substractCurves"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if (objParam_1->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->substractCurves(((DiscreteFunction *)objParam_0->object), (Indice)(*(Real *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "xyMerge"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if (objParam_1->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_DiscreteFunction))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->xyMerge(((DiscreteFunction *)objParam_0->object), (Indice)(*(Real *)objParam_1->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "DiscreteFunction::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class DiscreteFunction with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "DiscreteFunction::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class DiscreteFunction with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_PolynomialFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((PolynomialFunction *)obj->object)->set((Indice)(*(Real *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "PolynomialFunction::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class PolynomialFunction with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "PolynomialFunction::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class PolynomialFunction with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_SinusFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setCos"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((SinusFunction *)obj->object)->setCos((*(Real *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setSin"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((SinusFunction *)obj->object)->setSin((*(Real *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "SinusFunction::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class SinusFunction with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "SinusFunction::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class SinusFunction with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsoHardMetafor:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setParameter"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardMetafor *)obj->object)->setParameter((Indice)(*(Real *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardMetafor::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardMetafor with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardMetafor::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardMetafor with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_IsoHardPower:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setParameter"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardPower *)obj->object)->setParameter((Indice)(*(Real *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardPower::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardPower with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardPower::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardPower with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_HistoryFile:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "append"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_String))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((HistoryFile *)obj->object)->append(((NodeSet *)objParam_0->object), (*(String *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "HistoryFile::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class HistoryFile with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "HistoryFile::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class HistoryFile with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Solver:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setIncrements"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Solver *)obj->object)->setIncrements((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setTimes"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Solver *)obj->object)->setTimes((*(Real *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Solver::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class Solver with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Solver::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Solver with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_ExplicitSolver:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setIncrements"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolver *)obj->object)->setIncrements((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setTimes"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolver *)obj->object)->setTimes((*(Real *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "ExplicitSolver::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class ExplicitSolver with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "ExplicitSolver::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class ExplicitSolver with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_ExplicitSolverCH:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setIncrements"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolverCH *)obj->object)->setIncrements((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setTimes"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ExplicitSolverCH *)obj->object)->setTimes((*(Real *)objParam_0->object), (*(Real *)objParam_1->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "ExplicitSolverCH::%s(2 args);\nAround line %d in file %s\nMethod %s not exist for class ExplicitSolverCH with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "ExplicitSolverCH::%s(2 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class ExplicitSolverCH with 2 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 2 argument(s)\nWith object %s", meth_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Method_3:
			{
				String obj_name;
				String meth_name;
				obj_name = inputDyn->bufferGetString();
				meth_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("case CMDyn_DYG_Object_Method", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters
				Object *objParam_2 = inputDyn->stackObjects.pop();
				Object *objParam_1 = inputDyn->stackObjects.pop();
				Object *objParam_0 = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s method %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							meth_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_Matrix:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "computeEigenVectors"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Matrix))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Matrix))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->computeEigenVectors((*(Vector *)objParam_0->object), (*(Matrix *)objParam_1->object), (*(Matrix *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "forwBackLU"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Vector))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Vector))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vector))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->forwBackLU((*(Vector *)objParam_0->object), (*(Vector *)objParam_1->object), (*(Vector *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Matrix::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class Matrix with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Matrix::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Matrix with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_NodeSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((NodeSet *)obj->object)->add((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "NodeSet::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class NodeSet with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "NodeSet::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class NodeSet with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_ElementSet:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "add"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((ElementSet *)obj->object)->add((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "ElementSet::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class ElementSet with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "ElementSet::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class ElementSet with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Structure:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "getNodalValue"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if (objParam_2->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_String))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if (objParam_0->getObjectType() != DYG_Real)
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |Real|
							Class_Real *ob = new Class_Real;
							ob->allocate();
							*((Real *)ob->object) = (((Structure *)obj->object)->getNodalValue((Indice)(*(Real *)objParam_0->object), (*(String *)objParam_1->object), (Indice)(*(Real *)objParam_2->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "rotate"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Vec3D))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->rotate((*(Vec3D *)objParam_0->object), (*(Real *)objParam_1->object), ((NodeSet *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "rotate"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->rotate((*(String *)objParam_0->object), (*(Real *)objParam_1->object), ((NodeSet *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setSaveTimes"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->setSaveTimes((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Structure::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class Structure with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Structure::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Structure with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_DiscreteFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "fromFile"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((DiscreteFunction *)obj->object)->fromFile((*(String *)objParam_0->object), (Indice)(*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "lowPass_1"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if (objParam_2->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if (objParam_1->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->lowPass_1((*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "DiscreteFunction::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class DiscreteFunction with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "DiscreteFunction::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class DiscreteFunction with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_SinusFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setCos"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((SinusFunction *)obj->object)->setCos((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setSin"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((SinusFunction *)obj->object)->setSin((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "SinusFunction::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class SinusFunction with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "SinusFunction::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class SinusFunction with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_RampFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((RampFunction *)obj->object)->set((*(String *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "RampFunction::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class RampFunction with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "RampFunction::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class RampFunction with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Material:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "setColor"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Material *)obj->object)->setColor((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Material::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class Material with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Material::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Material with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundarySpeed:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundarySpeed *)obj->object)->set((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundarySpeed::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class BoundarySpeed with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundarySpeed::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundarySpeed with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryDisplacement:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryDisplacement *)obj->object)->set((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryDisplacement::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryDisplacement with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryDisplacement::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryDisplacement with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryRestrain:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryRestrain *)obj->object)->set((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryRestrain::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryRestrain with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryRestrain::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryRestrain with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoundaryForce:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "set"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoundaryForce *)obj->object)->set((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoundaryForce::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class BoundaryForce with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoundaryForce::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoundaryForce with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_HistoryFile:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "append"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_String))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_NodeSet))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((HistoryFile *)obj->object)->append(((NodeSet *)objParam_0->object), (*(String *)objParam_1->object), (Indice)(*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "setSaveTime"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((HistoryFile *)obj->object)->setSaveTime((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "HistoryFile::%s(3 args);\nAround line %d in file %s\nMethod %s not exist for class HistoryFile with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "HistoryFile::%s(3 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class HistoryFile with 3 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 3 argument(s)\nWith object %s", meth_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Method_4:
			{
				String obj_name;
				String meth_name;
				obj_name = inputDyn->bufferGetString();
				meth_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("case CMDyn_DYG_Object_Method", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters
				Object *objParam_3 = inputDyn->stackObjects.pop();
				Object *objParam_2 = inputDyn->stackObjects.pop();
				Object *objParam_1 = inputDyn->stackObjects.pop();
				Object *objParam_0 = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s method %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							meth_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_Matrix:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "computePseudoInverse"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 4 matches request form
						if ((objParam_3->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Boolean))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Matrix))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Matrix))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Matrix *)obj->object)->computePseudoInverse((*(Matrix *)objParam_0->object), (*(Matrix *)objParam_1->object), (*(Boolean *)objParam_2->object), (*(Real *)objParam_3->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Matrix::%s(4 args);\nAround line %d in file %s\nMethod %s not exist for class Matrix with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Matrix::%s(4 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Matrix with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_Structure:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "createElement"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 4 matches request form
						if ((objParam_3)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->createElement((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object), (Indice)(*(Real *)objParam_3->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (meth_name == "createNode"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 4 matches request form
						if ((objParam_3->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->createNode((Indice)(*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object), (*(Real *)objParam_3->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Structure::%s(4 args);\nAround line %d in file %s\nMethod %s not exist for class Structure with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Structure::%s(4 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Structure with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_DiscreteFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "lowPass_2"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 4 matches request form
						if (objParam_3->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if (objParam_2->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}
						// ok, now apply the method on the object
						if (run)
						{
							// Constructor of |DiscreteFunction*|
							Class_DiscreteFunction *ob = new Class_DiscreteFunction;
							ob->object = (((DiscreteFunction *)obj->object)->lowPass_2((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object), (Indice)(*(Real *)objParam_3->object)));

							inputDyn->stackObjects.push(ob);
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "DiscreteFunction::%s(4 args);\nAround line %d in file %s\nMethod %s not exist for class DiscreteFunction with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "DiscreteFunction::%s(4 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class DiscreteFunction with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_PolynomialFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "toGnuplot"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 4 matches request form
						if ((objParam_3->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((PolynomialFunction *)obj->object)->toGnuplot((*(String *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object), (*(Real *)objParam_3->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "PolynomialFunction::%s(4 args);\nAround line %d in file %s\nMethod %s not exist for class PolynomialFunction with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "PolynomialFunction::%s(4 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class PolynomialFunction with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_SinusFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "toGnuplot"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 4 matches request form
						if ((objParam_3->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_String))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((SinusFunction *)obj->object)->toGnuplot((*(String *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object), (*(Real *)objParam_3->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "SinusFunction::%s(4 args);\nAround line %d in file %s\nMethod %s not exist for class SinusFunction with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "SinusFunction::%s(4 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class SinusFunction with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				case DYG_BoxMesher:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "rectangle"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 4 matches request form
						if ((objParam_3)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((BoxMesher *)obj->object)->rectangle((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object), (Indice)(*(Real *)objParam_3->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "BoxMesher::%s(4 args);\nAround line %d in file %s\nMethod %s not exist for class BoxMesher with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "BoxMesher::%s(4 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class BoxMesher with 4 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 4 argument(s)\nWith object %s", meth_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Method_5:
			{
				String obj_name;
				String meth_name;
				obj_name = inputDyn->bufferGetString();
				meth_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("case CMDyn_DYG_Object_Method", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters
				Object *objParam_4 = inputDyn->stackObjects.pop();
				Object *objParam_3 = inputDyn->stackObjects.pop();
				Object *objParam_2 = inputDyn->stackObjects.pop();
				Object *objParam_1 = inputDyn->stackObjects.pop();
				Object *objParam_0 = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s method %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							meth_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_Structure:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "createElement"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 5 matches request form
						if ((objParam_4)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 4 matches request form
						if ((objParam_3)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->createElement((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object), (Indice)(*(Real *)objParam_3->object), (Indice)(*(Real *)objParam_4->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Structure::%s(5 args);\nAround line %d in file %s\nMethod %s not exist for class Structure with 5 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Structure::%s(5 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Structure with 5 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 5 argument(s)\nWith object %s", meth_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Method_8:
			{
				String obj_name;
				String meth_name;
				obj_name = inputDyn->bufferGetString();
				meth_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("case CMDyn_DYG_Object_Method", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters
				Object *objParam_7 = inputDyn->stackObjects.pop();
				Object *objParam_6 = inputDyn->stackObjects.pop();
				Object *objParam_5 = inputDyn->stackObjects.pop();
				Object *objParam_4 = inputDyn->stackObjects.pop();
				Object *objParam_3 = inputDyn->stackObjects.pop();
				Object *objParam_2 = inputDyn->stackObjects.pop();
				Object *objParam_1 = inputDyn->stackObjects.pop();
				Object *objParam_0 = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s method %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							meth_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_IsoHardJohnsonCook:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "defineLaw"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 8 matches request form
						if ((objParam_7->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 7 matches request form
						if ((objParam_6->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 6 matches request form
						if ((objParam_5->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 5 matches request form
						if ((objParam_4->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 4 matches request form
						if ((objParam_3->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0->getObjectType() != DYG_Real))
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((IsoHardJohnsonCook *)obj->object)->defineLaw((*(Real *)objParam_0->object), (*(Real *)objParam_1->object), (*(Real *)objParam_2->object), (*(Real *)objParam_3->object), (*(Real *)objParam_4->object), (*(Real *)objParam_5->object), (*(Real *)objParam_6->object), (*(Real *)objParam_7->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "IsoHardJohnsonCook::%s(8 args);\nAround line %d in file %s\nMethod %s not exist for class IsoHardJohnsonCook with 8 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "IsoHardJohnsonCook::%s(8 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class IsoHardJohnsonCook with 8 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 8 argument(s)\nWith object %s", meth_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Method_9:
			{
				String obj_name;
				String meth_name;
				obj_name = inputDyn->bufferGetString();
				meth_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("case CMDyn_DYG_Object_Method", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters
				Object *objParam_8 = inputDyn->stackObjects.pop();
				Object *objParam_7 = inputDyn->stackObjects.pop();
				Object *objParam_6 = inputDyn->stackObjects.pop();
				Object *objParam_5 = inputDyn->stackObjects.pop();
				Object *objParam_4 = inputDyn->stackObjects.pop();
				Object *objParam_3 = inputDyn->stackObjects.pop();
				Object *objParam_2 = inputDyn->stackObjects.pop();
				Object *objParam_1 = inputDyn->stackObjects.pop();
				Object *objParam_0 = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s method %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							meth_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_Structure:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "createElement"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 9 matches request form
						if ((objParam_8)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 8 matches request form
						if ((objParam_7)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 7 matches request form
						if ((objParam_6)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 6 matches request form
						if ((objParam_5)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 5 matches request form
						if ((objParam_4)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 4 matches request form
						if ((objParam_3)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->createElement((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object), (Indice)(*(Real *)objParam_3->object), (Indice)(*(Real *)objParam_4->object), (Indice)(*(Real *)objParam_5->object), (Indice)(*(Real *)objParam_6->object), (Indice)(*(Real *)objParam_7->object), (Indice)(*(Real *)objParam_8->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Structure::%s(9 args);\nAround line %d in file %s\nMethod %s not exist for class Structure with 9 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Structure::%s(9 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Structure with 9 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 9 argument(s)\nWith object %s", meth_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Method_11:
			{
				String obj_name;
				String meth_name;
				obj_name = inputDyn->bufferGetString();
				meth_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("case CMDyn_DYG_Object_Method", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}

				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters
				Object *objParam_10 = inputDyn->stackObjects.pop();
				Object *objParam_9 = inputDyn->stackObjects.pop();
				Object *objParam_8 = inputDyn->stackObjects.pop();
				Object *objParam_7 = inputDyn->stackObjects.pop();
				Object *objParam_6 = inputDyn->stackObjects.pop();
				Object *objParam_5 = inputDyn->stackObjects.pop();
				Object *objParam_4 = inputDyn->stackObjects.pop();
				Object *objParam_3 = inputDyn->stackObjects.pop();
				Object *objParam_2 = inputDyn->stackObjects.pop();
				Object *objParam_1 = inputDyn->stackObjects.pop();
				Object *objParam_0 = inputDyn->stackObjects.pop();

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s method %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							meth_name.chars());

				// now apply correct method on object
				switch (obj->getObjectType())
				{
				case DYG_Structure:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existMethod = False;
					if ((!DYG_ok) && (meth_name == "createElement"))
					{
						DYG_existMethod = True;
						Boolean run = True;

						// test if the argument 11 matches request form
						if ((objParam_10)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 10 matches request form
						if ((objParam_9)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 9 matches request form
						if ((objParam_8)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 8 matches request form
						if ((objParam_7)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 7 matches request form
						if ((objParam_6)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 6 matches request form
						if ((objParam_5)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 5 matches request form
						if ((objParam_4)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 4 matches request form
						if ((objParam_3)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 3 matches request form
						if ((objParam_2)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 2 matches request form
						if ((objParam_1)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// test if the argument 1 matches request form
						if ((objParam_0)->getObjectType() != DYG_Real)
						{
							run = False;
						}

						// ok, now apply the method on the object
						if (run)
						{
							((Structure *)obj->object)->createElement((Indice)(*(Real *)objParam_0->object), (Indice)(*(Real *)objParam_1->object), (Indice)(*(Real *)objParam_2->object), (Indice)(*(Real *)objParam_3->object), (Indice)(*(Real *)objParam_4->object), (Indice)(*(Real *)objParam_5->object), (Indice)(*(Real *)objParam_6->object), (Indice)(*(Real *)objParam_7->object), (Indice)(*(Real *)objParam_8->object), (Indice)(*(Real *)objParam_9->object), (Indice)(*(Real *)objParam_10->object));
							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existMethod)
							fatalError("Action unknown", "Structure::%s(11 args);\nAround line %d in file %s\nMethod %s not exist for class Structure with 11 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
						else
							fatalError("Wrong parameters", "Structure::%s(11 args);\nAround line %d in file %s\nIn method %s parameters types are not correct for class Structure with 11 arguments", meth_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), meth_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 11 argument(s)\nWith object %s", meth_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Operator_0:
			{
				String obj_name;
				String oper_name;
				obj_name = inputDyn->bufferGetString();
				oper_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("VARIABLE_SET", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s operator %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							oper_name.chars());

				// now apply correct operator on object
				switch (obj->getObjectType())
				{
				case DYG_String:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "String::%s(0 args);\nAround line %d in file %s\nOperator %s not exist for class String with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "String::%s(0 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class String with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_Vec3D:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Vec3D::%s(0 args);\nAround line %d in file %s\nOperator %s not exist for class Vec3D with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Vec3D::%s(0 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Vec3D with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_Vector:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Vector::%s(0 args);\nAround line %d in file %s\nOperator %s not exist for class Vector with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Vector::%s(0 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Vector with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_Matrix:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Matrix::%s(0 args);\nAround line %d in file %s\nOperator %s not exist for class Matrix with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Matrix::%s(0 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Matrix with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_Tensor2:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Tensor2::%s(0 args);\nAround line %d in file %s\nOperator %s not exist for class Tensor2 with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Tensor2::%s(0 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Tensor2 with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_DiscreteFunction:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "DiscreteFunction::%s(0 args);\nAround line %d in file %s\nOperator %s not exist for class DiscreteFunction with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "DiscreteFunction::%s(0 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class DiscreteFunction with 0 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 0 argument(s)\nWith object %s", oper_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Operator_1:
			{
				String obj_name;
				String oper_name;
				obj_name = inputDyn->bufferGetString();
				oper_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("VARIABLE_SET", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s operator %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							oper_name.chars());

				// now apply correct operator on object
				switch (obj->getObjectType())
				{
				case DYG_Real:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Real::%s(1 args);\nAround line %d in file %s\nOperator %s not exist for class Real with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Real::%s(1 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Real with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_String:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "String::%s(1 args);\nAround line %d in file %s\nOperator %s not exist for class String with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "String::%s(1 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class String with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_Vec3D:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if ((!DYG_ok) && (oper_name == "()"))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// get the argument 1 of the operator
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							Class_Real *ob = new Class_Real;
							ob->allocate();
							(*(Real *)ob->object) = (*(Vec3D *)(((Class_Vec3D *)obj)->object))((Indice)(*(Real *)(objParam_0)->object));

							inputDyn->stackObjects.push(ob); // put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (oper_name == "()="))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// apply the operator
						Object *objVar = inputDyn->stackObjects.pop();
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							(*(Vec3D *)(((Class_Vec3D *)obj)->object))((Indice)(*(Real *)(objParam_0)->object)) = (*(Real *)((Class_Real *)objVar)->object);

							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Vec3D::%s(1 args);\nAround line %d in file %s\nOperator %s not exist for class Vec3D with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Vec3D::%s(1 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Vec3D with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_Vector:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if ((!DYG_ok) && (oper_name == "()"))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// get the argument 1 of the operator
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							Class_Real *ob = new Class_Real;
							ob->allocate();
							(*(Real *)ob->object) = (*(Vector *)(((Class_Vector *)obj)->object))((Indice)(*(Real *)(objParam_0)->object));

							inputDyn->stackObjects.push(ob); // put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (oper_name == "()="))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// apply the operator
						Object *objVar = inputDyn->stackObjects.pop();
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							(*(Vector *)(((Class_Vector *)obj)->object))((Indice)(*(Real *)(objParam_0)->object)) = (*(Real *)((Class_Real *)objVar)->object);

							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Vector::%s(1 args);\nAround line %d in file %s\nOperator %s not exist for class Vector with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Vector::%s(1 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Vector with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_Matrix:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Matrix::%s(1 args);\nAround line %d in file %s\nOperator %s not exist for class Matrix with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Matrix::%s(1 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Matrix with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_Tensor2:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Tensor2::%s(1 args);\nAround line %d in file %s\nOperator %s not exist for class Tensor2 with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Tensor2::%s(1 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Tensor2 with 1 argument", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 1 argument(s)\nWith object %s", oper_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Operator_2:
			{
				String obj_name;
				String oper_name;
				obj_name = inputDyn->bufferGetString();
				oper_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("VARIABLE_SET", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s operator %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							oper_name.chars());

				// now apply correct operator on object
				switch (obj->getObjectType())
				{
				case DYG_Matrix:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if ((!DYG_ok) && (oper_name == "()"))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// get the argument 2 of the operator
						Object *objParam_1 = inputDyn->stackObjects.pop();

						// test if the argument 2 matches request form
						if ((run) && ((objParam_1)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_1);
							run = False;
						}
						// get the argument 1 of the operator
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objParam_1);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							Class_Real *ob = new Class_Real;
							ob->allocate();
							(*(Real *)ob->object) = (*(Matrix *)(((Class_Matrix *)obj)->object))((Indice)(*(Real *)(objParam_0)->object), (Indice)(*(Real *)(objParam_1)->object));

							inputDyn->stackObjects.push(ob); // put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (oper_name == "()="))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// apply the operator
						Object *objVar = inputDyn->stackObjects.pop();
						Object *objParam_1 = inputDyn->stackObjects.pop();

						// test if the argument 2 matches request form
						if ((run) && ((objParam_1)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							(*(Matrix *)(((Class_Matrix *)obj)->object))((Indice)(*(Real *)(objParam_0)->object), (Indice)(*(Real *)(objParam_1)->object)) = (*(Real *)((Class_Real *)objVar)->object);

							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Matrix::%s(2 args);\nAround line %d in file %s\nOperator %s not exist for class Matrix with 2 arguments", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Matrix::%s(2 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Matrix with 2 arguments", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				case DYG_Tensor2:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if ((!DYG_ok) && (oper_name == "()"))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// get the argument 2 of the operator
						Object *objParam_1 = inputDyn->stackObjects.pop();

						// test if the argument 2 matches request form
						if ((run) && ((objParam_1)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_1);
							run = False;
						}
						// get the argument 1 of the operator
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objParam_1);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							Class_Real *ob = new Class_Real;
							ob->allocate();
							(*(Real *)ob->object) = (*(Tensor2 *)(((Class_Tensor2 *)obj)->object))((Indice)(*(Real *)(objParam_0)->object), (Indice)(*(Real *)(objParam_1)->object));

							inputDyn->stackObjects.push(ob); // put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (oper_name == "()="))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// apply the operator
						Object *objVar = inputDyn->stackObjects.pop();
						Object *objParam_1 = inputDyn->stackObjects.pop();

						// test if the argument 2 matches request form
						if ((run) && ((objParam_1)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							(*(Tensor2 *)(((Class_Tensor2 *)obj)->object))((Indice)(*(Real *)(objParam_0)->object), (Indice)(*(Real *)(objParam_1)->object)) = (*(Real *)((Class_Real *)objVar)->object);

							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Tensor2::%s(2 args);\nAround line %d in file %s\nOperator %s not exist for class Tensor2 with 2 arguments", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Tensor2::%s(2 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Tensor2 with 2 arguments", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 2 argument(s)\nWith object %s", oper_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Operator_3:
			{
				String obj_name;
				String oper_name;
				obj_name = inputDyn->bufferGetString();
				oper_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("VARIABLE_SET", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s operator %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							oper_name.chars());

				// now apply correct operator on object
				switch (obj->getObjectType())
				{
				case DYG_Tensor3:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if ((!DYG_ok) && (oper_name == "()"))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// get the argument 3 of the operator
						Object *objParam_2 = inputDyn->stackObjects.pop();

						// test if the argument 3 matches request form
						if ((run) && ((objParam_2)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_2);
							run = False;
						}
						// get the argument 2 of the operator
						Object *objParam_1 = inputDyn->stackObjects.pop();

						// test if the argument 2 matches request form
						if ((run) && ((objParam_1)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objParam_2);
							run = False;
						}
						// get the argument 1 of the operator
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objParam_2);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							Class_Real *ob = new Class_Real;
							ob->allocate();
							(*(Real *)ob->object) = (*(Tensor3 *)(((Class_Tensor3 *)obj)->object))((Indice)(*(Real *)(objParam_0)->object), (Indice)(*(Real *)(objParam_1)->object), (Indice)(*(Real *)(objParam_2)->object));

							inputDyn->stackObjects.push(ob); // put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (oper_name == "()="))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// apply the operator
						Object *objVar = inputDyn->stackObjects.pop();
						Object *objParam_2 = inputDyn->stackObjects.pop();

						// test if the argument 3 matches request form
						if ((run) && ((objParam_2)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_2);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}
						Object *objParam_1 = inputDyn->stackObjects.pop();

						// test if the argument 2 matches request form
						if ((run) && ((objParam_1)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objParam_2);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objParam_2);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							(*(Tensor3 *)(((Class_Tensor3 *)obj)->object))((Indice)(*(Real *)(objParam_0)->object), (Indice)(*(Real *)(objParam_1)->object), (Indice)(*(Real *)(objParam_2)->object)) = (*(Real *)((Class_Real *)objVar)->object);

							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Tensor3::%s(3 args);\nAround line %d in file %s\nOperator %s not exist for class Tensor3 with 3 arguments", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Tensor3::%s(3 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Tensor3 with 3 arguments", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 3 argument(s)\nWith object %s", oper_name.chars(), obj_name.chars());
				}
			}
			break;

			case CMDyn_DYG_Object_Operator_4:
			{
				String obj_name;
				String oper_name;
				obj_name = inputDyn->bufferGetString();
				oper_name = inputDyn->bufferGetString();

				// scan the object list
				if (!objectList.existObject(obj_name))
				{
					fatalError("VARIABLE_SET", "Object %s not existing line %ld", obj_name.chars(), scanned_line[current_scanned_file]);
				}
				// object exists, get it
				Object *obj = objectList.getObject(obj_name);

				// now get the list of parameters

				// trace the language
				if (inputDyn->traceLanguage)
					fprintf(inputDyn->pfileTraceLanguage, "file %s line %d -> Object %s::%s operator %s applied\n",
							scanned_file[current_scanned_file].chars(),
							scanned_line[current_scanned_file],
							obj->getObjectClass().chars(),
							obj_name.chars(),
							oper_name.chars());

				// now apply correct operator on object
				switch (obj->getObjectType())
				{
				case DYG_Tensor4:
				{
					Boolean DYG_ok = False;
					Boolean DYG_existOperator = False;
					if ((!DYG_ok) && (oper_name == "()"))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// get the argument 4 of the operator
						Object *objParam_3 = inputDyn->stackObjects.pop();

						// test if the argument 4 matches request form
						if ((run) && ((objParam_3)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_3);
							run = False;
						}
						// get the argument 3 of the operator
						Object *objParam_2 = inputDyn->stackObjects.pop();

						// test if the argument 3 matches request form
						if ((run) && ((objParam_2)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_2);
							inputDyn->stackObjects.push(objParam_3);
							run = False;
						}
						// get the argument 2 of the operator
						Object *objParam_1 = inputDyn->stackObjects.pop();

						// test if the argument 2 matches request form
						if ((run) && ((objParam_1)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objParam_2);
							inputDyn->stackObjects.push(objParam_3);
							run = False;
						}
						// get the argument 1 of the operator
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objParam_2);
							inputDyn->stackObjects.push(objParam_3);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							Class_Real *ob = new Class_Real;
							ob->allocate();
							(*(Real *)ob->object) = (*(Tensor4 *)(((Class_Tensor4 *)obj)->object))((Indice)(*(Real *)(objParam_0)->object), (Indice)(*(Real *)(objParam_1)->object), (Indice)(*(Real *)(objParam_2)->object), (Indice)(*(Real *)(objParam_3)->object));

							inputDyn->stackObjects.push(ob); // put ok to true
							DYG_ok = True;
						}
					}
					if ((!DYG_ok) && (oper_name == "()="))
					{
						DYG_existOperator = True;
						Boolean run = True;

						// apply the operator
						Object *objVar = inputDyn->stackObjects.pop();
						Object *objParam_3 = inputDyn->stackObjects.pop();

						// test if the argument 4 matches request form
						if ((run) && ((objParam_3)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_3);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}
						Object *objParam_2 = inputDyn->stackObjects.pop();

						// test if the argument 3 matches request form
						if ((run) && ((objParam_2)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_2);
							inputDyn->stackObjects.push(objParam_3);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}
						Object *objParam_1 = inputDyn->stackObjects.pop();

						// test if the argument 2 matches request form
						if ((run) && ((objParam_1)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objParam_2);
							inputDyn->stackObjects.push(objParam_3);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}
						Object *objParam_0 = inputDyn->stackObjects.pop();

						// test if the argument 1 matches request form
						if ((run) && ((objParam_0)->getObjectType() != DYG_Real))
						{

							// push the parameters to the stack
							inputDyn->stackObjects.push(objParam_0);
							inputDyn->stackObjects.push(objParam_1);
							inputDyn->stackObjects.push(objParam_2);
							inputDyn->stackObjects.push(objParam_3);
							inputDyn->stackObjects.push(objVar);
							run = False;
						}

						// ok, now apply the operator on the object
						if (run)
						{
							(*(Tensor4 *)(((Class_Tensor4 *)obj)->object))((Indice)(*(Real *)(objParam_0)->object), (Indice)(*(Real *)(objParam_1)->object), (Indice)(*(Real *)(objParam_2)->object), (Indice)(*(Real *)(objParam_3)->object)) = (*(Real *)((Class_Real *)objVar)->object);

							// put ok to true
							DYG_ok = True;
						}
					}
					if (!DYG_ok)
					{
						if (!DYG_existOperator)
							fatalError("Action unknown", "Tensor4::%s(4 args);\nAround line %d in file %s\nOperator %s not exist for class Tensor4 with 4 arguments", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
						else
							fatalError("Wrong parameters", "Tensor4::%s(4 args);\nAround line %d in file %s\nIn operator %s parameters types are not correct for class Tensor4 with 4 arguments", oper_name.chars(), scanned_line[current_scanned_file], scanned_file[current_scanned_file].chars(), oper_name.chars());
					}
				}
				break;
				default:
					fatalError("Action unknown", "Tried to apply %s method with 4 argument(s)\nWith object %s", oper_name.chars(), obj_name.chars());
				}
			}
			break;

			case BINARY_OPERATOR:
			{
				Boolean ok = False;
				// operator to use
				String op = inputDyn->bufferGetString();

				// get the arguments
				Object *obj2 = inputDyn->stackObjects.pop();
				Object *obj1 = inputDyn->stackObjects.pop();

				if (op == "+")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) + (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_String) && (obj2->getObjectType() == DYG_String))
					{
						Class_String *sol = new Class_String;
						(*(String *)sol->object) = (*(String *)(obj1)->object) + (*(String *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vec3D) && (obj2->getObjectType() == DYG_Vec3D))
					{
						Class_Vec3D *sol = new Class_Vec3D;
						(*(Vec3D *)sol->object) = (*(Vec3D *)(obj1)->object) + (*(Vec3D *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vector) && (obj2->getObjectType() == DYG_Vector))
					{
						Class_Vector *sol = new Class_Vector;
						(*(Vector *)sol->object) = (*(Vector *)(obj1)->object) + (*(Vector *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Matrix) && (obj2->getObjectType() == DYG_Matrix))
					{
						Class_Matrix *sol = new Class_Matrix;
						(*(Matrix *)sol->object) = (*(Matrix *)(obj1)->object) + (*(Matrix *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Tensor2) && (obj2->getObjectType() == DYG_Tensor2))
					{
						Class_Tensor2 *sol = new Class_Tensor2;
						(*(Tensor2 *)sol->object) = (*(Tensor2 *)(obj1)->object) + (*(Tensor2 *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply + operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == "-")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) - (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vec3D) && (obj2->getObjectType() == DYG_Vec3D))
					{
						Class_Vec3D *sol = new Class_Vec3D;
						(*(Vec3D *)sol->object) = (*(Vec3D *)(obj1)->object) - (*(Vec3D *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vector) && (obj2->getObjectType() == DYG_Vector))
					{
						Class_Vector *sol = new Class_Vector;
						(*(Vector *)sol->object) = (*(Vector *)(obj1)->object) - (*(Vector *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Matrix) && (obj2->getObjectType() == DYG_Matrix))
					{
						Class_Matrix *sol = new Class_Matrix;
						(*(Matrix *)sol->object) = (*(Matrix *)(obj1)->object) - (*(Matrix *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Tensor2) && (obj2->getObjectType() == DYG_Tensor2))
					{
						Class_Tensor2 *sol = new Class_Tensor2;
						(*(Tensor2 *)sol->object) = (*(Tensor2 *)(obj1)->object) - (*(Tensor2 *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply - operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == "*")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) * (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Vector))
					{
						Class_Vector *sol = new Class_Vector;
						(*(Vector *)sol->object) = (*(Real *)(obj1)->object) * (*(Vector *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Vec3D))
					{
						Class_Vec3D *sol = new Class_Vec3D;
						(*(Vec3D *)sol->object) = (*(Real *)(obj1)->object) * (*(Vec3D *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vector) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Vector *sol = new Class_Vector;
						(*(Vector *)sol->object) = (*(Vector *)(obj1)->object) * (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Matrix) && (obj2->getObjectType() == DYG_Vector))
					{
						Class_Vector *sol = new Class_Vector;
						(*(Vector *)sol->object) = (*(Matrix *)(obj1)->object) * (*(Vector *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Matrix) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Matrix *sol = new Class_Matrix;
						(*(Matrix *)sol->object) = (*(Matrix *)(obj1)->object) * (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Matrix) && (obj2->getObjectType() == DYG_Matrix))
					{
						Class_Matrix *sol = new Class_Matrix;
						(*(Matrix *)sol->object) = (*(Matrix *)(obj1)->object) * (*(Matrix *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Tensor2) && (obj2->getObjectType() == DYG_Tensor2))
					{
						Class_Tensor2 *sol = new Class_Tensor2;
						(*(Tensor2 *)sol->object) = (*(Tensor2 *)(obj1)->object) * (*(Tensor2 *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Tensor2) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Tensor2 *sol = new Class_Tensor2;
						(*(Tensor2 *)sol->object) = (*(Tensor2 *)(obj1)->object) * (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Tensor2) && (obj2->getObjectType() == DYG_Vec3D))
					{
						Class_Vec3D *sol = new Class_Vec3D;
						(*(Vec3D *)sol->object) = (*(Tensor2 *)(obj1)->object) * (*(Vec3D *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply * operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == "/")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) / (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vec3D) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Vec3D *sol = new Class_Vec3D;
						(*(Vec3D *)sol->object) = (*(Vec3D *)(obj1)->object) / (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vector) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Vector *sol = new Class_Vector;
						(*(Vector *)sol->object) = (*(Vector *)(obj1)->object) / (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Matrix) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Matrix *sol = new Class_Matrix;
						(*(Matrix *)sol->object) = (*(Matrix *)(obj1)->object) / (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Tensor2) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Tensor2 *sol = new Class_Tensor2;
						(*(Tensor2 *)sol->object) = (*(Tensor2 *)(obj1)->object) / (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply / operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == "^")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = pow((*(Real *)(obj1)->object), (*(Real *)(obj2)->object));
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply ^ operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				// operator to use
				if (op == "<")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) < (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_String) && (obj2->getObjectType() == DYG_String))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(String *)(obj1)->object) < (*(String *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply < operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == ">")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) > (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_String) && (obj2->getObjectType() == DYG_String))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(String *)(obj1)->object) > (*(String *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply > operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == "<=")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) <= (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_String) && (obj2->getObjectType() == DYG_String))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(String *)(obj1)->object) <= (*(String *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply <= operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == ">=")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) >= (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_String) && (obj2->getObjectType() == DYG_String))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(String *)(obj1)->object) >= (*(String *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply >= operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == "==")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) == (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_String) && (obj2->getObjectType() == DYG_String))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(String *)(obj1)->object) == (*(String *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vec3D) && (obj2->getObjectType() == DYG_Vec3D))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(Vec3D *)(obj1)->object) == (*(Vec3D *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vector) && (obj2->getObjectType() == DYG_Vector))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(Vector *)(obj1)->object) == (*(Vector *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Matrix) && (obj2->getObjectType() == DYG_Matrix))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(Matrix *)(obj1)->object) == (*(Matrix *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply == operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == "!=")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) != (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_String) && (obj2->getObjectType() == DYG_String))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(String *)(obj1)->object) != (*(String *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vec3D) && (obj2->getObjectType() == DYG_Vec3D))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(Vec3D *)(obj1)->object) != (*(Vec3D *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Vector) && (obj2->getObjectType() == DYG_Vector))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(Vector *)(obj1)->object) != (*(Vector *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if ((obj1->getObjectType() == DYG_Matrix) && (obj2->getObjectType() == DYG_Matrix))
					{
						Class_Boolean *sol = new Class_Boolean;
						(*(Boolean *)sol->object) = (*(Matrix *)(obj1)->object) != (*(Matrix *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply != operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == "&&")
				{
					if ((obj1->getObjectType() == DYG_Real) && (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) && (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply && operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}

				if (op == "||")
				{
					if ((obj1->getObjectType() == DYG_Real) || (obj2->getObjectType() == DYG_Real))
					{
						Class_Real *sol = new Class_Real;
						(*(Real *)sol->object) = (*(Real *)(obj1)->object) || (*(Real *)(obj2)->object);
						inputDyn->stackObjects.push(sol);
						ok = True;
					}
					if (!ok)
					{
						fatalError("BINARY_OPERATOR", "Don't know how to apply || operator on %s and %s\n", obj1->getObjectClass().chars(), obj2->getObjectClass().chars());
					}
				}
			}
			break;

				/* Auto include part ends here */

			case CMDyn_INTERNAL_INCLUDE:
			{
				current_scanned_file++;
				scanned_line[current_scanned_file] = 0;
				scanned_file[current_scanned_file] = inputDyn->bufferGetString();
			}
			break;
			case CMDyn_INTERNAL_END_INCLUDE:
			{
				current_scanned_file--;
				break;
			}

			default:
				fatalError("Unknown command in yacc grammar", "must correct %d \n", command);
			}
		}

		// restart the read buffer
		inputDyn->currentBufferIndice = 0;

		// reset the maxBuffer
		inputDyn->maxBuffer = 0;
		inputDyn->stackObjects.flush();
	}

	// write the log file if structure exists
	if (Global_Structure != NULL)
	{
		(*Global_Structure->logFile) << "\n\nEnd of Preprocessing Phase .............\n\n";
		(*Global_Structure->logFile) << "List of objects\n";
		(*Global_Structure->logFile) << objectList.list.size() << " objects in memory\n";
		for (Indice i = 0; i < objectList.list.size(); i++)
		{
			(*Global_Structure->logFile) << objectList.getObject(i)->getObjectClass();
			for (Indice j = objectList.getObject(i)->getObjectClass().length(); j < 30; j++)
				(*Global_Structure->logFile) << " ";
			(*Global_Structure->logFile) << " " << objectList.getObject(i)->getObjectName() << "\n";
		}
		(*Global_Structure->logFile) << "\n";

		// online display
		Global_Structure->displayOnline();
	}

	// message display
	cout << "\nEnd parsing source file\n";

	// close trace file if open
	if (inputDyn->pfileTraceLanguage != NULL)
		fclose(inputDyn->pfileTraceLanguage);
}
// This method has been introduced since DynELA v. 1.1
//
// This one is used to check if an object is valuable for a given type
// For example you can test if an object of Type ElQua4n2D has a relation with Element
//
// Usage:
// if (isHeritedFrom(objectList.getObject (ob)->getObjectType(), DYG_IsotropicHardening)) {}
//
// return True if the object is IsotropicHardening or any one of the derived objects from this class
//
//-----------------------------------------------------------------------------
Boolean isHeritedFrom(Indice obj, Indice top)
//-----------------------------------------------------------------------------
{
	if (obj == top)
		return True;

	switch (top)
	{
	case DYG_ContactLaw:
	{
		if (obj == DYG_CoulombLaw)
			return True;
	}
	break;
	case DYG_Element:
	{
		if (obj == DYG_ElQua4n2D)
			return True;
		if (obj == DYG_ElHex8n3D)
			return True;
		if (obj == DYG_ElTri3n2D)
			return True;
		if (obj == DYG_ElQua4nAx)
			return True;
		if (obj == DYG_ElTet4n3D)
			return True;
		if (obj == DYG_ElTet10n3D)
			return True;
	}
	break;
	case DYG_IsotropicHardening:
	{
		if (obj == DYG_IsoHardElastic)
			return True;
		if (obj == DYG_IsoHardElastoplasticTabular)
			return True;
		if (obj == DYG_IsoHardElastoplastic)
			return True;
		if (obj == DYG_IsoHardMetafor)
			return True;
		if (obj == DYG_IsoHardPower)
			return True;
		if (obj == DYG_IsoHardJohnsonCook)
			return True;
	}
	break;
	case DYG_Boundary:
	{
		if (obj == DYG_BoundaryDisplacement)
			return True;
		if (obj == DYG_BoundaryRestrain)
			return True;
		if (obj == DYG_BoundaryAcceleration)
			return True;
		if (obj == DYG_BoundaryForce)
			return True;
		if (obj == DYG_BoundarySpeed)
			return True;
		if (obj == DYG_BoundaryTemperature)
			return True;
		if (obj == DYG_BoundaryFlux)
			return True;
		if (obj == DYG_BoundaryConvection)
			return True;
	}
	break;
	case DYG_Solver:
	{
		if (obj == DYG_ExplicitSolver)
			return True;
		if (obj == DYG_ExplicitSolverCH)
			return True;
	}
	break;
	case DYG_io_Structure:
	{
		if (obj == DYG_io_Data)
			return True;
	}
	break;
	case DYG_Function:
	{
		if (obj == DYG_PolynomialFunction)
			return True;
		if (obj == DYG_SinusFunction)
			return True;
		if (obj == DYG_RampFunction)
			return True;
		if (obj == DYG_DiscreteFunction)
			return True;
	}
	break;
	case DYG_Mesher:
	{
		if (obj == DYG_BoxMesher)
			return True;
	}
	break;
	}
	return False;
}
