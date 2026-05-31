//source file
#include "../include/add_unused_template_parameter_129.h"

// ========================================================================================================
#define MUT129_OUTPUT 1

void MutatorFrontendAction_129::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        //Filter nodes in header files
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                CTD->getLocation()))
            return;

        // Avoid processing the same template multiple times
        if (processedTemplates.find(CTD) != processedTemplates.end())
            return;
        processedTemplates.insert(CTD);

        // Get the source code text of target node
        auto templateDecl = CTD->getTemplatedDecl();
        if (!templateDecl)
            return;

        // Perform mutation on the source code text by applying string replacement
        std::string newTemplateParam = "typename UnusedParam = int, ";
        auto templateRange = CTD->getSourceRange();
        auto templateText = stringutils::rangetoStr(*(Result.SourceManager), templateRange);
        auto insertPos = templateText.find("template <") + 10;  // After "template <"
        templateText.insert(insertPos, newTemplateParam);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(templateRange), templateText);
    }
}

void MutatorFrontendAction_129::MutatorASTConsumer_129::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}