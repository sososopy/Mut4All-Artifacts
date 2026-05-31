//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Class_Template_Partial_Specialization_With_Concept_To_Primary_Template_50
 */ 
class MutatorFrontendAction_50 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(50)
    
    private:
      class MutatorASTConsumer_50 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_50(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<std::string> concept_names;
      };
    };

//source file
#include "../include/Modify_Class_Template_Partial_Specialization_With_Concept_To_Primary_Template_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *CD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
    if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CD->getLocation()))
      return;

    auto specs = CD->specializations();
    for (auto spec : specs) {
      if (spec->isPartialSpecialization() == false)
        continue;
      auto partial = dyn_cast<ClassTemplatePartialSpecializationDecl>(spec);
      auto params = partial->getTemplateParameters();
      for (auto param : *params) {
        if (auto named = dyn_cast<NamedDecl>(param)) {
          if (named->getNameAsString() == "")
            continue;
          for (auto concept_name : concept_names) {
            if (named->getNameAsString() == concept_name) {
              llvm::outs() << "mut50: " << named->getNameAsString() << '\n';
              auto content = stringutils::rangetoStr(
                  *(Result.SourceManager), partial->getSourceRange());
              content = "/*mut50*/" + content;
              Rewrite.ReplaceText(
                  CharSourceRange::getTokenRange(partial->getSourceRange()),
                  content);
            }
          }
        }
      }
    }
  } else if (auto *CT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
    if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CT->getLocation()))
      return;
    concept_names.push_back(CT->getNameAsString());
  }
}

void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto concept_matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.matchAST(Context);
}