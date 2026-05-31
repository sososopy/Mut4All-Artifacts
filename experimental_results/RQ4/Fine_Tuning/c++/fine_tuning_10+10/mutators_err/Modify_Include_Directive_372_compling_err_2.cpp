//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Include_Directive_372
 */ 
class MutatorFrontendAction_372 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(372)

private:
    class MutatorASTConsumer_372 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_372(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_include_directive_372.h"

// ========================================================================================================
#define MUT372_OUTPUT 1

void MutatorFrontendAction_372::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ID = Result.Nodes.getNodeAs<clang::InclusionDirective>("IncludeDirective")) {
      //Filter nodes in header files
      if (!ID || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ID->getLocation()))
        return;
      //Get the source code text of target node
      auto includeText = ID->getSourceRange().getBegin();
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedInclude = "#include <nonexistent_header>";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ID->getSourceRange()), mutatedInclude);
    }
}
  
void MutatorFrontendAction_372::MutatorASTConsumer_372::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = inclusionDirective().bind("IncludeDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}