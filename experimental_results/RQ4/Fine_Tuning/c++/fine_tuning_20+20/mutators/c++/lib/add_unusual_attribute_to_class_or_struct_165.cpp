//source file
#include "../include/add_unusual_attribute_to_class_or_struct_165.h"

// ========================================================================================================
#define MUT165_OUTPUT 1

void MutatorFrontendAction_165::Callback::run(const MatchFinder::MatchResult &Result) {
  //Check whether the matched AST node is the target node
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    //Filter nodes in header files
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    //Get the source code text of target node
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    //Perform mutation on the source code text by applying string replacement
    content += " __attribute__((dllexport))";
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  }
}

void MutatorFrontendAction_165::MutatorASTConsumer_165::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}