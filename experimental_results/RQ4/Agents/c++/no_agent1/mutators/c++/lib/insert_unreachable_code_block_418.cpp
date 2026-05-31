//source file
#include "../include/insert_unreachable_code_block_418.h"

// ========================================================================================================
#define MUT418_OUTPUT 1

void MutatorFrontendAction_418::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("FunctionBody")) {
        // Filter nodes in header files
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FS->getBeginLoc()))
            return;

        // Get the source code text of the target node
        auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), FS->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string unreachableBlock = "\n/*mut418*/if (false) { throw \"Unreachable code executed!\"; }\n";
        bodyStr.insert(bodyStr.find("{") + 1, unreachableBlock);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), bodyStr);
    }
}

void MutatorFrontendAction_418::MutatorASTConsumer_418::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = compoundStmt().bind("FunctionBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}