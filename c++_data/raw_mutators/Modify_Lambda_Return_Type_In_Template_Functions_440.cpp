//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Return_Type_In_Template_Functions_440
 */ 
class MutatorFrontendAction_440 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(440)

private:
    class MutatorASTConsumer_440 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_440(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_return_type_in_template_functions_440.h"

// ========================================================================================================
#define MUT440_OUTPUT 1

void MutatorFrontendAction_440::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      // Get the source code text of target node
      auto lambdaSource = Lexer::getSourceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), 
                                               Result.Context->getSourceManager(), 
                                               Result.Context->getLangOpts());

      // Check if the lambda has a return type using decltype
      std::string lambdaStr = lambdaSource.str();
      size_t returnTypePos = lambdaStr.find("-> decltype");
      if (returnTypePos != std::string::npos) {
          // Perform mutation on the source code text by applying string replacement
          lambdaStr.replace(returnTypePos, lambdaStr.find('{', returnTypePos) - returnTypePos, "-> void");

          // Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaStr);
      }
    }
}
  
void MutatorFrontendAction_440::MutatorASTConsumer_440::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasType(templateSpecializationType())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}