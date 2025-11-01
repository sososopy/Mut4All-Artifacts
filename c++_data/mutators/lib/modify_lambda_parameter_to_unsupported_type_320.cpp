//source file
#include "../include/Mutator_320.h"

// ========================================================================================================
#define MUT320_OUTPUT 1

void MutatorFrontendAction_320::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto *LambdaType = LambdaExpr->getLambdaClass();
        if (!LambdaType)
            return;

        for (auto *Field : LambdaType->fields()) {
            if (Field->getType()->isBuiltinType()) {
                auto FieldRange = Field->getSourceRange();
                auto FieldTypeStr = stringutils::rangetoStr(*(Result.SourceManager), FieldRange);
                
                if (FieldTypeStr.find("__fp16") == std::string::npos) {
                    std::string NewType = "__fp16";
                    Rewrite.ReplaceText(FieldRange, NewType);
                    break;
                }
            }
        }
    }
}

void MutatorFrontendAction_320::MutatorASTConsumer_320::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr(hasParent(functionTemplateDecl())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}