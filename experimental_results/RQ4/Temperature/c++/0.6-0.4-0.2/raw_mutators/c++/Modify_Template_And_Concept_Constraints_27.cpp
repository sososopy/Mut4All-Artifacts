//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_and_concept_constraints_27
 */ 
class MutatorFrontendAction_27 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(27)

private:
    class MutatorASTConsumer_27 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_27(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_and_concept_constraints_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      auto conceptDef = stringutils::rangetoStr(*(Result.SourceManager),
                                                CD->getSourceRange());
      size_t pos = conceptDef.find(";");
      if (pos != std::string::npos) {
        conceptDef.insert(pos, " && false");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), conceptDef);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      auto templateDef = stringutils::rangetoStr(*(Result.SourceManager),
                                                 TD->getSourceRange());
      size_t pos = templateDef.find(">");
      if (pos != std::string::npos) {
        templateDef.insert(pos, " T");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDef);
    }
}
  
void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto conceptMatcher = conceptDecl().bind("Concepts");
    auto templateMatcher = templateDecl(has(conceptSpecializationExpr())).bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}