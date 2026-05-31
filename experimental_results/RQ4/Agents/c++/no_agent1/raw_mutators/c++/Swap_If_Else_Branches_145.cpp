//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_If_Else_Branches_145
 */ 
class MutatorFrontendAction_145 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(145)

private:
    class MutatorASTConsumer_145 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_145(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/swap_if_else_branches_145.h"

// ========================================================================================================
#define MUT145_OUTPUT 1

void MutatorFrontendAction_145::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
        if (!IfStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                        IfStmt->getIfLoc()))
            return;

        // Check if there is an else branch
        if (const Stmt *ElseStmt = IfStmt->getElse()) {
            // Get the source code text of the if and else branches
            auto IfBranchText = stringutils::rangetoStr(*(Result.SourceManager),
                                                        IfStmt->getThen()->getSourceRange());
            auto ElseBranchText = stringutils::rangetoStr(*(Result.SourceManager),
                                                          ElseStmt->getSourceRange());

            // Perform mutation by swapping the branches
            std::string MutatedIfStmt = "if (" + stringutils::rangetoStr(*(Result.SourceManager), 
                                    IfStmt->getCond()->getSourceRange()) + ") " + ElseBranchText + " else " + IfBranchText;

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(IfStmt->getSourceRange()), MutatedIfStmt);
        }
    }
}

void MutatorFrontendAction_145::MutatorASTConsumer_145::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = ifStmt(hasElse(stmt())).bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}