//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_return_type_4
 */ 
class MutatorFrontendAction_4 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)

private:
    class MutatorASTConsumer_4 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_return_type_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      const TypeSourceInfo *TSI = LE->getLambdaClass()->getTypeSourceInfo();
      if (!TSI)
        return;
      
      auto returnTypeRange = TSI->getTypeLoc().getSourceRange();
      std::string modifiedReturnType = "int(int)";
      
      //Perform mutation on the source code text by applying string replacement
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(returnTypeRange), modifiedReturnType);
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}