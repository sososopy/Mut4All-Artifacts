//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_With_Recursive_Call_381
 */ 
class MutatorFrontendAction_381 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(381)

private:
    class MutatorASTConsumer_381 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_381(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> targetFunctions;
    };
};

//source file
#include "../include/replace_function_with_recursive_call_381.h"

// ========================================================================================================
#define MUT381_OUTPUT 1

void MutatorFrontendAction_381::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (!FD->hasBody() || FD->isMain())
            return;
        
        // Record the function for potential mutation
        targetFunctions.push_back(FD);
    } else if (auto *CS = Result.Nodes.getNodeAs<clang::CallExpr>("CallSites")) {
        if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CS->getBeginLoc()))
            return;

        for (const auto *FD : targetFunctions) {
            if (CS->getDirectCallee() == FD) {
                // Get the source code text of target node
                auto callText = stringutils::rangetoStr(*(Result.SourceManager), CS->getSourceRange());
                // Perform mutation on the source code text by applying string replacement
                callText = FD->getNameAsString() + "(" + callText + ")";
                // Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CS->getSourceRange()), callText);
                break;
            }
        }
    }
}

void MutatorFrontendAction_381::MutatorASTConsumer_381::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto functionMatcher = functionDecl(isDefinition()).bind("Functions");
    auto callExprMatcher = callExpr().bind("CallSites");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.matchAST(Context);
}