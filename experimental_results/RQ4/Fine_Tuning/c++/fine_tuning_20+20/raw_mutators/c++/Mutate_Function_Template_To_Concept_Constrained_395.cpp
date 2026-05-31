//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Function_Template_To_Concept_Constrained_395
 */ 
class MutatorFrontendAction_395 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(395)

private:
    class MutatorASTConsumer_395 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_395(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl *> cur_concepts;
    };
};

//source file
#include "../include/Mutate_Function_Template_To_Concept_Constrained_395.h"

// ========================================================================================================
#define MUT395_OUTPUT 1

void MutatorFrontendAction_395::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (!cur_concepts.empty()) {
      size_t index = getrandom::getRandomIndex(cur_concepts.size() - 1);
      auto target = cur_concepts[index];
      content.insert(content.find('('), "/*mut395*/ requires " +
                                           target->getNameAsString() + ' ');
    } else {
      content.insert(content.find('('),
                     "/*mut395*/ requires std::integral ");
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        content);
  } else if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                 "Concepts")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
    if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CD->getBeginLoc()))
      return;
    cur_concepts.push_back(CD);
  }
}
  
void MutatorFrontendAction_395::MutatorASTConsumer_395::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("FuncTemplate");
  auto concept_matcher = conceptDecl().bind("Concepts");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(concept_matcher, &callback);
  matchFinder.matchAST(Context);
}