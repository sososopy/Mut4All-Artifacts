//source file
#include "../include/Mutator_Modify_Enum_Forward_Declaration_Base_Type_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;

      if (ED->isCompleteDefinition() || !ED->isScoped())
        return;

      //Get the source code text of target node
      auto enumSource = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      if (ED->getIntegerTypeSourceInfo()) {
          std::string baseType = ED->getIntegerTypeSourceInfo()->getType().getAsString();
          std::string newBaseType = "int"; // Example mutation: change to int
          if (baseType != newBaseType) {
              size_t pos = enumSource.find(baseType);
              if (pos != std::string::npos) {
                  enumSource.replace(pos, baseType.length(), newBaseType);
              }
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), enumSource);
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl(isScoped(), unless(isDefinition())).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}