%{
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

// includes classiques
#include <ObjectList.h>
#include <InputDyn.h>
#include <basicTools.h>

// moa.. j'aime pas les warnings de compilation alors je mets
#define YY_NO_UNPUT

extern int yylex();
int InputDynwrap() {return yywrap();}
extern FILE* InputDynin;

// inclusion des d�finitions du yacc
#include "InputDyn_yacc.hpp"

%}
D       [0-9]
E       [DEde][-+]?{D}+
%a 	1500
%o	2000
%p	1600
%n	1850 
%s COMMENT
%%
"/*"                        {BEGIN COMMENT;}
<COMMENT>.*"*/"             {BEGIN 0;}
<COMMENT>.*                 ;
"//".*\n                    scanned_line[current_scanned_file]++;
\"[^\"]+\"                  {
                              strcpy(InputDynlval.nom,&yytext[1]);
                              InputDynlval.nom[strlen(yytext)-2]='\0';
                              return(STRING);
                            }
[ \t]+                      ;
\n                          {
                              scanned_line[current_scanned_file]++;
                            }
{D}+                        {
                              InputDynlval.entier=atoi(yytext);
                              return(ENTIER);
                            }
{D}+"."{D}*({E})? |
{D}*"."{D}+({E})? |
{D}+{E}                     {
                              sscanf(yytext,"%lf",&InputDynlval.real); 
                              return(REEL);
                            } 
"++"                        {return(PLUSPLUS);}
"--"                        {return(MOINSMOINS);}
"+="                        {return(PLUEGAL);}
"-="                        {return(MOIEGAL);}
"*="                        {return(MULEGAL);}
"/="                        {return(DIVEGAL);}
"=="                        {return(EQUAL);}
"!="                        {return(DIFFERENT);}
"<="                        {return(INFEQUAL);}
">="                        {return(SUPEQUAL);}
"&&"                        {return(AND);}
"||"                        {return(OR);}
"="                         {return(AFFECT);}
"<"                         {return(INFERIOR);}
">"                         {return(SUPERIOR);}
"!"                         {return(NOT);}
"<<"                        {return(OUTPUT);}
"."                         {return(DOT);}

"abs"                       {return(ABS);}
"acos"                      {return(ACOS);}
"asin"                      {return(ASIN);}
"atan"                      {return(ATAN);}

"cos"                       {return(COS);}
"cosh"                      {return(COSH);}

"DYNELA_BEGIN_INCLUDE"      {return(INTERNAL_INCLUDE);}
"DYNELA_END_INCLUDE"        {return(INTERNAL_END_INCLUDE);}

"END"                       {return(FIN);}
"erf"                       {return(ERF);}
"exp"                       {return(EXP);}
"EXTRACT"                   {return(EXTRACT);}

"fclose"                    {return(FCLOSE);}
"fopen"                     {return(FOPEN);}
"for"                       {return(FOR);}

"if"                        {return(IF);}
"int"                       {return(INT);}

"log"                       {return(LOG);}
"ln"                        {return(LN);}

"PI"                        {return(PI_VALUE);}
"pragma"                    {return(PRAGMA);}
"PREPROCESSOR"              {return(PREPROCESSOR);}

"round"                     {return(ROUND);}

"sin"                       {return(SIN);}
"sinh"                      {return(SINH);}
"sqrt"                      {return(SQRT);}

"tan"                       {return(TAN);}
"tanh"                      {return(TANH);}

"Real" {
#ifdef lexParserVerbose
cout << "lex word Real found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Real);
}

"Boolean" {
#ifdef lexParserVerbose
cout << "lex word Boolean found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Boolean);
}

"String" {
#ifdef lexParserVerbose
cout << "lex word String found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_String);
}

"Node" {
#ifdef lexParserVerbose
cout << "lex word Node found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Node);
}

"Domain" {
#ifdef lexParserVerbose
cout << "lex word Domain found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Domain);
}

"ElQua4n2D" {
#ifdef lexParserVerbose
cout << "lex word ElQua4n2D found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ElQua4n2D);
}

"ElHex8n3D" {
#ifdef lexParserVerbose
cout << "lex word ElHex8n3D found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ElHex8n3D);
}

"ElTri3n2D" {
#ifdef lexParserVerbose
cout << "lex word ElTri3n2D found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ElTri3n2D);
}

"ElQua4nAx" {
#ifdef lexParserVerbose
cout << "lex word ElQua4nAx found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ElQua4nAx);
}

"ElTet4n3D" {
#ifdef lexParserVerbose
cout << "lex word ElTet4n3D found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ElTet4n3D);
}

"ElTet10n3D" {
#ifdef lexParserVerbose
cout << "lex word ElTet10n3D found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ElTet10n3D);
}

"Vec3D" {
#ifdef lexParserVerbose
cout << "lex word Vec3D found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Vec3D);
}

"Vector" {
#ifdef lexParserVerbose
cout << "lex word Vector found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Vector);
}

"Matrix" {
#ifdef lexParserVerbose
cout << "lex word Matrix found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Matrix);
}

"Tensor2" {
#ifdef lexParserVerbose
cout << "lex word Tensor2 found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Tensor2);
}

"Tensor3" {
#ifdef lexParserVerbose
cout << "lex word Tensor3 found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Tensor3);
}

"Tensor4" {
#ifdef lexParserVerbose
cout << "lex word Tensor4 found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Tensor4);
}

"ContactLaw" {
#ifdef lexParserVerbose
cout << "lex word ContactLaw found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ContactLaw);
}

"Element" {
#ifdef lexParserVerbose
cout << "lex word Element found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Element);
}

"Select" {
#ifdef lexParserVerbose
cout << "lex word Select found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Select);
}

"NodeSet" {
#ifdef lexParserVerbose
cout << "lex word NodeSet found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_NodeSet);
}

"ElementSet" {
#ifdef lexParserVerbose
cout << "lex word ElementSet found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ElementSet);
}

"Structure" {
#ifdef lexParserVerbose
cout << "lex word Structure found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Structure);
}

"DiscreteFunction" {
#ifdef lexParserVerbose
cout << "lex word DiscreteFunction found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_DiscreteFunction);
}

"PolynomialFunction" {
#ifdef lexParserVerbose
cout << "lex word PolynomialFunction found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_PolynomialFunction);
}

"SinusFunction" {
#ifdef lexParserVerbose
cout << "lex word SinusFunction found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_SinusFunction);
}

"DiscreteFunctionSet" {
#ifdef lexParserVerbose
cout << "lex word DiscreteFunctionSet found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_DiscreteFunctionSet);
}

"RampFunction" {
#ifdef lexParserVerbose
cout << "lex word RampFunction found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_RampFunction);
}

"Material" {
#ifdef lexParserVerbose
cout << "lex word Material found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Material);
}

"IsotropicHardening" {
#ifdef lexParserVerbose
cout << "lex word IsotropicHardening found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_IsotropicHardening);
}

"IsoHardElastic" {
#ifdef lexParserVerbose
cout << "lex word IsoHardElastic found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_IsoHardElastic);
}

"IsoHardElastoplasticTabular" {
#ifdef lexParserVerbose
cout << "lex word IsoHardElastoplasticTabular found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_IsoHardElastoplasticTabular);
}

"IsoHardElastoplastic" {
#ifdef lexParserVerbose
cout << "lex word IsoHardElastoplastic found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_IsoHardElastoplastic);
}

"IsoHardMetafor" {
#ifdef lexParserVerbose
cout << "lex word IsoHardMetafor found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_IsoHardMetafor);
}

"IsoHardPower" {
#ifdef lexParserVerbose
cout << "lex word IsoHardPower found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_IsoHardPower);
}

"IsoHardJohnsonCook" {
#ifdef lexParserVerbose
cout << "lex word IsoHardJohnsonCook found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_IsoHardJohnsonCook);
}

"Interface" {
#ifdef lexParserVerbose
cout << "lex word Interface found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Interface);
}

"Boundary" {
#ifdef lexParserVerbose
cout << "lex word Boundary found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Boundary);
}

"BoundaryCondition" {
#ifdef lexParserVerbose
cout << "lex word BoundaryCondition found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoundaryCondition);
}

"BoundarySpeed" {
#ifdef lexParserVerbose
cout << "lex word BoundarySpeed found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoundarySpeed);
}

"BoundaryAcceleration" {
#ifdef lexParserVerbose
cout << "lex word BoundaryAcceleration found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoundaryAcceleration);
}

"BoundaryDisplacement" {
#ifdef lexParserVerbose
cout << "lex word BoundaryDisplacement found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoundaryDisplacement);
}

"BoundaryRestrain" {
#ifdef lexParserVerbose
cout << "lex word BoundaryRestrain found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoundaryRestrain);
}

"BoundaryForce" {
#ifdef lexParserVerbose
cout << "lex word BoundaryForce found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoundaryForce);
}

"BoundaryTemperature" {
#ifdef lexParserVerbose
cout << "lex word BoundaryTemperature found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoundaryTemperature);
}

"BoundaryFlux" {
#ifdef lexParserVerbose
cout << "lex word BoundaryFlux found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoundaryFlux);
}

"BoundaryConvection" {
#ifdef lexParserVerbose
cout << "lex word BoundaryConvection found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoundaryConvection);
}

"HistoryFile" {
#ifdef lexParserVerbose
cout << "lex word HistoryFile found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_HistoryFile);
}

"Solver" {
#ifdef lexParserVerbose
cout << "lex word Solver found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Solver);
}

"ExplicitSolver" {
#ifdef lexParserVerbose
cout << "lex word ExplicitSolver found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ExplicitSolver);
}

"ExplicitSolverCH" {
#ifdef lexParserVerbose
cout << "lex word ExplicitSolverCH found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_ExplicitSolverCH);
}

"io_Structure" {
#ifdef lexParserVerbose
cout << "lex word io_Structure found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_io_Structure);
}

"Function" {
#ifdef lexParserVerbose
cout << "lex word Function found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Function);
}

"Side" {
#ifdef lexParserVerbose
cout << "lex word Side found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Side);
}

"CoulombLaw" {
#ifdef lexParserVerbose
cout << "lex word CoulombLaw found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_CoulombLaw);
}

"io_Data" {
#ifdef lexParserVerbose
cout << "lex word io_Data found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_io_Data);
}

"Mesher" {
#ifdef lexParserVerbose
cout << "lex word Mesher found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_Mesher);
}

"BoxMesher" {
#ifdef lexParserVerbose
cout << "lex word BoxMesher found line "<<scanned_line[current_scanned_file]<<"\n";
#endif
return(DYG_Class_BoxMesher);
}

[a-zA-Z0-9_]+               {
                              strcpy(InputDynlval.nom,yytext);
                              return(MOT);
                            }
.                           {return(yytext[0]);}
