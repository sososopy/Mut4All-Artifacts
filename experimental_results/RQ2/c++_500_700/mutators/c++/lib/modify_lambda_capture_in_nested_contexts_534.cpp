//source file
#include "../include/Modify_Lambda_Capture_In_Nested_Contexts_534.h"

// ========================================================================================================
#define MUT534_OUTPUT 1

void MutatorFrontendAction_534::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *InnerLambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("InnerLambda")) {
        if (!InnerLambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                InnerLambda->getBeginLoc()))
            return;

        if (auto *OuterLambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("OuterLambda")) {
            if (!OuterLambda)
                return;

            if (outerVariables.empty()) {
                for (auto *Decl : OuterLambda->getLambdaClass()->decls()) {
                    if (auto *VD = llvm::dyn_cast<clang::VarDecl>(Decl)) {
                        outerVariables.push_back(VD);
                    }
                }
            }

            if (!outerVariables.empty()) {
                std::string newCapture = "";
                for (auto *Var : outerVariables) {
                    bool isCaptured = false;
                    for (auto &Capture : InnerLambda->captures()) {
                        if (Capture.capturesVariable() && Capture.getCapturedVar() == Var) {
                            isCaptured = true;
                            break;
                        }
                    }
                    if (!isCaptured) {
                        newCapture = Var->getNameAsString();
                        break;
                    }
                }

                if (!newCapture.empty()) {
                    auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                                InnerLambda->getSourceRange());
                    size_t bracketPos = lambdaSource.find(']');
                    if (bracketPos != std::string::npos) {
                        lambdaSource.insert(bracketPos, ", " + newCapture);
                        size_t bodyPos = lambdaSource.find('{', bracketPos);
                        if (bodyPos != std::string::npos) {
                            lambdaSource.insert(bodyPos + 1, newCapture + "; ");
                        }
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(InnerLambda->getSourceRange()), lambdaSource);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_534::MutatorASTConsumer_534::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto innerLambdaMatcher = lambdaExpr(hasAncestor(lambdaExpr().bind("OuterLambda"))).bind("InnerLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(innerLambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}