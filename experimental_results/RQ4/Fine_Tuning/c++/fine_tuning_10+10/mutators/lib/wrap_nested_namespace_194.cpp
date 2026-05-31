//source file
#include "../include/Wrap_Nested_Namespace_194.h"

// ========================================================================================================
#define MUT194_OUTPUT 1

void MutatorFrontendAction_194::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NestedNamespace")) {
        if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NSD->getLocation()))
            return;

        auto &SM = *Result.SourceManager;
        auto startLoc = NSD->getBeginLoc();
        auto endLoc = NSD->getEndLoc();

        std::string originalCode = Lexer::getSourceText(CharSourceRange::getTokenRange(startLoc, endLoc), SM, LangOptions()).str();
        std::string wrappedCode = "namespace wrapper {\n" + originalCode + "\n}";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), wrappedCode);
    }
}
  
void MutatorFrontendAction_194::MutatorASTConsumer_194::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(has(namespaceDecl())).bind("NestedNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}