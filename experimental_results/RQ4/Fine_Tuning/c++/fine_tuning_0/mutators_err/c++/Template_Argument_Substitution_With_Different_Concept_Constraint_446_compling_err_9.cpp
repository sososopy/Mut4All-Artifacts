//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Argument_Substitution_With_Different_Concept_Constraint_446
 */ 
class MutatorFrontendAction_446 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(446)

private:
    class MutatorASTConsumer_446 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_446(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::ConceptDecl *> concepts;
    };
};

//source file
#include "../include/Template_Argument_Substitution_With_Different_Concept_Constraint_446.h"

// ========================================================================================================
#define MUT446_OUTPUT 1

void MutatorFrontendAction_446::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      concepts.push_back(CD);
    } else if (auto *TPL = Result.Nodes.getNodeAs<clang::TemplateDecl>(
                   "Templates")) {
      if (!TPL || !Result.Context->getSourceManager().isWrittenInMainFile(
                      TPL->getLocation()))
        return;
      if (concepts.empty())
        return;
      auto params = TPL->getTemplateParameters();
      for (auto param : *params) {
        if (auto NTTP = dyn_cast<NonTypeTemplateParmDecl>(param)) {
          if (NTTP->hasTypeConstraint()) {
            auto constraint = NTTP->getTypeConstraint();
            auto constraint_name = constraint->getNameInfo().getAsString();
            for (auto concept : concepts) {
              if (concept->getNameAsString() != constraint_name) {
                llvm::outs() << "/*mut446*/" << concept->getNameAsString()
                             << '\n';
                Rewrite.ReplaceText(
                    constraint->getSourceRange(),
                    "/*mut446*/" + concept->getNameAsString());
                break;
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_446::MutatorASTConsumer_446::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto concept_matcher = conceptDecl().bind("Concepts");
    auto template_matcher = templateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}