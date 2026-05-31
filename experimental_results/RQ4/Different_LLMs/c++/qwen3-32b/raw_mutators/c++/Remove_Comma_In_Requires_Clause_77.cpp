//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Comma_In_Requires_Clause_77
 */ 
class MutatorFrontendAction_77 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(77)

private:
    class MutatorASTConsumer_77 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_77(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Remove_Comma_In_Requires_Clause_77.h"

// ========================================================================================================
#define MUT77_OUTPUT 1

void MutatorFrontendAction_77::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("requiresExpr")) {
        // Filter nodes in header files
        if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(RE->getLocation()))
            return;

        // Check if there are at least two requirements
        if (RE->getNumRequirements() < 2)
            return;

        // Get the comma locations
        const auto &commaLocs = RE->getCommaLocs();
        if (commaLocs.size() < 1)
            return;

        // Randomly select a comma to remove
        int index = getrandom::getRandomIndex(commaLocs.size());
        SourceLocation commaLoc = commaLocs[index];

        // Remove the comma
        Rewrite.RemoveText(commaLoc, 1);
    }
}

void MutatorFrontendAction_77::MutatorASTConsumer_77::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = requiresExpr().bind("requiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}