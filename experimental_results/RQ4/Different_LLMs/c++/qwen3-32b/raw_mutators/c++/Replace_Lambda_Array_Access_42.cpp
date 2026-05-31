//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Array_Access_42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)
private:
    class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Lambda_Array_Access_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("returnStmt")) {
        if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RS->getBeginLoc()))
            return;

        auto *RetVal = RS->getRetValue();
        if (!RetVal)
            return;

        SourceRange retValRange = RetVal->getSourceRange();
        if (retValRange.isInvalid())
            return;

        std::string originalText = stringutils::getSourceText(
            *Result.SourceManager, retValRange, Rewrite.getLangOpts());

        size_t pos = originalText.rfind("[0]");
        if (pos == std::string::npos)
            return;

        std::string newPart = originalText.substr(0, pos) + "[1]";
        std::string modifiedText = originalText + " + " + newPart;

        Rewrite.ReplaceText(retValRange, modifiedText);
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        lambdaExpr(
            hasBody(compoundStmt(
                hasDescendant(returnStmt(
                    hasAncestor(lambdaExpr()),
                    hasReturnValue(arraySubscriptExpr(
                        hasBase(arraySubscriptExpr())
                    ))
                ).bind("returnStmt")
            ))
        )).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}