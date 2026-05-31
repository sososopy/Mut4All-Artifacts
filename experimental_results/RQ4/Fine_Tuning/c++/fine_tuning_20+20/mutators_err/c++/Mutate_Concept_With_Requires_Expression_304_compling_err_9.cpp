//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Concept_With_Requires_Expression_304
 */ 
class MutatorFrontendAction_304 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(304)

private:
    class MutatorASTConsumer_304 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_304(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl *> candidates;
    };
};

//source file
#include "../include/Mutate_Concept_With_Requires_Expression_304.h"

// ========================================================================================================
#define MUT304_OUTPUT 1

void MutatorFrontendAction_304::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      if (CT->getTemplateParameters()->size() == 0)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CT->getSourceRange());
      if (content.find("requires") == string::npos)
        return;
      candidates.push_back(CT);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TUD")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (candidates.empty())
        return;
      size_t index = getrandom::getRandomIndex(candidates.size() - 1);
      auto target = candidates[index];
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             target->getSourceRange());
      if (content.find("requires") == string::npos)
        return;
      auto req_start = content.find("requires");
      auto req_end = content.rfind('}');
      if (req_end == string::npos)
        return;
      auto req_content = content.substr(req_start, req_end - req_start);
      if (req_content.find(';') != string::npos) {
        auto reqs = llvm::split(req_content, ';');
        size_t req_index = getrandom::getRandomIndex(
            std::distance(reqs.begin(), reqs.end()) - 1);
        auto it = reqs.begin();
        std::advance(it, req_index);
        *it = "f(t)";
        req_content = llvm::join(reqs.begin(), reqs.end(), ";");
      }
      content = content.substr(0, req_start) + req_content +
                content.substr(req_end, content.size() - req_end);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(target->getSourceRange()), content);
      Rewrite.ReplaceText(DL->getEndLoc(), 0,
                          "/*mut304*/auto f(C auto t){return t;}\n");
    }
}
  
void MutatorFrontendAction_304::MutatorASTConsumer_304::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::conceptDecl().bind("Concepts");
    auto translationunit_matcher = translationUnitDecl().bind("TUD");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(translationunit_matcher, &callback);
    matchFinder.matchAST(Context);
}