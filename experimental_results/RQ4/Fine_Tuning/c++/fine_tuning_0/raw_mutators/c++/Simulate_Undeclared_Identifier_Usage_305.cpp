//header file
#pragma once
#include "Mutator_base.h"

/**
 * Simulate_Undeclared_Identifier_Usage_305
 */ 
class MutatorFrontendAction_305 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(305)
    
    private:
      class MutatorASTConsumer_305 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_305(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/simulate_undeclared_identifier_usage_305.h"

// ========================================================================================================
#define MUT305_OUTPUT 1

void MutatorFrontendAction_305::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      auto funcname = FD->getNameAsString();
      llvm::outs() << funcname << '\n';
      auto typo = funcname + funcname;
      auto ifstmt = "if constexpr(" + typo + "<>){}";
      ifstmt = "/*mut305*/" + ifstmt;
      Rewrite.ReplaceText(body->getBeginLoc(), 0, ifstmt);
    }
}

void MutatorFrontendAction_305::MutatorASTConsumer_305::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}