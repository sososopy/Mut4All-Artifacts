//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Return_Type_Of_Lambda_Expressions_4
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
#include "../include/modify_return_type_of_lambda_expressions_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      const auto *LambdaType = LambdaExpr->getCallOperator()->getType()->getAs<FunctionProtoType>();
      if (!LambdaType)
        return;

      // Get the original return type
      QualType OriginalReturnType = LambdaType->getReturnType();
      std::string OriginalReturnTypeStr = OriginalReturnType.getAsString();

      // Construct the new return type
      std::string NewReturnTypeStr = OriginalReturnTypeStr + "(" + OriginalReturnTypeStr + ")";

      // Get the source code text of target node
      auto LambdaRange = LambdaExpr->getSourceRange();
      std::string LambdaText = Lexer::getSourceText(CharSourceRange::getTokenRange(LambdaRange), 
                                                    Result.Context->getSourceManager(), 
                                                    Result.Context->getLangOpts()).str();

      // Perform mutation on the source code text by applying string replacement
      size_t returnTypePos = LambdaText.find("->");
      if (returnTypePos != std::string::npos) {
        size_t endPos = LambdaText.find("{", returnTypePos);
        if (endPos != std::string::npos) {
          LambdaText.replace(returnTypePos + 2, endPos - returnTypePos - 2, NewReturnTypeStr);
        }
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(LambdaRange, LambdaText);
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasAncestor(callExpr(callee(functionDecl(hasName("std::for_each")))))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}