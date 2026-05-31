//source file
#include "../include/modify_template_deduction_guide_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TDG = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDeductionGuide")) {
        if (!TDG || !Result.Context->getSourceManager().isWrittenInMainFile(TDG->getLocation()))
            return;

        // Assuming this should retrieve the templated function declaration
        auto *associatedTemplate = TDG->getTemplatedDecl();
        if (!associatedTemplate)
            return;

        if (const auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
            currentNamespace = NS;
        }

        std::string nsPrefix = currentNamespace ? currentNamespace->getNameAsString() + "::" : "";

        std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), TDG->getSourceRange());
        std::string modifiedText = "template <class U> " + nsPrefix + associatedTemplate->getNameAsString() + "() -> " + nsPrefix + associatedTemplate->getNameAsString() + "<U>;";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TDG->getSourceRange()), modifiedText);
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl(hasAncestor(namespaceDecl().bind("Namespace"))).bind("TemplateDeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}