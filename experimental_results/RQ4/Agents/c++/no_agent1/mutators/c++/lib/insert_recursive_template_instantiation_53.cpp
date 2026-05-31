//source file
#include "../include/insert_recursive_template_instantiation_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (visitedTemplates.find(TD) != visitedTemplates.end())
            return;

        visitedTemplates.insert(TD);

        auto templateName = TD->getNameAsString();
        if (templateName.empty())
            return;

        std::string recursiveInstantiation = "template<> struct " + templateName + "<0> { enum { value = " + templateName + "<0>::value }; };";
        recursiveInstantiation = "\n/*mut53*/" + recursiveInstantiation;

        Rewrite.InsertTextAfterToken(TD->getEndLoc(), recursiveInstantiation);
    }
}
  
void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}