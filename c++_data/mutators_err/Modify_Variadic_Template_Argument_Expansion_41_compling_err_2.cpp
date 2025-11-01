//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Variadic_Template_Argument_Expansion_41
 */ 
class MutatorFrontendAction_41 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(41)

private:
    class MutatorASTConsumer_41 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_41(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_variadic_template_argument_expansion_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("variadicExpansion")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getExprLoc()))
            return;

        auto sourceRange = CE->getSourceRange();
        auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        std::string mutatedText = "([](auto... args) { (" + originalText + ", ...); }(args...))";
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}

void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(hasDescendant(packExpansion())).bind("variadicExpansion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}