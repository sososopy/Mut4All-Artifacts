//source file
#include "../include/insert_recursive_template_instantiation_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->isThisDeclarationADefinition()) {
            auto templateName = TD->getNameAsString();
            std::string recursiveTemplate = "template<int N> struct " + templateName + " {\n";
            recursiveTemplate += "static const int value = " + templateName + "<N-1>::value;\n";
            recursiveTemplate += "};\n";
            recursiveTemplate += "template<> struct " + templateName + "<0> {\n";
            recursiveTemplate += "static const int value = 0;\n";
            recursiveTemplate += "};\n";
            recursiveTemplate = "/*mut140*/" + recursiveTemplate;

            Rewrite.InsertTextAfter(TD->getEndLoc(), recursiveTemplate);
        }
    }
}

void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}