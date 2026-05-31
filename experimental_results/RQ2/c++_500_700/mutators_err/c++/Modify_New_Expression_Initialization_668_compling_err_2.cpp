//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_new_expression_initialization_668
 */ 
class MutatorFrontendAction_668 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(668)

private:
    class MutatorASTConsumer_668 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_668(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_new_expression_initialization_668.h"

// ========================================================================================================
#define MUT668_OUTPUT 1

void MutatorFrontendAction_668::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NE = Result.Nodes.getNodeAs<clang::CXXNewExpr>("newExpr")) {
        if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       NE->getBeginLoc()))
            return;

        if (NE->hasInitializer()) {
            auto *initExpr = NE->getInitializer();
            auto initRange = initExpr->getSourceRange();
            auto initText = stringutils::rangetoStr(*(Result.SourceManager), initRange);

            if (initText.find('{') != std::string::npos && initText.find('(') != std::string::npos) {
                std::string mutatedInit = initText;
                mutatedInit.erase(std::remove(mutatedInit.begin(), mutatedInit.end(), '('), mutatedInit.end());
                mutatedInit.erase(std::remove(mutatedInit.begin(), mutatedInit.end(), ')'), mutatedInit.end());

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(initRange), mutatedInit);
            }
        }
    }
}

void MutatorFrontendAction_668::MutatorASTConsumer_668::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxNewExpr(hasInitializer(anyOf(initListExpr())))
                   .bind("newExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}