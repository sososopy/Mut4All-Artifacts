//source file
#include "../include/template_class_specialization_with_constraints_532.h"

// ========================================================================================================
#define MUT532_OUTPUT 1

void MutatorFrontendAction_532::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;

        for (auto *spec : CTD->specializations()) {
            if (spec->isExplicitSpecialization())
                continue;

            auto templateName = CTD->getNameAsString();
            auto specSource = spec->getSourceRange().printToString(*Result.SourceManager);

            std::string newConstraint = "requires (std::is_integral_v<T>)";
            std::string newSpecialization = "template <typename T> " + newConstraint + " class " + templateName + "<T> { /* specialized behavior */ };";

            if (specSource.find(newConstraint) == std::string::npos) {
                Rewrite.InsertTextAfter(spec->getEndLoc(), "\n/*mut532*/" + newSpecialization);
            }
        }
    }
}

void MutatorFrontendAction_532::MutatorASTConsumer_532::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(isTemplateInstantiation()))).bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}