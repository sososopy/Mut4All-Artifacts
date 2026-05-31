//source file
#include "../include/insert_recursive_function_call_91.h"

// ========================================================================================================
#define MUT91_OUTPUT 1

void MutatorFrontendAction_91::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (!FD->hasBody() || visitedFunctions.count(FD))
            return;

        visitedFunctions.insert(FD);

        auto body = FD->getBody();
        std::string funcName = FD->getNameAsString();
        std::string recursiveCall = "\n/*mut91*/" + funcName + "();\n";

        if (body) {
            std::string bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            size_t pos = bodyStr.find_last_of('}');
            if (pos != std::string::npos) {
                bodyStr.insert(pos, recursiveCall);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
            }
        }
    }
}

void MutatorFrontendAction_91::MutatorASTConsumer_91::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}