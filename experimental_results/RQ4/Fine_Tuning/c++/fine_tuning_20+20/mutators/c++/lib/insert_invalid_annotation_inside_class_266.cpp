//source file
#include "../include/insert_invalid_annotation_inside_class_266.h"

// ========================================================================================================
#define MUT266_OUTPUT 1

void MutatorFrontendAction_266::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
      if (!DL->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto pragma = "\n#pragma clang annotation \"invalid\"\n";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), pragma);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_266::MutatorASTConsumer_266::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}