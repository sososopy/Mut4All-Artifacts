//header file
#pragma once
#include "Mutator_base.h"

/**
 * alter_switch_statement_with_missing_braces_59
 */ 
class MutatorFrontendAction_59 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(59)

private:
    class MutatorASTConsumer_59 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_59(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/alter_switch_statement_with_missing_braces_59.h"

// ========================================================================================================
#define MUT59_OUTPUT 1

void MutatorFrontendAction_59::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SC = Result.Nodes.getNodeAs<clang::SwitchCase>("SwitchCase")) {
        if (!SC || !Result.Context->getSourceManager().isWrittenInMainFile(SC->getBeginLoc()))
            return;

        if (const Stmt *SubStmt = SC->getSubStmt()) {
            SourceLocation StartLoc = SubStmt->getBeginLoc();
            SourceLocation EndLoc = SubStmt->getEndLoc();

            if (StartLoc.isValid() && EndLoc.isValid() && !isa<CompoundStmt>(SubStmt)) {
                std::string OriginalText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), 
                                                                *Result.SourceManager, Result.Context->getLangOpts()).str();
                std::string MutatedText = "{ " + OriginalText + " }";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), MutatedText);
            }
        }
    }
}

void MutatorFrontendAction_59::MutatorASTConsumer_59::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = switchCase().bind("SwitchCase");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}