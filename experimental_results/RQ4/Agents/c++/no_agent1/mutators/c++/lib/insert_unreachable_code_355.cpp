//source file
#include "../include/insert_unreachable_code_355.h"

// ========================================================================================================
#define MUT355_OUTPUT 1

void MutatorFrontendAction_355::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            if (body) {
                // Get the source code text of target node
                auto functionBody = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
                // Perform mutation on the source code text by applying string replacement
                std::string unreachableCode = "\n/*mut355*/if (false) { __builtin_unreachable(); }\n";
                functionBody.insert(functionBody.find("{") + 1, unreachableCode);
                // Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), functionBody);
            }
        }
    }
}

void MutatorFrontendAction_355::MutatorASTConsumer_355::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}