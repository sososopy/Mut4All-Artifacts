//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Unreachable_Code_110
 */ 

class MutatorFrontendAction_110 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(110)
private:
    class MutatorASTConsumer_110 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_110(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/inject_unreachable_code_110.h"

// ========================================================================================================
#define MUT110_OUTPUT 1

void MutatorFrontendAction_110::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->hasBody()) {
            targetFunctions.push_back(FD);
        }
    } else if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
        if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(RS->getBeginLoc()))
            return;

        if (!targetFunctions.empty()) {
            auto *targetFunc = targetFunctions.back();
            targetFunctions.pop_back();

            auto funcName = targetFunc->getNameAsString();
            std::string unreachableCode = "\n/*mut110*/if (false) { std::cout << \"Unreachable code in " + funcName + "\"; }\n";

            SourceLocation insertLoc = RS->getBeginLoc();
            Rewrite.InsertTextBefore(insertLoc, unreachableCode);
        }
    }
}

void MutatorFrontendAction_110::MutatorASTConsumer_110::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher funcMatcher = functionDecl().bind("Functions");
    StatementMatcher returnMatcher = returnStmt().bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(returnMatcher, &callback);
    matchFinder.matchAST(Context);
}