//source file
#include "../include/replace_virtual_function_with_pure_virtual_397.h"

// ========================================================================================================
#define MUT397_OUTPUT 1

void MutatorFrontendAction_397::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("VirtualMethods")) {
        // Filter nodes in header files
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;

        if (MD->isVirtual() && !MD->isPure()) {
            // Get the source code text of target node
            auto methodDecl = stringutils::rangetoStr(*(Result.SourceManager), MD->getSourceRange());
            // Perform mutation on the source code text by applying string replacement
            methodDecl += " = 0";
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), methodDecl);
        }
    }
}

void MutatorFrontendAction_397::MutatorASTConsumer_397::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxMethodDecl(isVirtual(), unless(isPure())).bind("VirtualMethods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}