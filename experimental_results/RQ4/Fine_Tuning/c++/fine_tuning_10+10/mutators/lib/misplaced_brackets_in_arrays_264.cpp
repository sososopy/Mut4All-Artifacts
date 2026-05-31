//source file
#include "../include/misplaced_brackets_in_arrays_264.h"

// ========================================================================================================
#define MUT264_OUTPUT 1

void MutatorFrontendAction_264::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      if (VD->getType()->isArrayType()) {
          //Get the source code text of target node
          auto arrayType = VD->getType()->getAsArrayTypeUnsafe();
          if (auto *CAT = dyn_cast<ConstantArrayType>(arrayType)) {
              auto sizeExpr = CAT->getSizeExpr();
              if (!sizeExpr)
                  return;
              
              //Perform mutation on the source code text by applying string replacement
              auto varName = VD->getNameAsString();
              auto originalText = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
              std::string mutatedText = originalText;
              auto bracketPos = mutatedText.find("]");
              if (bracketPos != std::string::npos) {
                  mutatedText.insert(bracketPos + 1, " " + varName);
              }
              
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), mutatedText);
          }
      }
    }
}
  
void MutatorFrontendAction_264::MutatorASTConsumer_264::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("ArrayVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}