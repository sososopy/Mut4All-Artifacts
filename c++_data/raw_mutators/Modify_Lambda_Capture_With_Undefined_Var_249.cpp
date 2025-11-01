//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Capture_With_Undefined_Var_249
 */ 
class MutatorFrontendAction_249 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(249)

private:
    class MutatorASTConsumer_249 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_249(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Modify_Lambda_Capture_With_Undefined_Var_249.h"

// ========================================================================================================
#define MUT249_OUTPUT 1

void MutatorFrontendAction_249::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t captureEnd = lambdaSource.find("]") + 1;
      std::string newCapture = "[nonExistentVar, ";
      lambdaSource.insert(captureEnd, newCapture);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_249::MutatorASTConsumer_249::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}