//source file
#include "../include/insert_unreachable_code_block_426.h"

// ========================================================================================================
#define MUT426_OUTPUT 1

void MutatorFrontendAction_426::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto body = FD->getBody();
            if (body) {
                auto bodyText = stringutils::rangetoStr(*(Result.SourceManager),
                                                        body->getSourceRange());
                std::string unreachableCode = "\n/*mut426*/if (false) {\n    int unreachable_var = 0;\n}\n";
                bodyText.insert(bodyText.find("{") + 1, unreachableCode);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
            }
        }
    }
}

void MutatorFrontendAction_426::MutatorASTConsumer_426::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}