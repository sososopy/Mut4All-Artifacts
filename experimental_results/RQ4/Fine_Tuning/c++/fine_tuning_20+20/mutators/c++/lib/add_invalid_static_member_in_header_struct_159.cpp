//source file
#include "../include/Add_Invalid_Static_Member_In_Header_Struct_159.h"

// ========================================================================================================
#define MUT159_OUTPUT 1

void MutatorFrontendAction_159::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), ST->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto ins = "\n/*mut159*/static _S_mut159 mut_159;\n";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), ins);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_159::MutatorASTConsumer_159::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}