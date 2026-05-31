//source file
#include "../include/insert_recursive_template_instantiation_120.h"

// ========================================================================================================
#define MUT120_OUTPUT 1

void MutatorFrontendAction_120::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CTD->getLocation()))
            return;
        if (!CTD->isThisDeclarationADefinition())
            return;

        templates.push_back(CTD);

        // Get the source code text of target node
        auto templateName = CTD->getNameAsString();
        auto templateSource = stringutils::rangetoStr(*(Result.SourceManager), CTD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string recursiveInstantiation = "\n/*mut120*/template<> struct " + templateName + "<0> { using type = " + templateName + "<0>; };";
        templateSource += recursiveInstantiation;

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CTD->getSourceRange()), templateSource);
    }
}

void MutatorFrontendAction_120::MutatorASTConsumer_120::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}