//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Concept_Requires_Clause_128
 */ 
class MutatorFrontendAction_128 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(128)

private:
    class MutatorASTConsumer_128 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_128(Rewriter &R) : TheRewriter(R) {}
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
        const clang::ConceptDecl *target_concept = nullptr;
    };
};

//source file
#include "../include/Apply_Concept_Requires_Clause_128.h"

// ========================================================================================================
#define MUT128_OUTPUT 1

void MutatorFrontendAction_128::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isThisDeclarationADefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("requires") != string::npos) {
        if (target_concept == nullptr)
          return;
        auto concept_name = target_concept->getNameAsString();
        auto pos = content.find("requires");
        if (pos != string::npos) {
          content.replace(pos, content.size() - pos,
                          "requires " + concept_name);
        }
        llvm::outs() << content << '\n';
      } else {
        if (target_concept == nullptr)
          return;
        auto concept_name = target_concept->getNameAsString();
        content.insert(content.rfind('>'), " requires " + concept_name);
        llvm::outs() << content << '\n';
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      target_concept = DL;
    }
}
  
void MutatorFrontendAction_128::MutatorASTConsumer_128::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("FuncTemplates");
    auto concept_matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.matchAST(Context);
}