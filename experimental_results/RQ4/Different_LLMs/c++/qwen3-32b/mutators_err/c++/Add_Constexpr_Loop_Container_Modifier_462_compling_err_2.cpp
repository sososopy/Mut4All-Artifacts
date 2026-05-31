//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Constexpr_Loop_Container_Modifier_462
 */ 
class MutatorFrontendAction_462 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(462)

private:
    class MutatorASTConsumer_462 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_462(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Constexpr_Loop_Container_Modifier_462.h"

// ========================================================================================================
#define MUT462_OUTPUT 1

void MutatorFrontendAction_462::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callToModify")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                           CE->getBeginLoc()))
            return;

        int loopCount = getrandom::getRandomIndex(10, 100);
        std::string loopCode = "for (int i = 0; i < " + std::to_string(loopCount) + "; ++i)\n    ";
        loopCode += Rewrite.getRewrittenText(CharSourceRange::getTokenRange(CE->getSourceRange()));

        Rewrite.ReplaceText(CE->getSourceRange(), loopCode);
    }
}

void MutatorFrontendAction_462::MutatorASTConsumer_462::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(
        hasAncestor(functionDecl(isConstexpr()))
    ).bind("callToModify");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}