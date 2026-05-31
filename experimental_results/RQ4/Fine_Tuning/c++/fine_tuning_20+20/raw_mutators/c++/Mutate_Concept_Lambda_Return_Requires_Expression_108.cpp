//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Concept_Lambda_Return_Requires_Expression_108
 */ 
class MutatorFrontendAction_108 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(108)

private:
    class MutatorASTConsumer_108 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_108(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Concept_Lambda_Return_Requires_Expression_108.h"

// ========================================================================================================
#define MUT108_OUTPUT 1

void MutatorFrontendAction_108::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.rfind("}") == string::npos)
        return;
      content.insert(content.rfind("}"), "&& true");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}

void MutatorFrontendAction_108::MutatorASTConsumer_108::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(returnStmt(has(ignoringImpCasts(
                              requiresExpr().bind("Requires"))))))
                       .bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}