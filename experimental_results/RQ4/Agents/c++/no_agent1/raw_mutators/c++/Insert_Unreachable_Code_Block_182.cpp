//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_182
 */

class MutatorFrontendAction_182 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(182)

private:
    class MutatorASTConsumer_182 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_182(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Unreachable_Code_Block_182.h"

// ========================================================================================================
#define MUT182_OUTPUT 1

void MutatorFrontendAction_182::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            // Get the source code text of target node
            auto body = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());

            // Perform mutation by inserting an unreachable code block
            std::string unreachableBlock = "\n/*mut182*/if (false) { while(true) {} }\n";
            body.insert(body.find("{") + 1, unreachableBlock);

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), body);
        }
    }
}

void MutatorFrontendAction_182::MutatorASTConsumer_182::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}