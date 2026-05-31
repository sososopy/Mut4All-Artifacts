//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Static_Assert_Closing_Paren_208
 */ 
class MutatorFrontendAction_208 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(208)
private:
    class MutatorASTConsumer_208 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_208(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Static_Assert_Closing_Paren_208.h"

// ========================================================================================================
#define MUT208_OUTPUT 1

void MutatorFrontendAction_208::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("staticAssert")) {
        // Filter nodes in header files
        if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(SA->getLocation()))
            return;
        // Get the closing parenthesis location
        SourceLocation rParenLoc = SA->getRParenLoc();
        if (rParenLoc.isInvalid())
            return;
        // Remove the closing parenthesis
        Rewrite.RemoveText(rParenLoc, 1);
    }
}

void MutatorFrontendAction_208::MutatorASTConsumer_208::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify static_assert with string literal message
    DeclarationMatcher matcher = staticAssertDecl(hasMessage(ignoringImpCasts(stringLiteral()))).bind("staticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}