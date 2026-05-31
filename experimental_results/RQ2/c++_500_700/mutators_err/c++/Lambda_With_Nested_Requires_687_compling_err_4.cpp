//header file
#pragma once
#include "Mutator_base.h"

/**
 * lambda_with_nested_requires_687
 */ 
class MutatorFrontendAction_687 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(687)

private:
    class MutatorASTConsumer_687 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_687(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/lambda_with_nested_requires_687.h"

// ========================================================================================================
#define MUT687_OUTPUT 1

void MutatorFrontendAction_687::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LambdaExpr->getSourceRange());

      std::string nestedLambda = R"(
        [] {
            return requires(T inner) { { inner } -> doesnt_matter; };
        }();
      )";

      size_t insertPos = lambdaSource.rfind("};");
      if (insertPos != std::string::npos) {
          lambdaSource.insert(insertPos, nestedLambda);
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_687::MutatorASTConsumer_687::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(cxxRequiresExpr().bind("LambdaExpr")));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}