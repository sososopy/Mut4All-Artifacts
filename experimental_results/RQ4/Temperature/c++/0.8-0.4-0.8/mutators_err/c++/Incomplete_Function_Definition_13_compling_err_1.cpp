//header file
#pragma once
#include "Mutator_base.h"

/**
 * incomplete_function_definition_13
 */ 
class MutatorFrontendAction_13 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(13)

private:
    class MutatorASTConsumer_13 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_13(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incomplete_function_definition_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        auto bodySourceRange = body->getSourceRange();
        //Perform mutation on the source code text by applying string replacement
        if (body->size() > 0) {
          auto endLoc = body->getEndLoc();
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(endLoc, 1, "/*mut13*/");
        }
      }
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}