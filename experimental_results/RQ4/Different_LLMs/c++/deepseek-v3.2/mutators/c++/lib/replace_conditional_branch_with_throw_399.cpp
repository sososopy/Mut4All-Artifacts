//source file
#include "../include/Mutator_Replace_Conditional_Branch_With_Throw_399.h"

// ========================================================================================================
#define MUT399_OUTPUT 1

void MutatorFrontendAction_399::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConditionalOperator>("ConditionalOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      SourceManager &SM = *Result.SourceManager;
      LangOptions LangOpts = Result.Context->getLangOpts();
      std::string OriginalText = stringutils::rangetoStr(SM, MT->getSourceRange());
      
      //Check if branches are throw expressions
      Expr *TrueExpr = MT->getTrueExpr();
      Expr *FalseExpr = MT->getFalseExpr();
      bool TrueIsThrow = isa<CXXThrowExpr>(TrueExpr);
      bool FalseIsThrow = isa<CXXThrowExpr>(FalseExpr);
      
      //If both branches are throw, skip mutation
      if (TrueIsThrow && FalseIsThrow) return;
      
      //Determine which branch to mutate (prioritize false branch)
      bool MutateTrue = !TrueIsThrow;
      bool MutateFalse = !FalseIsThrow && (!MutateTrue || getrandom::getRandomIndex(1) == 1);
      
      if (!MutateTrue && !MutateFalse) return;
      
      Expr *TargetExpr = MutateFalse ? FalseExpr : TrueExpr;
      std::string TargetText = stringutils::rangetoStr(SM, TargetExpr->getSourceRange());
      
      //Check if expression type can be thrown
      QualType TargetType = TargetExpr->getType();
      bool CanThrowDirectly = !TargetType->isVoidType() && 
                              (TargetType->isIntegralOrEnumerationType() || 
                               TargetType->isPointerType() || 
                               TargetType->isRecordType());
      
      std::string ThrowExprText;
      if (CanThrowDirectly) {
          ThrowExprText = "throw " + TargetText;
      } else {
          //Find suitable default throw value based on context
          if (TargetType->isIntegralOrEnumerationType()) {
              ThrowExprText = "throw 0";
          } else if (TargetType->isPointerType()) {
              ThrowExprText = "throw nullptr";
          } else {
              //Cannot determine suitable type, skip mutation
              return;
          }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = OriginalText;
      size_t ReplaceStart, ReplaceEnd;
      
      if (MutateFalse) {
          //Find false expression position
          ReplaceStart = MutatedText.find(TargetText, MutatedText.find(":") + 1);
      } else {
          //Find true expression position (after '?')
          ReplaceStart = MutatedText.find(TargetText, MutatedText.find("?") + 1);
      }
      
      if (ReplaceStart == std::string::npos) return;
      ReplaceEnd = ReplaceStart + TargetText.length();
      
      MutatedText.replace(ReplaceStart, TargetText.length(), ThrowExprText);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), MutatedText);
    }
}
  
void MutatorFrontendAction_399::MutatorASTConsumer_399::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = conditionalOperator().bind("ConditionalOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}