//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_With_Function_Call_327
 */ 
class MutatorFrontendAction_327 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(327)
private:
    class MutatorASTConsumer_327 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_327(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_operator_with_function_call_327.h"

// ========================================================================================================
#define MUT327_OUTPUT 1

void MutatorFrontendAction_327::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getExprLoc()))
            return;

        auto lhsText = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()), 
                                            *Result.SourceManager, 
                                            Result.Context->getLangOpts());
        auto rhsText = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getRHS()->getSourceRange()), 
                                            *Result.SourceManager, 
                                            Result.Context->getLangOpts());

        std::string opText = BO->getOpcodeStr().str();
        std::string replacement = "/*mut327*/performOperation(" + lhsText.str() + ", \"" + opText + "\", " + rhsText.str() + ")";
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), replacement);
    }
}

void MutatorFrontendAction_327::MutatorASTConsumer_327::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = binaryOperator(isExpansionInMainFile()).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}