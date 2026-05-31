//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Nested_Block_493
 */ 
class MutatorFrontendAction_493 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(493)

private:
    class MutatorASTConsumer_493 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_493(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Insert_Nested_Block_493.h"

// ========================================================================================================
#define MUT493_OUTPUT 1

void MutatorFrontendAction_493::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CS = Result.Nodes.getNodeAs<clang::CompoundStmt>("compoundStmt")) {
        if (auto *FD = dyn_cast<clang::FunctionDecl>(CS->getDeclContext())) {
            if (!FD || !Result.SourceManager->isWrittenInMainFile(FD->getLocation()))
                return;
            if (CS->size() < 1)
                return;
            const clang::Stmt *FirstStmt = CS->body_front();
            clang::SourceRange FirstStmtRange = FirstStmt->getSourceRange();
            std::string FirstStmtText = stringutils::rangetoStr(*Result.SourceManager, FirstStmtRange);
            std::string NewText = "{" + FirstStmtText + "}";
            Rewrite.ReplaceText(FirstStmtRange, NewText);
        }
    }
}
  
void MutatorFrontendAction_493::MutatorASTConsumer_493::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt().bind("compoundStmt")));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}