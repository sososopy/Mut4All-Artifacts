//header file
#pragma once
#include "Mutator_base.h"

/**
 * Attribute_Remover_434
 */ 
class MutatorFrontendAction_434 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(434)

private:
    class MutatorASTConsumer_434 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_434(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Attribute_Remover_434.h"

// ========================================================================================================
#define ATTRREM_434_OUTPUT 1

void MutatorFrontendAction_434::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Attr = Result.Nodes.getNodeAs<clang::Attr>(("Attribute"))) {
      //Filter nodes in header files
      if (!Attr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Attr->getLocStart()))
        return;
      //Get the source code text of target node
      auto AttrRange = Attr->getLocStart();
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.RemoveText(AttrRange, Attr->getLocEnd());
    }
}

void MutatorFrontendAction_434::MutatorASTConsumer_434::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::attr().bind("Attribute");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}