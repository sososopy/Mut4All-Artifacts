//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Body_With_Throw_93
 */ 

class MutatorFrontendAction_93 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(93)
private:
    class MutatorASTConsumer_93 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_93(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_function_body_with_throw_93.h"

// ========================================================================================================
#define MUT93_OUTPUT 1

void MutatorFrontendAction_93::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto bodyRange = FD->getBody()->getSourceRange();
        std::string throwStatement = "{ throw std::runtime_error(\"mutated\"); }";
        //Perform mutation on the source code text by applying string replacement
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), throwStatement);
      }
    }
}
  
void MutatorFrontendAction_93::MutatorASTConsumer_93::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition(), unless(isMain())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}