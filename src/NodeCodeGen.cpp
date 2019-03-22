#include "Node.h"
#include "parser.hpp"

void NBlock::CodeGen(CodeContext& context)
{
    for(NStatement* elem: mStatements){
        elem->CodeGen(context);
    }
}

void NData::CodeGen(CodeContext& context)
{
    for(NDecl* elem:mDecls){
        elem->CodeGen(context);
    }
}

void NProgram::CodeGen(CodeContext& context)
{
    context.stackCount = 0;
    context.regCount = 0;
    mData->CodeGen(context);
    mMain->CodeGen(context);
    auto finalOp = std::make_tuple("exit","","","");
    context.opList.emplace_back(finalOp);
}

void NNumeric::CodeGen(CodeContext& context)
{
    //acquire last used register
    std::string usedReg = "%" + std::to_string(context.regCount);
    ++context.regCount;
    context.opList.emplace_back(std::make_tuple("movi",usedReg,std::to_string(mValue),""));
}

void NVarDecl::CodeGen(CodeContext& context)
{
    //associate varibale name with stack index
    context.var_regID[mName] = std::to_string(context.stackCount);
    ++context.stackCount;
    //update stack counter in code
    context.opList.emplace_back(std::make_tuple("push","r0","",""));
}

void NArrayDecl::CodeGen(CodeContext& context)
{
    for(int i = 0; i < mSize->GetValue(); i++){
        context.var_regID[mName+"["+std::to_string(i)+"]"] = std::to_string(context.stackCount);
        //update stack counter in code
        context.opList.emplace_back(std::make_tuple("push","r0","",""));
    }
}

void NNumericExpr::CodeGen(CodeContext& context)
{
    mNumeric->CodeGen(context);
}

void NVarExpr::CodeGen(CodeContext& context)
{
    std::string storeReg = "%" + std::to_string(context.regCount);
    ++context.regCount;
    context.opList.emplace_back(std::make_tuple("loadi",storeReg,context.var_regID[mName],""));
}

void NBinaryExpr::CodeGen(CodeContext& context)
{
    std::string firstReg;
    std::string secondReg;
    int firstCount;
    int secondCount;
    firstCount = context.regCount;
    mLhs->CodeGen(context);
    if(firstCount < context.regCount - 1){
        firstCount = context.regCount - 1;
    }
    firstReg = "%" + std::to_string(firstCount);
    secondCount = context.regCount;
    mRhs->CodeGen(context);
    if(secondCount < context.regCount - 1){
        secondCount = context.regCount - 1;
    }
    secondReg = "%" + std::to_string(secondCount);
    std::string storeReg = "%" + std::to_string(context.regCount);
    
    switch (mType) {
        case TADD:{
            
            context.opList.emplace_back(std::make_tuple("add",storeReg,firstReg,secondReg));
            break;
        }
        
        case TMUL:{
           
            context.opList.emplace_back(std::make_tuple("mul",storeReg,firstReg,secondReg));
            break;
        }
            
        case TDIV:{
            
            context.opList.emplace_back(std::make_tuple("div",storeReg,firstReg,secondReg));
            break;
        }
        
        case TSUB:{
           
            context.opList.emplace_back(std::make_tuple("sub",storeReg,firstReg,secondReg));
            break;
        }
    }
    ++context.regCount;
}

//subscript expression analysis is not final
void NArrayExpr::CodeGen(CodeContext& context)
{
    mSubscript->CodeGen(context);
    context.opList.emplace_back(std::make_tuple("movi","%"+std::to_string(context.regCount),std::to_string(context.stackCount),""));
    ++context.regCount;
    context.opList.emplace_back(std::make_tuple("add","%"+std::to_string(context.regCount),"%"+std::to_string(context.regCount-1),"%"+std::to_string(context.regCount-2)));
   ++context.regCount; context.opList.emplace_back(std::make_tuple("load","%"+std::to_string(context.regCount),"%"+std::to_string(context.regCount-1),""));
    ++context.regCount;
}

void NAssignVarStmt::CodeGen(CodeContext& context)
{
    mRhs->CodeGen(context);
    
    std::string usedReg = "%" + std::to_string(context.regCount-1);
    auto finalOp = std::make_tuple("storei",context.var_regID[mName],usedReg,"");
    context.opList.emplace_back(finalOp);
}

void NAssignArrayStmt::CodeGen(CodeContext& context)
{
    mRhs->CodeGen(context);
    std::string usedReg = "%" + std::to_string(context.regCount-1);
    
    mSubscript->CodeGen(context);
    context.opList.emplace_back(std::make_tuple("movi","%"+std::to_string(context.regCount),std::to_string(context.stackCount),""));
    ++context.regCount;
    context.opList.emplace_back(std::make_tuple("add","%"+std::to_string(context.regCount),"%"+std::to_string(context.regCount-1),"%"+std::to_string(context.regCount-2)));
    context.opList.emplace_back(std::make_tuple("store","%"+std::to_string(context.regCount),usedReg,""));
    ++context.regCount;
}

void NIncStmt::CodeGen(CodeContext& context)
{
    std::string varReg = context.var_regID[mName];
    context.opList.emplace_back(std::make_tuple("loadi","%"+std::to_string(context.regCount),varReg,""));
    context.opList.emplace_back(std::make_tuple("inc","%"+std::to_string(context.regCount),"",""));
    context.opList.emplace_back(std::make_tuple("storei",varReg,"%"+std::to_string(context.regCount),""));
    ++context.regCount;
    
}

void NDecStmt::CodeGen(CodeContext& context)
{
    std::string varReg = context.var_regID[mName];
    context.opList.emplace_back(std::make_tuple("loadi","%"+std::to_string(context.regCount),varReg,""));
    context.opList.emplace_back(std::make_tuple("dec","%"+std::to_string(context.regCount),"",""));
    context.opList.emplace_back(std::make_tuple("storei",varReg,"%"+std::to_string(context.regCount),""));
    ++context.regCount;
}

void NComparison::CodeGen(CodeContext& context)
{
    std::string firstReg;
    std::string secondReg;
    int firstCount;
    int secondCount;
    firstCount = context.regCount;
    mLhs->CodeGen(context);
    if(firstCount < context.regCount - 1){
        firstCount = context.regCount - 1;
    }
    firstReg = "%" + std::to_string(firstCount);
    secondCount = context.regCount;
    mRhs->CodeGen(context);
    if(secondCount < context.regCount - 1){
        secondCount = context.regCount - 1;
    }
    secondReg = "%" + std::to_string(secondCount);
    if(mType == TLESS){
        context.opList.emplace_back(std::make_tuple("cmplt",firstReg,secondReg,""));
    }else{
        context.opList.emplace_back(std::make_tuple("cmpeq",firstReg,secondReg,""));
    }
}

void NIfStmt::CodeGen(CodeContext& context)
{
    mComp->CodeGen(context);
    std::string jumpAddr;
    int initialPC = context.opList.size();
    context.opList.emplace_back(std::make_tuple("movi","%"+std::to_string(context.regCount),"",""));
    context.opList.emplace_back(std::make_tuple("jnt","%"+std::to_string(context.regCount),"",""));
    ++context.regCount;
    mIfBlock->CodeGen(context);
    jumpAddr = std::to_string(context.opList.size());
    std::get<3>(context.opList[initialPC]) = jumpAddr;
    //else
    if(mElseBlock != nullptr){
    int initialPC2 = context.opList.size();
    context.opList.emplace_back(std::make_tuple("movi","%"+std::to_string(context.regCount),"",""));
    context.opList.emplace_back(std::make_tuple("jmp","%"+std::to_string(context.regCount),"",""));
    jumpAddr = std::to_string(context.opList.size());
    std::get<3>(context.opList[initialPC]) = jumpAddr;
    ++context.regCount;
    mElseBlock->CodeGen(context);
    jumpAddr = std::to_string(context.opList.size());
    std::get<3>(context.opList[initialPC2]) = jumpAddr;
    }
}

void NWhileStmt::CodeGen(CodeContext& context)
{
    int initialPC = context.opList.size();
    mComp->CodeGen(context);
    int jntInd = context.opList.size();
    context.opList.emplace_back(std::make_tuple("movi","%"+std::to_string(context.regCount),"",""));
    context.opList.emplace_back(std::make_tuple("jnt","%"+std::to_string(context.regCount),"",""));
    ++context.regCount;
    mBlock->CodeGen(context);
    context.opList.emplace_back(std::make_tuple("movi","%"+std::to_string(context.regCount),std::to_string(initialPC),""));
    context.opList.emplace_back(std::make_tuple("jmp","%"+std::to_string(context.regCount),"",""));
    std::string firstBranchAdd = std::to_string(context.opList.size());
    std::get<3>(context.opList[jntInd]) = firstBranchAdd;
    ++context.regCount;

}

void NPenUpStmt::CodeGen(CodeContext& context)
{
    context.opList.emplace_back(std::make_tuple("penup","","",""));
}

void NPenDownStmt::CodeGen(CodeContext& context)
{
    context.opList.emplace_back(std::make_tuple("pendown","","",""));
}

void NSetPosStmt::CodeGen(CodeContext& context)
{
    std::string firstReg;
    std::string secondReg;
    int firstCount;
    int secondCount;
    firstCount = context.regCount;
    mXExpr->CodeGen(context);
    if(firstCount < context.regCount - 1){
        firstCount = context.regCount - 1;
    }
    firstReg = "%" + std::to_string(firstCount);
    secondCount = context.regCount;
    mYExpr->CodeGen(context);
    if(secondCount < context.regCount - 1){
        secondCount = context.regCount - 1;
    }
    secondReg = "%" + std::to_string(secondCount);
    context.opList.emplace_back(std::make_tuple("mov","tx",firstReg,""));
    context.opList.emplace_back(std::make_tuple("mov","ty",secondReg,""));

}

void NSetColorStmt::CodeGen(CodeContext& context)
{
    mColor->CodeGen(context);
    context.opList.emplace_back(std::make_tuple("mov","tc","%"+std::to_string(context.regCount-1),""));

}

void NFwdStmt::CodeGen(CodeContext& context)
{
    mParam->CodeGen(context);
    context.opList.emplace_back(std::make_tuple("fwd","%"+std::to_string(context.regCount-1),"",""));

}

void NBackStmt::CodeGen(CodeContext& context)
{
    mParam->CodeGen(context);
    context.opList.emplace_back(std::make_tuple("back","%"+std::to_string(context.regCount-1),"",""));
}

void NRotStmt::CodeGen(CodeContext& context)
{
    mParam->CodeGen(context);
    context.opList.emplace_back(std::make_tuple("add","tr","tr","%"+std::to_string(context.regCount-1)));

}
