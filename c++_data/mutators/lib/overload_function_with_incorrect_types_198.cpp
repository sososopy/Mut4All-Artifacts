//source file
#include "../include/Overload_Function_With_Incorrect_Types_198.h"

// ========================================================================================================
#define MUT198_OUTPUT 1

void MutatorFrontendAction_198::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getNumParams() < 2)
            return;

        candidateFunctions.push_back(FD);
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        if (candidateFunctions.empty())
            return;

        auto *FD = CE->getDirectCallee();
        if (!FD)
            return;

        for (const auto *Candidate : candidateFunctions) {
            if (Candidate->getNameAsString() == FD->getNameAsString() &&
                Candidate->getNumParams() == FD->getNumParams()) {

                std::string mutatedCall = Candidate->getNameInfo().getAsString() + "(";
                for (unsigned i = 0; i < Candidate->getNumParams(); ++i) {
                    if (i > 0) mutatedCall += ", ";
                    if (i == 0) mutatedCall += "\"string\"";
                    else if (i == 1) mutatedCall += "std::vector<int>{1, 2, 3}";
                    else mutatedCall += "0";
                }
                mutatedCall += ");";

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCall);
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_198::MutatorASTConsumer_198::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto functionMatcher = functionDecl().bind("Function");
    auto callExprMatcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.matchAST(Context);
}