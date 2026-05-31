//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Return_Type_199
 */ 
class MutatorFrontendAction_199 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(199)

private:
    class MutatorASTConsumer_199 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_199(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Lambda_Return_Type_199.h"

// ========================================================================================================
#define MUT199_OUTPUT 1

void MutatorFrontendAction_199::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!Lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Lambda->getBeginLoc()))
        return;

      // Check if lambda has an explicit return type
      if (Lambda->hasExplicitResultType()) {
        //Get the source code text of target node
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    Lambda->getSourceRange());

        // Find the position of the return type in the lambda expression
        size_t arrowPos = lambdaSource.find("->");
        if (arrowPos != std::string::npos) {
          //Perform mutation on the source code text by applying string replacement
          std::string mutatedLambda = lambdaSource.substr(0, arrowPos + 2) + " non_existent_type" + 
                                      lambdaSource.substr(lambdaSource.find('{'));
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(Lambda->getSourceRange()), mutatedLambda);
        }
      }
    }
}
  
void MutatorFrontendAction_199::MutatorASTConsumer_199::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasExplicitResultType()).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}