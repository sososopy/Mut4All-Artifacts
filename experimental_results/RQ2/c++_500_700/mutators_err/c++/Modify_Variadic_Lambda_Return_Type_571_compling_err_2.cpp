//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_variadic_lambda_return_type_571
 */ 
class MutatorFrontendAction_571 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(571)

private:
    class MutatorASTConsumer_571 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_571(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/modify_variadic_lambda_return_type_571.h"

// ========================================================================================================
#define MUT571_OUTPUT 1

void MutatorFrontendAction_571::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("VariadicLambda")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LambdaExpr->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string modifiedLambda = lambdaSource;
      size_t pos = modifiedLambda.find("->");
      if (pos == std::string::npos) {
          pos = modifiedLambda.find("{");
          modifiedLambda.insert(pos, " -> decltype(lambda(args...)) ");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), modifiedLambda);
    }
}
  
void MutatorFrontendAction_571::MutatorASTConsumer_571::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasAnyCapture()).bind("VariadicLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}