//source file
#include "../include/insert_unreachable_code_block_168.h"

// ========================================================================================================
#define MUT168_OUTPUT 1

void MutatorFrontendAction_168::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                            FD->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string unreachableBlock = "\n/*mut168*/if (false) { volatile int dummy = 0; dummy++; }\n";
        body.insert(body.find("{") + 1, unreachableBlock);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), body);
      }
    }
}
  
void MutatorFrontendAction_168::MutatorASTConsumer_168::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}