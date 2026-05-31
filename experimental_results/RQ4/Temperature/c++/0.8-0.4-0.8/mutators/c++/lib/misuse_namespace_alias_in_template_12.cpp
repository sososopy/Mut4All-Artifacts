//source file
#include "../include/misuse_namespace_alias_in_template_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        // Record the namespace alias declaration
        if (auto *Alias = Result.Nodes.getNodeAs<clang::NamespaceAliasDecl>("Alias")) {
            aliasDecl = Alias;
        }
    } else if (auto *TDG = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TDG || !Result.Context->getSourceManager().isWrittenInMainFile(TDG->getLocation()))
            return;

        if (aliasDecl) {
            //Get the source code text of target node
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), TDG->getSourceRange());
            std::string aliasName = aliasDecl->getNameAsString();
            std::string originalNamespace = aliasDecl->getNamespace()->getNameAsString();

            //Perform mutation on the source code text by applying string replacement
            size_t pos = declaration.find(originalNamespace + "::");
            if (pos != std::string::npos) {
                declaration.replace(pos, originalNamespace.length(), aliasName);
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(TDG->getSourceRange()), declaration);
            }
        }
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher namespaceMatcher = namespaceDecl().bind("Namespace");
    DeclarationMatcher aliasMatcher = namespaceAliasDecl().bind("Alias");
    DeclarationMatcher templateMatcher = clang::ast_matchers::decl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}