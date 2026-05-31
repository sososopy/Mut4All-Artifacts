//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_variadic_template_pack_20
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(20)

private:
    class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_20(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_variadic_template_pack_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("VariadicFunction")) {
      if (!FDecl || !Result.Context->getSourceManager().isWrittenInMainFile(FDecl->getLocation()))
        return;

      if (FDecl->isVariadic()) {
        auto body = FDecl->getBody();
        if (!body)
          return;

        auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
        size_t pos = functionSource.find("...");

        if (pos != std::string::npos) {
          functionSource.erase(pos, 3);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), functionSource);
        }
      }
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isVariadic()).bind("VariadicFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}