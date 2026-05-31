//source file
#include "../include/add_basic_function_attributes_434.h"

// ========================================================================================================
#define MUT434_OUTPUT 1

void MutatorFrontendAction_434::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (decl.find("__attribute__") != string::npos) {
        if (decl.find("noreturn") != string::npos)
          decl.replace(decl.find("noreturn"), 8, "const");
        else if (decl.find("const") != string::npos)
          decl.replace(decl.find("const"), 5, "pure");
        else if (decl.find("pure") != string::npos)
          decl.replace(decl.find("pure"), 4, "noreturn");
      } else {
        decl += " __attribute__((noreturn))";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(FD->getSourceRange()), decl);
    }
}
  
void MutatorFrontendAction_434::MutatorASTConsumer_434::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}