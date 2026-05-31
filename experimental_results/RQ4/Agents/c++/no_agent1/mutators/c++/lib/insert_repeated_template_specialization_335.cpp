//source file
#include "../include/insert_repeated_template_specialization_335.h"

// ========================================================================================================
#define MUT335_OUTPUT 1

void MutatorFrontendAction_335::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto templateName = TD->getNameAsString();
        if (visitedTemplates.find(templateName) != visitedTemplates.end())
            return;

        visitedTemplates.insert(templateName);

        std::string specializationText = "template<> class " + templateName + "<int> { public: static const int value = 42; };";
        specializationText = "\n/*mut335*/" + specializationText;

        SourceLocation insertLoc = TD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertTextAfterToken(insertLoc, specializationText);
    }
}

void MutatorFrontendAction_335::MutatorASTConsumer_335::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}