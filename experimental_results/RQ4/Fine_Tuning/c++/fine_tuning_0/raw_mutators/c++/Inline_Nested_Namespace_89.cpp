//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Nested_Namespace_89
 */ 
class MutatorFrontendAction_89 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(89)

private:
    class MutatorASTConsumer_89 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_89(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Inline_Nested_Namespace_89.h"

// ========================================================================================================
#define MUT89_OUTPUT 1

void MutatorFrontendAction_89::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NS")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->isInlineNamespace()) {
        declaration = std::regex_replace(
            declaration, std::regex("inline namespace"), "namespace");
      }
      declaration = std::regex_replace(declaration, std::regex("namespace"),
                                       "inline namespace");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_89::MutatorASTConsumer_89::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl(hasParent(namespaceDecl())).bind("NS");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}