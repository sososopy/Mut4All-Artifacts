//source file
#include "../include/increase_namespace_nesting_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(NSD->getLocation()))
            return;

        auto namespaceName = NSD->getNameAsString();
        if (namespaceName.empty())
            namespaceName = "extra";

        std::string newNamespace = "\nnamespace " + namespaceName + " {\n";
        std::string endNamespace = "\n} // namespace " + namespaceName + ";\n";

        auto startLoc = NSD->getRBraceLoc();
        Rewrite.InsertTextAfterToken(startLoc, newNamespace);
        Rewrite.InsertTextAfterToken(startLoc, endNamespace);
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl().bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}