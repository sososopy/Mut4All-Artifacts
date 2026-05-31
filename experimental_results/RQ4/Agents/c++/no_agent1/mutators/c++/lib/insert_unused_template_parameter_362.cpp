//source file
#include "../include/insert_unused_template_parameter_362.h"

// ========================================================================================================
#define MUT362_OUTPUT 1

void MutatorFrontendAction_362::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        // Check if the function is templated
        if (FD->getDescribedFunctionTemplate()) {
            return;
        }

        // Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());

        // Perform mutation by adding an unused template parameter
        std::string templatePrefix = "template<typename UnusedType362> ";
        declaration.insert(0, templatePrefix);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_362::MutatorASTConsumer_362::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(cxxMethodDecl())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}