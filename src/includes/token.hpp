/*
 *  
 *  Copyright(C) 2018 Gerald Coe, Devantech Ltd <gerry@devantech.co.uk>
 * 
 *  Permission to use, copy, modify, and/or distribute this software for any purpose with or
 *  without fee is hereby granted, provided that the above copyright notice and 
 *  this permission notice appear in all copies.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 *  THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. 
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL 
 *  DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include <iostream>

/*
optcode values fot the Avalanche processors instruction set
*/
const int INSTRUCTION_ADD_VALUE = 0x00;
const int INSTRUCTION_OR_VALUE = 0x02;
const int INSTRUCTION_AND_VALUE = 0x03;
const int INSTRUCTION_XOR_VALUE = 0x04;
const int INSTRUCTION_SRL_VALUE = 0x06;
const int INSTRUCTION_LDI_VALUE = 0x11;
const int INSTRUCTION_JAL_VALUE = 0x12;
const int INSTRUCTION_JZ_VALUE = 0x94;
const int INSTRUCTION_JNZ_VALUE = 0x95;
const int INSTRUCTION_JALR_VALUE = 0x98;
const int INSTRUCTION_JEQR_VALUE = 0x99;
const int INSTRUCTION_JNER_VALUE = 0x9A;
const int INSTRUCTION_JLTR_VALUE = 0x9B;
const int INSTRUCTION_JGER_VALUE = 0x9C;
const int INSTRUCTION_JBSR_VALUE = 0x9D;
const int INSTRUCTION_JBCR_VALUE = 0x9E;
const int INSTRUCTION_NOP_VALUE = 0x00;

/*
String values that are the instructions to the processor as written in an assembly file
*/
const std::string INSTRUCTION_ADD = "add";
const std::string INSTRUCTION_OR = "or";
const std::string INSTRUCTION_AND = "and";
const std::string INSTRUCTION_XOR = "xor";
const std::string INSTRUCTION_SRL = "srl";
const std::string INSTRUCTION_LDI = "ldi";
const std::string INSTRUCTION_JAL = "jal";
const std::string INSTRUCTION_JZ = "jz";
const std::string INSTRUCTION_JNZ = "jnz";
const std::string INSTRUCTION_JALR = "jalr";
const std::string INSTRUCTION_JEQR = "jeqr";
const std::string INSTRUCTION_JNER = "jner";
const std::string INSTRUCTION_JLTR = "jltr";
const std::string INSTRUCTION_JGER = "jger";
const std::string INSTRUCTION_JBSR = "jbsr";
const std::string INSTRUCTION_JBCR = "jbcr";
const std::string INSTRUCTION_NOP = "nop";

/*
String values that represent the macro commands available
*/
const std::string MACRO_SUB = "sub";
const std::string MACRO_DJNZ = "djnz";
const std::string MACRO_JMP = "jmp";
const std::string MACRO_CALL = "call";
const std::string MACRO_RETURN = "ret";
const std::string MACRO_SLL = "sll";
const std::string MACRO_JLER = "jler";
const std::string MACRO_JGTR = "jgtr";
const std::string MACRO_INC = "inc";
const std::string MACRO_DEC = "dec";
const std::string MACRO_JEQ = "jeq";
const std::string MACRO_JNE = "jne";
const std::string MACRO_JBS = "jbs";
const std::string MACRO_JBC = "jbc";
const std::string MACRO_JGT = "jgt";
const std::string MACRO_JLE = "jle";
const std::string MACRO_JLT = "jlt";
const std::string MACRO_JGE = "jge";
const std::string MACRO_LD = "ld";
const std::string MACRO_ST = "st";

/*
The data types that can be defined
*/
const std::string REG = ".reg";
const std::string CON = ".const";
const std::string DAT = ".data";

const std::string PROC = "process";
const std::string EPROC = "endprocess";

/*
Values denoting what types of token can be found
*/
enum Types
{
  NONE,
  IDENTIFIER,
  INSTRUCTION,
  COMMA,
  PROCESS,
  ENDPROCESS,
  REGISTER,
  DATA,
  CONST,
  MACRO,
  NUMBER,
  INDIRECTION,
  SIZE,
  STRING,
  LABEL,
  OPERATOR
};

class Token
{
private:
  std::string identifier;

  /*
  process the identifier that was used to create this token and 
  evalueate what type of token we should create
  */
  void createToken();

  /*
  Checks to see if the current token being passed in is an instruction.
  
  returns non zero is instruction is found.
  */
  int checkInstruction(void);

  /*
   Checks to see if the token being checked is a number.
   123  - decimal number 123
   011  - ocatal number 9
   0x11 - hex 17
   0X11 - hex 17
   returns non zero if a valid number was found
   */
  int checkNumber(void);

  /*
   Check for a directive to the compiler.
   .reg     - a register declaration
   .data    - a data declatation
   .const   - aconstant value
   */
  int checkType(void);

  /*
  Check if this token is a process or endprocess commands

  returns non zero if valid
  */
  int checkProcess(void);

  /*
   Matches an identifier here. Identifiers must start with a letter and then can only contain
   characters a-z A-Z 0-9 and _

   returns non zero if valid
   */
  int checkIdentifier(void);

  /*
  Check if this Token is a valid macro commands

  returns non zero if valid
  */
  int checkMacros(void);

  /*
  Check if this token is a data size token

  returns non zero if valid
  */
  int checkSize(void);

  /*
  Check if this token is an idirection to a register

  returns non zero if valid
  */
  int checkIndirection(void);

  /*
  Check if this token is a string definition

  returns non zero if valid
  */
  int checkString(void);

  /*
  Check if this token is a lable

  returns non zero if valid
  */
  int checkLabel(void);

  /*
  Check if this token is a mathematical operator

  returns non zero if valid
  */
  int checkOperator(void);

public:
  Token(){};
  Token(std::string s) : identifier(s)
  {
    createToken();
  };

  /*
  The type of token this is
  */
  int type = NONE;

  /*
  The value of this token if it is a number
  */
  int value;

  /*
  The string that represents this token or instruction
  */
  std::string s_value;

  /*
  A map that links the instructions string value to its optcode values.
  */
  static std::unordered_map<std::string, int> instructions;

  /*
  Holds all of the macro strings for lookup
  */
  static std::vector<std::string> macros;
};

#endif