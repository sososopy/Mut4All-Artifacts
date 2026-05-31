//source file
#include "../include/Replace_Target_Attribute_401.h"

// ========================================================================================================
#define MUT401_OUTPUT 1

void MutatorFrontendAction_401::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string targetVersion = "avx2"; // Replace with another valid target version
      size_t pos = declaration.find("target_version");
      if (pos != std::string::npos) {
        size_t start = declaration.find('"', pos);
        size_t end = declaration.find('"', start + 1);
        if (start != std::string::npos && end != std::string::npos) {
          declaration.replace(start + 1, end - start - 1, targetVersion);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_401::MutatorASTConsumer_401::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr(attr::Target)).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}