//source file
#include "../include/modify_template_specialization_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
        if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(CTSD->getLocation()))
            return;

        auto specializationSource = stringutils::rangetoStr(*(Result.SourceManager), CTSD->getSourceRange());
        std::string conflictingSpecialization = "template<typename T> class " + CTSD->getNameAsString() + "<T*> { /*mut19*/ };";
        Rewrite.InsertTextAfter(CTSD->getEndLoc(), "\n" + conflictingSpecialization);
    }
}

void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}