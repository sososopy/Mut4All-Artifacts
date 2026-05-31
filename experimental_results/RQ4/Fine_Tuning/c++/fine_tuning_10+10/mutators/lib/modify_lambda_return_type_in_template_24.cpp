//source file
#include "../include/Modify_Lambda_Return_Type_In_Template_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        if (!CurFuncTemplate)
            return;

        const FunctionDecl *FuncDecl = CurFuncTemplate->getTemplatedDecl();
        const ParmVarDecl *FirstParam = nullptr;

        for (const ParmVarDecl *Param : FuncDecl->parameters()) {
            // Assuming the logic is to check if the parameter is captured by the lambda
            if (std::none_of(LambdaExpr->capture_begin(), LambdaExpr->capture_end(),
                             [Param](const LambdaCapture &Capture) {
                                 return Capture.capturesVariable() && Capture.getCapturedVar() == Param;
                             })) {
                FirstParam = Param;
                break;
            }
        }

        if (!FirstParam)
            return;

        std::string LambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LambdaExpr->getSourceRange());
        std::string ParamName = FirstParam->getNameAsString();
        std::string MutatedLambda = LambdaSource;
        size_t ArrowPos = MutatedLambda.find("->");

        if (ArrowPos == std::string::npos) {
            size_t BracePos = MutatedLambda.find("{");
            if (BracePos != std::string::npos) {
                MutatedLambda.insert(BracePos, " -> decltype(" + ParamName + ")");
            }
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), MutatedLambda);
    }
}

void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr(hasAncestor(functionTemplateDecl().bind("funcTemplate"))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}