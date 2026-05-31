//source file
#include "../include/modify_function_template_return_type_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        // Filter nodes in header files
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;
        
        if (auto *FD = FTD->getTemplatedDecl()) {
            auto returnType = FD->getReturnType();
            if (returnType->isUndeducedAutoType()) {
                // Perform mutation on the source code text by applying string replacement
                std::string newReturnType = "int"; // Default to int if no suitable type is found
                Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), newReturnType);
            }
        }
    }
}

void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(functionDecl(returns(autoType())))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}