//source file
#include "../include/insert_recursive_template_instantiation_273.h"

// ========================================================================================================
#define MUT273_OUTPUT 1

void MutatorFrontendAction_273::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateDeclStr = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        // Perform mutation by inserting a recursive instantiation
        std::string recursiveInstantiation = "\n/*mut273*/template<> class " + TD->getNameAsString() + "<0> { public: " + TD->getNameAsString() + "<1> next; };";
        templateDeclStr += recursiveInstantiation;

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDeclStr);
    }
}

void MutatorFrontendAction_273::MutatorASTConsumer_273::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}