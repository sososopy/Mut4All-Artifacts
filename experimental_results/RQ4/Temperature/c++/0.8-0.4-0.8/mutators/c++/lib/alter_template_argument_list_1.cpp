//source file
#include "../include/alter_template_argument_list_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateFunctionCall")) {
        // Filter nodes in header files
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                CallExpr->getExprLoc()))
            return;

        if (auto *TemplateArgs = CallExpr->getDirectCallee()->getTemplateSpecializationArgs()) {
            unsigned numArgs = TemplateArgs->size();
            unsigned numParams = CallExpr->getDirectCallee()->getNumTemplateParameterLists();

            // Get the source code text of target node
            auto sourceRange = CallExpr->getSourceRange();
            auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

            // Perform mutation on the source code text by applying string replacement
            if (numArgs > numParams) {
                // Remove one argument
                sourceText = sourceText.substr(0, sourceText.find_last_of(',')) + ">";
            } else if (numArgs < numParams) {
                // Add a random type
                std::string randomType = random_types[getrandom::getRandomIndex(random_types.size())];
                sourceText.insert(sourceText.find_last_of('>'), ", " + randomType);
            }

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
        }
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("TemplateFunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}