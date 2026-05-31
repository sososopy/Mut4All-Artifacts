//header file
#pragma once
#include "Mutator_base.h"

/**
 * MalformedRequiresClause_116
 */ 
class MutatorFrontendAction_116 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(116)

private:
    class MutatorASTConsumer_116 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_116(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/MalformedRequiresClause_116.h"

// ========================================================================================================
#define MUT116_OUTPUT 1

void MutatorFrontendAction_116::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("requiresFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        clang::CXXRequiresExpr *requiresExpr = FD->getRequiresClause();
        if (!requiresExpr)
            return;
        SourceLocation requiresKeywordLoc = requiresExpr->getRequiresKeywordLoc();
        SourceLocation bodyStartLoc = Rewrite.getLocForEndOfToken(requiresKeywordLoc);
        SourceLocation bodyEndLoc = requiresExpr->getSourceRange().getEnd();
        if (bodyStartLoc.isInvalid() || bodyEndLoc.isInvalid())
            return;
        Rewrite.ReplaceText(SourceRange(bodyStartLoc, bodyEndLoc), "requires ( ; }");
    }
}
  
void MutatorFrontendAction_116::MutatorASTConsumer_116::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasRequiresClause()).bind("requiresFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}