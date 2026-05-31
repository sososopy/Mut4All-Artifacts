//source file
#include "../include/add_random_template_instantiation_320.h"

// ========================================================================================================
#define MUT320_OUTPUT 1

void MutatorFrontendAction_320::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClasses")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CTD->getLocation()))
            return;
        template_classes.push_back(CTD);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;

        if (template_classes.empty())
            return;

        size_t index = getrandom::getRandomIndex(template_classes.size());
        const clang::ClassTemplateDecl *chosenTemplate = template_classes[index];

        std::string templateName = chosenTemplate->getNameAsString();
        std::string instantiation = templateName + "<int> instance;";

        SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "/*mut320*/" + instantiation + "\n");
    }
}

void MutatorFrontendAction_320::MutatorASTConsumer_320::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("TemplateClasses");
    DeclarationMatcher functionMatcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}