//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Trailing_Return_Type_Mutation_To_Function_Pointer_Type_199
 */ 
class MutatorFrontendAction_199 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(199)

private:
    class MutatorASTConsumer_199 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_199(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::FunctionDecl*> candidateFunctions;
    };
};

//source file
#include "../include/Mutator_Lambda_Trailing_Return_Type_Mutation_To_Function_Pointer_Type_199.h"

// ========================================================================================================
#define MUT199_OUTPUT 1

void MutatorFrontendAction_199::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      //Check if lambda has explicit trailing return type
      if (!MT->hasExplicitResultType()) return;
      
      //Get original return type
      auto originalReturnType = MT->getReturnType();
      //Check if original return type is already function or pointer-to-function type
      if (originalReturnType->isFunctionType() || originalReturnType->isPointerType() && originalReturnType->getPointeeType()->isFunctionType()) return;
      
      //Find candidate function for constructing function pointer type
      if (candidateFunctions.empty()) return;
      
      //Select a candidate function (first non-template, non-member function)
      const clang::FunctionDecl* targetFunc = nullptr;
      for (auto func : candidateFunctions) {
        if (!func->isTemplateInstantiation() && !func->getDeclContext()->isRecord()) {
          targetFunc = func;
          break;
        }
      }
      if (!targetFunc) return;
      
      //Construct function pointer type string
      std::string funcPtrType;
      auto funcReturnType = targetFunc->getReturnType().getAsString();
      auto funcParams = targetFunc->parameters();
      std::string paramList;
      for (unsigned i = 0; i < funcParams.size(); ++i) {
        if (i != 0) paramList += ", ";
        paramList += funcParams[i]->getType().getAsString();
      }
      funcPtrType = funcReturnType + " (*)(" + paramList + ")";
      
      //Find the position of trailing return type in lambda text
      size_t arrowPos = lambdaText.find("->");
      if (arrowPos == std::string::npos) return;
      
      //Find the end of return type (before '{' or end of lambda)
      size_t returnTypeEnd = lambdaText.find('{', arrowPos);
      if (returnTypeEnd