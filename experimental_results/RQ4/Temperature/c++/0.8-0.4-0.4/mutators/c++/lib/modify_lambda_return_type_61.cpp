//source file
#include "../include/modify_lambda_return_type_61.h"

// ========================================================================================================
#define MUT61_OUTPUT 1

void MutatorFrontendAction_61::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        const auto TypeInfo = LambdaExpr->getLambdaClass()->getLambdaCallOperator()->getReturnTypeSourceRange();
        if (TypeInfo.isInvalid())
            return;

        auto OriginalSource = stringutils::rangetoStr(*(Result.SourceManager), TypeInfo);
        std::string MutatedSource = "decltype(auto)";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TypeInfo), MutatedSource);
    }
}
  
void MutatorFrontendAction_61::MutatorASTConsumer_61::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasType(autoType())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}