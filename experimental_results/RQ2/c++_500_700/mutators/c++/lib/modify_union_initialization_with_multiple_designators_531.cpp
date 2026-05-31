//source file
#include "../include/modify_union_initialization_with_multiple_designators_531.h"

// ========================================================================================================
#define MUT531_OUTPUT 1

void MutatorFrontendAction_531::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UC = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("UnionInit")) {
      //Filter nodes in header files
      if (!UC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UC->getLocation()))
        return;
      //Get the source code text of target node
      auto initList = stringutils::rangetoStr(*(Result.SourceManager),
                                              UC->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (initList.find("{") != std::string::npos) {
          auto pos = initList.find("}");
          if (pos != std::string::npos) {
              initList.insert(pos, ", .b = 3.14");
          }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UC->getSourceRange()), initList);
    }
}
  
void MutatorFrontendAction_531::MutatorASTConsumer_531::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxConstructExpr(hasType(recordDecl(isUnion()))).bind("UnionInit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}