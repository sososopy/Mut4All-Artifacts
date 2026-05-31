//source file
#include "../include/replace_function_parameter_type_with_ambiguous_enum_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        // Ensure the function has parameters
        if (FD->getNumParams() == 0)
            return;

        // Get the first parameter
        auto *param = FD->getParamDecl(0);

        // Create an ambiguous enum declaration
        std::string enumDecl = "enum AmbiguousEnum { Value1 };";

        // Replace the parameter type with the ambiguous enum
        std::string newParamType = "AmbiguousEnum";
        std::string paramName = param->getNameAsString();
        std::string newParamDecl = newParamType + " " + paramName;

        // Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(param->getSourceRange(), newParamDecl);

        // Insert the enum declaration at the beginning of the function's scope
        SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, enumDecl + "\n");
    }
}

void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(0, hasType(isInteger()))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}