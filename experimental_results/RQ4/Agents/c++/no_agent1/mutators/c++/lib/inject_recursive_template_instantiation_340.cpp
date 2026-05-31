//source file
#include "../include/inject_recursive_template_instantiation_340.h"

// ========================================================================================================
#define MUT340_OUTPUT 1

void MutatorFrontendAction_340::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isThisDeclarationADefinition())
            return;

        auto templateName = TD->getNameAsString();
        std::string recursiveTemplate = "template<int N> struct " + templateName + " { enum { value = " + templateName + "<N-1>::value }; };\n";
        recursiveTemplate += "template<> struct " + templateName + "<0> { enum { value = 0 }; };\n";
        recursiveTemplate = "/*mut340*/" + recursiveTemplate;

        Rewrite.InsertTextAfterToken(TD->getEndLoc(), recursiveTemplate);
    }
}

void MutatorFrontendAction_340::MutatorASTConsumer_340::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}