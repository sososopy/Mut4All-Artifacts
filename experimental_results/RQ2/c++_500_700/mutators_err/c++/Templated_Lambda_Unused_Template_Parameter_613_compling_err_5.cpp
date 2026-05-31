//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ExprCXX.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Tooling.h"

/**
 * templated_lambda_unused_template_parameter_613
 */ 
class MutatorFrontendAction_613 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(613)

private:
    class MutatorASTConsumer_613 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_613(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/templated_lambda_unused_template_parameter_613.h"

// ========================================================================================================
#define MUT613_OUTPUT 1

void MutatorFrontendAction_613::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      if (LambdaExpr->hasTemplateParameters()) {
        const auto *TemplateParams = LambdaExpr->getTemplateParameterList();
        if (TemplateParams && TemplateParams->size() > 0) {
          std::string newTemplateParams = "<";
          for (unsigned i = 0; i < TemplateParams->size(); ++i) {
            if (i > 0) newTemplateParams += ", ";
            newTemplateParams += TemplateParams->getParam(i)->getNameAsString();
          }
          newTemplateParams += ", typename Unused>";
          
          auto lambdaSourceRange = LambdaExpr->getSourceRange();
          std::string lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), lambdaSourceRange);
          size_t pos = lambdaSource.find('>');
          if (pos != std::string::npos) {
            lambdaSource.replace(1, pos, newTemplateParams);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaSourceRange), lambdaSource);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_613::MutatorASTConsumer_613::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasTemplateParameters()).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}