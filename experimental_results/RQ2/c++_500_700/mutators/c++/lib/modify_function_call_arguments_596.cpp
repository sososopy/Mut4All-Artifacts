//source file
#include "../include/modify_function_call_arguments_596.h"

// ========================================================================================================
#define MUT596_OUTPUT 1

void MutatorFrontendAction_596::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getBeginLoc()))
            return;

        auto numArgs = CallExpr->getNumArgs();
        if (numArgs == 0)
            return;

        std::string mutatedCall;
        llvm::raw_string_ostream s(mutatedCall);
        CallExpr->printPretty(s, nullptr, Result.Context->getPrintingPolicy());

        if (numArgs > 1) {
            // Remove an argument
            auto argToRemove = getrandom::getRandomIndex(numArgs);
            std::string argText = stringutils::rangetoStr(*(Result.SourceManager), CallExpr->getArg(argToRemove)->getSourceRange());
            size_t pos = mutatedCall.find(argText);
            if (pos != std::string::npos) {
                mutatedCall.erase(pos, argText.length());
                if (mutatedCall[pos] == ',') {
                    mutatedCall.erase(pos, 1); // Remove comma
                }
            }
        } else {
            // Add an extra argument
            std::string newVarDecl = "int new_var_596 = 0; ";
            std::string newVarUse = "new_var_596";
            mutatedCall.insert(mutatedCall.find(')'), ", " + newVarUse);
            SourceLocation insertLoc = CallExpr->getBeginLoc();
            Rewrite.InsertTextBefore(insertLoc, newVarDecl);
        }

        Rewrite.ReplaceText(CallExpr->getSourceRange(), mutatedCall);
    }
}
  
void MutatorFrontendAction_596::MutatorASTConsumer_596::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(callee(functionDecl(hasAnyParameter(parmVarDecl())))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}