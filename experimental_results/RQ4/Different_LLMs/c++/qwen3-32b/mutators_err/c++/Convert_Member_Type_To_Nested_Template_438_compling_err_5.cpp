//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ExprConcepts.h"

/**
 * Convert_Member_Type_To_Nested_Template_438
 */ 
class MutatorFrontendAction_438 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(438)

private:
    class MutatorASTConsumer_438 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_438(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Convert_Member_Type_To_Nested_Template_438.h"

// ========================================================================================================
#define MUT438_OUTPUT 1

void MutatorFrontendAction_438::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("requiresExpr")) {
        // Filter nodes in header files
        if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(RE->getLocStart()))
            return;

        if (auto *TR = Result.Nodes.getNodeAs<clang::TypenameRequirement>("typenameReq")) {
            SourceLocation start = TR->getBeginLoc();
            SourceLocation end = TR->getEndLoc();
            if (start.isInvalid() || end.isInvalid())
                return;

            std::string newCode = "typename T::template Y<int>";
            Rewrite.ReplaceText(SourceRange(start, end), newCode);
        }
    }
}

void MutatorFrontendAction_438::MutatorASTConsumer_438::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::requiresExpr(
        clang::ast_matchers::has(clang::ast_matchers::requirement(
            clang::ast_matchers::typenameRequirement().bind("typenameReq")
        ))
    ).bind("requiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}