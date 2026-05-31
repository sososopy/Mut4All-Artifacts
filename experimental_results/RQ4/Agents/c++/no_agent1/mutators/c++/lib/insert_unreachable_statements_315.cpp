//source file
#include "../include/insert_unreachable_statements_315.h"

// ========================================================================================================
#define MUT315_OUTPUT 1

void MutatorFrontendAction_315::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

            // Insert unreachable code after each return statement
            std::string unreachableCode = "\n/*mut315*/if (false) { __builtin_unreachable(); }\n";
            size_t pos = 0;
            while ((pos = bodyText.find("return", pos)) != std::string::npos) {
                size_t endPos = bodyText.find(";", pos);
                if (endPos != std::string::npos) {
                    bodyText.insert(endPos + 1, unreachableCode);
                    pos = endPos + unreachableCode.length();
                } else {
                    break;
                }
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}
  
void MutatorFrontendAction_315::MutatorASTConsumer_315::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}