//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_instantiation_with_dependent_name_38
 */ 
class MutatorFrontendAction_38 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)

private:
    class MutatorASTConsumer_38 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_instantiation_with_dependent_name_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::DependentScopeDeclRefExpr>("templateInst")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      auto sourceText = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()),
          *Result.SourceManager, Result.Context->getLangOpts());

      std::string mutatedText = sourceText.str();
      size_t pos = mutatedText.find("template ");
      if (pos != std::string::npos) {
          mutatedText.erase(pos, 9); // Remove 'template' keyword
      }
      
      Rewrite.ReplaceText(MT->getSourceRange(), mutatedText);
    }
}
  
void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = dependentScopeDeclRefExpr(hasAncestor(functionTemplateDecl())).bind("templateInst");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}