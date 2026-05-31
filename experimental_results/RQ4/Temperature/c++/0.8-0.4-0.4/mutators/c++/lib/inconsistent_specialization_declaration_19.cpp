//source file
#include "../include/inconsistent_specialization_declaration_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;

        std::string templateName = CTD->getNameAsString();
        if (specializedTemplates.find(templateName) != specializedTemplates.end())
            return;

        specializedTemplates.insert(templateName);
        auto loc = CTD->getEndLoc();
        std::string specialization = "template<typename T> class " + templateName + "<T*> { /*mut19*/ };";
        Rewrite.InsertTextAfterToken(loc, "\n" + specialization);
    }
    else if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ClassTemplateSpecialization")) {
        if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(CTSD->getLocation()))
            return;

        std::string templateName = CTSD->getSpecializedTemplate()->getNameAsString();
        if (specializedTemplates.find(templateName) == specializedTemplates.end())
            return;

        auto loc = CTSD->getEndLoc();
        std::string conflictingSpecialization = "template<typename T> class " + templateName + "<T*> { /*mut19*/ };";
        Rewrite.InsertTextAfterToken(loc, "\n" + conflictingSpecialization);
    }
}

void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto classTemplateMatcher = classTemplateDecl().bind("ClassTemplate");
    auto classTemplateSpecializationMatcher = classTemplateSpecializationDecl().bind("ClassTemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classTemplateMatcher, &callback);
    matchFinder.addMatcher(classTemplateSpecializationMatcher, &callback);
    matchFinder.matchAST(Context);
}