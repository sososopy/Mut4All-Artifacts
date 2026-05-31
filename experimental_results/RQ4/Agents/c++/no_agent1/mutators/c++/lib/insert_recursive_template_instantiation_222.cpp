//source file
#include "../include/insert_recursive_template_instantiation_222.h"

// ========================================================================================================
#define MUT222_OUTPUT 1

void MutatorFrontendAction_222::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto templateName = TD->getNameAsString();
        if (visitedTemplates.find(templateName) != visitedTemplates.end())
            return;

        visitedTemplates.insert(templateName);

        std::string recursiveInstantiation = "template<> struct " + templateName + "<0> { static const int value = 0; };\n";
        recursiveInstantiation += "template<int N> struct " + templateName + " { static const int value = " + templateName + "<N-1>::value + 1; };\n";

        SourceLocation insertLoc = TD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut222*/" + recursiveInstantiation);
    }
}

void MutatorFrontendAction_222::MutatorASTConsumer_222::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}