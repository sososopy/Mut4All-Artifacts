//header file
#pragma once
#include "Mutator_base.h"

/**
 * divide_by_zero_in_function_615
 */ 
class MutatorFrontendAction_615 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(615)

private:
    class MutatorASTConsumer_615 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_615(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/divide_by_zero_in_function_615.h"

// ========================================================================================================
#define MUT615_OUTPUT 1

void MutatorFrontendAction_615::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("divisionOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;

      // Ensure the operator is a division
      if (BO->getOpcode() == clang::BinaryOperatorKind::BO_Div) {
        // Get the divisor expression
        auto *divisor = BO->getRHS();

        // Check if the divisor is not already zero
        if (auto *intLiteral = llvm::dyn_cast<clang::IntegerLiteral>(divisor)) {
          if (intLiteral->getValue() == 0)
            return; // Already zero, no need to mutate
        }

        // Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(divisor->getSourceRange(), "0");
      }
    }
}
  
void MutatorFrontendAction_615::MutatorASTConsumer_615::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasOperatorName("/")).bind("divisionOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}