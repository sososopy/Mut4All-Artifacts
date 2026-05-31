//header file
#pragma once
#include "Mutator_base.h"

/**
 * misplaced_semicolon_in_nested_namespaces_28
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
#include "../include/misplaced_semicolon_in_nested_namespaces_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;

      if (NS->isAnonymousNamespace() || NS->isInline())
        return;

      auto startLoc = NS->getBeginLoc();
      auto endLoc = NS->getRBraceLoc();
      if (startLoc.isInvalid() || endLoc.isInvalid())
        return;

      auto sourceRange = clang::SourceRange(startLoc, endLoc);
      auto text = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      size_t pos = text.find('{');
      if (pos != std::string::npos) {
        text.insert(pos, ";");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), text);
      }
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(has(namespaceDecl())).bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}