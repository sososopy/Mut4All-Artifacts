//source file
#include "../include/inject_recursive_template_instantiation_352.h"

// ========================================================================================================
#define MUT352_OUTPUT 1

void MutatorFrontendAction_352::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        
        if (visitedTemplates.find(TD->getTemplatedDecl()) != visitedTemplates.end())
            return;

        visitedTemplates.insert(TD->getTemplatedDecl());

        // Get the source code text of target node
        auto templateName = TD->getNameAsString();
        std::string recursiveInstantiation = "\n/*mut352*/template<> struct " + templateName + "<0> { using type = " + templateName + "<1>; };";

        // Perform mutation on the source code text by applying string replacement
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), recursiveInstantiation);
    }
}

void MutatorFrontendAction_352::MutatorASTConsumer_352::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}