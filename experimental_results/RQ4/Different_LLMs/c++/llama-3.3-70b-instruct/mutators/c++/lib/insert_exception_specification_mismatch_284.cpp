//source file
#include "../include/Mutator_Insert_Exception_Specification_Mismatch_284.h"

// ========================================================================================================
#define MUT284_OUTPUT 1

void MutatorFrontendAction_284::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (FD->getExceptionSpecType() != clang::EST_None) {
        auto exceptionSpec = FD->getExceptionSpecType();
        if (exceptionSpec == clang::EST_BasicNoexcept) {
          declaration.replace(declaration.find("throw()"), 7, "throw(std::runtime_error)");
        } else if (exceptionSpec == clang::EST_Dynamic) {
          auto exceptionType = "std::invalid_argument";
          declaration.replace(declaration.find("throw("), 6, "throw(" + std::string(exceptionType) + ")");
        }
      } else {
        declaration.replace(declaration.find(")"), 1, ") throw(std::runtime_error)");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_284::MutatorASTConsumer_284::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}