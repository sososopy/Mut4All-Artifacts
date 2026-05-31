//source file
#include "../include/Enum_Forward_Declaration_in_Template_Metaprogramming_231.h"

// ========================================================================================================
#define MUT231_OUTPUT 1

void MutatorFrontendAction_231::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("enum class") != string::npos)
        content.replace(content.find("enum class"), 10, "enum struct");
      else if (content.find("enum") != string::npos)
        content.replace(content.find("enum"), 4, "enum struct");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_231::MutatorASTConsumer_231::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}