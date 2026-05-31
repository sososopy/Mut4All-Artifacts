//header file
#pragma once
#include "Mutator_base.h"

/**
 * LambdaPackInDecltype_126
 */ 
class MutatorFrontendAction_LambdaPackInDecltype_126 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(LambdaPackInDecltype_126)

private:
    class MutatorASTConsumer_LambdaPackInDecltype_126 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_LambdaPackInDecltype_126(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/LambdaPackInDecltype_126.h"

// ========================================================================================================
#define MUTLambdaPackInDecltype_126_OUTPUT 1

void MutatorFrontendAction_LambdaPackInDecltype_126::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getEndLoc()))
            return;

        std::string insertText = " decltype([](auto... pack) { return 0; }()) __mut_lambda_126;";
        Rewrite.InsertText(CE->getEndLoc(), insertText, true, true);
    }
}
  
void MutatorFrontendAction_LambdaPackInDecltype_126::MutatorASTConsumer_LambdaPackInDecltype_126::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = callExpr().bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}