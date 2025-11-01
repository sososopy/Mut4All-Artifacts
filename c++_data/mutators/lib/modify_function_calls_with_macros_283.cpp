//source file
#include "../include/Modify_Function_Calls_With_Macros_283.h"

// ========================================================================================================
#define MUT283_OUTPUT 1

void MutatorFrontendAction_283::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
        if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getBeginLoc()))
            return;

        const FunctionDecl *FD = FC->getDirectCallee();
        if (!FD)
            return;

        std::string functionName = FD->getNameInfo().getName().getAsString();
        if (modifiedFunctions.find(functionName) != modifiedFunctions.end())
            return;

        std::string macroName = "MACRO_" + functionName;
        std::string macroDef = "#define " + macroName + "(x, y) (x + y + __LINE__)\n";
        modifiedFunctions.insert(functionName);

        SourceLocation startLoc = Result.SourceManager->getLocForStartOfFile(
            Result.SourceManager->getMainFileID());
        Rewrite.InsertText(startLoc, macroDef, true, true);

        SourceRange callRange = FC->getSourceRange();
        std::string callReplacement = macroName + "(";
        for (unsigned i = 0; i < FC->getNumArgs(); ++i) {
            if (i > 0)
                callReplacement += ", ";
            callReplacement += Lexer::getSourceText(
                CharSourceRange::getTokenRange(FC->getArg(i)->getSourceRange()),
                *Result.SourceManager, Result.Context->getLangOpts()).str();
        }
        callReplacement += ")";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(callRange), callReplacement);
    }
}

void MutatorFrontendAction_283::MutatorASTConsumer_283::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(callee(functionDecl())).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}