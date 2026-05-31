//source file
#include "../include/insert_recursive_template_instantiation_347.h"

// ========================================================================================================
#define MUT347_OUTPUT 1

void MutatorFrontendAction_347::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateName = TD->getNameAsString();
        templateNames.push_back(templateName);

        // Perform mutation by adding a recursive instantiation
        std::string recursiveInstance = "template<> class " + templateName + "<0> { public: static const int value = " + templateName + "<1>::value; };";
        std::string recursiveInstance2 = "template<> class " + templateName + "<1> { public: static const int value = " + templateName + "<0>::value; };";
        std::string mutationText = "\n/*mut347*/" + recursiveInstance + "\n" + recursiveInstance2 + "\n";
        
        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), mutationText);
    }
}

void MutatorFrontendAction_347::MutatorASTConsumer_347::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}