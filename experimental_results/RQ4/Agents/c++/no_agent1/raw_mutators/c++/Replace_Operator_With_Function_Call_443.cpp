//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_With_Function_Call_443
 */ 
class MutatorFrontendAction_443 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(443)

private:
    class MutatorASTConsumer_443 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_443(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_operator_with_function_call_443.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        // Filter nodes in header files
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(BO->getExprLoc()))
            return;

        // Get the source code text of target node
        std::string lhsText = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
        std::string rhsText = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getRHS()->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
        
        // Perform mutation on the source code text by applying string replacement
        std::string operatorText;
        switch (BO->getOpcode()) {
            case BO_Add: operatorText = "add"; break;
            case BO_Sub: operatorText = "subtract"; break;
            case BO_Mul: operatorText = "multiply"; break;
            case BO_Div: operatorText = "divide"; break;
            default: return; // Only handle basic arithmetic operators
        }
        
        std::string replacementText = operatorText + "(" + lhsText + ", " + rhsText + ")";
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), replacementText);
    }
}

void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = binaryOperator(hasOperatorName("+").bind("BinaryOp"))
                                .bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}