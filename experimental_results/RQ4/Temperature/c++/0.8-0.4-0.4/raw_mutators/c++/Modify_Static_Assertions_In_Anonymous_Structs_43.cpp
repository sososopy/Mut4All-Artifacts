//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_static_assertions_in_anonymous_structs_43
 */ 
class MutatorFrontendAction_43 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(43)

private:
    class MutatorASTConsumer_43 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_43(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_static_assertions_in_anonymous_structs_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *StaticAssert = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      if (!StaticAssert || !Result.Context->getSourceManager().isWrittenInMainFile(
                     StaticAssert->getLocation()))
        return;

      auto originalExpr = StaticAssert->getAssertExpr();
      if (!originalExpr)
        return;

      auto originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(originalExpr->getSourceRange()), 
                                               *Result.SourceManager, 
                                               Result.Context->getLangOpts());

      std::string mutatedText = "sizeof(void)";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(originalExpr->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = staticAssertDecl(hasAncestor(recordDecl(isAnonymousStructOrUnion()))).bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}