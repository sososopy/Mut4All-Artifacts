//source file
#include "../include/Mutator_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->hasBody()) {
        std::string externDecl;
        for (auto param : FD->parameters()) {
          //Get the source code text of target node
          auto paramType = param->getType().getAsString();
          auto paramName = param->getNameAsString();
          //Perform mutation on the source code text by applying string replacement
          externDecl += "extern " + paramType + " " + paramName + "; /*mut44*/\n";
        }
        //Replace the original AST node with the mutated one
        if (!externDecl.empty()) {
          auto bodyStart = FD->getBody()->getBeginLoc();
          Rewrite.InsertTextAfterToken(bodyStart, externDecl);
        }
      }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(parameterCountIs(1)).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}