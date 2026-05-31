//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Template_Parameter_143
 */ 
class MutatorFrontendAction_143 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(143)
    
    private:
      class MutatorASTConsumer_143 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_143(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/duplicate_template_parameter_143"

// ========================================================================================================
#define MUT143_OUTPUT 1

void MutatorFrontendAction_143::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateDecl>("Template")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), TP->getSourceRange());
    if (auto *FT = dyn_cast<FunctionTemplateDecl>(TP)) {
      auto params = FT->getTemplateParameters();
      auto param =
          stringutils::rangetoStr(*(Result.SourceManager), params->getSourceRange());
      llvm::outs() << param << '\n';
      int choice = getrandom::getRandomIndex(params->size() - 1);
      auto append = stringutils::rangetoStr(
          *(Result.SourceManager), params->getParam(choice)->getSourceRange());
      param += "," + append;
      llvm::outs() << param << '\n';
      content.replace(content.find('<'), content.rfind('>') - content.find('<') + 1,
                      param);
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TP->getSourceRange()), content);
  }
}

void MutatorFrontendAction_143::MutatorASTConsumer_143::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}