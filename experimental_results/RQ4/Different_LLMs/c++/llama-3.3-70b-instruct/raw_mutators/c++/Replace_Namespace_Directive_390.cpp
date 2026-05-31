//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Namespace_Directive_390
 */ 
class MutatorFrontendAction_390 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(390)

private:
    class MutatorASTConsumer_390 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_390(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Namespace_Directive_390.h"

// ========================================================================================================
#define MUT390_OUTPUT 1

void MutatorFrontendAction_390::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      //Filter nodes in header files
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   NS->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (NS->isInline()) {
        declaration.replace(declaration.find("inline"), 6, "");
      } else {
        declaration.insert(declaration.find("{") + 1, " inline");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NS->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_390::MutatorASTConsumer_390::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl().bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}