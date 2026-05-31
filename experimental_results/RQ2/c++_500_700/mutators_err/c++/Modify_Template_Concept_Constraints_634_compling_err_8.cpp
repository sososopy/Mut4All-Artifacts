//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_template_concept_constraints_634
 */ 
class MutatorFrontendAction_634 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(634)

private:
    class MutatorASTConsumer_634 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_634(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_concept_constraints_634.h"

// ========================================================================================================
#define MUT634_OUTPUT 1

void MutatorFrontendAction_634::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      auto conceptSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                   CT->getSourceRange());

      // Perform mutation on the source code text by applying string replacement
      size_t requiresPos = conceptSource.find("requires");
      if (requiresPos != std::string::npos) {
          size_t endPos = conceptSource.find(";", requiresPos);
          if (endPos != std::string::npos) {
              std::string originalConstraint = conceptSource.substr(requiresPos, endPos - requiresPos);
              std::string newConstraint = "requires { Q.template operator()<float>(); };";
              conceptSource.replace(requiresPos, endPos - requiresPos, newConstraint);
          }
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()), conceptSource);
    }
}
  
void MutatorFrontendAction_634::MutatorASTConsumer_634::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}