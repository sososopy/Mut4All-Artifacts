//source file
#include "../include/modify_template_specialization_150.h"

// ========================================================================================================
#define MUT150_OUTPUT 1

void MutatorFrontendAction_150::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpec")) {
        if (!TS)
            return;

        const TemplateDecl *templateDecl = TS->getTemplateName().getAsTemplateDecl();
        if (!templateDecl)
            return;

        templateSpecializations[templateDecl].push_back(TS);

        // Perform mutation by altering the template argument
        if (templateSpecializations[templateDecl].size() == 1) {
            std::string mutatedSpec = "void(*)(int)";
            if (const auto *TST = Result.Nodes.getNodeAs<clang::TypeLoc>("TemplateSpecLoc")) {
                auto sourceRange = TST->getSourceRange();
                std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
                std::string mutatedText = originalText;
                size_t start = mutatedText.find('<') + 1;
                size_t end = mutatedText.find('>');
                mutatedText.replace(start, end - start, mutatedSpec);

                Rewrite.ReplaceText(sourceRange, mutatedText);
            }
        }
    }
}

void MutatorFrontendAction_150::MutatorASTConsumer_150::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    auto matcher = templateSpecializationType().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}