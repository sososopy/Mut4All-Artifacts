//source file
#include "../include/insert_infinite_recursive_function_call_383.h"

// ========================================================================================================
#define MUT383_OUTPUT 1

void MutatorFrontendAction_383::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody() && FD->getReturnType()->isVoidType()) {
        //Get the source code text of target node
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string recursiveCall = FD->getNameAsString() + "(); /*mut383*/";
        functionBody.insert(functionBody.find("{") + 1, recursiveCall);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_383::MutatorASTConsumer_383::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition(), returns(voidType())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}