//source file
#include "../include/Insert_Unreachable_Code_Block_74.h"

// ========================================================================================================
#define MUT74_OUTPUT 1

void MutatorFrontendAction_74::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        // Filter nodes in header files
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
            return;
        
        // Get the source code text of target node
        if (FS->hasBody()) {
            auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                        FS->getBody()->getSourceRange());
            
            // Perform mutation by inserting an unreachable code block
            std::string unreachableBlock = "\n/*mut74*/if (false) { volatile int x = 0; while (true) { ++x; } }\n";
            functionBody.insert(functionBody.find("{") + 1, unreachableBlock);
            
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), functionBody);
        }
    }
}

void MutatorFrontendAction_74::MutatorASTConsumer_74::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(hasBody(stmt())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}