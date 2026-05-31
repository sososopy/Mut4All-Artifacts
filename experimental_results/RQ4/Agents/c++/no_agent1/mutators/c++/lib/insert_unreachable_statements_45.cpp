//source file
#include "../include/insert_unreachable_statements_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

            // Insert unreachable statements at the beginning of the function body
            std::string unreachableCode = "\n/*mut45*/ if (false) { __builtin_unreachable(); }\n";
            bodyStr.insert(bodyStr.find("{") + 1, unreachableCode);

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
        }
    }
}
  
void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}