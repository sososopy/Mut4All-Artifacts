//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Arg_With_Parens_212
 */ 
class MutatorFrontendAction_212 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(212)

private:
    class MutatorASTConsumer_212 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_212(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Default_Arg_With_Parens_212.h"

// ========================================================================================================
#define MUT212_OUTPUT 1

void MutatorFrontendAction_212::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PV = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
        if (!PV->hasDefaultArg()) return;
        if (!Result.Context->getSourceManager().isWrittenInMainFile(PV->getLocation()))
            return;

        const clang::Expr *DefaultArg = PV->getDefaultArg();
        if (!DefaultArg) return;

        SourceLocation startLoc = DefaultArg->getBeginLoc();
        SourceLocation endLoc = DefaultArg->getEndLoc();
        if (startLoc.isInvalid() || endLoc.isInvalid()) return;

        CharSourceRange range = CharSourceRange::getCharRange(startLoc, endLoc);
        Rewrite.ReplaceText(range, "()");
    }
}

void MutatorFrontendAction_212::MutatorASTConsumer_212::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        parmVarDecl(hasDefaultArgument(), 
            hasAncestor(cxxConstructorDecl(isConstexpr()))).bind("param") ||
        parmVarDecl(hasDefaultArgument(), 
            hasAncestor(functionDecl(isConstexpr()))).bind("param");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}