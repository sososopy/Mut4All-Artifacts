//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Auto_Return_Type_In_Lambda_177
 */ 
class MutatorFrontendAction_177 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(177)

private:
    class MutatorASTConsumer_177 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_177(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_auto_return_type_in_lambda_177.h"

// ========================================================================================================
#define MUT177_OUTPUT 1

void MutatorFrontendAction_177::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LambdaExpr->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      // Modify the return type to a specific incompatible type
      std::string newReturnType = "double"; // Chosen to cause type mismatch
      size_t arrowPos = lambdaSource.find("->");
      if (arrowPos != std::string::npos) {
          lambdaSource.replace(arrowPos, lambdaSource.find("{", arrowPos) - arrowPos, "-> " + newReturnType);
      } else {
          lambdaSource.insert(lambdaSource.find("{"), " -> " + newReturnType);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_177::MutatorASTConsumer_177::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasAutoReturnType()).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}