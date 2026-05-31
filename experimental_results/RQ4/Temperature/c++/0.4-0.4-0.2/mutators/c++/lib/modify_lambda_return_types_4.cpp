//source file
#include "../include/modify_lambda_return_types_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        const auto *LambdaType = LambdaExpr->getCallOperator()->getReturnType().getTypePtrOrNull();
        if (LambdaType && LambdaType->isFunctionType()) {
            return; // Skip if already a function type
        }

        QualType returnType = LambdaExpr->getCallOperator()->getReturnType();
        std::string originalReturnType = returnType.getAsString();
        std::string mutatedReturnType = originalReturnType + "(" + originalReturnType + ")";

        SourceLocation arrowLoc = LambdaExpr->getIntroducerRange().getEnd();
        SourceLocation returnTypeLoc = Lexer::findLocationAfterToken(
            arrowLoc, tok::arrow, *Result.SourceManager, Result.Context->getLangOpts(), false);

        if (returnTypeLoc.isValid()) {
            Rewrite.ReplaceText(returnTypeLoc, originalReturnType.length(), mutatedReturnType);
        }
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}