//source file
#include "../include/replace_operator_with_function_call_379.h"

// ========================================================================================================
#define MUT379_OUTPUT 1

void MutatorFrontendAction_379::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getExprLoc()))
            return;

        auto lhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getLHS()->getSourceRange());
        auto rhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getRHS()->getSourceRange());
        std::string replacement;

        switch (BO->getOpcode()) {
            case BO_Add:
                replacement = "add(" + lhs + ", " + rhs + ")";
                break;
            case BO_Sub:
                replacement = "subtract(" + lhs + ", " + rhs + ")";
                break;
            case BO_Mul:
                replacement = "multiply(" + lhs + ", " + rhs + ")";
                break;
            case BO_Div:
                replacement = "divide(" + lhs + ", " + rhs + ")";
                break;
            default:
                return;
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), replacement);
    }
}

void MutatorFrontendAction_379::MutatorASTConsumer_379::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = binaryOperator(isExpansionInMainFile()).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}