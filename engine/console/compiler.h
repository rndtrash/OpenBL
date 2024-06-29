//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------


#ifndef _COMPILER_H_
#define _COMPILER_H_

class Stream;
class DataChunker;

#include "platform/platform.h"
#include "console/ast.h"
#include "console/codeBlock.h"

// Autogenerated, so we should only ever include from once place - here.
// (We can't stick include guards in it without patching bison.)
#ifndef _CMDGRAM_H_
#define _CMDGRAM_H_
#include "console/cmdgram.h"
#endif

namespace Compiler
{
   /// The opcodes for the TorqueScript VM.
   enum CompiledInstructions
   {
      OP_CREATE_OBJECT = 0x00, // confirmed
      OP_FUNC_DECL = 0x01, // confirmed
      NOOP1 = 0x02, // confirmed
      NOOP2 = 0x03, // confirmed
      OP_ADD_OBJECT = 0x04, // confirmed
      OP_JMPIFNOT = 0x05,
      OP_JMPIFF = 0x06,
      OP_JMPIF = 0x07,
      OP_SETCUROBJECT = 0x08, // confirmed
      OP_STR_TO_FLT = 0x09, // confirmed
      NOOP3 = 0x0A, // confirmed
      OP_MUL = 0x0B,
      OP_CMPEQ = 0x0C,
      OP_CMPGR = 0x0D,
      OP_CMPGE = 0x0E,
      OP_CMPLT = 0x0F,
      OP_CMPLE = 0x10,
      OP_CMPNE = 0x11,
      OP_XOR = 0x12,
      OP_MOD = 0x13,
      OP_BITAND = 0x14,
      OP_BITOR = 0x15,
      OP_NOT = 0x16,
      OP_NOTF = 0x17,
      OP_ONESCOMPLEMENT = 0x18,

      OP_SHR = 0x19,
      OP_SHL = 0x1A,
      OP_AND = 0x1B,
      OP_OR = 0x1C,

      OP_ADD = 0x1D,
      OP_SUB = 0x1E,
      OP_RETURN = 0x1F, // confirmed
      OP_DIV = 0x20,
      OP_NEG = 0x21,

      OP_STR_TO_UINT = 0x22,
      OP_JMPIF_NP = 0x23,
      OP_FLT_TO_UINT = 0x24,
      OP_TERMINATE_REWIND_STR = 0x25,

      OP_LOADVAR_UINT = 0x26,
      OP_LOADVAR_FLT = 0x27,
      OP_LOADVAR_STR = 0x28,

      OP_SAVEVAR_UINT = 0x29,
      OP_SAVEVAR_FLT = 0x2A,
      OP_SAVEVAR_STR = 0x2B,

      OP_SAVEFIELD_UINT = 0x2C, // confirmed
      OP_CALLFUNC = 0x2D, // confirmed

      OP_REWIND_STR = 0x2E, // confirmed
      OP_SETCURFIELD_ARRAY = 0x2F,

      OP_BREAK = 0x30, // confirmed
      OP_LOADFIELD_FLT = 0x31,
      OP_LOADFIELD_STR = 0x32,

      OP_JMPIFNOT_NP = 0x33,
      OP_SAVEFIELD_FLT = 0x34,
      OP_SAVEFIELD_STR = 0x35,

      OP_SETCURVAR = 0x36, // confirmed
      OP_SETCURVAR_CREATE = 0x37, // confirmed
      OP_STR_TO_NONE = 0x38,
      OP_SETCURVAR_ARRAY = 0x39, // confirmed
      OP_FLT_TO_STR = 0x3A,
      OP_FLT_TO_NONE = 0x3B,
      OP_UINT_TO_FLT = 0x3C,
      OP_SETCURVAR_ARRAY_CREATE = 0x3D, // confirmed
      OP_UINT_TO_NONE = 0x3E,

      OP_LOADIMMED_UINT = 0x3F,
      OP_LOADIMMED_FLT = 0x40,
      NOOP4 = 0x41, // confirmed
      UNKNOWN2 = 0x42,
      OP_ADVANCE_STR_APPENDCHAR = 0x43, // confirmed

      UNKNOWN = 0x44,
      OP_SETCUROBJECT_NEW = 0x45,

      OP_ADVANCE_STR_NUL = 0x46, // confirmed
      OP_ADVANCE_STR_COMMA = 0x47, // confirmed
      OP_ADVANCE_STR_NUL2 = 0x48, // confirmed
      UNKNOWN4 = 0x49,
      OP_SETCURFIELD = 0x4A,
      OP_UINT_TO_STR = 0x4B, // confirmed
      OP_COMPARE_STR = 0x4C,

      OP_PUSH = 0x4D,
      OP_PUSH_FRAME = 0x4E,

      OP_TAG_TO_STR = 0x4F, // confirmed
      OP_LOADIMMED_STR = 0x50, // confirmed

      OP_CALLFUNC_RESOLVE = 0x52, // confirmed

      OP_INVALID,

      // unknown opcodes:
      OP_JMPIFFNOT,
      OP_END_OBJECT,
      OP_JMP,
      OP_LOADFIELD_UINT,
      OP_LOADIMMED_IDENT,
      OP_ADVANCE_STR,
   };

   //------------------------------------------------------------

   F64 consoleStringToNumber(const char *str, StringTableEntry file = 0, U32 line = 0);
   U32 precompileBlock(StmtNode *block, U32 loopCount);
   U32 compileBlock(StmtNode *block, U32 *codeStream, U32 ip, U32 continuePoint, U32 breakPoint);

   //------------------------------------------------------------

   struct CompilerIdentTable
   {
      struct Entry
      {
         U32 offset;
         U32 ip;
         Entry *next;
         Entry *nextIdent;
      };
      Entry *list;
      void add(StringTableEntry ste, U32 ip);
      void reset();
      void write(Stream &st);
   };

   //------------------------------------------------------------

   struct CompilerStringTable
   {
      U32 totalLen;
      struct Entry
      {
         char *string;
         U32 start;
         U32 len;
         bool tag;
         Entry *next;
      };
      Entry *list;

      char buf[256];

      U32 add(const char *str, bool caseSens = true, bool tag = false);
      U32 addIntString(U32 value);
      U32 addFloatString(F64 value);
      void reset();
      char *build();
      void write(Stream &st);
   };

   //------------------------------------------------------------

   struct CompilerFloatTable
   {
      struct Entry
      {
         F64 val;
         Entry *next;
      };
      U32 count;
      Entry *list;

      U32 add(F64 value);
      void reset();
      F64 *build();
      void write(Stream &st);
   };

   //------------------------------------------------------------

   inline StringTableEntry U32toSTE(U32 u)
   {
      return *((StringTableEntry *) &u);
   }

   extern U32 (*STEtoU32)(StringTableEntry ste, U32 ip);

   U32 evalSTEtoU32(StringTableEntry ste, U32);
   U32 compileSTEtoU32(StringTableEntry ste, U32 ip);

   CompilerStringTable *getCurrentStringTable();
   CompilerStringTable &getGlobalStringTable();
   CompilerStringTable &getFunctionStringTable();

   void setCurrentStringTable (CompilerStringTable* cst);

   CompilerFloatTable *getCurrentFloatTable();
   CompilerFloatTable &getGlobalFloatTable();
   CompilerFloatTable &getFunctionFloatTable();

   void setCurrentFloatTable (CompilerFloatTable* cst);

   CompilerIdentTable &getIdentTable();

   void precompileIdent(StringTableEntry ident);

   CodeBlock *getBreakCodeBlock();
   void setBreakCodeBlock(CodeBlock *cb);

   /// Helper function to reset the float, string, and ident tables to a base
   /// starting state.
   void resetTables();

   void *consoleAlloc(U32 size);
   void consoleAllocReset();

   extern bool gSyntaxError;
};

#endif
