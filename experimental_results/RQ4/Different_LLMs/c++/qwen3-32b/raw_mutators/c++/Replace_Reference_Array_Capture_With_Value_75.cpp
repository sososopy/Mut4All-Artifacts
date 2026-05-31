//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Reference_Array_Capture_With_Value_75
 */ 
class MutatorFrontendAction_75 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(75)
private:
    class MutatorASTConsumer_75 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_75(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_reference_array_capture_with_value_75.h"

// ========================================================================================================
#define MUT75_OUTPUT 1

void MutatorFrontendAction_75::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        for (unsigned i = 0; i < lambda->capture_size(); ++i) {
            const LambdaCapture *capture = &lambda->capture_begin()[i];
            if (capture->isImplicit())
                continue;

            if (capture->isByRef()) {
                const VarDecl *capturedVar = capture->getCapturedVar();
                if (capturedVar && capturedVar->getType()->isArrayType()) {
                    SourceRange captureRange = capture->getSourceRange();
                    std::string originalText = stringutils::rangetoStr(*Result.SourceManager, captureRange);
                    if (originalText.find('&') != std::string::npos) {
                        std::string newValue = originalText;
                        size_t pos = newValue.find('&');
                        newValue.erase(pos, 1);
                        Rewrite.ReplaceText(captureRange, newValue);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_75::MutatorASTConsumer_75::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasConstEvalSpecifier(), hasDescendant(lambdaExpr().bind("lambda"))).bind("constevalFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}