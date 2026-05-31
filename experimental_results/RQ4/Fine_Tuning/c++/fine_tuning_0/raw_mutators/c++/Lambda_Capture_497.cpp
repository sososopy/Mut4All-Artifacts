//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Capture_497
 */ 
class MutatorFrontendAction_497 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(497)

private:
    class MutatorASTConsumer_497 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_497(Rewriter &R) : TheRewriter(R) {}
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
        const CXXRecordDecl *cur_class;
    };
};

//source file
#include "../include/Lambda_Capture_497.h"

// ========================================================================================================
#define MUT497_OUTPUT 1

void MutatorFrontendAction_497::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_class = DL;
  } else if (auto *LB = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
    if (!LB || !Result.Context->getSourceManager().isWrittenInMainFile(
                   LB->getBeginLoc()))
      return;
    if (!LB->hasExplicitParameters())
      return;
    if (LB->capture_empty())
      return;
    auto captures = LB->captures();
    for (auto capture : captures) {
      if (capture.capturesThis()) {
        auto fields = cur_class->fields();
        if (fields.empty())
          return;
        auto field = fields.front();
        auto fieldname = field->getNameAsString();
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(capture.getSourceRange()),
            fieldname);
        return;
      }
    }
  }
}
  
void MutatorFrontendAction_497::MutatorASTConsumer_497::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto lambda_matcher = lambdaExpr().bind("Lambdas");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(lambda_matcher, &callback);
  matchFinder.matchAST(Context);
}