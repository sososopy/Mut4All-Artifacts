//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_with_undeclared_template_683
 */ 
class MutatorFrontendAction_683 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(683)

private:
    class MutatorASTConsumer_683 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_683(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_with_undeclared_template_683.h"

// ========================================================================================================
#define MUT683_OUTPUT 1

void MutatorFrontendAction_683::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunction")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        auto lambdaSourceRange = LE->getSourceRange();
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), lambdaSourceRange);

        size_t insertPos = lambdaSource.find("{");
        if (insertPos != std::string::npos) {
          std::string mutation = "\nArgs someVariable;\n";
          lambdaSource.insert(insertPos + 1, mutation);

          Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaSourceRange), lambdaSource);
        }
      }
    }
}
  
void MutatorFrontendAction_683::MutatorASTConsumer_683::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(
                      isTemplateInstantiation(),
                      hasDescendant(lambdaExpr().bind("lambdaExpr"))
                   ).bind("templateFunction");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}