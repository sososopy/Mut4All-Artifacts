//source file
#include "../include/Target_Enum_Declaration_332.h"

// ========================================================================================================
#define MUT332_OUTPUT 1

void MutatorFrontendAction_332::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto enum_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  MT->getSourceRange());
      auto enumelems = MT->enumerators();
      if (enumelems.begin() == enumelems.end())
        return;
      auto choice = getrandom::getRandomIndex(std::distance(enumelems.begin(), enumelems.end()) - 1);
      auto elem = *(std::next(enumelems.begin(), choice));
      auto elem_name = elem->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      enum_content.insert(enum_content.rfind('}'), ", " + elem_name);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), enum_content);
    }
}
  
void MutatorFrontendAction_332::MutatorASTConsumer_332::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}