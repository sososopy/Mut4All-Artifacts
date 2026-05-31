//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Assembly_Syntax_Alteration_272
 */ 
class MutatorFrontendAction_272 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(272)

private:
    class MutatorASTConsumer_272 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_272(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Inline_Assembly_Syntax_Alteration_272.h"

// ========================================================================================================
#define MUT272_OUTPUT 1

void MutatorFrontendAction_272::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AS = Result.Nodes.getNodeAs<clang::AsmStmt>("asmStmt")) {
        // Filter nodes in header files
        if (!AS || !Result.Context->getSourceManager().isWrittenInMainFile(AS->getBeginLoc()))
            return;

        // Ensure asm has at least one output or input operand
        if (AS->getNumOutputOperands() == 0 && AS->getNumInputOperands() == 0) return;

        // Get source text of the asm statement
        SourceLocation start = AS->getBeginLoc();
        SourceLocation end = AS->getEndLoc();
        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, CharSourceRange::getTokenRange(start, end));

        // Insert trailing colon before closing parenthesis
        size_t closingParenPos = originalText.find_last_of(')');
        if (closingParenPos != std::string::npos) {
            originalText.insert(closingParenPos, ":");
        }

        // Replace original text with mutated version
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(start, end), originalText);
    }
}

void MutatorFrontendAction_272::MutatorASTConsumer_272::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = asmStmt().bind("asmStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}