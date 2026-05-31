//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_311
 */ 
class MutatorFrontendAction_311 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(311)

private:
    class MutatorASTConsumer_311 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_311(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_block_311.h"

// ========================================================================================================
#define MUT311_OUTPUT 1

void MutatorFrontendAction_311::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("FunctionBody")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FS->getBeginLoc()))
            return;

        // Get the source code text of target node
        auto body = stringutils::rangetoStr(*(Result.SourceManager), FS->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string unreachable_code = "\n/*mut311*/if (false) { while(true) {} }\n";
        if (body.find("{") != std::string::npos)
            body.insert(body.find("{") + 1, unreachable_code);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), body);
    }
}

void MutatorFrontendAction_311::MutatorASTConsumer_311::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = compoundStmt(hasParent(functionDecl())).bind("FunctionBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}