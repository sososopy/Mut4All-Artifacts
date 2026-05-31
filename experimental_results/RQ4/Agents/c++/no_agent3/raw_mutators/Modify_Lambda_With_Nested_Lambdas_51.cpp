//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_With_Nested_Lambdas_51
 */ 
class MutatorFrontendAction_51 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(51)

private:
    class MutatorASTConsumer_51 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_51(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *outerVarDecl = nullptr; // Record the outer variable to capture
    };
};

//source file
#include "../include/modify_lambda_with_nested_lambdas_51.h"

// ========================================================================================================
#define MUT51_OUTPUT 1

void MutatorFrontendAction_51::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      // Ensure the lambda is using C++20 features
      if (!LambdaExpr->isGenericLambda())
        return;

      // Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LambdaExpr->getSourceRange());

      // Perform mutation on the source code text by introducing a nested lambda
      if (outerVarDecl) {
        std::string outerVarName = outerVarDecl->getNameAsString();
        std::string nestedLambdaCode = 
            "\n/*mut51*/auto nestedLambda = [&]() { " + outerVarName + " += x; };"
            "\nnestedLambda();\n";

        size_t insertPos = lambdaSource.find("{") + 1;
        lambdaSource.insert(insertPos, nestedLambdaCode);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
      }
    } else if (auto *Var = Result.Nodes.getNodeAs<clang::VarDecl>("outerVar")) {
      // Record the outer variable to capture
      outerVarDecl = Var;
    }
}
  
void MutatorFrontendAction_51::MutatorASTConsumer_51::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto lambdaMatcher = lambdaExpr().bind("lambdaExpr");
    auto outerVarMatcher = varDecl(hasType(isInteger())).bind("outerVar");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(outerVarMatcher, &callback);
    matchFinder.matchAST(Context);
}