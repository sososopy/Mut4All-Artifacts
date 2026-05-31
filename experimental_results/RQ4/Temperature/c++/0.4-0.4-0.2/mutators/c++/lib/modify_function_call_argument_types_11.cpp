//source file
#include "../include/modify_function_call_argument_types_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getExprLoc()))
            return;

        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            auto *Arg = CE->getArg(i);
            QualType ArgType = Arg->getType();

            if (ArgType->isIntegerType()) {
                Rewrite.ReplaceText(Arg->getSourceRange(), "\"invalid_type\"");
                break;
            } else if (ArgType->isFloatingType()) {
                Rewrite.ReplaceText(Arg->getSourceRange(), "\"invalid_type\"");
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr().bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}