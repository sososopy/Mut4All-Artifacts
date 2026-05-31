//source file
#include "../include/insert_nested_sizeof_in_function_585.h"

// ========================================================================================================
#define MUT585_OUTPUT 1

void MutatorFrontendAction_585::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        auto param = FD->parameters();
        if (!param.empty()) {
          auto paramName = param[0]->getNameAsString();
          std::string newBody = "{ return sizeof(sizeof(sizeof(" + paramName + "))); }";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), newBody);
        }
      }
    }
}

void MutatorFrontendAction_585::MutatorASTConsumer_585::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody(stmt())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}