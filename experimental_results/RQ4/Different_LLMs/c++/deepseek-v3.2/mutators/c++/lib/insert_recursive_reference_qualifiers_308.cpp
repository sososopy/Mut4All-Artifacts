//source file
#include "../include/Mutator_308.h"

// ========================================================================================================
#define MUT308_OUTPUT 1

void MutatorFrontendAction_308::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = VD->getType();
      if (!type->isReferenceType())
        return;
      auto sourceRange = VD->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Perform mutation on the source code text by applying string replacement
      std::string newTypeText = "";
      auto qualifiers = type.getQualifiers();
      bool hasConst = qualifiers.hasConst();
      bool hasVolatile = qualifiers.hasVolatile();
      int depth = getrandom::getRandomIndex(15) + 5; // 5 to 20 layers
      for (int i = 0; i < depth; ++i) {
          if (i % 2 == 0) {
              newTypeText += "const ";
              if (hasVolatile)
                  newTypeText += "volatile ";
          } else {
              if (type->isLValueReferenceType())
                  newTypeText += "& ";
              else if (type->isRValueReferenceType())
                  newTypeText += "&& ";
          }
      }
      //Replace the original AST node with the mutated one
      std::string mutatedText = newTypeText + sourceText;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_308::MutatorASTConsumer_308::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(referenceType())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}