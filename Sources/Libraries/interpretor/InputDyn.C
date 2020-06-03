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

#include <InputDyn.h>

extern int InputDynerror(const char *msg);
int parenLevel;

//-----------------------------------------------------------------------------
InputDynStack::InputDynStack()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
InputDynStack::~InputDynStack()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
InputDynStackObject *InputDynStack::exist_pile(String identif)
//-----------------------------------------------------------------------------
{
  InputDynStackObject *object;

  // balayage de la pile de variables
  for (Indice i = 0; i < stack.size(); i++)
  {

    // recuperation de l'objet de la pile
    object = stack(i);

    // test d'existence de l'objet dans la pile
    if (object->variable == identif)
      return object;
  }

  // pas trouve
  return (NULL);
}

//-----------------------------------------------------------------------------
void InputDynStack::createVector(String identif, Indice taille)
//-----------------------------------------------------------------------------
{
  // initialisation d'un pointeur
  InputDynStackObject *object = NULL;

  // test si l'objet existe deja dans la pile
  if ((object = exist_pile(identif)) == NULL)
  {

    // creer un objet
    object = new InputDynStackObject;

    // l'ajouter a la pile
    stack << object;
    object->type = typeVector;
  }
  else
  {
    String str;
    str = identif + " already exists";
    InputDynerror(str.chars());
  }

  // modifier l'element de la pile
  object->variable = identif;
  object->vec.redim(taille);
}

//-----------------------------------------------------------------------------
void InputDynStack::addVariable(String identif, Real valeur)
//-----------------------------------------------------------------------------
{
  // initialisation d'un pointeur
  InputDynStackObject *object = NULL;

  // test si l'objet existe deja dans la pile
  if ((object = exist_pile(identif)) == NULL)
  {

    // creer un objet
    object = new InputDynStackObject;

    // l'ajouter a la pile
    stack << object;
    object->type = typeValue;
  }

  if (object->type != typeValue)
  {
    String str;
    str = identif + " is not a scalar";
    InputDynerror(str.chars());
  }

  // modifier l'element de la pile
  object->variable = identif;
  object->value = valeur;
}

//-----------------------------------------------------------------------------
void InputDynStack::addVariable(String identif, Indice i, Real valeur)
//-----------------------------------------------------------------------------
{
  // initialisation d'un pointeur
  InputDynStackObject *object = NULL;

  // test si l'objet existe deja dans la pile
  if ((object = exist_pile(identif)) == NULL)
  {

    String str;
    str = "vector " + identif + " must be declared before used";
    InputDynerror(str.chars());
  }

  // test si l'objet est bien un vecteur
  if (object->type != typeVector)
  {
    String str;
    str = identif + " is not a vector";
    InputDynerror(str.chars());
  }

  // test de la taille de l'objet
  if (i >= object->vec.size())
  {
    String str;
    str = "vector size of vector \"" + identif + "\" isn't sufficient";
    InputDynerror(str.chars());
  }

  // modifier l'element de la pile
  object->variable = identif;
  object->vec(i) = valeur;
}

//-----------------------------------------------------------------------------
Real InputDynStack::getValue(String st)
//-----------------------------------------------------------------------------
{
  // recherche de l'objet dans la pile
  InputDynStackObject *object = exist_pile(st);

  if (object == NULL)
  {

    /* test if obligatory parameter */
    cerr << st << " unknown RETURNING 0\n";
    return (0.);
  }

  if (object->type != typeValue)
  {
    String str;
    str = st + " is not a scalar";
    InputDynerror(str.chars());
  }

  // renvoi de la valeur correspondante
  return (object->value);
}

//-----------------------------------------------------------------------------
Real InputDynStack::getValue(String st, Indice i)
//-----------------------------------------------------------------------------
{
  // recherche de l'objet dans la pile
  InputDynStackObject *object = exist_pile(st);

  if (object == NULL)
  {

    /* test if obligatory parameter */
    cerr << st << " unknown RETURNING 0\n";
    return (0.);
  }

  if (object->type != typeVector)
  {
    String str;
    str = st + " is not a vector";
    InputDynerror(str.chars());
  }

  // test de la taille de l'objet
  if (i >= object->vec.size())
  {
    String str;
    str = "vector size of vector \"" + st + "\" isn't sufficient";
    InputDynerror(str.chars());
  }

  // renvoi de la valeur correspondante
  return (object->vec(i));
}

//-----------------------------------------------------------------------------
Real InputDynStack::getNecessaryValue(String st)
//-----------------------------------------------------------------------------
{
  // recherche de l'objet dans la pile
  InputDynStackObject *object = exist_pile(st);

  if (object == NULL)
  {
    fatalError("Input error", "%s not defined line %d", st.chars(), scanned_line);
    cerr << st << " unknown RETURNING 0\n";
    return (0.);
  }

  // renvoi de la valeur correspondante
  return (object->value);
}

//-----------------------------------------------------------------------------
Indice InputDynStack::size()
//-----------------------------------------------------------------------------
{
  return stack.size();
}

//-----------------------------------------------------------------------------
void InputDynStack::flush()
//-----------------------------------------------------------------------------
{
  // balayage de la pile de variables
  for (Indice i = 0; i < stack.size(); i++)
  {

    // destruction de l'objet
    delete stack(i);
  }

  // remise � zero
  stack.flush();
}

//-----------------------------------------------------------------------------
Boolean InputDynStack::getInfo(Indice i, String &name, String &type, Real &value)
//-----------------------------------------------------------------------------
{
  InputDynStackObject *object;

  // recuperation de l'objet de la pile
  object = stack(i);

  name = object->variable;
  value = object->value;
  type = "Unknown";
  if ((object->type) == typeValue)
    type = "Real";
  if ((object->type) == typeVector)
  {
    type = "Vector";
    value = object->vec.size();
  }

  return True;
}

//-----------------------------------------------------------------------------
void InputDynStack::getInfo(void)
//-----------------------------------------------------------------------------
{
  int i;
  InputDynStackObject *object;

  // affichage
  cout << "contenu de la pile\n";

  // affichage des objets de la pile
  for (i = 0; i < stack.size(); i++)
  {

    // recuperation de l'objet de la pile
    object = stack(i);

    // affichage de l'objet
    object->print(cout);
  }
}

//-----------------------------------------------------------------------------
InputDynStackObject::InputDynStackObject(Indice numero)
//-----------------------------------------------------------------------------
{
  Id = numero;
}

//-----------------------------------------------------------------------------
InputDynStackObject::~InputDynStackObject()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, InputDynStackObject &object)
//-----------------------------------------------------------------------------
{
  object.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void InputDynStackObject::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  // affichage
  os << variable << " = " << value << endl;
}

//-----------------------------------------------------------------------------
InputDyn::InputDyn()
//-----------------------------------------------------------------------------
{
  // initialisations
  currentBufferIndice = 0;
  inputBuffer = (char *)malloc(BufferReadSize * sizeof(char *));

  // set default angle
  angleDef = Degrees;

  // set trace language to off
  traceLanguage = False;
  pfileTraceLanguage = NULL;
}

//-----------------------------------------------------------------------------
InputDyn::~InputDyn()
//-----------------------------------------------------------------------------
{
  delete inputBuffer;
}

//-----------------------------------------------------------------------------
void InputDyn::bufferSetReal(Real reel)
//-----------------------------------------------------------------------------
{
  bufferSetPattern(CMDyn_REEL);

  memcpy(&inputBuffer[currentBufferIndice], &reel, sizeof(Real));
  currentBufferIndice += sizeof(Real);
}

//-----------------------------------------------------------------------------
Real InputDyn::bufferGetReal()
//-----------------------------------------------------------------------------
{
  Real tmp;

  if (bufferGetPattern(False) != CMDyn_REEL)
    fatalError("bufferGetReal", "encountered\n");
  memcpy(&tmp, &inputBuffer[currentBufferIndice], sizeof(Real));
  currentBufferIndice += sizeof(Real);
  return tmp;
}

//-----------------------------------------------------------------------------
void InputDyn::bufferSetInteger(int reel)
//-----------------------------------------------------------------------------
{
  bufferSetPattern(CMDyn_INT);

  memcpy(&inputBuffer[currentBufferIndice], &reel, sizeof(int));
  currentBufferIndice += sizeof(int);
  maxBuffer = currentBufferIndice;
}

//-----------------------------------------------------------------------------
int InputDyn::bufferGetInteger()
//-----------------------------------------------------------------------------
{
  int tmp;
  if (bufferGetPattern(False) != CMDyn_INT)
    fatalError("bufferGetInteger", "encountered\n");
  memcpy(&tmp, &inputBuffer[currentBufferIndice], sizeof(int));
  currentBufferIndice += sizeof(int);
  return tmp;
}

//-----------------------------------------------------------------------------
void InputDyn::bufferSetString(String str)
//-----------------------------------------------------------------------------
{

  bufferSetPattern(CMDyn_STRING);

  int strl = str.length();
  memcpy(&inputBuffer[currentBufferIndice], &strl, sizeof(int));
  currentBufferIndice += sizeof(int);

  for (int i = 0; i < strl; i++)
    inputBuffer[currentBufferIndice++] = str[i];
  maxBuffer = currentBufferIndice;
}

//-----------------------------------------------------------------------------
String InputDyn::bufferGetString()
//-----------------------------------------------------------------------------
{
  int strl;
  if (bufferGetPattern(False) != CMDyn_STRING)
    fatalError("bufferGetString", "encountered\n");
  String str;
  memcpy(&strl, &inputBuffer[currentBufferIndice], sizeof(int));
  currentBufferIndice += sizeof(int);
  for (int i = 0; i < strl; i++)
    str += inputBuffer[currentBufferIndice++];

  return str;
}

//-----------------------------------------------------------------------------
void InputDyn::bufferSetPattern(int command)
//-----------------------------------------------------------------------------
{
  memcpy(&inputBuffer[currentBufferIndice], &command, sizeof(int));
  currentBufferIndice += sizeof(int);
  maxBuffer = currentBufferIndice;
}

//-----------------------------------------------------------------------------
int InputDyn::bufferGetPatternCommand(Boolean toget)
//-----------------------------------------------------------------------------
{
  int cur = currentBufferIndice;
  int cmd;
  cmd = bufferGetPattern(False);
  currentBufferIndice = cur;
  if (cmd == CMDyn_CMD)
    return bufferGetCommand();

  return bufferGetPattern(True);
}

//-----------------------------------------------------------------------------
int InputDyn::bufferGetPattern(Boolean toget)
//-----------------------------------------------------------------------------
{
  int tmp;
  memcpy(&tmp, &inputBuffer[currentBufferIndice], sizeof(int));
  currentBufferIndice += sizeof(int);

  if (toget)
  {
    switch (tmp)
    {
    case CMDyn_CMD:
    {
      int tmp;
      memcpy(&tmp, &inputBuffer[currentBufferIndice], sizeof(int));
      currentBufferIndice += sizeof(int);
    }
    break;
    case CMDyn_REEL:
    {
      Real tmp;
      memcpy(&tmp, &inputBuffer[currentBufferIndice], sizeof(Real));
      currentBufferIndice += sizeof(Real);
    }
    break;
    case CMDyn_INT:
    {
      int tmp;
      memcpy(&tmp, &inputBuffer[currentBufferIndice], sizeof(int));
      currentBufferIndice += sizeof(int);
    }
    break;
    case CMDyn_STRING:
    {
      int strl;
      String str;
      memcpy(&strl, &inputBuffer[currentBufferIndice], sizeof(int));
      currentBufferIndice += sizeof(int);
      for (int i = 0; i < strl; i++)
        str += inputBuffer[currentBufferIndice++];
    }
    break;
    }
  }
  return tmp;
}

//-----------------------------------------------------------------------------
void InputDyn::bufferSetCommand(int command)
//-----------------------------------------------------------------------------
{
  // push the command
  bufferSetPattern(CMDyn_CMD);

  // copy the command
  memcpy(&inputBuffer[currentBufferIndice], &command, sizeof(int));

  // increase the buffer
  currentBufferIndice += sizeof(int);

  // increase the max buffer
  maxBuffer = currentBufferIndice;
}

//-----------------------------------------------------------------------------
void InputDyn::bufferNextCommand(int command)
//-----------------------------------------------------------------------------
{
  int cmd;
  int level = 1;
  Boolean conti = True;
  while (conti)
  {
    cmd = bufferGetPatternCommand(True);
    if (cmd == command)
    {
      level--;
    }
    if (cmd == command + 1)
    {
      level++;
    }
    if (level == 0)
      conti = False;

    if (currentBufferIndice > maxBuffer)
      fatalError("bufferNextCommand", "Not found item");
  }
}

//-----------------------------------------------------------------------------
int InputDyn::bufferGetCommand()
//-----------------------------------------------------------------------------
{
  int tmp;

  // test if command is valid
  if ((tmp = bufferGetPattern(False)) != CMDyn_CMD)
    fatalError("bufferGetCommand", "encountered %ld\n", tmp);

  // push the command
  memcpy(&tmp, &inputBuffer[currentBufferIndice], sizeof(int));

  // increase the indice
  currentBufferIndice += sizeof(int);

  // return the value
  return tmp;
}

//-----------------------------------------------------------------------------
FilesFluxes::FilesFluxes()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
FilesFluxes::~FilesFluxes()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
FilesFluxesObject *FilesFluxes::exist_pile(String identif)
//-----------------------------------------------------------------------------
{
  FilesFluxesObject *object;

  // balayage de la pile de variables
  for (Indice i = 0; i < stackFiles.size(); i++)
  {

    // recuperation de l'objet de la pile
    object = stackFiles(i);

    // test d'existence de l'objet dans la pile
    if (object->name == identif)
      return object;
  }

  // pas trouve
  return (NULL);
}

//-----------------------------------------------------------------------------
void FilesFluxes::addFlux(String identif, FILE *valeur)
//-----------------------------------------------------------------------------
{
  // initialisation d'un pointeur
  FilesFluxesObject *object = NULL;

  // test si l'objet existe deja dans la pile
  if ((object = exist_pile(identif)) == NULL)
  {

    // creer un objet
    object = new FilesFluxesObject;

    // l'ajouter a la pile
    stackFiles << object;
  }

  // modifier l'element de la pile
  object->name = identif;
  object->flux = valeur;
}

//-----------------------------------------------------------------------------
FILE *FilesFluxes::getFlux(String st)
//-----------------------------------------------------------------------------
{
  // recherche de l'objet dans la pile
  FilesFluxesObject *object = exist_pile(st);

  if (object == NULL)
  {

    /* test if obligatory parameter */
    cerr << st << " unknown flux RETURNING NULL\n";
    return (NULL);
  }

  // renvoi de la valeur correspondante
  return (object->flux);
}

//-----------------------------------------------------------------------------
Boolean FilesFluxes::remove(String st)
//-----------------------------------------------------------------------------
{
  // recherche de l'objet dans la pile
  FilesFluxesObject *object = exist_pile(st);

  if (object == NULL)
  {

    /* test if obligatory parameter */
    cerr << st << " unknown flux RETURNING NULL\n";
    return (False);
  }

  object->name = "";

  // renvoi de la valeur correspondante
  return (True);
}

//-----------------------------------------------------------------------------
Indice FilesFluxes::size()
//-----------------------------------------------------------------------------
{
  return stackFiles.size();
}

//-----------------------------------------------------------------------------
void FilesFluxes::flush()
//-----------------------------------------------------------------------------
{
  // balayage de la pile de variables
  for (Indice i = 0; i < stackFiles.size(); i++)
  {

    // destruction de l'objet
    delete stackFiles(i);
  }

  // remise � zero
  stackFiles.flush();
}

//-----------------------------------------------------------------------------
FilesFluxesObject::FilesFluxesObject(Indice numero)
//-----------------------------------------------------------------------------
{
  Id = numero;
}

//-----------------------------------------------------------------------------
FilesFluxesObject::~FilesFluxesObject()
//-----------------------------------------------------------------------------
{
}
