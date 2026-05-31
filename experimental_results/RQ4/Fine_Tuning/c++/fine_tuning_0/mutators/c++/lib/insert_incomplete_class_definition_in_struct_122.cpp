//source file
#include "../include/Insert_Incomplete_Class_Definition_In_Struct_122.h"

// ========================================================================================================
#define MUT122_OUTPUT 1

void MutatorFrontendAction_122::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (MT->isStruct() == false)
        return;
      if (MT->isCompleteDefinition() == false)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto insert_content = "class A ; ";
      if (content.find('{') != string::npos)
        content.insert(content.find('{') + 1, insert_content);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_122::MutatorASTConsumer_122::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}