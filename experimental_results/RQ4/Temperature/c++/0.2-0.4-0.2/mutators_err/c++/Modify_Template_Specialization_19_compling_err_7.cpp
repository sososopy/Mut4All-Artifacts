//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <set>
#include <string>

/**
 * modify_template_specialization_19
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)

private:
    class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> existingSpecializations;
    };
};

//source file
#include "../include/modify_template_specialization_19.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("PartialSpec")) {
      if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(CTSD->getLocation()))
        return;

      auto name = CTSD->getNameAsString();
      if (existingSpecializations.find(name) != existingSpecializations.end()) {
          return;
      }

      existingSpecializations.insert(name);

      std::string specializationText = "template<typename T> class " + name + "<T, int> {}; /*mut19*/\n";
      Rewrite.InsertTextAfterToken(CTSD->getEndLoc(), specializationText);
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(
        hasAnyTemplateArgument(matchesName("int"))
    ).bind("PartialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}