//source file
#include "../include/misplaced_incomplete_template_specialization_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                        CTD->getLocation()))
            return;

        auto templateName = CTD->getNameAsString();
        std::string specialization = "template<> class " + templateName + "<int>"; // Incomplete specialization

        auto insertLocation = CTD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLocation, "\n/*mut33*/" + specialization);
    }
}

void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}