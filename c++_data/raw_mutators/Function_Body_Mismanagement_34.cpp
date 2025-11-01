//header file
#pragma once
#include "Mutator_base.h"

/**
 * Function_Body_Mismanagement_34
 */ 
class MutatorFrontendAction_34 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(34)

private:
    class MutatorASTConsumer_34 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_34(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/function_body_mismanagement_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        auto srcText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        std::string mutation = "{\n{}\n" + srcText + "\n}";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), mutation);
      }
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}