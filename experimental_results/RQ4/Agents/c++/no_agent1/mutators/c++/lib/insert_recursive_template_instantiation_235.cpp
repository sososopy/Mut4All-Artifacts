//source file
#include "../include/insert_recursive_template_instantiation_235.h"

// ========================================================================================================
#define MUT235_OUTPUT 1

void MutatorFrontendAction_235::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->isThisDeclarationADefinition()) {
            auto templateName = TD->getNameAsString();
            std::string recursiveTemplate = "\n/*mut235*/template<int N> struct " + templateName + " { " +
                                            "static const int value = " + templateName + "<N-1>::value; }; " +
                                            "template<> struct " + templateName + "<0> { static const int value = 0; };";
            Rewrite.InsertTextAfterToken(TD->getEndLoc(), recursiveTemplate);
        }
    }
}

void MutatorFrontendAction_235::MutatorASTConsumer_235::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}