//header file
#pragma once
#include "Mutator_base.h"

/**
 * incomplete_static_assert_expression_13
 */ 
class MutatorFrontendAction_13 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(13)

private:
    class MutatorASTConsumer_13 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_13(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incomplete_static_assert_expression_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;

      auto messageExpr = SA->getMessage();
      if (auto *SL = llvm::dyn_cast<clang::StringLiteral>(messageExpr)) {
        if (SL->getEndLoc().isValid()) {
          auto range = SL->getSourceRange();
          auto text = Lexer::getSourceText(CharSourceRange::getTokenRange(range), 
                                           Result.Context->getSourceManager(), 
                                           Result.Context->getLangOpts());

          if (!text.empty() && text.back() == '"') {
            text.pop_back(); // remove the closing quotation mark
            text += " /*mut13*/"; // add mutation marker
            Rewrite.ReplaceText(range, text);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}