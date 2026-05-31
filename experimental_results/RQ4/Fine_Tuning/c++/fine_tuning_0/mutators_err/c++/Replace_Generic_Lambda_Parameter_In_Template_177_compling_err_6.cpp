//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Generic_Lambda_Parameter_In_Template_177
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
#include "../include/Replace_Generic_Lambda_Parameter_In_Template_177.h"

// ========================================================================================================
#define MUT177_OUTPUT 1

void MutatorFrontendAction_177::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      if (!MT->isGenericLambda())
        return;
      auto DL = MT->getLambdaClass();
      if (DL->isInStdNamespace())
        return;
      if (!DL->isCompleteDefinition())
        return;
      auto PL = MT->getLambdaClass()->getLambdaCallOperator()->parameters();
      if (PL.size() == 0)
        return;
      //Get the source code text of target node
      auto PT = PL[0]->getType();
      if (!PT->isDependentType())
        return;
      auto param =
          stringutils::rangetoStr(*(Result.SourceManager), PL[0]->getSourceRange());
      auto lambda =
          stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      string replace_type = "int";
      string replace_lambda = stringutils::strReplace(lambda, param, replace_type);
      llvm::outs() << replace_lambda;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replace_lambda);
    }
}
  
void MutatorFrontendAction_177::MutatorASTConsumer_177::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}