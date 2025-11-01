//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Structured_Binding_In_Lambda_Capture_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Structured_Binding_In_Lambda_Capture_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        const auto &CaptureList = LambdaExpr->captures();
        for (const auto &Capture : CaptureList) {
            if (Capture.capturesVariable() && Capture.getCapturedVar()->isStructuredBinding()) {
                const auto *VarDecl = Capture.getCapturedVar();
                if (const auto *DecompDecl = llvm::dyn_cast<clang::DecompositionDecl>(VarDecl)) {
                    std::string newCapture = "[";
                    for (const auto *Binding : DecompDecl->bindings()) {
                        if (Capture.isByRef()) {
                            newCapture += "&" + Binding->getNameAsString() + ", ";
                        } else {
                            newCapture += Binding->getNameAsString() + ", ";
                        }
                    }
                    if (!newCapture.empty()) {
                        newCapture.pop_back(); // Remove last space
                        newCapture.pop_back(); // Remove last comma
                    }
                    newCapture += "]";
                    SourceRange CaptureRange = Capture.getSourceRange();
                    Rewrite.ReplaceText(CaptureRange, newCapture);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}