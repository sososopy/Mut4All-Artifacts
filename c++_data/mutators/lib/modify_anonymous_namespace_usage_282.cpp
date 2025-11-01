//source file
#include "../include/Modify_Anonymous_Namespace_Usage_282.h"

// ========================================================================================================
#define MUT282_OUTPUT 1

void MutatorFrontendAction_282::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("anonymousNamespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        if (NS->isAnonymousNamespace()) {
            auto startLoc = NS->getBeginLoc();
            auto endLoc = NS->getRBraceLoc().getLocWithOffset(1);
            auto sourceRange = CharSourceRange::getCharRange(startLoc, endLoc);
            auto namespaceText = Lexer::getSourceText(sourceRange, *(Result.SourceManager), Result.Context->getLangOpts());

            std::string modifiedNamespaceText = "inline namespace " + uniqueNamespaceName + " " + namespaceText.str().substr(9); // Replace "namespace " with "inline namespace UniqueNamespace_282 "

            Rewrite.ReplaceText(sourceRange, modifiedNamespaceText);
        }
    }
}

void MutatorFrontendAction_282::MutatorASTConsumer_282::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(hasName("")).bind("anonymousNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}