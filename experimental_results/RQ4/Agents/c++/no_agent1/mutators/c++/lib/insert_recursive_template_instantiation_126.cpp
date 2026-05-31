//source file
#include "../include/insert_recursive_template_instantiation_126.h"

// ========================================================================================================
#define MUT126_OUTPUT 1

void MutatorFrontendAction_126::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto templateName = TD->getNameAsString();
        if (visitedTemplates.find(templateName) != visitedTemplates.end())
            return;
        
        visitedTemplates.insert(templateName);

        auto templateSrc = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        std::string recursiveInstance = "\n/*mut126*/template<>\nstruct " + templateName + "<0> {\n"
                                        "  using type = " + templateName + "<0>;\n};\n";

        if (templateSrc.rfind('}') != std::string::npos)
            templateSrc.insert(templateSrc.rfind('}'), recursiveInstance);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateSrc);
    }
}

void MutatorFrontendAction_126::MutatorASTConsumer_126::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}