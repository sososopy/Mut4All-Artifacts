//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Default_Argument_Capture_384
 */ 
class MutatorFrontendAction_384 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(384)

private:
    class MutatorASTConsumer_384 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_384(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const FunctionDecl*, std::string> functionParams;
    };
};

//source file
#include "../include/Modify_Lambda_Default_Argument_Capture_384.h"

// ========================================================================================================
#define MUT384_OUTPUT 1

void MutatorFrontendAction_384::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithLambda")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (auto param : FD->parameters()) {
            if (auto *LambdaExpr = dyn_cast<LambdaExpr>(param->getDefaultArg())) {
                auto captureSourceRange = LambdaExpr->getIntroducerRange();
                std::string originalCapture = stringutils::rangetoStr(*(Result.SourceManager), captureSourceRange);

                bool needsCapture = false;
                std::string captureList;
                for (auto capture : LambdaExpr->captures()) {
                    if (capture.capturesVariable()) {
                        needsCapture = true;
                        auto varName = capture.getCapturedVar()->getNameAsString();
                        captureList += "&" + varName + ",";
                    }
                }

                if (needsCapture) {
                    captureList.pop_back(); // Remove trailing comma
                    std::string newCapture = "[" + captureList + "]";
                    Rewrite.ReplaceText(captureSourceRange, newCapture);
                }
            }
        }
    }
}

void MutatorFrontendAction_384::MutatorASTConsumer_384::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasDefaultArgument(lambdaExpr()))).bind("FunctionWithLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}