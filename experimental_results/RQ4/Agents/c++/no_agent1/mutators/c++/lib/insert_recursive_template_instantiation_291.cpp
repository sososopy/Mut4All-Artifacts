//source file
#include "../include/insert_recursive_template_instantiation_291.h"

// ========================================================================================================
#define MUT291_OUTPUT 1

void MutatorFrontendAction_291::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        std::string templateName = TD->getNameAsString();
        if (processedTemplates.find(templateName) != processedTemplates.end())
            return;

        processedTemplates.insert(templateName);

        std::string recursiveInstantiation = "template<> class " + templateName + "<" + templateName + "<0>> {};";
        SourceLocation insertLoc = TD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "\n/*mut291*/" + recursiveInstantiation + "\n");
    }
}

void MutatorFrontendAction_291::MutatorASTConsumer_291::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}