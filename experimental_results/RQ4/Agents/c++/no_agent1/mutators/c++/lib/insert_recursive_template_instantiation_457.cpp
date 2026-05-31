//source file
#include "../include/insert_recursive_template_instantiation_457.h"

// ========================================================================================================
#define MUT457_OUTPUT 1

void MutatorFrontendAction_457::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isThisDeclarationADefinition())
            return;

        auto templateName = TD->getNameAsString();
        std::string recursiveTemplate = 
            "template<int N>\n"
            "struct " + templateName + "_Recursive {\n"
            "    static const int value = " + templateName + "_Recursive<N-1>::value;\n"
            "};\n"
            "template<>\n"
            "struct " + templateName + "_Recursive<0> {\n"
            "    static const int value = 0;\n"
            "};\n";

        Rewrite.InsertTextAfter(TD->getEndLoc(), "\n/*mut457*/\n" + recursiveTemplate);
    }
}

void MutatorFrontendAction_457::MutatorASTConsumer_457::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}