//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Template_Parameters_474
 */ 
class MutatorFrontendAction_474 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(474)

private:
    class MutatorASTConsumer_474 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_474(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Lambda_Template_Parameters_474.h"

// ========================================================================================================
#define MUT474_OUTPUT 1

void MutatorFrontendAction_474::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      if (LambdaExpr->getExplicitTemplateParameters() != nullptr) {
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    LambdaExpr->getSourceRange());

        size_t templatePos = lambdaSource.find("<");
        if (templatePos != std::string::npos) {
          lambdaSource.insert(templatePos + 1, "typename Unused, ");
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
        }
      }
    }
}
  
void MutatorFrontendAction_474::MutatorASTConsumer_474::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasExplicitTemplateParameterList()).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}