//source file
#include "../include/incomplete_class_template_specialization_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                CT->getLocation()))
            return;

        auto templateName = CT->getNameAsString();
        if (specializedTemplates.find(templateName) != specializedTemplates.end())
            return;

        specializedTemplates.insert(templateName);

        std::string specialization = "template<> class " + templateName + "<int>;";
        Rewrite.InsertTextAfterToken(CT->getEndLoc(), "\n/*mut33*/" + specialization);
    }
}

void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}