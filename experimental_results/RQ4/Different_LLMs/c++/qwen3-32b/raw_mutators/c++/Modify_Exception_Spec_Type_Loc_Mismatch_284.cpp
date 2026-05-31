//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Exception_Spec_Type_Loc_Mismatch_284
 */ 
class MutatorFrontendAction_284 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(284)

private:
    class MutatorASTConsumer_284 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_284(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Modify_Exception_Spec_Type_Loc_Mismatch_284.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "stringutils.h"

// ========================================================================================================
#define MUT284_OUTPUT 1

void MutatorFrontendAction_284::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->getExceptionSpecType() != clang::FunctionDecl::EST_Noexcept)
            return;
        const clang::NoexceptExpr *NE = FD->getNoexceptExpr();
        if (!NE)
            return;
        const clang::Expr *E = NE->getExpr();
        if (!E)
            return;
        clang::SourceLocation Start = E->getBeginLoc();
        clang::SourceLocation End = E->getEndLoc();
        if (Start.isInvalid() || End.isInvalid())
            return;
        std::string originalExpr = stringutils::rangetoStr(*Result.SourceManager, clang::SourceRange(Start, End));
        std::string newExpr = "static_cast<int>(" + originalExpr + ")";
        Rewrite.ReplaceText(clang::CharSourceRange::getCharRange(Start, End), newExpr);
    }
}

void MutatorFrontendAction_284::MutatorASTConsumer_284::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::functionDecl(clang::ast_matchers::hasNoexceptSpec()).bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}