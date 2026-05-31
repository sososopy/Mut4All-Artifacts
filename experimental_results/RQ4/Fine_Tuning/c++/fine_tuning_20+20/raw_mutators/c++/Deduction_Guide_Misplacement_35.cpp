//header file
#pragma once
#include "Mutator_base.h"

/**
 * Deduction_Guide_Misplacement_35
 */ 
class MutatorFrontendAction_35 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(35)

private:
    class MutatorASTConsumer_35 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_35(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *cur_class = nullptr;
    };
};

//source file
#include "../include/Deduction_Guide_Misplacement_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isCompleteDefinition())
        return;
      if (!DL->isStruct() && !DL->isClass())
        return;
      cur_class = DL;
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>(
                       "DeductionGuides")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (cur_class == nullptr)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      llvm::outs() << content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          "");
      Rewrite.ReplaceText(cur_class->getEndLoc(), 0, content);
    }
  }

void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto deduction_guide_matcher = cxxDeductionGuideDecl().bind("DeductionGuides");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(deduction_guide_matcher, &callback);
    matchFinder.matchAST(Context);
}