//source file
#include "../include/insert_unused_variable_and_dead_code_385.h"

// ========================================================================================================
#define MUT385_OUTPUT 1

void MutatorFrontendAction_385::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        if (FD->hasBody()) {
            auto body = FD->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            
            // Insert an unused variable and dead code
            std::string mutation = "\n/*mut385*/ int unusedVar = 0; if (unusedVar) { return; }\n";
            bodyText.insert(bodyText.find("{") + 1, mutation);
            
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_385::MutatorASTConsumer_385::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}