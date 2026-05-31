//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Return_Type_Decltype_177
 */ 
class MutatorFrontendAction_177 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(177)

private:
    class MutatorASTConsumer_177 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_177(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_177.h"

// ========================================================================================================
#define MUT177_OUTPUT 1

void MutatorFrontendAction_177::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaExpr = MT;
      auto callOperator = lambdaExpr->getCallOperator();
      if (!callOperator)
        return;
      auto returnType = callOperator->getReturnType();
      if (returnType.isNull())
        return;
      auto lambdaClass = lambdaExpr->getLambdaClass();
      if (!lambdaClass)
        return;
      auto captures = lambdaExpr->captures();
      bool hasCapture = false;
      for (auto capture : captures) {
        if (capture.capturesVariable() || capture.capturesVariableByCopy()) {
          hasCapture = true;
          break;
        }
      }
      if (!hasCapture)
        return;
      auto returnTypeSourceRange = callOperator->getReturnTypeSourceRange();
      if (returnTypeSourceRange.isInvalid())
        return;
      SourceManager &SM = Rewrite.getSourceMgr();
      const LangOptions &LO = SM.getLangOpts();
      std::string returnTypeStr = stringutils::rangetoStr(SM, returnTypeSourceRange);
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedReturnType = "decltype(" + returnTypeStr + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(returnTypeSourceRange, mutatedReturnType);
    }
}
  
void MutatorFrontendAction_177::MutatorASTConsumer_177::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}