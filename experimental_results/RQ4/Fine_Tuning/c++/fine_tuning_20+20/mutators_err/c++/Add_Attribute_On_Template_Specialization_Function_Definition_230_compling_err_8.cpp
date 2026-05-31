//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Attribute_On_Template_Specialization_Function_Definition_230
 */ 
class MutatorFrontendAction_230 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(230)
    
    private:
      class MutatorASTConsumer_230 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_230(Rewriter &R) : TheRewriter(R) {}
          void HandleTranslationUnit(ASTContext &Context) override;
        private:
          Rewriter &TheRewriter;
      };
      
      class Callback : public MatchFinder::MatchCallback {
        public:
          Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
          void run(const MatchFinder::MatchResult &Result) override;
        private:
          Rewriter &Rewrite;
      };
    };

//source file
#include "../include/Add_Attribute_On_Template_Specialization_Function_Definition_230.h"

// ========================================================================================================
#define MUT230_OUTPUT 1

void MutatorFrontendAction_230::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Specialization")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isFunctionTemplateSpecialization())
        return;
      auto attr = getrandom::getRandomAttributeForFunction();
      llvm::outs() << "mut230: " << attr << '\n';
      Rewrite.ReplaceText(FD->getBeginLoc(), 0, "/*mut230*/[[" + attr + "]] ");
    }
  }

void MutatorFrontendAction_230::MutatorASTConsumer_230::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Specialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}