//source file
#include "../include/introduce_incomplete_enum_in_template_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("templateDecl")) {
        // Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateDeclStr = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        // Introduce an incomplete enum class into the template
        std::string enumDeclaration = "enum class IncompleteEnum;";
        std::string insertion = "IncompleteEnum";

        // Locate the position to insert the incomplete enum
        size_t pos = templateDeclStr.find("template <");
        if (pos != std::string::npos) {
            pos = templateDeclStr.find(">", pos);
            if (pos != std::string::npos) {
                templateDeclStr.insert(pos + 1, ", typename " + insertion);
                templateDeclStr = enumDeclaration + "\n" + templateDeclStr;
            }
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDeclStr);
    }
}

void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(has(templateTypeParmDecl())).bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}