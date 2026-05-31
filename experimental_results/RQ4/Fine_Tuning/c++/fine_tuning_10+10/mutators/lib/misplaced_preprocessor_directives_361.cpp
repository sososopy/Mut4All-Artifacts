//source file
#include "../include/Misplaced_Preprocessor_Directives_361.h"

// ========================================================================================================
#define MUT361_OUTPUT 1

void MutatorFrontendAction_361::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithBody")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto bodyRange = FD->getBody()->getSourceRange();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);

            std::string preprocessorDirective = "\n#include <iostream>\n";
            size_t insertionPoint = bodyText.find("{") + 1;
            bodyText.insert(insertionPoint, preprocessorDirective);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
        }
    }
}

void MutatorFrontendAction_361::MutatorASTConsumer_361::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("FunctionWithBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}