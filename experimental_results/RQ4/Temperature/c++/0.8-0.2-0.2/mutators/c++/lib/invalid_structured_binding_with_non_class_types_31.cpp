//source file
#include "../include/invalid_structured_binding_with_non_class_types_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DS = Result.Nodes.getNodeAs<clang::DecompositionDecl>("decompDecl")) {
        if (!DS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DS->getLocation()))
            return;
        
        // Get the source code text of target node
        auto declSource = stringutils::rangetoStr(*(Result.SourceManager), DS->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        // Change the right-hand side to a non-class type
        std::string replacement = "char c = 'x';\n" + declSource;
        replacement.replace(replacement.find("=") + 1, std::string::npos, " c;");

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DS->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = decompositionDecl().bind("decompDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}