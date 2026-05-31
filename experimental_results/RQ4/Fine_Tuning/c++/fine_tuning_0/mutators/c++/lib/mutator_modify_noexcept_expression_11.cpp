//source file
#include "../include/Mutator_Modify_Noexcept_Expression_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ND = Result.Nodes.getNodeAs<clang::CXXNoexceptExpr>("Noexcept")) {
      //Filter nodes in header files
      if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ND->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ND->getSourceRange());
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      content = "noexcept(std::declval<int>().mut11())";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(ND->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    auto noexcept_matcher = cxxNoexceptExpr().bind("Noexcept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(noexcept_matcher, &callback);
    matchFinder.matchAST(Context);
}