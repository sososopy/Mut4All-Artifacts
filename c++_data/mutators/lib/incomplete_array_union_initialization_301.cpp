//source file
#include "../include/incomplete_array_union_initialization_301.h"

// ========================================================================================================
#define MUT301_OUTPUT 1

void MutatorFrontendAction_301::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("UnionDecl")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;

      for (auto *Field : UD->fields()) {
        if (auto *AT = Field->getType()->getAsArrayTypeUnsafe()) {
          std::string unionName = UD->getNameAsString();
          std::string fieldType = Field->getType().getBaseTypeIdentifier()->getName().str();
          std::string mutatedUnion = "static union " + unionName + " { " + fieldType + " " + Field->getNameAsString() + "[]; } u = {2.5};";
          Rewrite.ReplaceText(UD->getSourceRange(), mutatedUnion);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_301::MutatorASTConsumer_301::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isUnion(), has(fieldDecl(hasType(arrayType())))).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}