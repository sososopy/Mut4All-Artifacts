//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_With_Function_Call_464
 */ 

class MutatorFrontendAction_464 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(464)
private:
    class MutatorASTConsumer_464 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_464(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::string> operatorToFunctionMap;
    };
};

//source file
#include "../include/replace_operator_with_function_call_464.h"

// ========================================================================================================
#define MUT464_OUTPUT 1

void MutatorFrontendAction_464::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getOperatorLoc()))
            return;

        std::string opText = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getOperatorLoc(), BO->getOperatorLoc()), 
                                                  *Result.SourceManager, 
                                                  Result.Context->getLangOpts()).str();
        
        if (operatorToFunctionMap.find(opText) == operatorToFunctionMap.end()) {
            operatorToFunctionMap["+"] = "add";
            operatorToFunctionMap["-"] = "subtract";
            operatorToFunctionMap["*"] = "multiply";
            operatorToFunctionMap["/"] = "divide";
        }

        if (operatorToFunctionMap.find(opText) != operatorToFunctionMap.end()) {
            std::string lhs = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()), 
                                                   *Result.SourceManager, 
                                                   Result.Context->getLangOpts()).str();
            std::string rhs = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getRHS()->getSourceRange()), 
                                                   *Result.SourceManager, 
                                                   Result.Context->getLangOpts()).str();
            std::string functionCall = operatorToFunctionMap[opText] + "(" + lhs + ", " + rhs + ")";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), functionCall);
        }
    }
}

void MutatorFrontendAction_464::MutatorASTConsumer_464::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = binaryOperator(isExpansionInMainFile()).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}