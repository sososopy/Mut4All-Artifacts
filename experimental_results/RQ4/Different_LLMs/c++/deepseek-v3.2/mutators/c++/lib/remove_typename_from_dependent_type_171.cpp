//source file
#include "../include/Mutator_Remove_Typename_From_Dependent_Type_171.h"

// ========================================================================================================
#define MUT171_OUTPUT 1

void MutatorFrontendAction_171::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TT = Result.Nodes.getNodeAs<clang::TypedefType>("TypedefType")) {
      //Filter nodes in header files
      if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TT->getDecl()->getLocation()))
        return;
      //Get the source code text of target node
      auto *TDT = TT->getDecl();
      if (!TDT->getTypeSourceInfo()) return;
      TypeLoc TL = TDT->getTypeSourceInfo()->getTypeLoc();
      if (!TL.getAs<ElaboratedTypeLoc>()) return;
      ElaboratedTypeLoc ETL = TL.getAs<ElaboratedTypeLoc>();
      if (ETL.getTypePtr()->getKeyword() != ETK_Typename) return;
      SourceRange TypeRange = ETL.getSourceRange();
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), TypeRange);
      //Perform mutation on the source code text by applying string replacement
      size_t TypenamePos = OriginalText.find("typename ");
      if (TypenamePos != std::string::npos) {
          std::string MutatedText = OriginalText.substr(TypenamePos + 9);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(TypeRange, MutatedText);
      }
    }
}
  
void MutatorFrontendAction_171::MutatorASTConsumer_171::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typedefDecl(hasType(elaboratedType())).bind("TypedefType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}