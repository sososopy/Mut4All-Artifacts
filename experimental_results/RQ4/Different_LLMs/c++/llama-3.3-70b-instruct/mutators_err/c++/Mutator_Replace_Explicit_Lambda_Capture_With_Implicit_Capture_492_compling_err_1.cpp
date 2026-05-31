//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Explicit_Lambda_Capture_With_Implicit_Capture_492
 */ 
class MutatorFrontendAction_492 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(492)

private:
    class MutatorASTConsumer_492 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_492(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Explicit_Lambda_Capture_With_Implicit_Capture_492.h"

// ========================================================================================================
#define MUT492_OUTPUT 1

void MutatorFrontendAction_492::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto capture = L->getCaptureAsWritten();
      std::string captureStr = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(capture->getBeginLoc(), capture->getEndLoc()),
          *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      if (captureStr == "[x]") {
        captureStr = "[=]"; // capture all variables by value
      } else if (captureStr == "[&x]") {
        captureStr = "[&]"; // capture all variables by reference
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(capture->getBeginLoc(), capture->getEndLoc()), captureStr);
    }
}
  
void MutatorFrontendAction_492::MutatorASTConsumer_492::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}