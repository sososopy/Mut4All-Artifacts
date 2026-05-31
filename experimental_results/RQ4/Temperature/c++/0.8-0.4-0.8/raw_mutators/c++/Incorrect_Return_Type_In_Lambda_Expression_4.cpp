//header file
#pragma once
#include "Mutator_base.h"

/**
 * incorrect_return_type_in_lambda_expression_4
 */ 
class MutatorFrontendAction_4 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)

private:
    class MutatorASTConsumer_4 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incorrect_return_type_in_lambda_expression_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      auto *LambdaDecl = LambdaExpr->getLambdaClass();
      if (!LambdaDecl)
        return;

      const auto *Method = LambdaDecl->getLambdaCallOperator();
      if (!Method)
        return;

      auto ReturnType = Method->getReturnType();
      std::string OriginalReturnType = ReturnType.getAsString();

      std::string MutatedReturnType = "int(int)";
      if (ReturnType->isFunctionType()) {
        MutatedReturnType = "void()";
      }

      auto ReturnTypeLoc = Method->getReturnTypeSourceRange();
      if (ReturnTypeLoc.isValid()) {
        Rewrite.ReplaceText(ReturnTypeLoc, MutatedReturnType);
      } else {
        auto Loc = LambdaExpr->getBeginLoc();
        Rewrite.InsertTextAfterToken(Loc, " -> " + MutatedReturnType);
      }
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}