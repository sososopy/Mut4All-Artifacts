//source file
#include "../include/insert_unreachable_code_329.h"

// ========================================================================================================
#define MUT329_OUTPUT 1

void MutatorFrontendAction_329::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            targetFunctions.push_back(FD);
            auto body = FD->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            std::string unreachableCode = "\n/*mut329*/if (false) { __builtin_unreachable(); }\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableCode);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_329::MutatorASTConsumer_329::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}