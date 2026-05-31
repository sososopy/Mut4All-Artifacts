//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Ternary_Condition_With_Sizeof_Template_456
 */ 
class MutatorFrontendAction_456 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(456)

private:
    class MutatorASTConsumer_456 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_456(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Ternary_Condition_With_Sizeof_Template_456.h"

// ========================================================================================================
#define MUT456_OUTPUT 1

void MutatorFrontendAction_456::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConditionalOperator>("ternaryOp")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        auto trueExpr = MT->getTrueExpr();
        auto falseExpr = MT->getFalseExpr();

        auto trueConstruct = dyn_cast<clang::CXXConstructExpr>(trueExpr);
        auto falseConstruct = dyn_cast<clang::CXXConstructExpr>(falseExpr);
        if (!trueConstruct || !falseConstruct)
            return;

        QualType trueType = trueConstruct->getConstructor()->getParent()->getType();
        QualType falseType = falseConstruct->getConstructor()->getParent()->getType();

        if (trueType != falseType)
            return;

        ASTContext &Context = *Result.Context;
        std::string typeName = Context.getTypeName(trueType).getAsString();

        std::string newCondition = "sizeof(" + typeName + ") > 0";

        SourceRange condRange = MT->getCond()->getSourceRange();
        Rewrite.ReplaceText(condRange, newCondition);
    }
}

void MutatorFrontendAction_456::MutatorASTConsumer_456::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conditionalOperator().bind("ternaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}