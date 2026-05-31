//source file
#include "../include/modify_lambda_expression_with_pack_expansion_460.h"

// ========================================================================================================
#define MUT460_OUTPUT 1

void MutatorFrontendAction_460::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       LE->getBeginLoc()))
            return;

        auto body = LE->getBody();
        if (!body)
            return;

        for (auto &stmt : body->children()) {
            if (auto *FE = llvm::dyn_cast<clang::BinaryOperator>(stmt)) {
                auto op = FE->getOpcode();
                std::string newOp;
                switch (op) {
                    case clang::BO_Add:
                        newOp = "*";
                        break;
                    case clang::BO_Comma:
                        newOp = "+";
                        break;
                    default:
                        continue;
                }
                auto opLoc = FE->getOperatorLoc();
                Rewrite.ReplaceText(opLoc, 1, newOp);
            }
        }
    }
}
  
void MutatorFrontendAction_460::MutatorASTConsumer_460::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(binaryOperator().bind("FoldExpr"))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}