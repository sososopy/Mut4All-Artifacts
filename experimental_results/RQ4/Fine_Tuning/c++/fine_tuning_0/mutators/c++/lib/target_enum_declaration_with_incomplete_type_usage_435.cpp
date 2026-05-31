//source file
#include "../include/Target_Enum_Declaration_with_Incomplete_Type_Usage_435.h"

// ========================================================================================================
#define MUT435_OUTPUT 1

void MutatorFrontendAction_435::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      if (ED->isScoped())
        return;
      if (!ED->isCompleteDefinition())
        return;
      auto name = ED->getNameAsString();
      if (name == "")
        return;
      auto definition =
          stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
      auto dice = getrandom::getRandomIndex(2);
      string ins = "";
      if (dice == 0) {
        ins = "class IncompleteClass;";
      } else if (dice == 1) {
        ins = "struct IncompleteStruct;";
      } else if (dice == 2) {
        ins = "enum IncompleteEnum : int;";
      } else
        assert(false &&
               "Mut435: Dice should have 3 possible values!");
      definition = ins + "/*mut435*/" + definition;
      if (dice == 0)
        definition.insert(definition.find('{'), " : IncompleteClass ");
      else if (dice == 1)
        definition.insert(definition.find('{'), " : IncompleteStruct ");
      else if (dice == 2)
        definition.insert(definition.find('{'), " : IncompleteEnum ");
      else
        assert(false &&
               "Mut435: Dice should have 3 possible values!");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), definition);
    }
}
  
void MutatorFrontendAction_435::MutatorASTConsumer_435::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}