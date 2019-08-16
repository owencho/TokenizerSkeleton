#include "assembleAllInstruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "unity.h"
#include "Common.h"
#include "Token.h"
#include "Error.h"
#include "Tokenizer.h"
#include "CException.h"
#include "Exception.h"
#include "operand.h"
#include "instruction.h"
#include "arithmeticOperationsInstruction.h"
#include "bitOperationInstruction.h"
#include "breakPointInstruction.h"
#include "compareAndTestsInstruction.h"
#include "conditionalBitTestBranchInstruction.h"
#include "conditionalBranchExecutionInstruction.h"
#include "conditionCodeFlagModificationInstruction.h"
#include "incrementDecrementInstruction.h"
#include "interruptManagementInstruction.h"
#include "loadTransferInstruction.h"
#include "logicalOperationsInstruction.h"
#include "shiftRotatesInstruction.h"
#include "unconditionalJumpCallInstruction.h"
#include "assembleAllInstruction.h"
#include "CustomAssert.h"
#include "stackInstruction.h"
CEXCEPTION_T ex;
void setUp(void){}
void tearDown(void){}

/*
* This is assembleAllInstruction test file which test
* all the assembleAllInstruction eg notNullCheck, getDataFlag,commarCheck
* bitOperationCheck and more..
*
**/

//notNullCheck function test/////////////////////////////////////////////////////////
//notNULLCheck throw exception when the token is not null
void test_notNullCheck_given_a_expect_thrown(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  ConversionData dataFlag;
  Try{
      tokenizer = createTokenizer("  a ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      notNullCheck(tokenizer);
      TEST_FAIL_MESSAGE("Expecting error exeception to be thrown.");
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_ASSERT_EQUAL(ERR_INVALID_SYNTAX, ex->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_notNullCheck_given_NULL_expect_no_exception_thrown(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  ConversionData dataFlag;
  Try{
      tokenizer = createTokenizer("   ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      notNullCheck(tokenizer);
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
  }
  freeTokenizer(tokenizer);
}
//getDataFlag function test/////////////////////////////////////////////////////////
//getDataFlag retrieve dataTable of the CodeInfo by comparing the operandType
void test_getDataFlag_given_adc_A_expect_pass(void) {
    Tokenizer *tokenizer = NULL;
		IntegerToken * token;
		ConversionData dataFlag;
    stm8Operand * operand;
    Try{
        tokenizer = createTokenizer("  ADC A,#$55 ");
				configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        token = (IntegerToken *)getToken(tokenizer);
				freeToken(token);
        operand = getOperand(tokenizer,ALL_OPERANDS);
        TEST_ASSERT_EQUAL_UINT16(A_OPERAND, operand->type);
				dataFlag = getDataFlag(&adcCodeInfo,tokenizer,operand);
        TEST_ASSERT_EQUAL_UINT16(A_OPERAND, dataFlag.type);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}
// it throw exception due to adc doesnt support X operand which in the flag
// which this function couldnt locate the flag for X_OPERAND
void test_getDataFlag_given_adc_X_expect_null_dataFlag_throw_error(void) {
    Tokenizer *tokenizer = NULL;
		IntegerToken * token;
		ConversionData dataFlag;
    stm8Operand * operand;
    Try{
        tokenizer = createTokenizer("  ADC X,#$55 ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        token = (IntegerToken *)getToken(tokenizer);
				freeToken(token);
        operand = getOperand(tokenizer,ALL_OPERANDS);
				dataFlag = getDataFlag(&adcCodeInfo,tokenizer,operand);
        TEST_FAIL_MESSAGE("Expecting error exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_DATATABLE_NULL, ex->errorCode);
    }
    freeTokenizer(tokenizer);
}
//commarCheck function test /////////////////////////////////////////////////////////
//commarCheck check whether is there commar
// it will throw exception if commar is not detected
void test_commarCheck_given_1_expect_thrown(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  ConversionData dataFlag;
  Try{
      tokenizer = createTokenizer("  1 ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      commarCheck(tokenizer);
      TEST_FAIL_MESSAGE("Expecting error exeception to be thrown.");
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_ASSERT_EQUAL(ERR_INVALID_SYNTAX, ex->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_commarCheck_given_NULL_expect_no_exception_thrown(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  ConversionData dataFlag;
  Try{
      tokenizer = createTokenizer("  , ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      commarCheck(tokenizer);
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
  }
  freeTokenizer(tokenizer);
}
//bitOperationCheck function test/////////////////////////////////////////////////////////
//bitOperationCheck is used to detect is the instruction is BCCM ,BCPL
// BSET,BRES,BTJT,BTJF to return the selection of nvalue which is 1+2*n
// or 2*n only
void test_bitOperationCheck_given_BCCM_expect_return2(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  int value;
  Try{
      tokenizer = createTokenizer("  BCCM ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      value = bitOperationCheck(token);
      TEST_ASSERT_EQUAL_INT(2,value);
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
  }
  freeTokenizer(tokenizer);
}
void test_bitOperationCheck_given_BRES_expect_return2(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  int value;
  Try{
      tokenizer = createTokenizer("  BRES ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      value = bitOperationCheck(token);
      TEST_ASSERT_EQUAL_INT(2,value);
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
  }
  freeTokenizer(tokenizer);
}
void test_bitOperationCheck_given_BTJF_expect_return2(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  int value;
  Try{
      tokenizer = createTokenizer("  Btjf ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      value = bitOperationCheck(token);
      TEST_ASSERT_EQUAL_INT(2,value);
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
  }
  freeTokenizer(tokenizer);
}

void test_bitOperationCheck_given_BCPL_expect_return1(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  int value;
  Try{
      tokenizer = createTokenizer("  BCPL ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      value = bitOperationCheck(token);
      TEST_ASSERT_EQUAL_INT(1,value);
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
  }
  freeTokenizer(tokenizer);
}
void test_bitOperationCheck_given_BSET_expect_return1(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  int value;
  Try{
      tokenizer = createTokenizer("  BSET ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      value = bitOperationCheck(token);
      TEST_ASSERT_EQUAL_INT(1,value);
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
  }
  freeTokenizer(tokenizer);
}
void test_bitOperationCheck_given_BTJT_expect_return1(void){
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  int value;
  Try{
      tokenizer = createTokenizer("  Btjt ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      value = bitOperationCheck(token);
      TEST_ASSERT_EQUAL_INT(1,value);
  } Catch(ex) {
      dumpTokenErrorMessage(ex, __LINE__);
      TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
  }
  freeTokenizer(tokenizer);
}
//get2ndCompValue function test/////////////////////////////////////////////////////////
// get2ndCompValue will return the value which is 2nd compliment and added with
// the length of opcode which is only limited to SHORT_OFF_OPERAND
// other operand negative value will be thrown
void test_get2ndCompValue_given_jrc_neg12_expect_return_F0(void) {
    Tokenizer *tokenizer = NULL;
		IntegerToken * token;
		ConversionData dataFlag;
    stm8Operand * operand;
    int value;
    Try{
        tokenizer = createTokenizer("   JRC -$12 ");
				configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        token = (IntegerToken *)getToken(tokenizer);
				freeToken(token);
        operand = getOperand(tokenizer,(1<<SHORT_OFF_OPERAND));
        TEST_ASSERT_EQUAL_UINT16(SHORT_OFF_OPERAND, operand->type);
				dataFlag = getDataFlag(&jrcCodeInfo,tokenizer,operand);
        TEST_ASSERT_EQUAL_UINT16(SHORT_OFF_OPERAND, dataFlag.type);
        value = get2ndCompValue(dataFlag,operand,tokenizer);
        TEST_ASSERT_EQUAL_HEX8(0xF0,value);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_get2ndCompValue_given_jrc_hex13_expect_hex15(void) {
    Tokenizer *tokenizer = NULL;
		IntegerToken * token;
		ConversionData dataFlag;
    stm8Operand * operand;
    int value;
    Try{
        tokenizer = createTokenizer("   JRC $13");
				configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        token = (IntegerToken *)getToken(tokenizer);
				freeToken(token);
        operand = getOperand(tokenizer,(1<<SHORT_OFF_OPERAND));
        TEST_ASSERT_EQUAL_UINT16(SHORT_OFF_OPERAND, operand->type);
				dataFlag = getDataFlag(&jrcCodeInfo,tokenizer,operand);
        TEST_ASSERT_EQUAL_UINT16(SHORT_OFF_OPERAND, dataFlag.type);
        value = get2ndCompValue(dataFlag,operand,tokenizer);
        TEST_ASSERT_EQUAL_HEX8(0x15,value);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_get2ndCompValue_given_jrc_hex0_expect_hex02(void) {
    Tokenizer *tokenizer = NULL;
		IntegerToken * token;
		ConversionData dataFlag;
    stm8Operand * operand;
    int value;
    Try{
        tokenizer = createTokenizer("   JRC $0 ");
				configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        token = (IntegerToken *)getToken(tokenizer);
				freeToken(token);
        operand = getOperand(tokenizer,(1<<SHORT_OFF_OPERAND));
        TEST_ASSERT_EQUAL_UINT16(SHORT_OFF_OPERAND, operand->type);
				dataFlag = getDataFlag(&jrcCodeInfo,tokenizer,operand);
        TEST_ASSERT_EQUAL_UINT16(SHORT_OFF_OPERAND, dataFlag.type);
        value = get2ndCompValue(dataFlag,operand,tokenizer);
        TEST_ASSERT_EQUAL_HEX8(0x02,value);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}
//addition2ndCompValueWithLength function test/////////////////////////////////////////////////////////
// addition2ndCompValueWithLength will return the value which is 2nd compliment and added with
// the input length
// other operand negative value will be thrown
void test_addition2ndCompValueWithLength_given_hex20_expect_hex25(void) {
    Tokenizer *tokenizer = NULL;
		IntegerToken * token;
    stm8Operand * operand;
    int value;
    Try{
        tokenizer = createTokenizer("   $20 ");
				configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        operand = getOperand(tokenizer,(1<<SHORT_OFF_OPERAND));
        TEST_ASSERT_EQUAL_UINT16(SHORT_OFF_OPERAND, operand->type);
        token = (IntegerToken *)getToken(tokenizer);
        value = addition2ndCompValueWithLength(operand,5,token);
        TEST_ASSERT_EQUAL_HEX8(0x25,value);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_addition2ndCompValueWithLength_given_neghex31_expect_hexD1(void) {
    Tokenizer *tokenizer = NULL;
		IntegerToken * token;
    stm8Operand * operand;
    int value;
    Try{
        tokenizer = createTokenizer("   -$31 ");
				configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        operand = getOperand(tokenizer,(1<<SHORT_OFF_OPERAND));
        TEST_ASSERT_EQUAL_UINT16(SHORT_OFF_OPERAND, operand->type);
        token = (IntegerToken *)getToken(tokenizer);
        value = addition2ndCompValueWithLength(operand,2,token);
        TEST_ASSERT_EQUAL_HEX8(0xD1,value);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}
// this function throw exception because the operand created by createOperand
// value is NA which the value is invalid , so it will thrown exception as
// input invalid value
void test_addition2ndCompValueWithLength_given_NA_expect_exception(void) {
    Tokenizer *tokenizer = NULL;
		IntegerToken * token;
    stm8Operand * operand;
    int value;
    Try{
        tokenizer = createTokenizer("   A ");
				configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        token = (IntegerToken *)getToken(tokenizer);
        pushBackToken(tokenizer,(Token*)token);
        operand = getOperand(tokenizer,ALL_OPERANDS);
        token = (IntegerToken *)getToken(tokenizer);
        value = addition2ndCompValueWithLength(operand,2,token);
        TEST_FAIL_MESSAGE("Expecting error exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_INTEGER_NULL, ex->errorCode);
    }
    freeTokenizer(tokenizer);
}
//getMOVOperand function test/////////////////////////////////////////////////////////
// getMOVOperand will return operand which MOV instruction have two complex operand
// that required special function to generate operand
// as we know short_mem are supported by long_mem_operand
// dst is shortmem and src are byte or long mem (it convert dst into LONG_MEM_OPERAND)
// dst is longmem and src are shortmem  (it will convert src  into LONG_MEM_OPERAND)
// then only start to check and return operand
// This function will return BYTE_OPERAND if(dst is longmem and src is BYTE_OPERAND)
// This function will return LONG_MEM_OPERAND if(dst is longmem and src is longmem)
// if return as LONG_MEM_OPERAND the src(dataSize.ls) will passed in to another function
// machineCodeAllocateOutput to be added into machineCode
// This function will return SHORT_MEM_OPERAND if
//(dst is SHORT_MEM_OPERAND and src is SHORT_MEM_OPERAND)
void test_getMOVOpcode_given_shortmem_byte_expect_return_byte_operand(void) {
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  stm8Operand *dst = NULL;
	stm8Operand *src = NULL;
  stm8Operand *operand = NULL;
	ConversionData dataFlag;
	Try{
      tokenizer = createTokenizer("   mov $12,#$12 ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      dst = getOperand(tokenizer,(1<<SHORT_MEM_OPERAND | 1<LONG_MEM_OPERAND));
      commarCheck(tokenizer);
      src = getOperand(tokenizer,( 1<<BYTE_OPERAND));
			operand = getMOVOperand(tokenizer,dst , src );
      TEST_ASSERT_NOT_NULL(operand);
			TEST_ASSERT_EQUAL_UINT16(BYTE_OPERAND, operand->type);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.extCode);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.code);
			TEST_ASSERT_EQUAL_UINT16(0x12, operand->dataSize.ms);
			TEST_ASSERT_EQUAL_UINT16(0x00, operand->dataSize.ls);
			TEST_ASSERT_EQUAL_UINT16(0x12, operand->dataSize.extB);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_getMOVOpcode_given_shortmem_longmem_expect_return_longmem_operand(void) {
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  stm8Operand *dst = NULL;
	stm8Operand *src = NULL;
  stm8Operand *operand = NULL;
	ConversionData dataFlag;
	Try{
      tokenizer = createTokenizer("   mov $12,$1200 ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      dst = getOperand(tokenizer,1<<SHORT_MEM_OPERAND);
      commarCheck(tokenizer);
      src = getOperand(tokenizer,1<<LONG_MEM_OPERAND);
      operand = getMOVOperand(tokenizer,dst , src );
      TEST_ASSERT_NOT_NULL(operand);
			TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, operand->type);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.extCode);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.code);
			TEST_ASSERT_EQUAL_UINT16(0x00, operand->dataSize.ms);
			TEST_ASSERT_EQUAL_UINT16(0x00, operand->dataSize.ls);
			TEST_ASSERT_EQUAL_UINT16(0x12, operand->dataSize.extB);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_getMOVOpcode_given_longmem_byte_expect_return_byte_operand(void) {
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  stm8Operand *dst = NULL;
	stm8Operand *src = NULL;
  stm8Operand *operand = NULL;
	ConversionData dataFlag;
	Try{
      tokenizer = createTokenizer("   mov $1200,#$12 ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      dst = getOperand(tokenizer,1<<LONG_MEM_OPERAND);
      TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, dst->type);
      commarCheck(tokenizer);
      src = getOperand(tokenizer,1<<BYTE_OPERAND);
      TEST_ASSERT_EQUAL_UINT16(BYTE_OPERAND, src->type);
      operand = getMOVOperand(tokenizer,dst , src );
      TEST_ASSERT_NOT_NULL(operand);
			TEST_ASSERT_EQUAL_UINT16(BYTE_OPERAND, operand->type);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.extCode);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.code);
			TEST_ASSERT_EQUAL_UINT16(0x12, operand->dataSize.ms);
			TEST_ASSERT_EQUAL_UINT16(0x12, operand->dataSize.ls);
			TEST_ASSERT_EQUAL_UINT16(0x00, operand->dataSize.extB);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_getMOVOpcode_given_longmem_shortmem_expect_return_longmem_operand(void) {
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  stm8Operand *dst = NULL;
	stm8Operand *src = NULL;
  stm8Operand *operand = NULL;
	ConversionData dataFlag;
	Try{
      tokenizer = createTokenizer("   mov $1200,$22 ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      dst = getOperand(tokenizer,1<<LONG_MEM_OPERAND);
      TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, dst->type);
      commarCheck(tokenizer);
      src = getOperand(tokenizer,1<<SHORT_MEM_OPERAND);
      TEST_ASSERT_EQUAL_UINT16(SHORT_MEM_OPERAND, src->type);
      operand = getMOVOperand(tokenizer,dst , src );
      TEST_ASSERT_NOT_NULL(operand);
			TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, operand->type);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.extCode);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.code);
			TEST_ASSERT_EQUAL_UINT16(0x22, operand->dataSize.ms);
			TEST_ASSERT_EQUAL_UINT16(0x12, operand->dataSize.ls);
			TEST_ASSERT_EQUAL_UINT16(0x00, operand->dataSize.extB);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_getMOVOpcode_given_longmem_longmem_expect_return_longmem_operand(void) {
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  stm8Operand *dst = NULL;
	stm8Operand *src = NULL;
  stm8Operand *operand = NULL;
	ConversionData dataFlag;
	Try{
      tokenizer = createTokenizer("   mov $2200,$3123 ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      dst = getOperand(tokenizer,1<<LONG_MEM_OPERAND);
      TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, dst->type);
      commarCheck(tokenizer);
      src = getOperand(tokenizer,1<<LONG_MEM_OPERAND);
      TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, src->type);
      operand = getMOVOperand(tokenizer,dst , src );
      TEST_ASSERT_NOT_NULL(operand);
			TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, operand->type);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.extCode);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.code);
			TEST_ASSERT_EQUAL_UINT16(0x23, operand->dataSize.ms);
			TEST_ASSERT_EQUAL_UINT16(0x22, operand->dataSize.ls);
			TEST_ASSERT_EQUAL_UINT16(0x00, operand->dataSize.extB);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_getMOVOpcode_given_shortmem_shortmem_expect_return_shortmem_operand(void) {
  Tokenizer *tokenizer = NULL;
  IntegerToken * token;
  stm8Operand *dst = NULL;
	stm8Operand *src = NULL;
  stm8Operand *operand = NULL;
	ConversionData dataFlag;
	Try{
      tokenizer = createTokenizer("   mov $12,$12 ");
      configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
      token = (IntegerToken *)getToken(tokenizer);
      dst = getOperand(tokenizer,(1<<SHORT_MEM_OPERAND | 1<LONG_MEM_OPERAND));
      TEST_ASSERT_EQUAL_UINT16(SHORT_MEM_OPERAND, dst->type);
      commarCheck(tokenizer);
      src = getOperand(tokenizer,(1<<SHORT_MEM_OPERAND | 1<LONG_MEM_OPERAND | 1<<BYTE_OPERAND));
      TEST_ASSERT_EQUAL_UINT16(SHORT_MEM_OPERAND, src->type);
      operand = getMOVOperand(tokenizer,dst , src );
      TEST_ASSERT_NOT_NULL(operand);
			TEST_ASSERT_EQUAL_UINT16(SHORT_MEM_OPERAND, operand->type);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.extCode);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.code);
			TEST_ASSERT_EQUAL_UINT16(0x12, operand->dataSize.ms);
			TEST_ASSERT_EQUAL_UINT16(0x12, operand->dataSize.ls);
			TEST_ASSERT_EQUAL_UINT16(NA, operand->dataSize.extB);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}
//hashNValueReturn function test/////////////////////////////////////////////////////////
// When the cmpType bring in as 2 which will return value = 1 + 2*n;
// When the cmpType bring in as 1 which will return value = 2*n;
// it will throw exception when #n is less than 0 and more than 7

void test_hashNValueReturn_given_commar_hash7_condition0_expect_return_0x0F(void) {
		int value;
		Tokenizer *tokenizer = NULL;
    Try{
				tokenizer = createTokenizer(" #7 ");
				value =  hashNValueReturn(tokenizer ,2);
        TEST_ASSERT_EQUAL_HEX16(0x0F,value);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_hashNValueReturn_given_commar_hash2_condition1_expect_return_0x04(void) {
		int value;
		Tokenizer *tokenizer = NULL;
    Try{
				tokenizer = createTokenizer(" #2 ");
				value =  hashNValueReturn(tokenizer ,1);
        TEST_ASSERT_EQUAL_HEX16(0x04,value);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_hashNValueReturn_given_commar_hash8_condition1_expect_fail_nlargerthan7(void) {
		Tokenizer *tokenizer = NULL;
		int value;
    Try{
				tokenizer = createTokenizer("  #8 ");
        value =  hashNValueReturn(tokenizer ,2);
        TEST_FAIL_MESSAGE("Expecting error exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_INVALID_NVALUE, ex->errorCode);
    }
    freeTokenizer(tokenizer);
}

void test_hashNValueReturn_given_commar_hash8_condition2_expect_fail_invalid_cmptype(void) {
		Tokenizer *tokenizer = NULL;
		int value;
    Try{
				tokenizer = createTokenizer("  #2 ");
        value =  hashNValueReturn(tokenizer ,3);
        TEST_FAIL_MESSAGE("Expecting error exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_INVALID_INPUT, ex->errorCode);
    }
    freeTokenizer(tokenizer);
}

//outputMachineCode function test/////////////////////////////////////////////////////////
// outputMachineCode will take value from operand , code and length of mcode
// to generate the mcode
// it will return NULL mcode when input length and output length are different
void test_outputMachineCode_given_shortmemAA_expect_0x27AA(void) {
		stm8Operand *operand = NULL;
    MachineCode* mcode;
    int a ;
    int expectedMcode[]={0x27,0xAA,END};
    ExtensionCodeAndCode code ={NA,0x27};

    Try{
        operand = createOperand(SHORT_MEM_OPERAND,NA,NA,0xAA,NA,NA);
        a = machineCodeLengthFinder(operand,code);
        TEST_ASSERT_EQUAL_INT(2, a);
				mcode =  outputMachineCode(operand ,code,a);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}

void test_outputMachineCode_given_longmemAABB_expect_0x3327AABB(void) {
		stm8Operand *operand = NULL;
    MachineCode* mcode;
    int a ;
    int expectedMcode[]={0x33,0x27,0xAA,0xBB,END};
    ExtensionCodeAndCode code ={0x33,0x27};

    Try{
        operand = createOperand(LONG_MEM_OPERAND,NA,NA,0xAA,0xBB,NA);
        a = machineCodeLengthFinder(operand,code);
        TEST_ASSERT_EQUAL_INT(4, a);
				mcode =  outputMachineCode(operand ,code,a);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}

void test_outputMachineCode_given_longmemAABB_invalid_length_input_expect_NULL(void) {
		stm8Operand *operand = NULL;
    MachineCode* mcode;
    int expectedMcode[]={0x33,0x27,0xAA,0xBB,END};
    ExtensionCodeAndCode code ={0x33,0x27};

    Try{
        operand = createOperand(LONG_MEM_OPERAND,NA,NA,0xAA,0xBB,NA);
				mcode =  outputMachineCode(operand ,code,1);

    } Catch(ex) {
        TEST_ASSERT_NULL(mcode);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}
//machineCodeLengthFinder function test/////////////////////////////////////////////////////////
// machineCodeLengthFinder will take value from operand , code to generate the mcode length
void test_machineCodeLengthFinder_given_longmemAABB_expect_4(void) {
		stm8Operand *operand = NULL;
    int a ;
    ExtensionCodeAndCode code ={0x33,0x27};

    Try{
        operand = createOperand(LONG_MEM_OPERAND,NA,NA,0xAA,0xBB,NA);
        a = machineCodeLengthFinder(operand,code);
        TEST_ASSERT_EQUAL_UINT16(4, a);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}

void test_machineCodeLengthFinder_given_shortmemBB_expect_2(void) {
		stm8Operand *operand = NULL;
    int a ;
    ExtensionCodeAndCode code ={NA,0x27};

    Try{
        operand = createOperand(SHORT_MEM_OPERAND,NA,NA,NA,0xBB,NA);
        a = machineCodeLengthFinder(operand,code);
        TEST_ASSERT_EQUAL_UINT16(2, a);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}

void test_machineCodeLengthFinder_given_extmem_expect_5(void) {
		stm8Operand *operand = NULL;
    int a ;
    ExtensionCodeAndCode code ={0x21,0x27};

    Try{
        operand = createOperand(EXT_MEM_OPERAND,NA,NA,0x33,0xBB,0x22);
        a = machineCodeLengthFinder(operand,code);
        TEST_ASSERT_EQUAL_UINT16(5, a);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}

//MachineCodeAllocateOutput function test/////////////////////////////////////////////////////////
//this movMs2ndOpValue on MachineCodeAllocateOutput used when both operand is LONG_MEM_OPERAND only for mov instruction
// nvalue is used when hashNValueReturn function is used for special instruction eg BJTF ,BJTH ,BCPL ,BCCM
//MachineCodeAllocateOutput check at last which it detect is null or not , if not null exception will be thrown
// it will take value of ExtensionCodeAndCode based on the dataFlag provided and use operand to find its value
void test_MachineCodeAllocateOutput_given_mov_expect_pass(void) {
    stm8Operand *dst = NULL;
    stm8Operand *src = NULL;
    stm8Operand *operand = NULL;
    IntegerToken * token;
		Tokenizer *tokenizer = NULL;
    ConversionData  dataFlag;
    MachineCode* mcode;
    int expectedMcode[]={0x55,0x12,0x00,0x13,0x10,END};

    Try{
        tokenizer = createTokenizer("   mov $1310,$1200 ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        token = (IntegerToken *)getToken(tokenizer);
        //first it tokenize the first $1310 check with getOperand
        dst = getOperand(tokenizer,(1<<LONG_MEM_OPERAND));
        TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, dst->type);
        //second it find the data Flag check with operand get from dst
        dataFlag = getDataFlag(&movCodeInfo,tokenizer,dst);
        TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, dataFlag.type);
        //check commar between two operand
        commarCheck(tokenizer);
        //second it tokenize the first $1200 check with getOperand
        src = getOperand(tokenizer,( 1<<LONG_MEM_OPERAND));
        TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, src->type);
        //check both operand with special mov operand
        operand = getMOVOperand(tokenizer,dst , src );
        TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, operand->type);
        //get machine code with this function
        mcode = machineCodeAllocateOutput(tokenizer,dataFlag ,operand, NA ,src->dataSize.ms);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_MachineCodeAllocateOutput_given_bset_longmem_hash_expect_pass(void) {
    stm8Operand *operand = NULL;
    IntegerToken * token;
		Tokenizer *tokenizer = NULL;
    ConversionData  dataFlag;
    MachineCode* mcode;
    int nvalue ;
    int bitOpCheck;
    int expectedMcode[]={0x72,0x12,0x10,0x00,END};

    Try{
        tokenizer = createTokenizer("  BSET $1000,#1");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        // firstly getToken "BSET"
        token = (IntegerToken *)getToken(tokenizer);
        // check the bitOperationCheck condition which will return 2,1,0 depends on which instruction
        bitOpCheck= bitOperationCheck(token);
        TEST_ASSERT_EQUAL_INT(1,bitOpCheck);
        // getOperand on $1000
        operand= getOperand(tokenizer,(1<<LONG_MEM_OPERAND));
        TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, operand->type);
        // getDataFlag on this $1000 operand
        dataFlag = getDataFlag(&bsetCodeInfo,tokenizer,operand);
        TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, dataFlag.type);
        //check commar between two operand
        commarCheck(tokenizer);
        // get nvalue on this second byte value
        nvalue = hashNValueReturn(tokenizer ,bitOpCheck);
        TEST_ASSERT_EQUAL_INT(2,nvalue);
        // get MachineCode with bringing in the nvalue
        mcode=machineCodeAllocateOutput(tokenizer,dataFlag ,operand,nvalue,NA);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_MachineCodeAllocateOutput_given_jp_longmem_expect_pass(void) {
    stm8Operand *operand = NULL;
    IntegerToken * token;
		Tokenizer *tokenizer = NULL;
    ConversionData  dataFlag;
    MachineCode* mcode;
    int nvalue ;
    int bitOpCheck;
    int expectedMcode[]={0xCC,0x10,0x00,END};

    Try{
        tokenizer = createTokenizer("  JP $1000 ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        // firstly getToken "BSET"
        token = (IntegerToken *)getToken(tokenizer);
        // getOperand on $1000
        operand= getOperand(tokenizer,(1<<LONG_MEM_OPERAND));
        TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, operand->type);
        // getDataFlag on this $1000 operand
        dataFlag = getDataFlag(&jpCodeInfo,tokenizer,operand);
        TEST_ASSERT_EQUAL_UINT16(LONG_MEM_OPERAND, dataFlag.type);
        // get MachineCode with bringing in the nvalue
        mcode=machineCodeAllocateOutput(tokenizer,dataFlag ,operand,NA,NA);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}
//assembleLDWOperand function test/////////////////////////////////////////////////////////
//an function to specially handle LDW instruction
// when a complex operand , symbol operand eg LDW (Y),Y detected as unsupported flags
// it will throw exception with the whole token "(Y),Y"
void test_assembleLDWOperand_given_ldw_Y_shortoffY_expect_0x90EE33(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0x90,0xEE,0x33,END};

    Try{
        tokenizer = createTokenizer("  LDW Y,($33,Y) ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleLDWOperand(&ldwCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex){
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
	  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_ldw_Y_BracY_expect_fail(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;

    Try{
        tokenizer = createTokenizer("  LDW Y,(X) ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleLDWOperand(&ldwCodeInfo,tokenizer);
        TEST_FAIL_MESSAGE("Expecting exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, ex->errorCode);
    }
	  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_ldw_BracY_Y_expect_fail(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;

    Try{
        tokenizer = createTokenizer("  LDW (Y),Y ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleLDWOperand(&ldwCodeInfo,tokenizer);
        TEST_FAIL_MESSAGE("Expecting exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, ex->errorCode);
    }
	  freeTokenizer(tokenizer);
}

//assembleBTJXOperand function test/////////////////////////////////////////////////////////
//an function to specially handle BTJF instruction which have 3 operand
// the second one must be #n which n must be more than zero and less than equal to 7
// third operand will be SHORT_OFF_OPERAND which handle negative and positive value

void test_assembleBTJXOperand_given_BTJF_OPERAND_expect_0x7207123426(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0x72,0x07,0x12,0x34,0x26,END};

    Try{
        tokenizer = createTokenizer("  BTJF $1234,#3,$21");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleBTJXOperand(&btjfCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    }Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_assembleBTJXOperand_given_BTJT_OPERAND_expect_0x7206122415(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0x72,0x06,0x12,0x24,0x15,END};

    Try{
        tokenizer = createTokenizer("  BTJT $1224,#3,$10");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleBTJXOperand(&btjtCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    }Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_assembleBTJXOperand_given_BTJF_OPERAND_error_on_n_value(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;

    Try{
        tokenizer = createTokenizer(" BTJF $22,#8,$33");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleBTJXOperand(&btjfCodeInfo,tokenizer);
        TEST_FAIL_MESSAGE("Expecting exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_INVALID_NVALUE, ex->errorCode);
    }
    freeTokenizer(tokenizer);
}

//assembleLDLDFOperand function test/////////////////////////////////////////////////////////
//an function to specially handle LD LDF instruction which have different type complex operand

void test_assembleLDLDFOperand_given_ld_BracX_A_expect_0xF7(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0xF7,END};

    Try{
        tokenizer = createTokenizer("  LD (X),A");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleLDLDFOperand(&ldCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex){
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
  	freeTokenizer(tokenizer);
}

void test_assembleLDLDFOperand_given_ldf_extmem_A_expect_0xBD95226A(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0xBD,0x95,0x22,0x6A,END};

    Try{
        tokenizer = createTokenizer("  LDF $95226A,A ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleLDLDFOperand(&ldfCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex){
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
  	freeTokenizer(tokenizer);
}

void test_assembleLDLDFOperand_given_ldf_X_A_OPERAND_expect_exception(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;

    Try{
        tokenizer = createTokenizer("  ldf X,A ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleLDLDFOperand(&ldfCodeInfo,tokenizer);
        TEST_FAIL_MESSAGE("Expecting exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, ex->errorCode);
    }
	  freeTokenizer(tokenizer);
}

//assembleOneOperand function test/////////////////////////////////////////////////////////
//function to handle instruction with one Operand

void test_assembleOneOperand_given_negw_x_OPERAND_expect_0x50(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0x50,END};

    Try{
        tokenizer = createTokenizer("  NeGw X ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleOneOperand(&negwCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    }Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_assembleOneOperand_given_jrh_shortoff67_0x902916(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0x90,0x29,0x16,END};
    Try{
        tokenizer = createTokenizer("  jrh $13 ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleOneOperand(&jrhCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    }Catch(ex){
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_assembleOneOperand_given_jrh_expect_fail(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;

    Try{
        tokenizer = createTokenizer("  jrh");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleOneOperand(&jrhCodeInfo,tokenizer);
        TEST_FAIL_MESSAGE("Expecting exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_DSTSRC_NULL, ex->errorCode);
    }
    freeTokenizer(tokenizer);
}

//assembleNoOperand function test/////////////////////////////////////////////////////////
//function to handle instruction that supported without Operand

void test_assembleNoOperand_given_halt_0x8c(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0x8e,END};

    Try{
        tokenizer = createTokenizer("  halt ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleNoOperand(&haltCodeInfo, tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    }Catch(ex){
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_trap_1_expect_fail(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;

    Try{
        tokenizer = createTokenizer("  trap A ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleNoOperand(&trapCodeInfo, tokenizer);
        TEST_FAIL_MESSAGE("Expecting exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_INVALID_SYNTAX, ex->errorCode);
    }
    freeTokenizer(tokenizer);
}


//assembleTwoComplexOperand function test/////////////////////////////////////////////////////////
//function to handle instruction that supported with two complex Operand

void test_assembleTwoComplexOperand_given_BSET_dollarsign33_expect_0x721A0033(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0x72,0x1A,0x00,0x33,END};
    Try{
        tokenizer = createTokenizer("  BSET $33,#5 ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleTwoComplexOperand(&bsetCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex){
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_assembleTwoComplexOperand_given_BCPL_dollarsign2233_expect_0x90142233(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0x90,0x16,0x22,0x33,END};

    Try{
        tokenizer = createTokenizer("  BCPL $2233,#3  ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleTwoComplexOperand(&bcplCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex){
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
	  freeTokenizer(tokenizer);
}

void test_assembleTwoComplexOperand_given_BCCM_extmem_expect_fail(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;

    Try{
        tokenizer = createTokenizer("  BCCM $234422,#0  ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleTwoComplexOperand(&bccmCodeInfo,tokenizer);
        TEST_FAIL_MESSAGE("Expecting exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, ex->errorCode);
    }
	  freeTokenizer(tokenizer);
}

//assembleSymbolComplexOperand function test/////////////////////////////////////////////////////////
//function to handle instruction that supported with one symbol Operand and
// one complex Operand

void test_assembleSymbolComplexOperand_given_adc_bracketX_expect_0xf9(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0xf9,END};
    Try{
        tokenizer = createTokenizer("  AdC a, (x) ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleSymbolComplexOperand(&adcCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_assembleSymbolComplexOperand_given_add_LONGOFF_X_OPERAND_expect_0xdbc123(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;
    int expectedMcode[]={0xdb,0xc1,0x23,END};
    Try{
        tokenizer = createTokenizer("  add a, ($c123,X) ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleSymbolComplexOperand(&addCodeInfo,tokenizer);
        TEST_ASSERT_EQUAL_MACHINECODE(expectedMcode,mcode);
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
    freeTokenizer(tokenizer);
}

void test_assembleSymbolComplexOperand_given_sub_a_dst_null_expect_exception_thrown(void) {
    MachineCode *mcode =NULL ;
    Tokenizer *tokenizer = NULL;

    Try{
        tokenizer = createTokenizer("  sub a ,  ");
        configureTokenizer(tokenizer,TOKENIZER_DOLLAR_SIGN_HEX);
        mcode = assembleSymbolComplexOperand(&subCodeInfo,tokenizer);
        TEST_FAIL_MESSAGE("Expecting exeception to be thrown.");
    } Catch(ex) {
        dumpTokenErrorMessage(ex, __LINE__);
        TEST_ASSERT_EQUAL(ERR_DSTSRC_NULL, ex->errorCode);
    }
}
