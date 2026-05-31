//header file
#pragma once
#include "Mutator_base.h"

/**
 * macro_function_argument_parenthesization_635
 */ 
class MutatorFrontendAction_635 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(635)

private:
    class MutatorASTConsumer_635 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_635(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_635.h"

// ========================================================================================================
#define MUT635_OUTPUT 1

void MutatorFrontendAction_635::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("macroCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                CE->getBeginLoc()))
            return;

        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            auto *Arg = CE->getArg(i);
            auto *ILE = dyn_cast<clang::InitListExpr>(Arg);
            if (ILE) {
                auto ArgRange = Arg->getSourceRange();
                std::string ArgText = Rewrite.getRewrittenText(ArgRange);
                std::string MutatedText = "(" + ArgText + ")";
                Rewrite.ReplaceText(ArgRange, MutatedText);
            }
        }
    }
}

void MutatorFrontendAction_635::MutatorASTConsumer_635::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAttr<clang::Attr>())).bind("macroCall"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}