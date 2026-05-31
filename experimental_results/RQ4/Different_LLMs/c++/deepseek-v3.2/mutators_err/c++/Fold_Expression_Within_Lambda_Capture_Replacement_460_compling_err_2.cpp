//header file
#pragma once
#include "Mutator_base.h"

/**
 * Fold_Expression_Within_Lambda_Capture_Replacement_460
 */ 
class MutatorFrontendAction_460 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(460)

private:
    class MutatorASTConsumer_460 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_460(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Fold_Expression_Within_Lambda_Capture_Replacement_460.h"

// ========================================================================================================
#define MUT460_OUTPUT 1

void MutatorFrontendAction_460::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Check if lambda has a capture list containing a fold expression
      auto capture = MT->captures();
      if (!capture.empty()) {
        for (auto cap : capture) {
          if (cap.capturesVariable() && cap.getCapturedVar()->getType()->isTemplateType()) {
            //Find fold expression within the lambda body
            auto body = MT->getBody();
            if (!body) return;
            //Search for fold expression nodes within the body
            //We need to traverse the body's AST to find fold expressions
            //For simplicity, we assume the fold expression is directly in the return statement
            //This is a simplified approach; a more robust implementation would use an AST matcher for fold expressions inside the lambda
            //Perform mutation on the source code text by applying string replacement
            //Identify fold expression pattern and replace it
            //Example: change (...,) to (+ ... +) or vice versa
            //We need to locate the fold expression substring within lambdaText
            size_t foldStart = lambdaText.find("...,");
            if (foldStart != std::string::npos) {
              //Mutate to binary left fold with plus operator
              std::string mutated = lambdaText.substr(0, foldStart) + "(0 + ... + [a]{})" + lambdaText.substr(foldStart + 5);
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutated);
            } else {
              foldStart = lambdaText.find("... +");
              if (foldStart != std::string::npos) {
                //Mutate to unary right fold with comma operator
                std::string mutated = lambdaText.substr(0, foldStart) + "([a]{}, ..., 0)" + lambdaText.substr(foldStart + 5);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutated);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_460::MutatorASTConsumer_460::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}