//source file
#include "../include/insert_unreachable_code_block_414.h"

// ========================================================================================================
#define MUT414_OUTPUT 1

void MutatorFrontendAction_414::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            if (body) {
                auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
                std::string unreachableCode = "\n/*mut414*/if (false) {\n    int unreachable_var = 0;\n}\n";
                bodyStr.insert(bodyStr.find("{") + 1, unreachableCode);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
            }
        }
    }
}

void MutatorFrontendAction_414::MutatorASTConsumer_414::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}