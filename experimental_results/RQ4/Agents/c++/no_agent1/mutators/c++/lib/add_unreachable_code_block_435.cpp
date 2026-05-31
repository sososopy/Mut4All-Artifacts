//source file
#include "../include/Add_Unreachable_Code_Block_435.h"

// ========================================================================================================
#define MUT435_OUTPUT 1

void MutatorFrontendAction_435::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                            FD->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string unreachableCode = "\nif (false) {\n/*mut435*/\nint unreachable = 0;\n}\n";
        body.insert(body.find("{") + 1, unreachableCode);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), body);
      }
    }
}
  
void MutatorFrontendAction_435::MutatorASTConsumer_435::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}