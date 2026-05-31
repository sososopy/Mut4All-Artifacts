//source file
#include "../include/insert_recursive_template_instantiation_412.h"

// ========================================================================================================
#define MUT412_OUTPUT 1

void MutatorFrontendAction_412::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TD->getLocation()))
            return;

        std::string templateName = TD->getNameAsString();
        if (visitedTemplates.find(templateName) != visitedTemplates.end())
            return;

        visitedTemplates.insert(templateName);

        // Perform mutation to create recursive template instantiation
        std::string instantiation = "template<> class " + templateName + "<0> { public: static const int value = " + templateName + "<1>::value; };";
        std::string insertion = "/*mut412*/" + instantiation + "\n";

        // Insert the recursive instantiation after the template declaration
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), insertion);
    }
}

void MutatorFrontendAction_412::MutatorASTConsumer_412::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}