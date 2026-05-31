//source file
#include "../include/misaligned_struct_declaration_567.h"

// ========================================================================================================
#define MUT567_OUTPUT 1

void MutatorFrontendAction_567::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("unionStruct")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      
      if (UD->isAnonymousStructOrUnion() && UD->isStruct()) {
        for (auto *Field : UD->fields()) {
          auto fieldType = Field->getType().getAsString();
          if (fieldType == "int" || fieldType == "unsigned") {
            std::string newType = "igned";
            Rewrite.ReplaceText(Field->getTypeSourceInfo()->getTypeLoc().getSourceRange(), newType);
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_567::MutatorASTConsumer_567::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isUnion(), isStruct()).bind("unionStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}