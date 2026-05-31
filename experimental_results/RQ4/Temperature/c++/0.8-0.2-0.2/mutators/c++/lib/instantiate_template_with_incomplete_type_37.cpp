//source file
#include "../include/instantiate_template_with_incomplete_type_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        // Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        // Get the template name
        std::string templateName = TD->getNameAsString();
        
        // Forward declare an incomplete type
        std::string incompleteType = "class IncompleteType;";

        // Attempt to instantiate the template class with the incomplete type
        std::string mutation = "\nvoid testFunction() {\n    " + templateName + "<IncompleteType> testInstance;\n}\n";

        // Insert the incomplete type declaration and template instantiation
        SourceLocation loc = TD->getEndLoc();
        Rewrite.InsertTextAfter(loc, "\n/*mut37*/" + incompleteType + mutation);
    }
}

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}