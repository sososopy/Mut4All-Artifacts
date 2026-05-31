//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Scope_With_Dot_310
 */ 
class MutatorFrontendAction_310 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(310)

private:
    class MutatorASTConsumer_310 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_310(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Scope_With_Dot_310.h"

// ========================================================================================================
#define MUT310_OUTPUT 1

void MutatorFrontendAction_310::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXDependentScopeMemberExpr>("memberExpr")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getBeginLoc()))
            return;
        SourceLocation opLoc = MT->getOperatorLoc();
        if (opLoc.isInvalid())
            return;
        Rewrite.ReplaceText(opLoc, 2, ".");
    }
}

void MutatorFrontendAction_310::MutatorASTConsumer_310::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDependentScopeMemberExpr().bind("memberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}