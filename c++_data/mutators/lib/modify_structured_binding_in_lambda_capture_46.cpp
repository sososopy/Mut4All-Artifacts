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
            if (Capture.capturesVariable()) {
                const auto *VarDecl = Capture.getCapturedVar();
                if (const auto *DecompDecl = llvm::dyn_cast<clang::DecompositionDecl>(VarDecl)) {
                    std::string newCapture = "[";
                    for (const auto *Binding : DecompDecl->bindings()) {
                        if (Capture.getCaptureKind() == clang::LambdaCaptureKind::LCK_ByRef) {
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
                    SourceRange CaptureRange = Capture.getLocation();
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