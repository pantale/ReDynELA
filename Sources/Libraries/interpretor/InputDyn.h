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

#ifndef __InputDyn_h__
#define __InputDyn_h__

#include <linearAlgebra.h>
#include <ObjectList.h>

#include <femLibrary.h>

extern int InputDynerror(const char *msg);
extern "C" int yywrap();
extern FILE *InputDynin;
extern int yyparse();
extern int yylex();
class InputDyn;

extern int scanned_line[150];
extern int current_scanned_file;
extern String scanned_file[150];
void parseInputDyn(char);
extern InputDyn *inputDyn;
extern ObjectsList objectList;
extern int parenLevel;
extern String parsedFileName;
extern Boolean isHeritedFrom(Indice, Indice);

enum
{
    CMDyn_END = 1 // definition line n'1
    ,
    CMDyn_INTERNAL_INCLUDE // definition line n'2
    ,
    CMDyn_INTERNAL_END_INCLUDE // definition line n'3
    ,
    CMDyn_QUIT // definition line n'4
    ,
    CMDyn_QUIT_PREPROCESSOR // definition line n'5
    ,
    CMDyn_QUIT_EXTRACT // definition line n'6
    ,
    STACK_FLUSH // definition line n'7
    ,
    CMDyn_FOR_END // definition line n'8
    ,
    CMDyn_FOR_START // definition line n'9
    ,
    CMDyn_FOR_START1 // definition line n'10
    ,
    CMDyn_VOID1 // definition line n'11
    ,
    CMDyn_FOR_START2 // definition line n'12
    ,
    CMDyn_VOID2 // definition line n'13
    ,
    CMDyn_IF // definition line n'14
    ,
    CMDyn_IF_START // definition line n'15
    ,
    REAL // definition line n'16
    ,
    INTEGER // definition line n'17
    ,
    VARIABLE_SET // definition line n'18
    ,
    VARIABLE_GET // definition line n'19
    ,
    BINARY_OPERATOR // definition line n'20
    ,
    UNARY_OPERATOR // definition line n'21
    ,
    NEG_OPERATOR // definition line n'22
    ,
    VARIABLE_PLUSEGAL // definition line n'23
    ,
    VARIABLE_MOINSEGAL // definition line n'24
    ,
    VARIABLE_MULTEGAL // definition line n'25
    ,
    VARIABLE_DIVEGAL // definition line n'26
    ,
    VARIABLE_POSTINCREMENT // definition line n'27
    ,
    VARIABLE_PREINCREMENT // definition line n'28
    ,
    VARIABLE_POSTDECREMENT // definition line n'29
    ,
    VARIABLE_PREDECREMENT // definition line n'30
    ,
    CMDyn_ELEMENTBLOC // definition line n'31
    ,
    CMDyn_DIRECT_STACK // definition line n'32
    ,
    CMDyn_REAL_STORE // definition line n'33
    ,
    CMDyn_FOPEN // definition line n'34
    ,
    CMDyn_FOPEN_APPEND // definition line n'35
    ,
    CMDyn_FCLOSE // definition line n'36
    ,
    CMDyn_OUTPUT // definition line n'37
    ,
    CMDyn_OUTPUT_END // definition line n'38
    ,
    CMDyn_OUTPUT_VAL // definition line n'39
    ,
    STRING_CREATE // definition line n'40
    ,
    CMDyn_PAREN_BEGIN // definition line n'41
    ,
    CMDyn_PAREN_END // definition line n'42
    ,
    CMDyn_PRAGMA // definition line n'43
                 /* Auto include part starts here */
    ,
    CMDyn_DYG_Object_Method_0,
    CMDyn_DYG_Object_Method_1,
    CMDyn_DYG_Object_Method_2,
    CMDyn_DYG_Object_Method_3,
    CMDyn_DYG_Object_Method_4,
    CMDyn_DYG_Object_Method_5,
    CMDyn_DYG_Object_Method_8,
    CMDyn_DYG_Object_Method_9,
    CMDyn_DYG_Object_Method_11,
    CMDyn_DYG_Object_Operator_0,
    CMDyn_DYG_Object_Operator_1,
    CMDyn_DYG_Object_Operator_2,
    CMDyn_DYG_Object_Operator_3,
    CMDyn_DYG_Object_Operator_4,
    CMDyn_DYG_Object_Real_Constructor,
    CMDyn_DYG_Object_Real_Pointer,
    CMDyn_DYG_Object_Boolean_Constructor,
    CMDyn_DYG_Object_Boolean_Pointer,
    CMDyn_DYG_Object_String_Constructor,
    CMDyn_DYG_Object_String_Pointer,
    CMDyn_DYG_Object_Node_Constructor,
    CMDyn_DYG_Object_Node_Pointer,
    CMDyn_DYG_Object_Domain_Constructor,
    CMDyn_DYG_Object_Domain_Pointer,
    CMDyn_DYG_Object_ElQua4n2D_Constructor,
    CMDyn_DYG_Object_ElQua4n2D_Pointer,
    CMDyn_DYG_Object_ElHex8n3D_Constructor,
    CMDyn_DYG_Object_ElHex8n3D_Pointer,
    CMDyn_DYG_Object_ElTri3n2D_Constructor,
    CMDyn_DYG_Object_ElTri3n2D_Pointer,
    CMDyn_DYG_Object_ElQua4nAx_Constructor,
    CMDyn_DYG_Object_ElQua4nAx_Pointer,
    CMDyn_DYG_Object_ElTet4n3D_Constructor,
    CMDyn_DYG_Object_ElTet4n3D_Pointer,
    CMDyn_DYG_Object_ElTet10n3D_Constructor,
    CMDyn_DYG_Object_ElTet10n3D_Pointer,
    CMDyn_DYG_Object_Vec3D_Constructor,
    CMDyn_DYG_Object_Vec3D_Pointer,
    CMDyn_DYG_Object_Vector_Constructor,
    CMDyn_DYG_Object_Vector_Pointer,
    CMDyn_DYG_Object_Matrix_Constructor,
    CMDyn_DYG_Object_Matrix_Pointer,
    CMDyn_DYG_Object_Tensor2_Constructor,
    CMDyn_DYG_Object_Tensor2_Pointer,
    CMDyn_DYG_Object_Tensor3_Constructor,
    CMDyn_DYG_Object_Tensor3_Pointer,
    CMDyn_DYG_Object_Tensor4_Constructor,
    CMDyn_DYG_Object_Tensor4_Pointer,
    CMDyn_DYG_Object_Select_Constructor,
    CMDyn_DYG_Object_Select_Pointer,
    CMDyn_DYG_Object_NodeSet_Constructor,
    CMDyn_DYG_Object_NodeSet_Pointer,
    CMDyn_DYG_Object_ElementSet_Constructor,
    CMDyn_DYG_Object_ElementSet_Pointer,
    CMDyn_DYG_Object_Structure_Constructor,
    CMDyn_DYG_Object_Structure_Pointer,
    CMDyn_DYG_Object_DiscreteFunction_Constructor,
    CMDyn_DYG_Object_DiscreteFunction_Pointer,
    CMDyn_DYG_Object_PolynomialFunction_Constructor,
    CMDyn_DYG_Object_PolynomialFunction_Pointer,
    CMDyn_DYG_Object_SinusFunction_Constructor,
    CMDyn_DYG_Object_SinusFunction_Pointer,
    CMDyn_DYG_Object_DiscreteFunctionSet_Constructor,
    CMDyn_DYG_Object_DiscreteFunctionSet_Pointer,
    CMDyn_DYG_Object_RampFunction_Constructor,
    CMDyn_DYG_Object_RampFunction_Pointer,
    CMDyn_DYG_Object_Material_Constructor,
    CMDyn_DYG_Object_Material_Pointer,
    CMDyn_DYG_Object_IsoHardElastic_Constructor,
    CMDyn_DYG_Object_IsoHardElastic_Pointer,
    CMDyn_DYG_Object_IsoHardElastoplasticTabular_Constructor,
    CMDyn_DYG_Object_IsoHardElastoplasticTabular_Pointer,
    CMDyn_DYG_Object_IsoHardElastoplastic_Constructor,
    CMDyn_DYG_Object_IsoHardElastoplastic_Pointer,
    CMDyn_DYG_Object_IsoHardMetafor_Constructor,
    CMDyn_DYG_Object_IsoHardMetafor_Pointer,
    CMDyn_DYG_Object_IsoHardPower_Constructor,
    CMDyn_DYG_Object_IsoHardPower_Pointer,
    CMDyn_DYG_Object_IsoHardJohnsonCook_Constructor,
    CMDyn_DYG_Object_IsoHardJohnsonCook_Pointer,
    CMDyn_DYG_Object_Interface_Constructor,
    CMDyn_DYG_Object_Interface_Pointer,
    CMDyn_DYG_Object_BoundaryCondition_Constructor,
    CMDyn_DYG_Object_BoundaryCondition_Pointer,
    CMDyn_DYG_Object_BoundarySpeed_Constructor,
    CMDyn_DYG_Object_BoundarySpeed_Pointer,
    CMDyn_DYG_Object_BoundaryAcceleration_Constructor,
    CMDyn_DYG_Object_BoundaryAcceleration_Pointer,
    CMDyn_DYG_Object_BoundaryDisplacement_Constructor,
    CMDyn_DYG_Object_BoundaryDisplacement_Pointer,
    CMDyn_DYG_Object_BoundaryRestrain_Constructor,
    CMDyn_DYG_Object_BoundaryRestrain_Pointer,
    CMDyn_DYG_Object_BoundaryForce_Constructor,
    CMDyn_DYG_Object_BoundaryForce_Pointer,
    CMDyn_DYG_Object_BoundaryTemperature_Constructor,
    CMDyn_DYG_Object_BoundaryTemperature_Pointer,
    CMDyn_DYG_Object_BoundaryFlux_Constructor,
    CMDyn_DYG_Object_BoundaryFlux_Pointer,
    CMDyn_DYG_Object_BoundaryConvection_Constructor,
    CMDyn_DYG_Object_BoundaryConvection_Pointer,
    CMDyn_DYG_Object_HistoryFile_Constructor,
    CMDyn_DYG_Object_HistoryFile_Pointer,
    CMDyn_DYG_Object_ExplicitSolver_Constructor,
    CMDyn_DYG_Object_ExplicitSolver_Pointer,
    CMDyn_DYG_Object_ExplicitSolverCH_Constructor,
    CMDyn_DYG_Object_ExplicitSolverCH_Pointer,
    CMDyn_DYG_Object_Side_Constructor,
    CMDyn_DYG_Object_Side_Pointer,
    CMDyn_DYG_Object_CoulombLaw_Constructor,
    CMDyn_DYG_Object_CoulombLaw_Pointer,
    CMDyn_DYG_Object_io_Data_Constructor,
    CMDyn_DYG_Object_io_Data_Pointer,
    CMDyn_DYG_Object_BoxMesher_Constructor,
    CMDyn_DYG_Object_BoxMesher_Pointer
};

enum
{
    DYG_Real,
    DYG_Boolean,
    DYG_String,
    DYG_Node,
    DYG_Domain,
    DYG_ElQua4n2D,
    DYG_ElHex8n3D,
    DYG_ElTri3n2D,
    DYG_ElQua4nAx,
    DYG_ElTet4n3D,
    DYG_ElTet10n3D,
    DYG_Vec3D,
    DYG_Vector,
    DYG_Matrix,
    DYG_Tensor2,
    DYG_Tensor3,
    DYG_Tensor4,
    DYG_ContactLaw,
    DYG_Element,
    DYG_Select,
    DYG_NodeSet,
    DYG_ElementSet,
    DYG_Structure,
    DYG_DiscreteFunction,
    DYG_PolynomialFunction,
    DYG_SinusFunction,
    DYG_DiscreteFunctionSet,
    DYG_RampFunction,
    DYG_Material,
    DYG_IsotropicHardening,
    DYG_IsoHardElastic,
    DYG_IsoHardElastoplasticTabular,
    DYG_IsoHardElastoplastic,
    DYG_IsoHardMetafor,
    DYG_IsoHardPower,
    DYG_IsoHardJohnsonCook,
    DYG_Interface,
    DYG_Boundary,
    DYG_BoundaryCondition,
    DYG_BoundarySpeed,
    DYG_BoundaryAcceleration,
    DYG_BoundaryDisplacement,
    DYG_BoundaryRestrain,
    DYG_BoundaryForce,
    DYG_BoundaryTemperature,
    DYG_BoundaryFlux,
    DYG_BoundaryConvection,
    DYG_HistoryFile,
    DYG_Solver,
    DYG_ExplicitSolver,
    DYG_ExplicitSolverCH,
    DYG_io_Structure,
    DYG_Function,
    DYG_Side,
    DYG_CoulombLaw,
    DYG_io_Data,
    DYG_Mesher,
    DYG_BoxMesher
};

class Class_Node : public Object
{
public:
    Class_Node() { object = NULL; }
    void allocate() { object = new Node; }
    ~Class_Node() {}
    int getObjectType() { return DYG_Node; }
    String getObjectClass() { return "Node"; }
};

class Class_Domain : public Object
{
public:
    Class_Domain() { object = NULL; }
    void allocate() { object = new Domain; }
    ~Class_Domain() {}
    int getObjectType() { return DYG_Domain; }
    String getObjectClass() { return "Domain"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((Domain *)object)->name = st;
    }
};

class Class_ElQua4n2D : public Object
{
public:
    Class_ElQua4n2D() { object = NULL; }
    void allocate() { object = new ElQua4n2D; }
    ~Class_ElQua4n2D() {}
    int getObjectType() { return DYG_ElQua4n2D; }
    String getObjectClass() { return "ElQua4n2D"; }
};

class Class_ElHex8n3D : public Object
{
public:
    Class_ElHex8n3D() { object = NULL; }
    void allocate() { object = new ElHex8n3D; }
    ~Class_ElHex8n3D() {}
    int getObjectType() { return DYG_ElHex8n3D; }
    String getObjectClass() { return "ElHex8n3D"; }
};

class Class_ElTri3n2D : public Object
{
public:
    Class_ElTri3n2D() { object = NULL; }
    void allocate() { object = new ElTri3n2D; }
    ~Class_ElTri3n2D() {}
    int getObjectType() { return DYG_ElTri3n2D; }
    String getObjectClass() { return "ElTri3n2D"; }
};

class Class_ElQua4nAx : public Object
{
public:
    Class_ElQua4nAx() { object = NULL; }
    void allocate() { object = new ElQua4nAx; }
    ~Class_ElQua4nAx() {}
    int getObjectType() { return DYG_ElQua4nAx; }
    String getObjectClass() { return "ElQua4nAx"; }
};

class Class_ElTet4n3D : public Object
{
public:
    Class_ElTet4n3D() { object = NULL; }
    void allocate() { object = new ElTet4n3D; }
    ~Class_ElTet4n3D() {}
    int getObjectType() { return DYG_ElTet4n3D; }
    String getObjectClass() { return "ElTet4n3D"; }
};

class Class_ElTet10n3D : public Object
{
public:
    Class_ElTet10n3D() { object = NULL; }
    void allocate() { object = new ElTet10n3D; }
    ~Class_ElTet10n3D() {}
    int getObjectType() { return DYG_ElTet10n3D; }
    String getObjectClass() { return "ElTet10n3D"; }
};

class Class_Vec3D : public Object
{
public:
    Class_Vec3D() { object = NULL; }
    void allocate() { object = new Vec3D; }
    ~Class_Vec3D() {}
    int getObjectType() { return DYG_Vec3D; }
    String getObjectClass() { return "Vec3D"; }
};

class Class_Vector : public Object
{
public:
    Class_Vector() { object = NULL; }
    void allocate() { object = new Vector; }
    ~Class_Vector() {}
    int getObjectType() { return DYG_Vector; }
    String getObjectClass() { return "Vector"; }
};

class Class_Matrix : public Object
{
public:
    Class_Matrix() { object = NULL; }
    void allocate() { object = new Matrix; }
    ~Class_Matrix() {}
    int getObjectType() { return DYG_Matrix; }
    String getObjectClass() { return "Matrix"; }
};

class Class_Tensor2 : public Object
{
public:
    Class_Tensor2() { object = NULL; }
    void allocate() { object = new Tensor2; }
    ~Class_Tensor2() {}
    int getObjectType() { return DYG_Tensor2; }
    String getObjectClass() { return "Tensor2"; }
};

class Class_Tensor3 : public Object
{
public:
    Class_Tensor3() { object = NULL; }
    void allocate() { object = new Tensor3; }
    ~Class_Tensor3() {}
    int getObjectType() { return DYG_Tensor3; }
    String getObjectClass() { return "Tensor3"; }
};

class Class_Tensor4 : public Object
{
public:
    Class_Tensor4() { object = NULL; }
    void allocate() { object = new Tensor4; }
    ~Class_Tensor4() {}
    int getObjectType() { return DYG_Tensor4; }
    String getObjectClass() { return "Tensor4"; }
};

class Class_Select : public Object
{
public:
    Class_Select() { object = NULL; }
    void allocate() { object = new Select; }
    ~Class_Select() {}
    int getObjectType() { return DYG_Select; }
    String getObjectClass() { return "Select"; }
};

class Class_NodeSet : public Object
{
public:
    Class_NodeSet() { object = NULL; }
    void allocate() { object = new NodeSet; }
    ~Class_NodeSet() {}
    int getObjectType() { return DYG_NodeSet; }
    String getObjectClass() { return "NodeSet"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((NodeSet *)object)->name = st;
    }
};

class Class_ElementSet : public Object
{
public:
    Class_ElementSet() { object = NULL; }
    void allocate() { object = new ElementSet; }
    ~Class_ElementSet() {}
    int getObjectType() { return DYG_ElementSet; }
    String getObjectClass() { return "ElementSet"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((ElementSet *)object)->name = st;
    }
};

class Class_Structure : public Object
{
public:
    Class_Structure() { object = NULL; }
    void allocate() { object = new Structure; }
    ~Class_Structure() {}
    int getObjectType() { return DYG_Structure; }
    String getObjectClass() { return "Structure"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((Structure *)object)->name = st;
    }
};

class Class_DiscreteFunction : public Object
{
public:
    Class_DiscreteFunction() { object = NULL; }
    void allocate() { object = new DiscreteFunction; }
    ~Class_DiscreteFunction() {}
    int getObjectType() { return DYG_DiscreteFunction; }
    String getObjectClass() { return "DiscreteFunction"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((DiscreteFunction *)object)->name = st;
    }
};

class Class_PolynomialFunction : public Object
{
public:
    Class_PolynomialFunction() { object = NULL; }
    void allocate() { object = new PolynomialFunction; }
    ~Class_PolynomialFunction() {}
    int getObjectType() { return DYG_PolynomialFunction; }
    String getObjectClass() { return "PolynomialFunction"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((PolynomialFunction *)object)->name = st;
    }
};

class Class_SinusFunction : public Object
{
public:
    Class_SinusFunction() { object = NULL; }
    void allocate() { object = new SinusFunction; }
    ~Class_SinusFunction() {}
    int getObjectType() { return DYG_SinusFunction; }
    String getObjectClass() { return "SinusFunction"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((SinusFunction *)object)->name = st;
    }
};

class Class_DiscreteFunctionSet : public Object
{
public:
    Class_DiscreteFunctionSet() { object = NULL; }
    void allocate() { object = new DiscreteFunctionSet; }
    ~Class_DiscreteFunctionSet() {}
    int getObjectType() { return DYG_DiscreteFunctionSet; }
    String getObjectClass() { return "DiscreteFunctionSet"; }
};

class Class_RampFunction : public Object
{
public:
    Class_RampFunction() { object = NULL; }
    void allocate() { object = new RampFunction; }
    ~Class_RampFunction() {}
    int getObjectType() { return DYG_RampFunction; }
    String getObjectClass() { return "RampFunction"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((RampFunction *)object)->name = st;
    }
};

class Class_Material : public Object
{
public:
    Class_Material() { object = NULL; }
    void allocate() { object = new Material; }
    ~Class_Material() {}
    int getObjectType() { return DYG_Material; }
    String getObjectClass() { return "Material"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((Material *)object)->name = st;
    }
};

class Class_IsoHardElastic : public Object
{
public:
    Class_IsoHardElastic() { object = NULL; }
    void allocate() { object = new IsoHardElastic; }
    ~Class_IsoHardElastic() {}
    int getObjectType() { return DYG_IsoHardElastic; }
    String getObjectClass() { return "IsoHardElastic"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((IsoHardElastic *)object)->name = st;
    }
};

class Class_IsoHardElastoplasticTabular : public Object
{
public:
    Class_IsoHardElastoplasticTabular() { object = NULL; }
    void allocate() { object = new IsoHardElastoplasticTabular; }
    ~Class_IsoHardElastoplasticTabular() {}
    int getObjectType() { return DYG_IsoHardElastoplasticTabular; }
    String getObjectClass() { return "IsoHardElastoplasticTabular"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((IsoHardElastoplasticTabular *)object)->name = st;
    }
};

class Class_IsoHardElastoplastic : public Object
{
public:
    Class_IsoHardElastoplastic() { object = NULL; }
    void allocate() { object = new IsoHardElastoplastic; }
    ~Class_IsoHardElastoplastic() {}
    int getObjectType() { return DYG_IsoHardElastoplastic; }
    String getObjectClass() { return "IsoHardElastoplastic"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((IsoHardElastoplastic *)object)->name = st;
    }
};

class Class_IsoHardMetafor : public Object
{
public:
    Class_IsoHardMetafor() { object = NULL; }
    void allocate() { object = new IsoHardMetafor; }
    ~Class_IsoHardMetafor() {}
    int getObjectType() { return DYG_IsoHardMetafor; }
    String getObjectClass() { return "IsoHardMetafor"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((IsoHardMetafor *)object)->name = st;
    }
};

class Class_IsoHardPower : public Object
{
public:
    Class_IsoHardPower() { object = NULL; }
    void allocate() { object = new IsoHardPower; }
    ~Class_IsoHardPower() {}
    int getObjectType() { return DYG_IsoHardPower; }
    String getObjectClass() { return "IsoHardPower"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((IsoHardPower *)object)->name = st;
    }
};

class Class_IsoHardJohnsonCook : public Object
{
public:
    Class_IsoHardJohnsonCook() { object = NULL; }
    void allocate() { object = new IsoHardJohnsonCook; }
    ~Class_IsoHardJohnsonCook() {}
    int getObjectType() { return DYG_IsoHardJohnsonCook; }
    String getObjectClass() { return "IsoHardJohnsonCook"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((IsoHardJohnsonCook *)object)->name = st;
    }
};

class Class_Interface : public Object
{
public:
    Class_Interface() { object = NULL; }
    void allocate() { object = new Interface; }
    ~Class_Interface() {}
    int getObjectType() { return DYG_Interface; }
    String getObjectClass() { return "Interface"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((Interface *)object)->name = st;
    }
};

class Class_BoundaryCondition : public Object
{
public:
    Class_BoundaryCondition() { object = NULL; }
    void allocate() { object = new BoundaryCondition; }
    ~Class_BoundaryCondition() {}
    int getObjectType() { return DYG_BoundaryCondition; }
    String getObjectClass() { return "BoundaryCondition"; }
};

class Class_BoundarySpeed : public Object
{
public:
    Class_BoundarySpeed() { object = NULL; }
    void allocate() { object = new BoundarySpeed; }
    ~Class_BoundarySpeed() {}
    int getObjectType() { return DYG_BoundarySpeed; }
    String getObjectClass() { return "BoundarySpeed"; }
};

class Class_BoundaryAcceleration : public Object
{
public:
    Class_BoundaryAcceleration() { object = NULL; }
    void allocate() { object = new BoundaryAcceleration; }
    ~Class_BoundaryAcceleration() {}
    int getObjectType() { return DYG_BoundaryAcceleration; }
    String getObjectClass() { return "BoundaryAcceleration"; }
};

class Class_BoundaryDisplacement : public Object
{
public:
    Class_BoundaryDisplacement() { object = NULL; }
    void allocate() { object = new BoundaryDisplacement; }
    ~Class_BoundaryDisplacement() {}
    int getObjectType() { return DYG_BoundaryDisplacement; }
    String getObjectClass() { return "BoundaryDisplacement"; }
};

class Class_BoundaryRestrain : public Object
{
public:
    Class_BoundaryRestrain() { object = NULL; }
    void allocate() { object = new BoundaryRestrain; }
    ~Class_BoundaryRestrain() {}
    int getObjectType() { return DYG_BoundaryRestrain; }
    String getObjectClass() { return "BoundaryRestrain"; }
};

class Class_BoundaryForce : public Object
{
public:
    Class_BoundaryForce() { object = NULL; }
    void allocate() { object = new BoundaryForce; }
    ~Class_BoundaryForce() {}
    int getObjectType() { return DYG_BoundaryForce; }
    String getObjectClass() { return "BoundaryForce"; }
};

class Class_BoundaryTemperature : public Object
{
public:
    Class_BoundaryTemperature() { object = NULL; }
    void allocate() { object = new BoundaryTemperature; }
    ~Class_BoundaryTemperature() {}
    int getObjectType() { return DYG_BoundaryTemperature; }
    String getObjectClass() { return "BoundaryTemperature"; }
};

class Class_BoundaryFlux : public Object
{
public:
    Class_BoundaryFlux() { object = NULL; }
    void allocate() { object = new BoundaryFlux; }
    ~Class_BoundaryFlux() {}
    int getObjectType() { return DYG_BoundaryFlux; }
    String getObjectClass() { return "BoundaryFlux"; }
};

class Class_BoundaryConvection : public Object
{
public:
    Class_BoundaryConvection() { object = NULL; }
    void allocate() { object = new BoundaryConvection; }
    ~Class_BoundaryConvection() {}
    int getObjectType() { return DYG_BoundaryConvection; }
    String getObjectClass() { return "BoundaryConvection"; }
};

class Class_HistoryFile : public Object
{
public:
    Class_HistoryFile() { object = NULL; }
    void allocate() { object = new HistoryFile; }
    ~Class_HistoryFile() {}
    int getObjectType() { return DYG_HistoryFile; }
    String getObjectClass() { return "HistoryFile"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((HistoryFile *)object)->name = st;
    }
};

class Class_ExplicitSolver : public Object
{
public:
    Class_ExplicitSolver() { object = NULL; }
    void allocate() { object = new ExplicitSolver; }
    ~Class_ExplicitSolver() {}
    int getObjectType() { return DYG_ExplicitSolver; }
    String getObjectClass() { return "ExplicitSolver"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((ExplicitSolver *)object)->name = st;
    }
};

class Class_ExplicitSolverCH : public Object
{
public:
    Class_ExplicitSolverCH() { object = NULL; }
    void allocate() { object = new ExplicitSolverCH; }
    ~Class_ExplicitSolverCH() {}
    int getObjectType() { return DYG_ExplicitSolverCH; }
    String getObjectClass() { return "ExplicitSolverCH"; }
    void setObjectName(String st)
    {
        Object::setObjectName(st);
        if (object != NULL)
            ((ExplicitSolverCH *)object)->name = st;
    }
};

class Class_Side : public Object
{
public:
    Class_Side() { object = NULL; }
    void allocate() { object = new Side; }
    ~Class_Side() {}
    int getObjectType() { return DYG_Side; }
    String getObjectClass() { return "Side"; }
};

class Class_CoulombLaw : public Object
{
public:
    Class_CoulombLaw() { object = NULL; }
    void allocate() { object = new CoulombLaw; }
    ~Class_CoulombLaw() {}
    int getObjectType() { return DYG_CoulombLaw; }
    String getObjectClass() { return "CoulombLaw"; }
};

class Class_io_Data : public Object
{
public:
    Class_io_Data() { object = NULL; }
    void allocate() { object = new io_Data; }
    ~Class_io_Data() {}
    int getObjectType() { return DYG_io_Data; }
    String getObjectClass() { return "io_Data"; }
};

class Class_BoxMesher : public Object
{
public:
    Class_BoxMesher() { object = NULL; }
    void allocate() { object = new BoxMesher; }
    ~Class_BoxMesher() {}
    int getObjectType() { return DYG_BoxMesher; }
    String getObjectClass() { return "BoxMesher"; }
};

/* Auto include part ends here */

enum
{
    CMDyn_CMD = 1,
    CMDyn_REEL,
    CMDyn_INT,
    CMDyn_STRING
};

#define BufferReadSize 1000000

enum
{
    typeValue,
    typeVec3D,
    typeVector
};

class Class_Real : public Object
{
public:
    Class_Real() { object = new Real; }
    void allocate() {}
    ~Class_Real() {}
    int getObjectType() { return DYG_Real; }
    String getObjectClass() { return "Real"; }
};

class Class_Boolean : public Object
{
public:
    Class_Boolean() { object = new Boolean; }
    void allocate() {}
    ~Class_Boolean() {}
    int getObjectType() { return DYG_Boolean; }
    String getObjectClass() { return "Boolean"; }
};

class Class_String : public Object
{
public:
    Class_String() { object = new String; }
    void allocate() {}
    ~Class_String() {}
    int getObjectType() { return DYG_String; }
    String getObjectClass() { return "String"; }
};

class InputDynStackObject
{
    friend class ListId<InputDynStackObject *>;
    Indice Id;

public:
    String variable;
    char type;
    double value;
    Vector vec;

public:
    // constructeurs
    InputDynStackObject(Indice numero = 0);
    ~InputDynStackObject();

    // gestion des flux entree et sortie
    void print(ostream &) const;
    friend ostream &operator<<(ostream &, const InputDynStackObject &);
};

class InputDynStack
{
protected:
    /** @dia:route 0,2;h,7,7,-3,8,3 */
    ListId<InputDynStackObject *> stack;

public:
    // constructeurs
    InputDynStack();
    ~InputDynStack();

    // fonctions membres
    InputDynStackObject *exist_pile(String identif);
    void addVariable(String identif, Real valeur);
    void addVariable(String identif, Indice i, Real valeur);
    void createVector(String identif, Indice taille);
    Real getNecessaryValue(String st);
    Real getValue(String st);
    Real getValue(String st, Indice i);
    void getInfo(void);
    Boolean getInfo(Indice i, String &name, String &type, Real &value);
    void flush();
    Indice size();
};

class ObjectReal : public Object
{
public:
    Real value;

public:
    // constructeurs
    ObjectReal() {}
    ~ObjectReal() {}
};

class FilesFluxesObject
{
    friend class ListId<FilesFluxesObject *>;
    Indice Id;

public:
    String name;
    FILE *flux;

public:
    // constructeurs
    FilesFluxesObject(Indice numero = 0);
    ~FilesFluxesObject();
};

class FilesFluxes
{
protected:
    /** @dia:route 0,2;h,7,7,-3,8,3 */
    ListId<FilesFluxesObject *> stackFiles;

public:
    // constructeurs
    FilesFluxes();
    ~FilesFluxes();

    // fonctions membres
    FilesFluxesObject *exist_pile(String identif);
    void addFlux(String identif, FILE *flux);
    FILE *getFlux(String st);
    void flush();
    Indice size();
    Boolean remove(String identif);
};

class InputDyn
{
public:
    char *inputBuffer;
    Indice currentBufferIndice;
    Indice maxBuffer;
    Stack<Object *> stackObjects;
    enum
    {
        Solve,
        Extract
    };
    enum
    {
        Degrees,
        Radians
    };
    char angleDef;
    Boolean traceLanguage;
    FILE *pfileTraceLanguage;

public:
    // constructeurs
    InputDyn();
    ~InputDyn();
    void bufferSetReal(Real reel);
    Real bufferGetReal();
    void bufferSetCommand(int command);
    int bufferGetCommand();
    void bufferSetInteger(int reel);
    int bufferGetInteger();
    void bufferSetString(String command);
    String bufferGetString();
    void bufferNextCommand(int command);
    void bufferSetPattern(int command);
    int bufferGetPattern(Boolean toget);
    int bufferGetPatternCommand(Boolean toget);
};

#endif
