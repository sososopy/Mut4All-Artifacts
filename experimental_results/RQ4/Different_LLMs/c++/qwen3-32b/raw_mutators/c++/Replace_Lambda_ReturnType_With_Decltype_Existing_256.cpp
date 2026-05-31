//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_ReturnType_With_Decltype_Existing_256
 */ 

class MutatorFrontendAction_256 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(256)
private:
    class MutatorASTConsumer_256 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_256(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamedDecl*> symbols; // Store collected symbols
    };
};

//source file
#include "../include/Replace_Lambda_ReturnType_With_Decltype_Existing_256.h"

// ========================================================================================================
#define MUT256_OUTPUT 1

void MutatorFrontendAction_256::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check if the matched node is a variable or function declaration
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        symbols.push_back(VD);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        symbols.push_back(FD);
    } else if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;

        // Get the call operator of the lambda
        auto *callOp = LE->getCallOperator();
        if (!callOp)
            return;

        // Get the return type source range
        SourceRange returnTypeRange = callOp->getReturnTypeSourceRange();
        if (returnTypeRange.isInvalid())
            return;

        // Check if there are symbols to replace with
        if (symbols.empty())
            return;

        // Select a random symbol
        int index = getrandom::getRandomIndex(symbols.size());
        const clang::NamedDecl *symbol = symbols[index];
        std::string symbolName = symbol->getNameAsString();

        // Construct the new return type
        std::string newReturnType = "decltype(" + symbolName + ")";

        // Replace the return type in the source code
        Rewrite.ReplaceText(returnTypeRange, newReturnType);
    }
}

void MutatorFrontendAction_256::MutatorASTConsumer_256::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    // Match variable and function declarations to collect symbols
    DeclarationMatcher symbolMatcher = varDecl().bind("var") | functionDecl().bind("func");

    // Match lambdas with explicit return type
    DeclarationMatcher lambdaMatcher = lambdaExpr(hasCallOperator(cxxMethodDecl(hasReturnType(...)))).bind("lambda");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(symbolMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}