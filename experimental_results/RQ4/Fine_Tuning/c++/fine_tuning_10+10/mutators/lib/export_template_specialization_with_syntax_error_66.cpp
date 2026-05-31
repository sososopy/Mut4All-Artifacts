//source file
#include "../include/Mutator_66.h"

// ========================================================================================================
#define MUT66_OUTPUT 1

void MutatorFrontendAction_66::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        // Get the source code text of target node
        auto specialization = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string mutatedSpecialization = "export " + specialization;
        mutatedSpecialization.insert(mutatedSpecialization.find("template") + 8, " <");

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), mutatedSpecialization);
    }
}

void MutatorFrontendAction_66::MutatorASTConsumer_66::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}