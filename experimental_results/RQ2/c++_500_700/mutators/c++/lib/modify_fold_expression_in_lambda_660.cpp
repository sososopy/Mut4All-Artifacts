//source file
#include "../include/modify_fold_expression_in_lambda_660.h"

// ========================================================================================================
#define MUT660_OUTPUT 1

bool isFoldExpression(const clang::BinaryOperator *FoldExpr) {
    // Placeholder logic for fold expression detection
    return FoldExpr->isAdditiveOp(); // Example: checks if it's an additive operation
}

void MutatorFrontendAction_660::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto LambdaBody = LambdaExpr->getBody();
        if (!LambdaBody)
            return;

        for (auto &Stmt : LambdaBody->children()) {
            if (auto *FoldExpr = llvm::dyn_cast<clang::BinaryOperator>(Stmt)) {
                if (isFoldExpression(FoldExpr)) {
                    std::string OriginalText = Lexer::getSourceText(
                        CharSourceRange::getTokenRange(FoldExpr->getSourceRange()),
                        Rewrite.getSourceMgr(), Rewrite.getLangOpts()).str();

                    std::string MutatedText = OriginalText;
                    if (FoldExpr->getOpcode() == clang::BO_Add) {
                        llvm::outs() << "Original Fold Expression: " << OriginalText << "\n";
                        MutatedText = std::regex_replace(MutatedText, std::regex("\\+"), "*");
                        llvm::outs() << "Mutated Fold Expression: " << MutatedText << "\n";
                    }

                    Rewrite.ReplaceText(FoldExpr->getSourceRange(), MutatedText);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_660::MutatorASTConsumer_660::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(binaryOperator())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}