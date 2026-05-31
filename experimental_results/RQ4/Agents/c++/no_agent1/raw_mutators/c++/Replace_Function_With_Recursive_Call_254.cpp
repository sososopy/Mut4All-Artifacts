//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_With_Recursive_Call_254
 */ 

class MutatorFrontendAction_254 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(254)
private:
    class MutatorASTConsumer_254 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_254(Rewriter &R) : TheRewriter(R) {}
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
        std::string functionName;
    };
};

//source file
#include "../include/replace_function_with_recursive_call_254.h"

// ========================================================================================================
#define MUT254_OUTPUT 1

void MutatorFrontendAction_254::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody() && !FD->isMain()) {
            functionName = FD->getNameAsString();
            auto bodyRange = FD->getBody()->getSourceRange();
            std::string recursiveCall = "{ /*mut254*/ return " + functionName + "(); }";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), recursiveCall);
        }
    }
}

void MutatorFrontendAction_254::MutatorASTConsumer_254::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}