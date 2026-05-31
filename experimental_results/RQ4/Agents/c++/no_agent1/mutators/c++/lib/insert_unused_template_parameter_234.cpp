//source file
#include "../include/insert_unused_template_parameter_234.h"

// ========================================================================================================
#define MUT234_OUTPUT 1

void MutatorFrontendAction_234::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (processedFunctions.find(FD) != processedFunctions.end())
            return;

        processedFunctions.insert(FD);

        // Get the source code text of target node
        auto functionText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = "template<typename UnusedParam>\n";
        functionText.insert(0, templateParam);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}

void MutatorFrontendAction_234::MutatorASTConsumer_234::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}