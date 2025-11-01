//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_
 */ 
class MutatorFrontendAction_{{bugno}} : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER({{bugno}})

private:
    class MutatorASTConsumer_{{bugno}} : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_{{bugno}}(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT{{bugno}}_OUTPUT 1

void MutatorFrontendAction_{{bugno}}::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::{{ASTNodeType}}>({{NodeName}})) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
    }
}
  
void MutatorFrontendAction_{{bugno}}::MutatorASTConsumer_{{bugno}}::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = {{MatchRule}}.bind({{NodeName}});
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}