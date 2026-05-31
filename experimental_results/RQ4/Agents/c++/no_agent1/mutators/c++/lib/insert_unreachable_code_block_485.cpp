//source file
#include "../include/insert_unreachable_code_block_485.h"

// ========================================================================================================
#define MUT485_OUTPUT 1

void MutatorFrontendAction_485::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

            std::string unreachableCode = "\n/*mut485*/if (false) {\n"
                                          "  int unreachable_var = 0;\n"
                                          "  unreachable_var++;\n"
                                          "}\n";

            bodyText.insert(bodyText.find("{") + 1, unreachableCode);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_485::MutatorASTConsumer_485::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}