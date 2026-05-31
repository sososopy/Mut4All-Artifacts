//source file
#include "../include/insert_recursive_template_instantiation_492.h"

// ========================================================================================================
#define MUT492_OUTPUT 1

void MutatorFrontendAction_492::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateName = TD->getNameAsString();
        auto templateDef = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        // Perform mutation by adding a recursive template instantiation
        std::string recursiveInst = "\n/*mut492*/template<> struct " + templateName + "<0> { using type = " + templateName + "<1>; };";
        templateDef += recursiveInst;

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDef);
        templateClasses.push_back(TD->getTemplatedDecl());
    }
}

void MutatorFrontendAction_492::MutatorASTConsumer_492::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}