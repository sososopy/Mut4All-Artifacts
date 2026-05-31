//source file
#include "../include/Replace_Noexcept_Specifier_259.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Type.h" // Added necessary include

// ========================================================================================================
#define MUT259_OUTPUT 1

void MutatorFrontendAction_259::Callback::run(const MatchFinder::MatchResult &Result) {
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
      if (FD->getExceptionSpecType() == clang::EST_BasicNoexcept) {
        declaration.replace(declaration.find("noexcept"), 7, "throw(std::exception)");
      } else if (FD->getExceptionSpecType() == clang::EST_DynamicNone) {
        declaration.replace(declaration.find("throw()"), 7, "noexcept");
      } else {
        declaration.replace(declaration.find("throw(std::exception)"), 17, "");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_259::MutatorASTConsumer_259::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}