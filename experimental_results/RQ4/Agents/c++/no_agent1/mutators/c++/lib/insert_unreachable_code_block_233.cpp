//source file
#include "../include/insert_unreachable_code_block_233.h"

// ========================================================================================================
#define MUT233_OUTPUT 1

void MutatorFrontendAction_233::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        //Filter nodes in header files
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FS->getLocation()))
            return;

        if (FS->hasBody()) {
            //Get the source code text of target node
            auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                        FS->getBody()->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            std::string unreachableBlock = "\n/*mut233*/if (false) {\n    int unreachable = 0;\n}\n";
            functionBody.insert(functionBody.find("{") + 1, unreachableBlock);

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getBody()->getSourceRange()), functionBody);
        }
    }
}

void MutatorFrontendAction_233::MutatorASTConsumer_233::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}