//source file
#include "../include/mismatched_type_comparison_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::initializeTypeReplacementMap() {
    typeReplacementMap["int"] = "float";
    typeReplacementMap["unsigned int"] = "double";
    typeReplacementMap["long"] = "double";
    // Add more type replacements as needed
}

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getExprLoc()))
            return;

        auto lhsType = BO->getLHS()->getType().getAsString();
        auto rhsType = BO->getRHS()->getType().getAsString();

        if (typeReplacementMap.find(lhsType) != typeReplacementMap.end()) {
            std::string newType = typeReplacementMap[lhsType];
            std::string lhsText = clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()),
                *Result.SourceManager, Result.Context->getLangOpts()).str();

            std::string newLhsText = "(" + newType + ")" + lhsText;
            Rewrite.ReplaceText(BO->getLHS()->getSourceRange(), newLhsText);
        }
    }
}

void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = binaryOperator(hasOperatorName("<")).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}