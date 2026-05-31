//header file
#pragma once
#include "Mutator_base.h"

/**
 * coroutine_with_vla_696
 */ 
class MutatorFrontendAction_696 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(696)

private:
    class MutatorASTConsumer_696 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_696(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/coroutine_with_vla_696.h"

// ========================================================================================================
#define MUT696_OUTPUT 1

void MutatorFrontendAction_696::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("CoroutineFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isCoroutine()) {
        for (auto *Stmt : FD->getBody()->children()) {
          if (auto *DS = llvm::dyn_cast<clang::DeclStmt>(Stmt)) {
            for (auto *D : DS->decls()) {
              if (auto *VD = llvm::dyn_cast<clang::VarDecl>(D)) {
                if (VD->getType()->isVariableArrayType()) {
                  auto sizeExpr = VD->getInit();
                  if (sizeExpr) {
                    std::string newSizeExpr = "/*mut696*/(" + 
                      clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(sizeExpr->getSourceRange()), 
                                                  *Result.SourceManager, 
                                                  Result.Context->getLangOpts()).str() + 
                      " * 2 + 1)";
                    Rewrite.ReplaceText(sizeExpr->getSourceRange(), newSizeExpr);
                  }
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_696::MutatorASTConsumer_696::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isCoroutine()).bind("CoroutineFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}