//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Unreachable_Code_After_Return_475
 */ 
class MutatorFrontendAction_475 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(475)

private:
    class MutatorASTConsumer_475 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_475(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/inject_unreachable_code_after_return_475.h"

// ========================================================================================================
#define MUT475_OUTPUT 1

void MutatorFrontendAction_475::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
        if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RS->getBeginLoc()))
            return;

        // Get the source code text of target node
        auto returnText = stringutils::rangetoStr(*(Result.SourceManager),
                                                  RS->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string unreachableCode = "\n/*mut475*/if (false) { throw std::runtime_error(\"Unreachable code\"); }\n";
        returnText.insert(returnText.find(";") + 1, unreachableCode);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()), returnText);
    }
}

void MutatorFrontendAction_475::MutatorASTConsumer_475::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = returnStmt().bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}