//source file
#include "../include/insert_recursive_function_call_387.h"

// ========================================================================================================
#define MUT387_OUTPUT 1

void MutatorFrontendAction_387::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionName = FD->getNameAsString();
            auto bodyRange = FD->getBody()->getSourceRange();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);

            std::string recursiveCall = "\n/*mut387*/" + functionName + "();\n";
            if (bodyText.find("{") != std::string::npos)
                bodyText.insert(bodyText.find("{") + 1, recursiveCall);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
        }
    }
}

void MutatorFrontendAction_387::MutatorASTConsumer_387::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}