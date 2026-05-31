//source file
#include "../include/insert_recursive_template_instantiation_374.h"

// ========================================================================================================
#define MUT374_OUTPUT 1

void MutatorFrontendAction_374::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isThisDeclarationADefinition())
            return;

        auto templateName = TD->getNameAsString();
        auto templateParams = TD->getTemplateParameters();
        if (templateParams->size() == 0)
            return;

        auto paramName = templateParams->getParam(0)->getNameAsString();
        std::string recursiveInst = "template<> struct " + templateName + "<0> { using type = " + templateName + "<1>; };";
        std::string recursiveInstCommented = "/*mut374*/" + recursiveInst;

        Rewrite.InsertTextAfterToken(TD->getEndLoc(), "\n" + recursiveInstCommented);
    }
}

void MutatorFrontendAction_374::MutatorASTConsumer_374::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}