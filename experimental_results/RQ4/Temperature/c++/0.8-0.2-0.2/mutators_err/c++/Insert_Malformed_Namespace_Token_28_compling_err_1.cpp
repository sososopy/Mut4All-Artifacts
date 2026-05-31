//header file
#pragma once
#include "Mutator_base.h"

/**
 * insert_malformed_namespace_token_28
 */ 
class MutatorFrontendAction_28 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(28)

private:
    class MutatorASTConsumer_28 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_28(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ND = Result.Nodes.getNodeAs<clang::NamespaceDecl>("namespaceDecl")) {
      if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(ND->getLocation()))
        return;

      if (ND->decls_begin() == ND->decls_end())
        return;

      auto namespaceText = stringutils::rangetoStr(*(Result.SourceManager), ND->getSourceRange());

      size_t bracePos = namespaceText.find("{");
      if (bracePos != std::string::npos) {
        namespaceText.insert(bracePos + 1, ";");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ND->getSourceRange()), namespaceText);
    }
}
  
void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl().bind("namespaceDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}