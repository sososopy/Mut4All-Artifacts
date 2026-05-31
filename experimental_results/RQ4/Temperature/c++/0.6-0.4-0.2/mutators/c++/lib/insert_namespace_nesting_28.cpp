//source file
#include "../include/insert_namespace_nesting_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NestedNS")) {
        if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       NSD->getLocation()))
            return;

        // Get the source code text of target node
        auto nsContent = stringutils::rangetoStr(*(Result.SourceManager), NSD->getSourceRange());

        // Perform mutation by adding a new namespace
        std::string newNamespace = "namespace X { int dummyVar; }";
        nsContent.insert(nsContent.find("{") + 1, "\n/*mut28*/" + newNamespace + "\n");

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(NSD->getSourceRange()), nsContent);
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(hasDescendant(namespaceDecl())).bind("NestedNS");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}