//source file
#include "../include/insert_recursive_template_instantiation_128.h"

// ========================================================================================================
#define MUT128_OUTPUT 1

void MutatorFrontendAction_128::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        // Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateName = TD->getNameAsString();
        if (templateName.empty())
            return;

        // Perform mutation on the source code text by applying string replacement
        std::string recursiveTemplate = "template<int N> struct " + templateName + " {\n"
                                        "  enum { value = " + templateName + "<N-1>::value };\n"
                                        "};\n"
                                        "template<> struct " + templateName + "<0> {\n"
                                        "  enum { value = 0 };\n"
                                        "};\n";

        std::string insertion = "/*mut128*/" + recursiveTemplate;
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(TD->getSourceRange(), insertion);
    }
}

void MutatorFrontendAction_128::MutatorASTConsumer_128::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}