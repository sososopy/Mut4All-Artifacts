//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * insert_malformed_namespace_token_28
 */ 
class MutatorFrontendAction_28 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(28)

private:
    class MutatorASTConsumer_28 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_28(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
    private:
        clang::Rewriter &Rewrite;
    };
};

//source file
#include "MutatorFrontendAction_28.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ND = Result.Nodes.getNodeAs<NamespaceDecl>("namespaceDecl")) {
      if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(ND->getLocation()))
        return;

      if (ND->decls_begin() == ND->decls_end())
        return;

      auto namespaceText = Lexer::getSourceText(CharSourceRange::getTokenRange(ND->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());

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