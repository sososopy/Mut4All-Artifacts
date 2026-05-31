//source file
#include "../include/forward_declare_enum_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      
      if (ED->isCompleteDefinition()) {
        std::string enumName = ED->getNameAsString();
        std::string forwardDecl = "enum " + enumName + "; /*mut36*/\n";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), forwardDecl);
      }
    }
}
  
void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(isDefinition()).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}