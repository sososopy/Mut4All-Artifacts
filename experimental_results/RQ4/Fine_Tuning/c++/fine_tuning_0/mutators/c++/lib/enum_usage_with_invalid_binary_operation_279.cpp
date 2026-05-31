//source file
#include "../include/Enum_Usage_with_Invalid_Binary_Operation_279.h"

// ========================================================================================================
#define MUT279_OUTPUT 1

void MutatorFrontendAction_279::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      auto enus = MT->enumerators();
      auto first_enu = enus.begin();
      auto first_enu_name = first_enu->getNameAsString();
      auto def =
          stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      def += "/*mut279*/ " + name + " " + "mut_279" + "=" + first_enu_name +
             "; mut_279 += \"\";";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), def);
    }
}
  
void MutatorFrontendAction_279::MutatorASTConsumer_279::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}