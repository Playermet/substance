/***************************************************************************
 * Runtime system
 *
 * Copyright (c) 2013 Randy Hollines
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the distribution.
 * - Neither the name of the Substance team nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

#ifndef __RUNTIME_H__
#define __RUNTIME_H__

#include "common.h"

/****************************
 * Runtime support structures
 ****************************/
// size of execution stack
#define EXECUTION_STACK_SIZE 128

// runtime types
enum RuntimeType {
  INT_VALUE,
  FLOAT_VALUE,
  STRING_VALUE
};

typedef union _BaseValue {
  int int_value;
  double float_value;
  void* pointer_value;
} BaseValue;

// runtime execution values
typedef struct _Value {
  RuntimeType type;
  BaseValue value;
} Value;

/****************************
 * Execution engine
 ****************************/
class Runtime {
  stack<Value*> execution_stack;
  stack<Value*> value_pool;
  vector<Instruction*> instructions;
  
  inline Value* GetPoolValue() {
    if(value_pool.empty()) {
      wcerr << L">>> execution value pool empty <<<" << endl;
      exit(1);
    }
    
    Value* value = value_pool.top();
    value_pool.pop();

    return value;
  }
  
  inline void ReleasePoolValue(Value* value) {
    value_pool.push(value);
  }

  inline void PushValue(Value* value) {
    execution_stack.push(value);
  }
  
  Value* PopValue() {
    if(execution_stack.empty()) {
      wcerr << L">>> execution stack bounds exceeded <<<" << endl;
      exit(1);
    }

    Value* value = execution_stack.top();
    execution_stack.pop();
    return value;
  }

  // member operations
  void ExecuteAdd();
  void ExecuteMultiply();
  
 public:
  Runtime(vector<Instruction*> instructions) {
    this->instructions = instructions;
    for(size_t i = 0; i < EXECUTION_STACK_SIZE; ++i) {
      value_pool.push(new Value);
    }
  }
  
  ~Runtime() {
    while(!value_pool.empty()) {
      Value* tmp = value_pool.top();
      value_pool.pop();
      // delete
      delete tmp;
      tmp = NULL;
    }
  }
  
  void Run();
};

#endif
