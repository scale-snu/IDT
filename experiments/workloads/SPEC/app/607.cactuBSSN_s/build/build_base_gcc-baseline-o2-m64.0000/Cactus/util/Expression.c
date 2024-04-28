 /*@@
   @file      Expression.c
   @date      Tue Sep 19 13:25:39 2000
   @author    Tom Goodale
   @desc 
   Expression evaluator.
   Can cope with arithmetic expressions and ones involving the standard 
   C library mathematical functions (or those taking only one argument).
   @enddesc
   @version $Header$
 @@*/

#ifndef TEST_EXPRESSION_PARSER
#ifdef SPEC 
#include "Cactus/cctk.h" 
#define THORN_DIR "Cactus" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "utili_Expression.h"
#include "util_Expression.h"

static const char *rcsid = "$Header$";

#ifndef TEST_EXPRESSION_PARSER
#include "util_String.h"
CCTK_FILEVERSION(util_Expression_c)
#ifdef strdup
#undef strdup
#endif
#include "util_String.h"
#define strdup(a) Util_Strdup(a)
#else
#define CCTK_VWarn(lvl, line, file, thorn, fmt, ...)  do { printf("(%s) L%d at %s line %d: "fmt"\n", thorn, lvl, file, line, ## __VA_ARGS__); if(lvl == 0) exit(1); } while(0)
#endif

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

typedef struct PToken
{
  struct PToken *last;
  struct PToken *next;
  char *token;
} pToken;

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

static pToken *Tokenise(const char *expression);
static int RPParse(pToken **current,uExpressionInternals *buffer);
static int VerifyParsedExpression(const uExpressionInternals *buffer);
static int EvaluateBinary(uExpressionValue *retval, 
                          const uExpressionValue *val1, 
                          uExpressionOpcode opcode, 
                          const uExpressionValue *val2);

static int EvaluateUnary(uExpressionValue *retval, 
                         uExpressionOpcode opcode, 
                         const uExpressionValue *value);

static int isoperator(const char *token);
static int cmpprecendence(const char *op1, const char *op2);
static int opencode(const char *operator, uExpressionType *type, uExpressionOpcode *opcode);

static int StoreVar(uExpressionInternals *buffer, const char *var);

static pToken *newtoken(const char *tokenstart, const char *tokenend);
static void FreeTokens(pToken *list);
static void insertafter(pToken *base, pToken *this);

#ifdef TEST_EXPRESSION_PARSER
static const char *opname(uExpressionOpcode opcode);
static void printtokens(pToken *start);
#endif


/********************************************************************
 ********************* Other Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 *********************     Local Data   *****************************
 ********************************************************************/

#define MAX_OPS 100
#define MAX_STACK_SIZE 256

static struct
{
  const char *operator;
  uExpressionType type;
  int precedence;
  uExpressionOpcode opcode;
} operators[] = {
  /* Binary operators. */
  {"||", binary, 1,OP_OR},
  {"&&", binary, 2,OP_AND},
  {"==", binary, 3,OP_EQUALS},
  {"!=", binary, 3,OP_NOTEQUALS},
  {"<",  binary, 4,OP_LESS_THAN},
  {">",  binary, 4,OP_GREATER_THAN},
  {"<=", binary, 4,OP_LEQUALS},
  {">=", binary, 4,OP_GEQUALS},
  /* Binary operators. */
  {"+",  binary, 5,OP_PLUS},
  {"-",  binary, 5,OP_MINUS},
  {"/",  binary, 6,OP_DIV},
  {"%",  binary, 6,OP_REMAINDER},
  {"*",  binary, 6,OP_TIMES},
  {"**", binary, 7,OP_POWER},
  /* Sign change */
  {"_",     unary, 8,OP_NEGATE}, /* unary '-' */
  {"@",     unary, 8,OP_PASS},   /* unary '+' */
  /* Unary Operators - these must have the highest precedence. */
  {"!",     unary, 8, OP_NOT},
  {"acos",  unary, 9, OP_ACOS},
  {"asin",  unary, 9, OP_ASIN},
  {"atan",  unary, 9, OP_ATAN},
  {"ceil",  unary, 9, OP_CEIL},
  {"cos" ,  unary, 9, OP_COS},  
  {"cosh",  unary, 9, OP_COSH},
  {"exp",   unary, 9, OP_EXP},
  {"fabs",  unary, 9, OP_FABS},  
  {"floor", unary, 9, OP_FLOOR},  
  {"log",   unary, 9, OP_LOG},  
  {"log10", unary, 9, OP_LOG10},
  {"sin",   unary, 9, OP_SIN},  
  {"sinh",  unary, 9, OP_SINH},  
  {"sqrt",  unary, 9, OP_SQRT},  
  {"tan",   unary, 9, OP_TAN},  
  {"tanh",  unary, 9, OP_TANH},  
  {"trunc", unary, 9, OP_TRUNC},  
  {NULL, binary, -1,OP_NONE}
};


/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/

 /*@@
   @routine    Util_ExpressionParse
   @date       Tue Sep 19 21:23:08 2000
   @author     Tom Goodale
   @desc 
   Parses an expression returning a predigested representation
   suitable for passing to ExpressionEvaluate.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     expression
   @vdesc   Expression to parse
   @vtype   const char *
   @vio     in
   @vcomment 
 
   @endvar 

   @returntype uExpression
   @returndesc
   The parsed form of the expression or NULL on error.
   @endreturndesc
@@*/
uExpression Util_ExpressionParse(const char *expression)
{
  pToken *list;
  pToken *temp;

  uExpressionInternals *buffer;

  buffer = (uExpressionInternals *)malloc(sizeof(uExpressionInternals));

  if(buffer)
  {
    /* Initialise the buffer */
    buffer->ntokens = 0;
    buffer->tokens  = NULL;
    buffer->nvars   = 0;
    buffer->vars    = NULL;

    /* FIXME: Don't really need a two pass algorithm here -
     *        it can all be done in one step.
     */
  
    /* Split the list into tokens */
    list = Tokenise(expression);
    
#ifdef TEST_EXPRESSION_PARSER    
    printtokens(list);
#endif

    temp = list;

    /* Convert the list into a string in RPN order */
    if(!RPParse(&temp, buffer) && temp == NULL)
    {
      /* Check if it is a valid expression */
      if(!VerifyParsedExpression(buffer))
      {
        Util_ExpressionFree(buffer);
        buffer = NULL;
      }
    }
    else
    {
      Util_ExpressionFree(buffer);
      buffer = NULL;
    }

    FreeTokens(list);

  }

  return buffer;
}

 /*@@
   @routine    Util_ExpressionEvaluate
   @date       Tue Sep 19 21:23:40 2000
   @author     Tom Goodale
   @desc 
   Evaluates a parsed expression created by Util_ExpressionParse.
   The user passes in a function which is used to evaluate all operands.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     buffer
   @vdesc   The parsed expression
   @vtype   const uExpression
   @vio     in
   @vcomment 
   This expression should have been created by Util_ExpressionParse
   @endvar 
   @var     retval
   @vdesc   The return value
   @vtype   uExpressionValue *
   @vio     out
   @vcomment 
   On exit contains the answer if there were no errors
   @endvar 
   @var     eval
   @vdesc   Variable evaluation function
   @vtype   int (*eval)(int, const char * const *, uExpressionValue *, const void *),
   @vio     in
   @vcomment 
   This function is called with an array of variables to determine the values of
   and should return the corresponding values.
   @endvar 
   @var     data
   @vdesc   Arbitrary data to be passed to user supplied function.
   @vtype   void *
   @vio     in
   @vcomment 
   This cna be anything the user needs to evaluate things.
   @endvar 

   @returntype int
   @returndesc
   0   - success
   -1  - memory allocation failure
   @endreturndesc

@@*/
int Util_ExpressionEvaluate(const uExpression buffer,
                            uExpressionValue *retval,
                            int (*eval)(int, const char * const *, uExpressionValue *, const void *),
                            const void *data)
{
  int retcode;
  uExpressionValue stack[MAX_STACK_SIZE];
  int stackpointer;
  int position;

  uExpressionValue *varvals;

  retcode = 0;
  stackpointer = 0;

  /* Assign memory for array to contain all variable values */
  varvals = (uExpressionValue *)malloc(buffer->nvars*sizeof(uExpressionValue));

  if(varvals || ! buffer->nvars)
  {
    /* Evaluate the variables in one go to help people doing parallel ops. */
    eval(buffer->nvars, (const char * const *) buffer->vars, varvals, data);

    /* Tokens are seperated by @ signs */
    for(position = 0; position < buffer->ntokens; position++)
    {
      if(buffer->tokens[position].type == val)
      {
        /* Put value on stack */
        stack[stackpointer] = varvals[buffer->tokens[position].token.varnum];
        
        stackpointer++;
      }
      else
      {
#ifdef TEST_EXPRESSION_PARSER
      
        printf("Stackpointer is %d, ", stackpointer); 
        if(buffer->tokens[position].type == binary)
        {
          switch(stack[stackpointer-2].type)
          {
            case ival:
              printf("%d " ,stack[stackpointer-2].value.ival); break;
            case rval:
              printf("%g " ,stack[stackpointer-2].value.rval); break;
            default:
              ;
          }
        }
        
        printf("%s ", opname(buffer->tokens[position].token.opcode));
        switch(stack[stackpointer-1].type)
        {
          case ival:
            printf("%d " ,stack[stackpointer-1].value.ival); break;
          case rval:
            printf("%g " ,stack[stackpointer-1].value.rval); break;
          default:
            ;
        }
        printf(" = ");
        fflush(stdout);
#endif
        /* Evaluate operation, clear operands from stack and add the result to the stack. */
        switch(buffer->tokens[position].type)
        {
          case binary:
            EvaluateBinary(&(stack[stackpointer-2]),
                           &(stack[stackpointer-2]),
                           buffer->tokens[position].token.opcode,
                           &(stack[stackpointer-1]));
            stackpointer--;
            break;
          case unary:
            EvaluateUnary(&(stack[stackpointer-1]),
                          buffer->tokens[position].token.opcode,
                          &(stack[stackpointer-1]));
            break;
          default :
            ;
        }
        
#ifdef TEST_EXPRESSION_PARSER
        switch(stack[stackpointer-1].type)
        {
          case ival:
            printf("%d\n" ,stack[stackpointer-1].value.ival); break;
          case rval:
            printf("%g\n" ,stack[stackpointer-1].value.rval); break;
          default:
            ;
        }
#endif
      }
    }
    if(varvals)
    {
      free(varvals);
    }

    *retval=stack[0];
    /* stackpointer should be 1 at this point if VerifyExpression
     * was called on the expression.
     */

    retcode = stackpointer-1;
  }
  else
  {
    /* memory failure */

    retcode = -1;
  }

  return retcode;
}

 /*@@
   @routine    Util_ExpressionFree
   @date       Sat Nov  3 11:56:04 2001
   @author     Tom Goodale
   @desc 
   Frees an expression buffer
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     buffer
   @vdesc   Expression buffer to be freed.
   @vtype   uExpression
   @vio     inout
   @vcomment 
 
   @endvar 

@@*/
void Util_ExpressionFree(uExpression buffer)
{
  int i;

  if(buffer)
  {
    free(buffer->tokens);
    
    for(i = 0; i < buffer->nvars; i++)
    {
      free(buffer->vars[i]);
    }
    free(buffer->vars);
  }
  free(buffer);
}


/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/

 /*@@
   @routine    Tokenise
   @date       Tue Sep 19 21:25:18 2000
   @author     Tom Goodale
   @desc 
   Split an expression into tokens
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     expression
   @vdesc   Expression to be tokenised.
   @vtype   const char *
   @vio     in
   @vcomment 
 
   @endvar 

   @returntype pToken *
   @returndesc
   Linked list of tokens.
   @endreturndesc

@@*/
static pToken *Tokenise(const char *expression)
{
  pToken *start;
  pToken *current;
  pToken *new;

  const char *tokenstart;
  const char *tokenend;
  const char *position;

  /* class '@' below is "everything else", "e" can be either scientific
   * notation or the literal character 'e' */
  const char *classes[] = {"-+", "0123456789", ".", "eEdD", "*/%=&|<>!", "(", ")", " \t\n", "@"};
  /* if we are in state s and read a charcter of class c we transition to state
   * states[s][c] */
  /* if this is -1 then this is a transition that does not happen in well
   * formed input and we complain, then transition to state 0 */
  int states[][sizeof(classes)/sizeof(classes[0])] = {
      /* -   0   .   e   *   (   )  ' '  @ */
/* 0*/ {10,  2,  3,  9, -1,  0, -1,  0,  9}, /* beginning of subexpression */
/* 1*/ { 1,  2,  3,  9,  1,  0, -1, 11,  9}, /* after first character of binary operator or '!' */
/* 2*/ { 1,  2,  3,  4,  1, -1,  5,  5, -1}, /* after first digit of integer */
/* 3*/ {-1,  7, -1,  4,  1, -1,  5,  5, -1}, /* after decimal point */
/* 4*/ { 8,  8, -1, -1, -1, -1, -1, -1, -1}, /* after 'e' of exponent */
/* 5*/ { 1, -1, -1, -1,  1, -1,  5,  5, -1}, /* after space after number */
/* 6*/ { 1, -1, -1, -1,  1,  0,  5,  6, -1}, /* after space after name*/
/* 7*/ {-1,  7, -1,  4,  1, -1,  5,  5, -1}, /* after first digit of fraction */
/* 8*/ {-1,  9, -1, -1,  1, -1,  5,  5, -1}, /* after first digit of exponent */
/* 9*/ { 1,  9, -1,  9,  1,  0,  5,  6,  9}, /* after first letter of name */
/*10*/ {-1,  2,  3,  9, -1,  0, -1, 10,  9}, /* after unary operator */
/*11*/ {10,  2,  3, -1,  1,  0, -1, 11,  9}, /* after space after binary op */
  };
  /* we possibly terminate a token when we transition out of a state. we check here
   * if this transition is a termination of a token */
  int end_of_token[][sizeof(classes)/sizeof(classes[0])] = {
      /* -   0   .   e   *   (   )  ' '  @ */
/* 0*/ { 1,  1,  1,  1, -1,  1, -1,  0,  1}, /* after beginning of subexpression */
/* 1*/ { 1,  1,  1,  1,  0,  1, -1,  0,  1}, /* after binary operator or '!' */
/* 2*/ { 1,  0,  0,  0,  1,  1,  1,  1, -1}, /* after first digit of integer */
/* 3*/ {-1,  0, -1,  0,  1, -1,  1,  1, -1}, /* after decimal point */
/* 4*/ { 0,  0, -1, -1, -1, -1, -1, -1, -1}, /* after 'e' of exponent */
/* 5*/ { 1, -1, -1, -1,  1, -1,  1,  0, -1}, /* after space after number */
/* 6*/ { 1, -1, -1, -1,  1,  1,  1,  0, -1}, /* after space after name*/
/* 7*/ {-1,  0, -1,  0,  1, -1,  1,  1, -1}, /* after first digit of fraction */
/* 8*/ {-1,  0, -1, -1,  1, -1,  1,  1, -1}, /* after first digit of exponent */
/* 9*/ { 1,  0, -1,  0,  1,  1,  1,  1,  0}, /* after first letter of name */
/*10*/ {-1,  0,  0,  1, -1,  1, -1,  1,  1}, /* after unary operator */
/*11*/ { 1,  1,  3, -1,  1,  1, -1,  0,  1}, /* after space after binary op */
  };
  int tokenstate, state, class, error;

  start   = NULL;
  current = NULL;

  tokenstart = expression;
  state = 0;
  while(*tokenstart) /* as long as we have tokens left */
  {

    /* Remove leading whitespace */ 
    for(; *tokenstart == ' ' || *tokenstart == '\t'; tokenstart++);

    tokenend = NULL;
    tokenstate = -1;

    /* classify first input character of token*/
    for(class = 0; class < (int)(sizeof(classes)/sizeof(classes[0]))-1; class++)
    {
      if(strchr(classes[class], *tokenstart))
        break;
    }
    if(states[state][class] >= 0)
      state = states[state][class];
    else
      state = 0;
#ifdef TEST_EXPRESSION_PARSER
    printf("Initial state: %d, token '%c', class '%c'\n", state, *tokenstart, classes[class][0]);
#endif

    /* collect remainder of token */
    error = 0;
    for(position = tokenstart+1; *position && *tokenstart; position++)
    {
      /* classify input character */
      for(class = 0; class < (int)(sizeof(classes)/sizeof(classes[0]))-1; class++)
      {
        if(strchr(classes[class], *position))
          break;
      }

#ifdef TEST_EXPRESSION_PARSER
      printf("read '%c', current state: %d, class '%c', will transition to %d, token = '%.*s', will %sappend token\n",
          *position, state, classes[class][0], states[state][class], (int)(position-tokenstart), tokenstart, end_of_token[state][class] ? "" : "not ");
#endif

      /* see if transition closes the current token */
      if(end_of_token[state][class])
      {
        tokenend = position-1;
        tokenstate = state;
      }
      else if(states[state][class] >= 0)
      {
        /* transition to new state or state 0 upon errors */
        state = states[state][class];
      }
      else
      {
        state = 0;
        error = 1;
      }

      if(tokenend)
      {
        if(error)
        {
          CCTK_VWarn (2, __LINE__, __FILE__, "Cactus",
                      "Tokenise: Parser in error state. Faulty token '%.*s', next input '%c'.",
                      (int)(position-tokenstart), tokenstart, *position);
        }
        break;
      }
    }

    /* Have we reached the end of the string ? If so, make sure to add last token */
    if(!tokenend)
    {
      tokenend = position;
    }
    
    /* Create a new token */
    new = newtoken(tokenstart, tokenend);

    if(new)
    {
      if(tokenstate == 10) /* after we have seen a "-+" at beginning of subexpression */
      {
        /* replace unary "+-" by different names */
        if(!strcmp(new->token, "+"))
          new->token[0] = '@';
        else if(!strcmp(new->token, "-"))
          new->token[0] = '_';
      }

      /* Insert on list */
      if(current)
      {
        insertafter(current, new);
      }
      current = new;
      
      if(!start)
      {
        start = current;
      }

      if(*tokenend)
      {
        tokenstart = tokenend+1;
      }
      else
      {
        break;
      }
    }
    else
    {
      CCTK_VWarn (0, __LINE__, __FILE__, "Cactus",
                  "Unable to allocate memory for new token !\n");
    }
  }
    
  return start;
}


 /*@@
   @routine    RPParse
   @date       Tue Sep 19 21:28:36 2000
   @author     Tom Goodale
   @desc 
   Parses a token list into Reverse Polish Notation (RPN).
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     current
   @vdesc   Current token
   @vtype   pToken **
   @vio     inout
   @vcomment 
   This is the current token on input, and the new current token on output. 
   @endvar 
   @var     buffer
   @vdesc   Expression buffer
   @vtype   uExpressionInternals *
   @vio     inout
   @vcomment 
   This is the buffer in which to store the parse results.
   @endvar 

   @returntype int
   @returndesc
   0 - success
   @endreturndesc
@@*/

#define RESIZE_STACK(stack)                                             \
do                                                                      \
{                                                                       \
  (stack)->ntokens++;                                                   \
  (stack)->tokens                                                       \
    = (uExpressionToken *)realloc((stack)->tokens,                      \
                                  (stack)->ntokens*sizeof(uExpressionToken)); \
} while(0)

#define PUSHOP(stack, xtype, value)                             \
do                                                              \
{                                                               \
  RESIZE_STACK(stack);                                          \
  (stack)->tokens[(stack)->ntokens-1].type = xtype;             \
  (stack)->tokens[(stack)->ntokens-1].token.opcode = value;     \
} while(0)

#define PUSHTOK(stack, xtype, value)                            \
do                                                              \
{                                                               \
  RESIZE_STACK(stack);                                          \
  (stack)->tokens[(stack)->ntokens-1].type = xtype;             \
  (stack)->tokens[(stack)->ntokens-1].token.varnum = value;     \
} while(0)

static int RPParse(pToken **current, uExpressionInternals *buffer)
{
  int retcode;
  pToken *this;
  char *operator;
  char *opstack[MAX_OPS];
  int numops;
  int precedence;
  int varnum;
  uExpressionType optype;
  uExpressionOpcode opcode;

  numops = 0;
  this = *current;

  retcode = 0;

  operator = NULL;

  for(this = *current; this && strcmp(this->token,")"); this = this->next)
  {
    if(!strcmp(this->token, "(") && this->next)
    {
      /* This is a sub-group, so parse recursively */
      this = this->next;
      retcode = RPParse(&this, buffer);
      if(retcode || ! this || strcmp(this->token,")"))
      {
        retcode = -1;
        break;
      }
    }
    else if(!isoperator(this->token))
    {
      varnum = StoreVar(buffer, this->token);
      PUSHTOK(buffer, val, varnum);
    }
    else
    {
      /* It's an operator */
      if(operator)
      {
        /* We already have an operator */
        precedence = cmpprecendence(operator, this->token);

        if(precedence > 0)
        {
          /* Higher precedence than previous one so store previous one */
          numops++;
          opstack[numops-1] = operator;
          operator = this->token;
        }
        else
        {
          /* Lower or equal precedence */
          opencode(operator, &optype, &opcode);
          PUSHOP(buffer, optype, opcode);
          operator = this->token;
          while(numops > 0)
          {
            if(cmpprecendence(opstack[numops-1], operator) <=0)
            {
              numops--;
              opencode(opstack[numops], &optype, &opcode);
              PUSHOP(buffer, optype, opcode);
            }
            else
            {
              break;
            }
          }
        }
      }
      else
      {
        operator = this->token;
      }
    }
  }

  if(operator)
  {
    opencode(operator, &optype, &opcode);
    PUSHOP(buffer, optype, opcode);
    while(numops > 0)
    {
      numops--;
      opencode(opstack[numops], &optype, &opcode);
      PUSHOP(buffer, optype, opcode);
    }
  }

  *current=this;

  return retcode;
}

 /*@@
   @routine    VerifyParsedExpression
   @date       Tue Nov  6 10:53:12 2001
   @author     Tom Goodale
   @desc 
   Verifies that an expression would complete successfully.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     buffer
   @vdesc   An expression buffer
   @vtype   const uExpressionInternals *
   @vio     in
   @vcomment 
 
   @endvar 

   @returntype int
   @returndesc
   1 - success
   0 - invalid expression
   @endreturndesc
@@*/
static int VerifyParsedExpression(const uExpressionInternals *buffer)
{
  int retcode;
  int stackpointer;
  int position;

  stackpointer = 0;

  retcode = 0;

  for(position = 0; position < buffer->ntokens; position++)
  {
    if(buffer->tokens[position].type == val)
    {
#if TEST_EXPRESSION_PARSER
      printf("%s ",buffer->vars[buffer->tokens[position].token.varnum]);
#endif
      stackpointer++;
    }
    else
    {
#if TEST_EXPRESSION_PARSER
      printf("%s ",opname(buffer->tokens[position].token.opcode));
#endif
      /* Evaluate operation, clear operands from stack and add the result to the stack. */
      switch(buffer->tokens[position].type)
      {
        case binary:
          if(stackpointer > 1)
          {
            stackpointer--;
          }
          else
          {
            retcode = -1;
          }
          break;
        case unary:
          if(stackpointer == 0)
          {
            retcode = -1;
          }
          break;
        default:
          ;
      }
    }
    if(stackpointer < 0 || stackpointer > MAX_STACK_SIZE || retcode )
    {
      retcode = -1;
      break;
    }
  }

  if(! retcode)
  {
    if(stackpointer == 1)
    {
      retcode = 1;
    }
    else
    {
      retcode = 0;
    }
  }
  else
  {
    retcode = 0;
  }

#if TEST_EXPRESSION_PARSER
      printf("\n");
#endif

  return retcode;
}

 /*@@
   @routine    EvaluateBinary
   @date       Tue Sep 19 21:35:34 2000
   @author     Tom Goodale
   @desc 
   Evaluates the binary operation val1 op val2 
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     retval
   @vdesc   The result
   @vtype   uExpressionValue *
   @vio     out
   @vcomment 
 
   @endvar 
   @var     val1
   @vdesc   The first operand
   @vtype   const uExpressionValue *
   @vio     in
   @vcomment 
 
   @endvar 
   @var     opcode
   @vdesc   The opcode of the operator.
   @vtype   uExpressionOpcode
   @vio     in
   @vcomment 
 
   @endvar 
   @var     val2
   @vdesc   The second operand
   @vtype   const uExpressionValue *
   @vio     in
   @vcomment 
 
   @endvar 

   @returntype int
   @returndesc
   0 - success
   @endreturndesc
@@*/
static int EvaluateBinary(uExpressionValue *retval, 
                          const uExpressionValue *val1, 
                          uExpressionOpcode opcode, 
                          const uExpressionValue *val2)
{
  
  /* Define a macro so only have to do this once 
   * irrespective of operand types.
   */
  #define EVALUATEBINARY(retval,val1,val2)             \
  switch(opcode)                                       \
  {                                                    \
    case OP_PLUS :                                     \
      (retval) = ((val1)+(val2));                      \
      break;                                           \
    case OP_MINUS :                                    \
      (retval) = ((val1)-(val2));                      \
      break;                                           \
    case OP_DIV :                                      \
      (retval) = ((val1)/(val2));                      \
      break;                                           \
    case OP_REMAINDER :                                \
      (retval) = (fmod(val1,val2));                    \
      break;                                           \
    case OP_TIMES :                                    \
      (retval) = ((val1)*(val2));                      \
      break;                                           \
    case OP_POWER :                                    \
      (retval) = pow(val1,val2);                       \
      break;                                           \
    case OP_AND :                                      \
      (retval) = ((val1) && (val2));                   \
      break;                                           \
    case OP_OR :                                       \
      (retval) = ((val1) || (val2));                   \
      break;                                           \
    case OP_EQUALS :                                   \
      (retval) = ( (val1) == (val2));                  \
      break;                                           \
    case OP_NOTEQUALS :                                \
      (retval) = ( (val1) != (val2));                  \
      break;                                           \
    case OP_LESS_THAN :                                \
      (retval) = ((val1) < (val2));                    \
      break;                                           \
    case OP_LEQUALS :                                  \
      (retval) = ((val1) <= (val2));                   \
      break;                                           \
    case OP_GREATER_THAN :                             \
      (retval) = ((val1) > (val2));                    \
      break;                                           \
    case OP_GEQUALS :                                  \
      (retval) = ((val1) >= (val2));                   \
      break;                                           \
    default :                                          \
      fprintf(stderr, "Unknown operation %d", opcode); \
      (retval) = 0;                                    \
  }

  /* Need to decide where to get operands from and where to put them. */
  if(val1->type==ival && val2->type==ival)
  {
    retval->type=ival;
    EVALUATEBINARY(retval->value.ival,val1->value.ival,val2->value.ival);
  }
  else if(val1->type==rval && val2->type==ival)
  {
    retval->type=rval;
    EVALUATEBINARY(retval->value.rval,val1->value.rval,val2->value.ival);
  }
  else if(val1->type==ival && val2->type==rval)
  {
    retval->type=rval;
    EVALUATEBINARY(retval->value.rval,val1->value.ival,val2->value.rval);
  }
  else /* if(val1->type==rval && val2->type==rval) */
  {
    retval->type=rval;
    EVALUATEBINARY(retval->value.rval,val1->value.rval,val2->value.rval);
  }

  return 0;
}

 /*@@
   @routine    EvaluateUnary
   @date       Sun Nov  4 22:18:50 2001
   @author     Tom Goodale
   @desc 
   Evaluates a unary operation on value
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     retval
   @vdesc   The result
   @vtype   uExpressionValue *
   @vio     out
   @vcomment 
 
   @endvar 
 
   @endvar 
   @var     opcode
   @vdesc   The opcode of the operator.
   @vtype   uExpressionOpcode
   @vio     in
   @vcomment 
 
   @endvar 
   @var     value
   @vdesc   The operand
   @vtype   const uExpressionValue *
   @vio     in
   @vcomment 

   @returntype int
   @returndesc
   0 - success
   @endreturndesc
@@*/
static int EvaluateUnary(uExpressionValue *retval, 
                         uExpressionOpcode opcode, 
                         const uExpressionValue *value)
{
  
  #define UNARYMATHOPERATE(a) \
    case OP_##a :             \
      (retval) = a(val);      \
      break;

  #define EVALUATEUNARY(retval, val)                   \
  switch(opcode)                                       \
  {                                                    \
    case OP_NOT :                                      \
      (retval) = !(val);                               \
      break;                                           \
    case OP_PASS :                                     \
      (retval) = +(val);                               \
      break;                                           \
    case OP_NEGATE :                                   \
      (retval) = -(val);                               \
      break;                                           \
    case OP_ACOS :                                     \
      (retval) = acos(val);                            \
      break;                                           \
    case OP_ASIN :                                     \
      (retval) = asin(val);                            \
      break;                                           \
    case OP_ATAN :                                     \
      (retval) = atan(val);                            \
      break;                                           \
    case OP_CEIL :                                     \
      (retval) = ceil(val);                            \
      break;                                           \
    case OP_COS :                                      \
      (retval) = cos(val);                             \
      break;                                           \
    case OP_COSH :                                     \
      (retval) = cosh(val);                            \
      break;                                           \
    case OP_EXP :                                      \
      (retval) = exp(val);                             \
      break;                                           \
    case OP_FABS :                                     \
      (retval) = fabs(val);                            \
      break;                                           \
    case OP_FLOOR :                                    \
      (retval) = floor(val);                           \
      break;                                           \
    case OP_LOG :                                      \
      (retval) = log(val);                             \
      break;                                           \
    case OP_LOG10 :                                    \
      (retval) = log10(val);                           \
      break;                                           \
    case OP_SIN :                                      \
      (retval) = sin(val);                             \
      break;                                           \
    case OP_SINH :                                     \
      (retval) = sinh(val);                            \
      break;                                           \
    case OP_SQRT :                                     \
      (retval) = sqrt(val);                            \
      break;                                           \
    case OP_TAN :                                      \
      (retval) = tan(val);                             \
      break;                                           \
    case OP_TANH :                                     \
      (retval) = tanh(val);                            \
      break;                                           \
    case OP_TRUNC :                                    \
      (retval) = trunc(val);                           \
      break;                                           \
    default :                                          \
      fprintf(stderr, "Unknown operation %d", opcode); \
      (retval) = 0;                                    \
  }

  if(value->type==ival)
  {
    retval->type=ival;
    EVALUATEUNARY(retval->value.ival,value->value.ival);
  }
  else /* if(val->type==rval) */
  {
    retval->type=rval;
    EVALUATEUNARY(retval->value.rval,value->value.rval);
  }

  return 0;
}  



 /*@@
   @routine    isoperator
   @date       Tue Sep 19 21:30:20 2000
   @author     Tom Goodale
   @desc 
   Tests if a string is an operator or not.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     token
   @vdesc   Token to test.
   @vtype   const char *
   @vio     in
   @vcomment 
 
   @endvar 

   @returntype int
   @returndesc
   1 - is an operator
   0 - is not an operator
   @endreturndesc
@@*/
static int isoperator(const char *token)
{
  int retval;
  
  int i;

  retval = 0;

  for(i=0; operators[i].operator; i++)
  {
    if(!strcmp(operators[i].operator,token))
    {
      retval = 1;
      break;
    }
  }

  return retval;
}

 /*@@
   @routine    cmpprecendence
   @date       Wed Sep 20 09:05:59 2000
   @author     Tom Goodale
   @desc 
   Compare the precedence of two operators.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     op1
   @vdesc   First operator
   @vtype   const char *
   @vio     in
   @vcomment 
 
   @endvar 
   @var     op2
   @vdesc   Second operator
   @vtype   const char *
   @vio     in
   @vcomment 
 
   @endvar 

   @returntype int
   @returndesc
   +ve - op1 is higher precedence than op2
     0 - op1 and op2 are of equal precedence
   -ve - op2 is higher precedence than op1
   @endreturndesc
@@*/
static int cmpprecendence(const char *op1, const char *op2)
{
  int retval;
  int i;
  int op1prec;
  int op2prec;

  op1prec = -1;
  op2prec = -1;

  for(i=0; operators[i].operator; i++)
  {
    if(!strcmp(operators[i].operator,op1))
    {
      op1prec = operators[i].precedence;
    }
    if(!strcmp(operators[i].operator,op2))
    {
      op2prec = operators[i].precedence;
    }

    if(op1prec != -1 && op2prec != -1)
    {
      break;
    }
  }

  /* Now see which has the higher precedence */
  retval = op2prec-op1prec;

  return retval;
}

 /*@@
   @routine    opencode
   @date       Sun Nov  4 22:05:36 2001
   @author     Tom Goodale
   @desc 
   Finds the encoding for a given operator.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     operator
   @vdesc   The operator to get info on.
   @vtype   const char *
   @vio     in
   @vcomment 
 
   @endvar 
   @var     type
   @vdesc   Operator type
   @vtype   uExpressionType *
   @vio     out
   @vcomment 
 
   @endvar 
   @var     opcode
   @vdesc   The operator's opcode
   @vtype   uExpressionOpcode
   @vio     out
   @vcomment 
 
   @endvar 

   @returntype int
   @returndesc
   0  - success
   -1 - operator doesn't exist
   @endreturndesc
@@*/
static int opencode(const char *operator, uExpressionType *type, uExpressionOpcode *opcode)
{
  int retcode;
  int i;

  retcode = -1;
  for(i=0; operators[i].operator; i++)
  {
    if(!strcmp(operators[i].operator,operator))
    {
      retcode = 0;
      *type   = operators[i].type;
      *opcode = operators[i].opcode;
      break;
    }
  }
  return retcode;
}


 /*@@
   @routine    StoreVar
   @date       Sun Nov  4 22:09:34 2001
   @author     Tom Goodale
   @desc 
   Stores a variable name in an expression buffer.
   If the variable already exists, it returns the
   old number, otherwise it saves the variable and
   returns its new number.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     buffer
   @vdesc   Buffer in which to store the name
   @vtype   uExpressionInternals *
   @vio     inout
   @vcomment 
 
   @endvar 
   @var     var
   @vdesc   The variable to be stored.
   @vtype   const char *
   @vio     in
   @vcomment 
 
   @endvar 

   @returntype int
   @returndesc
   The assigned variable number
   @endreturndesc
@@*/
static int StoreVar(uExpressionInternals *buffer, const char *var)
{
  int retval;
  int i;

  retval = -1;

  /* Look for old value. */
  for(i = 0; i < buffer->nvars; i++)
  {
    if(!strcmp(buffer->vars[i],var))
    {
      retval = i;
      break;
    }
  }

  /* If the variable is new, resize list and add it. */
  if(retval == -1)
  {
    buffer->nvars++;
    buffer->vars = (char **)realloc(buffer->vars,sizeof(char *)*buffer->nvars);
    
    buffer->vars[buffer->nvars-1] = strdup(var);

    retval = buffer->nvars-1;
  }

  return retval;
}
 
 /*@@
   @routine    newtoken
   @date       Tue Sep 19 21:30:42 2000
   @author     Tom Goodale
   @desc 
   Creates a new token item given the beginning and end of
   the string containing the token.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     tokenstart
   @vdesc   pointer to first charater in token
   @vtype   const char *
   @vio     in
   @vcomment 
 
   @endvar 
   @var     tokenend
   @vdesc   pointer to last charater in token
   @vtype   const char *
   @vio     in
   @vcomment 
 
   @endvar 

   @returntype pToken *
   @returndesc
   The new token object
   @endreturndesc
@@*/
static pToken *newtoken(const char *tokenstart, const char *tokenend)
{
  pToken *this;
  const char *position;
  char *newpos;

  this = (pToken *)malloc(sizeof(pToken));

  if(this)
  {
    this->last = NULL;
    this->next = NULL;

    this->token = (char *)malloc(tokenend-tokenstart+2);
    if(this->token)
    {
      for(position=tokenstart, newpos=this->token; 
          position <= tokenend; 
          position++, newpos++)
      {
        *newpos = *position;
      }
      /* Just in case not already null terminated */
      *newpos = 0;

      /* Strip off any trailing spaces */
      for(; newpos >= this->token && 
            (*newpos == 0 || *newpos == ' ' || *newpos == '\t'); newpos--)
      {
        *newpos = 0;
      }
    }
  }

  return this;
}

 /*@@
   @routine    FreeTokens
   @date       Tue Sep 19 21:39:07 2000
   @author     Tom Goodale
   @desc 
   Frees a list of tokens.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/
void FreeTokens(pToken *list)
{
  pToken *token;
  pToken *next;

  for(token = list; token; token = next)
  {
    next = token->next;
    free(token->token);
    free(token);
  }
}  

 /*@@
   @routine    insertafter
   @date       Tue Sep 19 21:34:02 2000
   @author     Tom Goodale
   @desc 
   Inserts a token after another one in a list.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     base
   @vdesc   The base of the list
   @vtype   pToken *
   @vio     inout
   @vcomment 
 
   @endvar 
   @var     this
   @vdesc   Token to add to list.
   @vtype   pToken *
   @vio     inout
   @vcomment 
 
   @endvar 

@@*/
static void insertafter(pToken *base, pToken *this)
{
  if(base && this)
  {
    this->last = base;
    this->next = base->next;
    base->next = this;
    
    if(this->next)
    {
      this->next->last = this;
    }
  }
}
  
#if TEST_EXPRESSION_PARSER

 /*@@
   @routine    opname
   @date       Sun Nov  4 22:08:10 2001
   @author     Tom Goodale
   @desc 
   Finds the name of an operator.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     opcode
   @vdesc   The operator's opcode
   @vtype   uExpressionOpcode
   @vio     in
   @vcomment 
 
   @endvar 

   @returntype const char *
   @returndesc
   The operator's name or NULL if it doesn't exist.
   @endreturndesc
@@*/
static const char *opname(uExpressionOpcode opcode)
{
  const char *retval;
  int i;

  retval = NULL;

  for(i=0; operators[i].operator; i++)
  {
    if(operators[i].opcode == opcode)
    {
      retval = operators[i].operator;
      break;
    }
  }
  return retval;
}
 /*@@
   @routine    printtokens
   @date       Tue Sep 19 21:34:24 2000
   @author     Tom Goodale
   @desc 
   Debugging function to print out the tokens.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
   @var     start
   @vdesc   First token in list
   @vtype   pToken *
   @vio     in
   @vcomment 
 
   @endvar 

@@*/
static void printtokens(pToken *start)
{
  pToken *token;

  for(token = start; token; token = token->next)
  {
    printf("->%s", token->token);
  }

  printf("\n");
}
#endif /* TEST_EXPRESSION_PARSER */


/********************************************************************
 *********************     TEST FUNCTIONS   *************************
 ********************************************************************/

#ifdef TEST_EXPRESSION_PARSER

int evaluator(int nvars, const char * const *vars, uExpressionValue *vals, void *data)
{
  int i;
  char *endp;

  for(i = 0; i < nvars; i++)
  {
    if (strcmp(vars[i], "pi") == 0)
    {
      vals[i].type = rval;
      vals[i].value.rval = M_PI;
    }
    else if (strcmp(vars[i], "one") == 0)
    {
      vals[i].type = ival;
      vals[i].value.ival = 1;
    }
    else if(strchr(vars[i],'.') || strchr(vars[i],'e') || strcasecmp(vars[i], "nan") == 0 || strcasecmp(vars[i], "inf") == 0)
    {
      vals[i].type = rval;
      vals[i].value.rval = strtod(vars[i], &endp);
      if(*endp)
        printf("Unrecognisable number format '%s' read as %g, unread '%s'\n", vars[i], vals[i].value.rval, endp);
    }
    else
    {
      vals[i].type = ival;
      vals[i].value.ival = strtol(vars[i], &endp,0);
      if(*endp)
        printf("Unrecognisable number format '%s' read as %d, unread '%s'\n", vars[i], vals[i].value.ival, endp);
    }    
  }

  return 0;
}


int main(int argc, char *argv[])
{
  uExpression buffer;
  uExpressionValue value;

  value.type = ival;
  
  if(argc < 2)
  {
    printf("Usage: %s \"string\"\n", argv[0]);
    exit(0);
  }

  buffer = Util_ExpressionParse(argv[1]);

  if(buffer)
  {
    Util_ExpressionEvaluate(buffer, &value,evaluator,NULL);

    if(value.type==ival)
    {
      printf("Value is %d\n", value.value.ival); 
    }
    else
    {
      printf("Value is %#g\n", value.value.rval); 
    }
    
    Util_ExpressionFree(buffer);
  }
  else
  {
    printf("Invalid expression : '%s'\n", argv[1]);
  }

  return 0;
}
  
#endif /* TEST_EXPRESSION_PARSER */    
