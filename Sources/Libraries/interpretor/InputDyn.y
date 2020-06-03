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

%{
#include <InputDyn.h>
%}

%start source
%token  <entier> ENTIER
%token  <real>   REEL
%token  <nom>    MOT STRING
%token  <cmd>    DOT
%token  <cmd>    ABS INT SQRT COS SIN TAN ACOS ASIN ATAN COSH SINH TANH ERF EXP LN LOG PI_VALUE ROUND
%token  <cmd>    EQUAL DIFFERENT INFEQUAL SUPEQUAL AND OR AFFECT INFERIOR SUPERIOR NOT
%token  <cmd>    FIN FOR IF INTERNAL_INCLUDE INTERNAL_END_INCLUDE
%token  <cmd>    FOPEN FCLOSE OUTPUT  PREPROCESSOR EXTRACT PRAGMA

/* Auto include part starts here */
%token  <cmd>    DYG_Class_Real
%token  <cmd>    DYG_Class_Boolean
%token  <cmd>    DYG_Class_String
%token  <cmd>    DYG_Class_Node
%token  <cmd>    DYG_Class_Domain
%token  <cmd>    DYG_Class_ElQua4n2D
%token  <cmd>    DYG_Class_ElHex8n3D
%token  <cmd>    DYG_Class_ElTri3n2D
%token  <cmd>    DYG_Class_ElQua4nAx
%token  <cmd>    DYG_Class_ElTet4n3D
%token  <cmd>    DYG_Class_ElTet10n3D
%token  <cmd>    DYG_Class_Vec3D
%token  <cmd>    DYG_Class_Vector
%token  <cmd>    DYG_Class_Matrix
%token  <cmd>    DYG_Class_Tensor2
%token  <cmd>    DYG_Class_Tensor3
%token  <cmd>    DYG_Class_Tensor4
%token  <cmd>    DYG_Class_ContactLaw
%token  <cmd>    DYG_Class_Element
%token  <cmd>    DYG_Class_Select
%token  <cmd>    DYG_Class_NodeSet
%token  <cmd>    DYG_Class_ElementSet
%token  <cmd>    DYG_Class_Structure
%token  <cmd>    DYG_Class_DiscreteFunction
%token  <cmd>    DYG_Class_PolynomialFunction
%token  <cmd>    DYG_Class_SinusFunction
%token  <cmd>    DYG_Class_DiscreteFunctionSet
%token  <cmd>    DYG_Class_RampFunction
%token  <cmd>    DYG_Class_Material
%token  <cmd>    DYG_Class_IsotropicHardening
%token  <cmd>    DYG_Class_IsoHardElastic
%token  <cmd>    DYG_Class_IsoHardElastoplasticTabular
%token  <cmd>    DYG_Class_IsoHardElastoplastic
%token  <cmd>    DYG_Class_IsoHardMetafor
%token  <cmd>    DYG_Class_IsoHardPower
%token  <cmd>    DYG_Class_IsoHardJohnsonCook
%token  <cmd>    DYG_Class_Interface
%token  <cmd>    DYG_Class_Boundary
%token  <cmd>    DYG_Class_BoundaryCondition
%token  <cmd>    DYG_Class_BoundarySpeed
%token  <cmd>    DYG_Class_BoundaryAcceleration
%token  <cmd>    DYG_Class_BoundaryDisplacement
%token  <cmd>    DYG_Class_BoundaryRestrain
%token  <cmd>    DYG_Class_BoundaryForce
%token  <cmd>    DYG_Class_BoundaryTemperature
%token  <cmd>    DYG_Class_BoundaryFlux
%token  <cmd>    DYG_Class_BoundaryConvection
%token  <cmd>    DYG_Class_HistoryFile
%token  <cmd>    DYG_Class_Solver
%token  <cmd>    DYG_Class_ExplicitSolver
%token  <cmd>    DYG_Class_ExplicitSolverCH
%token  <cmd>    DYG_Class_io_Structure
%token  <cmd>    DYG_Class_Function
%token  <cmd>    DYG_Class_Side
%token  <cmd>    DYG_Class_CoulombLaw
%token  <cmd>    DYG_Class_io_Data
%token  <cmd>    DYG_Class_Mesher
%token  <cmd>    DYG_Class_BoxMesher
/* Auto include part ends here */

%union {
  double  real;
  int     entier;
  int     cmd;
  int     arg;
  char    nom[5000];
}

%type <real> type_val operation increment fonction compare
%right AFFECT PLUEGAL MOIEGAL DIVEGAL MULEGAL
%left EQUAL DIFFERENT INFEQUAL SUPEQUAL AND OR INFERIOR SUPERIOR NOT
%left '+' '-'
%left '*' '/'
%left '^'
%left PLUSPLUS MOINSMOINS
%nonassoc UMINUS
%%

/*****************************************************************************************
 * ANALYSE DU SOURCE
 *****************************************************************************************/
source:
commandes
| commandes source
;

commandes :
 instruction ';'  {
  inputDyn->bufferSetCommand(CMDyn_END);
  return Success; 
}
| BLOCK_D instructions BLOCK_E ';' {
inputDyn->bufferSetCommand(CMDyn_END);
  return Success; 
}
;

commandes2 :
instruction  {
  inputDyn->bufferSetCommand(STACK_FLUSH);
}
| BLOCK_D instructions BLOCK_E {
inputDyn->bufferSetCommand(STACK_FLUSH);
}
;

instructions:
instruction ';' {
  inputDyn->bufferSetCommand(STACK_FLUSH);
}
| instruction ';' instructions {
  inputDyn->bufferSetCommand(STACK_FLUSH);
}
| BLOCK_D instructions BLOCK_E ';' instructions {
inputDyn->bufferSetCommand(STACK_FLUSH);
}
| BLOCK_D instructions BLOCK_E ';' {
inputDyn->bufferSetCommand(STACK_FLUSH);
}
;

BLOCK_D:
'{' {
  inputDyn->bufferSetCommand(CMDyn_PAREN_BEGIN);
}
;
BLOCK_E:
'}' {
  inputDyn->bufferSetCommand(CMDyn_PAREN_END);
}
;

instruction :
expression
| FOR '(' expression {
  inputDyn->bufferSetCommand(CMDyn_FOR_START);
}
';' expression {
  inputDyn->bufferSetCommand(CMDyn_FOR_START1);
}
';' expression {
  inputDyn->bufferSetCommand(CMDyn_FOR_START2);
}
')' commandes2 {
  inputDyn->bufferSetCommand(CMDyn_FOR_END);
}
| IF '(' expression {
  inputDyn->bufferSetCommand(CMDyn_IF_START);
}  
')' commandes2 {
  inputDyn->bufferSetCommand(CMDyn_IF);
}
| fileInOut
| '#' pragma
| cmdfin
;

pragma:
PRAGMA MOT {
  inputDyn->bufferSetCommand(CMDyn_PRAGMA);
  inputDyn->bufferSetString($2);
}
| PRAGMA MOT MOT {
  String str;
  inputDyn->bufferSetCommand(CMDyn_PRAGMA);
  str=$2;
  str+=" ";
  str+=$3;
  inputDyn->bufferSetString(str);
}
;

/*****************************************************************************************
 * TRAITEMENT DES OBJETS
 *****************************************************************************************/
/* Auto include part starts here */
objectCommand:
DYG_Class_Real MOT AFFECT expression {
#ifdef yaccParserVerbose
cout << "yacc object Real constructor and initialisation found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Real_Constructor);
inputDyn->bufferSetString($2);
inputDyn->bufferSetCommand(VARIABLE_SET);
inputDyn->bufferSetString($2);
}
| DYG_Class_String MOT AFFECT expression {
#ifdef yaccParserVerbose
cout << "yacc object String constructor and initialisation found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_String_Constructor);
inputDyn->bufferSetString($2);
inputDyn->bufferSetCommand(VARIABLE_SET);
inputDyn->bufferSetString($2);
}
| MOT DOT MOT '('')' {
#ifdef yaccParserVerbose
cout << "yacc object method 0 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Method_0);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString($3);
}
| MOT DOT MOT '(' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object method 1 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Method_1);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString($3);
}
| MOT DOT MOT '(' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object method 2 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Method_2);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString($3);
}
| MOT DOT MOT '(' expression ',' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object method 3 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Method_3);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString($3);
}
| MOT DOT MOT '(' expression ',' expression ',' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object method 4 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Method_4);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString($3);
}
| MOT DOT MOT '(' expression ',' expression ',' expression ',' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object method 5 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Method_5);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString($3);
}
| MOT DOT MOT '(' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object method 8 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Method_8);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString($3);
}
| MOT DOT MOT '(' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object method 9 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Method_9);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString($3);
}
| MOT DOT MOT '(' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object method 11 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Method_11);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString($3);
}
| MOT '('')' {
#ifdef yaccParserVerbose
cout << "yacc object operator () with 0 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_0);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()");
}
| MOT '('')' AFFECT expression {
#ifdef yaccParserVerbose
cout << "yacc object operator () = with 0 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_0);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()=");
}
| MOT '(' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object operator () with 1 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_1);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()");
}
| MOT '(' expression ')' AFFECT expression {
#ifdef yaccParserVerbose
cout << "yacc object operator () = with 1 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_1);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()=");
}
| MOT '(' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object operator () with 2 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_2);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()");
}
| MOT '(' expression ',' expression ')' AFFECT expression {
#ifdef yaccParserVerbose
cout << "yacc object operator () = with 2 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_2);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()=");
}
| MOT '(' expression ',' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object operator () with 3 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_3);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()");
}
| MOT '(' expression ',' expression ',' expression ')' AFFECT expression {
#ifdef yaccParserVerbose
cout << "yacc object operator () = with 3 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_3);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()=");
}
| MOT '(' expression ',' expression ',' expression ',' expression ')' {
#ifdef yaccParserVerbose
cout << "yacc object operator () with 4 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_4);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()");
}
| MOT '(' expression ',' expression ',' expression ',' expression ')' AFFECT expression {
#ifdef yaccParserVerbose
cout << "yacc object operator () = with 4 arguments found\n";
#endif
inputDyn->bufferSetCommand(CMDyn_DYG_Object_Operator_4);
inputDyn->bufferSetString($1);
inputDyn->bufferSetString("()=");
}
| SpecificObjectCommand
;

SpecificObjectCommand:
DYG_Real_1
|DYG_Boolean_1
|DYG_String_1
|DYG_Node_1
|DYG_Domain_1
|DYG_ElQua4n2D_1
|DYG_ElHex8n3D_1
|DYG_ElTri3n2D_1
|DYG_ElQua4nAx_1
|DYG_ElTet4n3D_1
|DYG_ElTet10n3D_1
|DYG_Vec3D_1
|DYG_Vector_1
|DYG_Matrix_1
|DYG_Tensor2_1
|DYG_Tensor3_1
|DYG_Tensor4_1
|DYG_ContactLaw_1
|DYG_Element_1
|DYG_Select_1
|DYG_NodeSet_1
|DYG_ElementSet_1
|DYG_Structure_1
|DYG_DiscreteFunction_1
|DYG_PolynomialFunction_1
|DYG_SinusFunction_1
|DYG_DiscreteFunctionSet_1
|DYG_RampFunction_1
|DYG_Material_1
|DYG_IsotropicHardening_1
|DYG_IsoHardElastic_1
|DYG_IsoHardElastoplasticTabular_1
|DYG_IsoHardElastoplastic_1
|DYG_IsoHardMetafor_1
|DYG_IsoHardPower_1
|DYG_IsoHardJohnsonCook_1
|DYG_Interface_1
|DYG_Boundary_1
|DYG_BoundaryCondition_1
|DYG_BoundarySpeed_1
|DYG_BoundaryAcceleration_1
|DYG_BoundaryDisplacement_1
|DYG_BoundaryRestrain_1
|DYG_BoundaryForce_1
|DYG_BoundaryTemperature_1
|DYG_BoundaryFlux_1
|DYG_BoundaryConvection_1
|DYG_HistoryFile_1
|DYG_Solver_1
|DYG_ExplicitSolver_1
|DYG_ExplicitSolverCH_1
|DYG_io_Structure_1
|DYG_Function_1
|DYG_Side_1
|DYG_CoulombLaw_1
|DYG_io_Data_1
|DYG_Mesher_1
|DYG_BoxMesher_1
;

DYG_Real_1:
DYG_Class_Real MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Real constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Real_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Real '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Real pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Real_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Boolean_1:
DYG_Class_Boolean MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Boolean constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Boolean_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Boolean '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Boolean pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Boolean_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_String_1:
DYG_Class_String MOT {
#ifdef yaccParserVerbose
  cout << "yacc object String constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_String_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_String '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object String pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_String_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Node_1:
DYG_Class_Node MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Node constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Node_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Node '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Node pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Node_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Domain_1:
DYG_Class_Domain MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Domain constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Domain_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Domain '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Domain pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Domain_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_ElQua4n2D_1:
DYG_Class_ElQua4n2D MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElQua4n2D constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElQua4n2D_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_ElQua4n2D '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElQua4n2D pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElQua4n2D_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_ElHex8n3D_1:
DYG_Class_ElHex8n3D MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElHex8n3D constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElHex8n3D_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_ElHex8n3D '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElHex8n3D pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElHex8n3D_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_ElTri3n2D_1:
DYG_Class_ElTri3n2D MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElTri3n2D constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElTri3n2D_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_ElTri3n2D '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElTri3n2D pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElTri3n2D_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_ElQua4nAx_1:
DYG_Class_ElQua4nAx MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElQua4nAx constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElQua4nAx_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_ElQua4nAx '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElQua4nAx pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElQua4nAx_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_ElTet4n3D_1:
DYG_Class_ElTet4n3D MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElTet4n3D constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElTet4n3D_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_ElTet4n3D '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElTet4n3D pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElTet4n3D_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_ElTet10n3D_1:
DYG_Class_ElTet10n3D MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElTet10n3D constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElTet10n3D_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_ElTet10n3D '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElTet10n3D pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElTet10n3D_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Vec3D_1:
DYG_Class_Vec3D MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Vec3D constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Vec3D_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Vec3D '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Vec3D pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Vec3D_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Vector_1:
DYG_Class_Vector MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Vector constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Vector_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Vector '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Vector pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Vector_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Matrix_1:
DYG_Class_Matrix MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Matrix constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Matrix_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Matrix '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Matrix pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Matrix_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Tensor2_1:
DYG_Class_Tensor2 MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Tensor2 constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Tensor2_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Tensor2 '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Tensor2 pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Tensor2_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Tensor3_1:
DYG_Class_Tensor3 MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Tensor3 constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Tensor3_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Tensor3 '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Tensor3 pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Tensor3_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Tensor4_1:
DYG_Class_Tensor4 MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Tensor4 constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Tensor4_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Tensor4 '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Tensor4 pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Tensor4_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_ContactLaw_1:
DYG_Class_ContactLaw MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ContactLaw constructor found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass ContactLaw is virtual",scanned_line[current_scanned_file]);}
| DYG_Class_ContactLaw '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ContactLaw pointer found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass ContactLaw is virtual",scanned_line[current_scanned_file]);}
;

DYG_Element_1:
DYG_Class_Element MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Element constructor found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Element is virtual",scanned_line[current_scanned_file]);}
| DYG_Class_Element '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Element pointer found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Element is virtual",scanned_line[current_scanned_file]);}
;

DYG_Select_1:
DYG_Class_Select MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Select constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Select_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Select '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Select pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Select_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_NodeSet_1:
DYG_Class_NodeSet MOT {
#ifdef yaccParserVerbose
  cout << "yacc object NodeSet constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_NodeSet_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_NodeSet '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object NodeSet pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_NodeSet_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_ElementSet_1:
DYG_Class_ElementSet MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElementSet constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElementSet_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_ElementSet '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ElementSet pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ElementSet_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Structure_1:
DYG_Class_Structure MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Structure constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Structure_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Structure '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Structure pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Structure_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_DiscreteFunction_1:
DYG_Class_DiscreteFunction MOT {
#ifdef yaccParserVerbose
  cout << "yacc object DiscreteFunction constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_DiscreteFunction_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_DiscreteFunction '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object DiscreteFunction pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_DiscreteFunction_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_PolynomialFunction_1:
DYG_Class_PolynomialFunction MOT {
#ifdef yaccParserVerbose
  cout << "yacc object PolynomialFunction constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_PolynomialFunction_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_PolynomialFunction '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object PolynomialFunction pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_PolynomialFunction_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_SinusFunction_1:
DYG_Class_SinusFunction MOT {
#ifdef yaccParserVerbose
  cout << "yacc object SinusFunction constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_SinusFunction_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_SinusFunction '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object SinusFunction pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_SinusFunction_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_DiscreteFunctionSet_1:
DYG_Class_DiscreteFunctionSet MOT {
#ifdef yaccParserVerbose
  cout << "yacc object DiscreteFunctionSet constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_DiscreteFunctionSet_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_DiscreteFunctionSet '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object DiscreteFunctionSet pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_DiscreteFunctionSet_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_RampFunction_1:
DYG_Class_RampFunction MOT {
#ifdef yaccParserVerbose
  cout << "yacc object RampFunction constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_RampFunction_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_RampFunction '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object RampFunction pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_RampFunction_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Material_1:
DYG_Class_Material MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Material constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Material_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Material '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Material pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Material_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_IsotropicHardening_1:
DYG_Class_IsotropicHardening MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsotropicHardening constructor found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass IsotropicHardening is virtual",scanned_line[current_scanned_file]);}
| DYG_Class_IsotropicHardening '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsotropicHardening pointer found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass IsotropicHardening is virtual",scanned_line[current_scanned_file]);}
;

DYG_IsoHardElastic_1:
DYG_Class_IsoHardElastic MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardElastic constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardElastic_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_IsoHardElastic '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardElastic pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardElastic_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_IsoHardElastoplasticTabular_1:
DYG_Class_IsoHardElastoplasticTabular MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardElastoplasticTabular constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardElastoplasticTabular_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_IsoHardElastoplasticTabular '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardElastoplasticTabular pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardElastoplasticTabular_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_IsoHardElastoplastic_1:
DYG_Class_IsoHardElastoplastic MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardElastoplastic constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardElastoplastic_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_IsoHardElastoplastic '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardElastoplastic pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardElastoplastic_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_IsoHardMetafor_1:
DYG_Class_IsoHardMetafor MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardMetafor constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardMetafor_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_IsoHardMetafor '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardMetafor pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardMetafor_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_IsoHardPower_1:
DYG_Class_IsoHardPower MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardPower constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardPower_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_IsoHardPower '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardPower pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardPower_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_IsoHardJohnsonCook_1:
DYG_Class_IsoHardJohnsonCook MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardJohnsonCook constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardJohnsonCook_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_IsoHardJohnsonCook '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object IsoHardJohnsonCook pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_IsoHardJohnsonCook_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Interface_1:
DYG_Class_Interface MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Interface constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Interface_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Interface '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Interface pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Interface_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Boundary_1:
DYG_Class_Boundary MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Boundary constructor found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Boundary is virtual",scanned_line[current_scanned_file]);}
| DYG_Class_Boundary '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Boundary pointer found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Boundary is virtual",scanned_line[current_scanned_file]);}
;

DYG_BoundaryCondition_1:
DYG_Class_BoundaryCondition MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryCondition constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryCondition_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoundaryCondition '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryCondition pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryCondition_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_BoundarySpeed_1:
DYG_Class_BoundarySpeed MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundarySpeed constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundarySpeed_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoundarySpeed '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundarySpeed pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundarySpeed_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_BoundaryAcceleration_1:
DYG_Class_BoundaryAcceleration MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryAcceleration constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryAcceleration_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoundaryAcceleration '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryAcceleration pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryAcceleration_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_BoundaryDisplacement_1:
DYG_Class_BoundaryDisplacement MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryDisplacement constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryDisplacement_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoundaryDisplacement '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryDisplacement pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryDisplacement_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_BoundaryRestrain_1:
DYG_Class_BoundaryRestrain MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryRestrain constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryRestrain_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoundaryRestrain '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryRestrain pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryRestrain_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_BoundaryForce_1:
DYG_Class_BoundaryForce MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryForce constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryForce_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoundaryForce '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryForce pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryForce_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_BoundaryTemperature_1:
DYG_Class_BoundaryTemperature MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryTemperature constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryTemperature_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoundaryTemperature '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryTemperature pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryTemperature_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_BoundaryFlux_1:
DYG_Class_BoundaryFlux MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryFlux constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryFlux_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoundaryFlux '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryFlux pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryFlux_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_BoundaryConvection_1:
DYG_Class_BoundaryConvection MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryConvection constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryConvection_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoundaryConvection '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoundaryConvection pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoundaryConvection_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_HistoryFile_1:
DYG_Class_HistoryFile MOT {
#ifdef yaccParserVerbose
  cout << "yacc object HistoryFile constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_HistoryFile_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_HistoryFile '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object HistoryFile pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_HistoryFile_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Solver_1:
DYG_Class_Solver MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Solver constructor found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Solver is virtual",scanned_line[current_scanned_file]);}
| DYG_Class_Solver '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Solver pointer found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Solver is virtual",scanned_line[current_scanned_file]);}
;

DYG_ExplicitSolver_1:
DYG_Class_ExplicitSolver MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ExplicitSolver constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ExplicitSolver_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_ExplicitSolver '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ExplicitSolver pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ExplicitSolver_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_ExplicitSolverCH_1:
DYG_Class_ExplicitSolverCH MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ExplicitSolverCH constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ExplicitSolverCH_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_ExplicitSolverCH '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object ExplicitSolverCH pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_ExplicitSolverCH_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_io_Structure_1:
DYG_Class_io_Structure MOT {
#ifdef yaccParserVerbose
  cout << "yacc object io_Structure constructor found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass io_Structure is virtual",scanned_line[current_scanned_file]);}
| DYG_Class_io_Structure '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object io_Structure pointer found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass io_Structure is virtual",scanned_line[current_scanned_file]);}
;

DYG_Function_1:
DYG_Class_Function MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Function constructor found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Function is virtual",scanned_line[current_scanned_file]);}
| DYG_Class_Function '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Function pointer found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Function is virtual",scanned_line[current_scanned_file]);}
;

DYG_Side_1:
DYG_Class_Side MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Side constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Side_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_Side '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Side pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_Side_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_CoulombLaw_1:
DYG_Class_CoulombLaw MOT {
#ifdef yaccParserVerbose
  cout << "yacc object CoulombLaw constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_CoulombLaw_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_CoulombLaw '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object CoulombLaw pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_CoulombLaw_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_io_Data_1:
DYG_Class_io_Data MOT {
#ifdef yaccParserVerbose
  cout << "yacc object io_Data constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_io_Data_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_io_Data '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object io_Data pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_io_Data_Pointer);
  inputDyn->bufferSetString($3);
}
;

DYG_Mesher_1:
DYG_Class_Mesher MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Mesher constructor found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Mesher is virtual",scanned_line[current_scanned_file]);}
| DYG_Class_Mesher '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object Mesher pointer found\n";
#endif
  fatalError("Cannot allocate object","Line %d\nClass Mesher is virtual",scanned_line[current_scanned_file]);}
;

DYG_BoxMesher_1:
DYG_Class_BoxMesher MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoxMesher constructor found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoxMesher_Constructor);
  inputDyn->bufferSetString($2);
}
| DYG_Class_BoxMesher '*' MOT {
#ifdef yaccParserVerbose
  cout << "yacc object BoxMesher pointer found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_DYG_Object_BoxMesher_Pointer);
  inputDyn->bufferSetString($3);
}
;

/* Auto include part ends here */

/*****************************************************************************************
 * FIN DE TRAITEMENT DU FICHIER
 *****************************************************************************************/
cmdfin:
FIN {
#ifdef yaccParserVerbose
  cout << "yacc expression FIN found\n";
#endif
  inputDyn->bufferSetCommand(CMDyn_QUIT);
}
| FIN PREPROCESSOR {
  inputDyn->bufferSetCommand(CMDyn_QUIT_PREPROCESSOR);
}
| FIN EXTRACT {
  inputDyn->bufferSetCommand(CMDyn_QUIT_EXTRACT);
}
| INTERNAL_INCLUDE STRING {
  inputDyn->bufferSetCommand(CMDyn_INTERNAL_INCLUDE);
  inputDyn->bufferSetString($2);
}
| INTERNAL_END_INCLUDE {
  inputDyn->bufferSetCommand(CMDyn_INTERNAL_END_INCLUDE);
}
;

/*****************************************************************************************
 * COMMANDES DE POST
 *****************************************************************************************/
fileInOut:
FOPEN '(' MOT ',' STRING ')' {
  inputDyn->bufferSetCommand(CMDyn_FOPEN);   
  inputDyn->bufferSetString($3);
  inputDyn->bufferSetString($5);
} 
| FOPEN '(' MOT ',' STRING ',' '+' ')' {
  inputDyn->bufferSetCommand(CMDyn_FOPEN_APPEND);   
  inputDyn->bufferSetString($3);
  inputDyn->bufferSetString($5);
} 
| FCLOSE '(' MOT ')' {
  inputDyn->bufferSetCommand(CMDyn_FCLOSE);   
  inputDyn->bufferSetString($3);
}
| MOT {
  inputDyn->bufferSetCommand(CMDyn_OUTPUT);
  inputDyn->bufferSetString($1);
} OUTPUT atomsOuts {
  inputDyn->bufferSetCommand(CMDyn_OUTPUT_END);
}
;

atomsOuts:
atomsOut
| atomsOut OUTPUT atomsOuts
;

atomsOut:
 expression {  
  inputDyn->bufferSetCommand(CMDyn_OUTPUT_VAL);   
}
;

/*****************************************************************************************
 * TRAITEMENT DE LA PILE DE CALCUL
 *****************************************************************************************/
expression:
operation {}
| increment {}
| fonction {}
| stackIn {}
| '(' expression ')' {}
| type_val {}
| string {}
| compare  {}
| objectCommand {}
;

stackIn:
MOT AFFECT expression {
  inputDyn->bufferSetCommand(VARIABLE_SET);
  inputDyn->bufferSetString($1);
}
| MOT PLUEGAL expression {
  inputDyn->bufferSetCommand(VARIABLE_PLUSEGAL);
  inputDyn->bufferSetString($1);
}
| MOT MOIEGAL expression {
  inputDyn->bufferSetCommand(VARIABLE_MOINSEGAL);
  inputDyn->bufferSetString($1);
}
| MOT DIVEGAL expression {
  inputDyn->bufferSetCommand(VARIABLE_DIVEGAL);
  inputDyn->bufferSetString($1);
}
| MOT MULEGAL expression {
  inputDyn->bufferSetCommand(VARIABLE_MULTEGAL);
  inputDyn->bufferSetString($1);
}
;

type_val: 
ENTIER {
  inputDyn->bufferSetCommand(INTEGER);
  inputDyn->bufferSetInteger($1);
}	
| REEL {
  inputDyn->bufferSetCommand(REAL);
  inputDyn->bufferSetReal($1);
}	
| MOT {
  inputDyn->bufferSetCommand(VARIABLE_GET);
  inputDyn->bufferSetString($1);
}
| PI_VALUE {
  inputDyn->bufferSetCommand(REAL);
  inputDyn->bufferSetReal(PI);
};
	
string:
STRING {
  inputDyn->bufferSetCommand(STRING_CREATE);
  inputDyn->bufferSetString($1);
}
;

compare:
expression INFERIOR expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("<");
}
| expression SUPERIOR expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString(">");
}
| expression INFEQUAL expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("<=");
}
| expression SUPEQUAL expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString(">=");
}
| expression EQUAL expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("==");
}
| expression DIFFERENT expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("!=");
}
| expression AND expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("&&");
}
| expression OR expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("||");
}
| NOT expression {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("!");
}
;

operation:
expression '+' expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("+");
}
| expression '-' expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("-");
}
| expression '*' expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("*");
}
| expression '/' expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("/");
}
| expression '^' expression {
  inputDyn->bufferSetCommand(BINARY_OPERATOR);
  inputDyn->bufferSetString("^");
}	
| '-' expression %prec UMINUS {
  inputDyn->bufferSetCommand(NEG_OPERATOR);
}
| '+' expression %prec UMINUS {}
;

increment:
MOT PLUSPLUS {
  inputDyn->bufferSetCommand(VARIABLE_POSTINCREMENT);
  inputDyn->bufferSetString($1);
}
| PLUSPLUS MOT {
  inputDyn->bufferSetCommand(VARIABLE_PREINCREMENT);
  inputDyn->bufferSetString($2);
}
| MOT MOINSMOINS {
  inputDyn->bufferSetCommand(VARIABLE_POSTDECREMENT);
  inputDyn->bufferSetString($1);
}
| MOINSMOINS MOT {
  inputDyn->bufferSetCommand(VARIABLE_PREDECREMENT);
  inputDyn->bufferSetString($2);
}
;

fonction:
ABS '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("ABS");
}			
| INT '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("INT");
}
| ROUND '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("ROUND");
}
| SQRT '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("SQRT");
}
| COS '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("COS");
}
| SIN '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("SIN");
}
| TAN '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("TAN");
}
| COSH '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("COSH");
}
| SINH '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("SINH");
}
| TANH '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("TANH");
}
| ACOS '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("ACOS");
}
| ASIN '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("ASIN");
}
| ATAN '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("ATAN");
}
| EXP '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("EXP");
}
| ERF '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("ERF");
}
| LN '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("LN");
}
| LOG '(' expression ')' {
  inputDyn->bufferSetCommand(UNARY_OPERATOR);
  inputDyn->bufferSetString("LOG");
}
;
%%
#include <InputDyn.h>
Boolean endOfFile=False;
ObjectsList objList;
extern InputDyn* inputDyn;
int current_scanned_file=0;
int scanned_line[150];
String scanned_file[150];
InputDyn* inputDyn;
ObjectsList objectList;
String parsedFileName;

#include <y.code.c>
