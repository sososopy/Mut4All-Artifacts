//source file
#include "../include/insert_unclosed_namespace_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespaces")) {
        if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       NSD->getLocation()))
            return;

        auto startLoc = NSD->getBeginLoc();
        if (startLoc.isInvalid())
            return;

        // Perform mutation by adding an unclosed namespace
        std::string unclosedNamespace = "namespace MutatedNamespace /*mut28*/";
        Rewrite.InsertTextBefore(startLoc, unclosedNamespace);
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(hasParent(namespaceDecl())).bind("Namespaces");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}