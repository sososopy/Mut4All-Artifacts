//source file
#include "../include/insert_unreachable_code_block_441.h"

// ========================================================================================================
#define MUT441_OUTPUT 1

void MutatorFrontendAction_441::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->hasBody()) {
            //Get the source code text of target node
            auto functionBody = FD->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    functionBody->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            std::string unreachableCode = "\n/*mut441*/if (false) {\n    int unreachable_var = 0;\n}\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableCode);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionBody->getSourceRange()), bodyText);
        }
    }
}
  
void MutatorFrontendAction_441::MutatorASTConsumer_441::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}