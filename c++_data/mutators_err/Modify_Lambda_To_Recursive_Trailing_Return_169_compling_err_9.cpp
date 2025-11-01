//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Modify_Lambda_To_Recursive_Trailing_Return_169
 */ 
class MutatorFrontendAction_169 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(169)

private:
    class MutatorASTConsumer_169 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_169(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_to_recursive_trailing_return_169.h"

// ========================================================================================================
#define MUT169_OUTPUT 1

void MutatorFrontendAction_169::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto sourceRange = LambdaExpr->getSourceRange();
      std::string lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      //Perform mutation on the source code text by applying string replacement
      size_t arrowPos = lambdaSource.find("->");
      if (arrowPos != std::string::npos) {
          size_t bracePos = lambdaSource.find("{", arrowPos);
          if (bracePos != std::string::npos) {
              std::string mutatedLambda = lambdaSource.substr(0, arrowPos) + "-> decltype(lambda(0)) " + lambdaSource.substr(bracePos);
              mutatedLambda.insert(mutatedLambda.find("{") + 1, "if (x > 0) return lambda(x - 1); else ");
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedLambda);
          }
      }
    }
}
  
void MutatorFrontendAction_169::MutatorASTConsumer_169::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(returns(anything())).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}