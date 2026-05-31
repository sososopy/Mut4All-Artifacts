//source file
#include "../include/Replace_Friend_Template_Function_With_Non_Friend_425.h"

// ========================================================================================================
#define MUT425_OUTPUT 1

void MutatorFrontendAction_425::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto friend_pos = content.find("friend");
      if (friend_pos == string::npos)
        return;
      friend_pos = content.find("friend", friend_pos + 1);
      if (friend_pos == string::npos)
        return;
      auto func_pos = content.find("template", friend_pos);
      if (func_pos == string::npos)
        return;
      auto func_end_pos = content.find('}', func_pos);
      if (func_end_pos == string::npos)
        return;
      auto func_content = content.substr(func_pos, func_end_pos - func_pos + 1);
      content.erase(func_pos, func_end_pos - func_pos + 1);
      content += "\n/*mut425*/" + func_content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_425::MutatorASTConsumer_425::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}