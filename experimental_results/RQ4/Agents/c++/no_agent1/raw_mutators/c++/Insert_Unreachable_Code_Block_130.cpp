//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_130
 */ 
class MutatorFrontendAction_130 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(130)

private:
    class MutatorASTConsumer_130 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_130(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_block_130.h"

// ========================================================================================================
#define MUT130_OUTPUT 1

void MutatorFrontendAction_130::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("FunctionBody")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getBeginLoc()))
            return;

        // Get the source code text of target node
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), FS->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string unreachableCode = "\nif (false) {\n /*mut130*/ int unreachableVar = 0;\n}\n";
        bodyText.insert(bodyText.find("{") + 1, unreachableCode);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), bodyText);
    }
}

void MutatorFrontendAction_130::MutatorASTConsumer_130::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = compoundStmt(hasParent(functionDecl())).bind("FunctionBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}