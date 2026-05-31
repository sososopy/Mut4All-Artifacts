//source file
#include "../include/insert_recursive_function_call_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody() || FD->isMain())
            return;
        
        if (visitedFunctions.find(FD) == visitedFunctions.end()) {
            visitedFunctions.insert(FD);
            auto body = FD->getBody();
            std::string funcName = FD->getNameAsString();
            std::string recursiveCall = "\n/*mut4*/" + funcName + "();\n";
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            if (bodyText.find("return") == std::string::npos) {
                bodyText.insert(bodyText.rfind('}'), recursiveCall);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
            }
        }
    }
}

void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}