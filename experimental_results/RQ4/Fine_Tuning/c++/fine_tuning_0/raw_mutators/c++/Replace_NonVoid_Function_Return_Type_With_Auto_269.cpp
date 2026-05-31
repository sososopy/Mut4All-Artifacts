//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonVoid_Function_Return_Type_With_Auto_269
 */ 
class MutatorFrontendAction_269 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(269)

private:
    class MutatorASTConsumer_269 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_269(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_NonVoid_Function_Return_Type_With_Auto_269.h"

// ========================================================================================================
#define MUT269_OUTPUT 1

void MutatorFrontendAction_269::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getReturnType()->isVoidType())
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (FD->isDependentContext())
        return;
      if (FD->getReturnType()->isDependentType())
        return;

      auto return_type = stringutils::rangetoStr(
          *(Result.SourceManager), FD->getReturnTypeSourceRange());
      if (return_type == "auto")
        return;
      if (FD->hasTrailingReturn()) {
        auto trailing_return_type = stringutils::rangetoStr(
            *(Result.SourceManager), FD->getTrailingReturnTypeSourceRange());
        if (trailing_return_type == return_type) {
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(
                  FD->getTrailingReturnTypeSourceRange()),
              "/*mut269*/auto");
        }
      } else {
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getReturnTypeSourceRange()),
            "/*mut269*/auto");
      }
    }
}
  
void MutatorFrontendAction_269::MutatorASTConsumer_269::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}