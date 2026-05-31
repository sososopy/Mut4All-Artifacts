```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_expression_with_invalid_decltype_51
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
    };
};

//source file
#include "../include/modify_lambda_expression_with_invalid_decltype_51.h"

// ========================================================================================================
#define MUT51_OUTPUT 1

void MutatorFrontendAction_51::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        // Filter nodes in header files
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
            LambdaExpr->getBeginLoc()))
            return;

        const auto *LambdaType = LambdaExpr->getLambdaClass()->getLambdaTypeInfo();
        if (!LambdaType || !LambdaType->getReturnType()->isDependentType())
            return;

        // Get the source code text of target node
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    LambdaExpr->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        size_t decltypePos = lambdaSource.find("decltype");
        if (decltypePos != std::string::npos) {
            size_t endPos = lambdaSource.find(')', decltypePos);
            if (endPos != std::string::npos) {
                lambdaSource.replace(decltypePos, endPos - decltypePos + 1, "decltype(nonExistentVar)");
            }
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}

void MutatorFrontendAction_51::MutatorASTConsumer_51::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasType(lambdaType(hasReturnType(decltypeType())))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```