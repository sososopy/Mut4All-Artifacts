//source file
#include "../include/replace_function_parameter_type_with_incompatible_type_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
            return;
        
        auto callee = CE->getDirectCallee();
        if (!callee || callee->getNumParams() == 0)
            return;

        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            auto arg = CE->getArg(i);
            auto paramType = callee->getParamDecl(i)->getType();

            if (paramType->isIntegerType()) {
                Rewrite.ReplaceText(arg->getSourceRange(), "\"string\"");
            } else if (paramType->isFloatingType()) {
                Rewrite.ReplaceText(arg->getSourceRange(), "42");
            } else if (paramType->isPointerType()) {
                Rewrite.ReplaceText(arg->getSourceRange(), "nullptr");
            }
        }
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAnyParameter(anything())))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}