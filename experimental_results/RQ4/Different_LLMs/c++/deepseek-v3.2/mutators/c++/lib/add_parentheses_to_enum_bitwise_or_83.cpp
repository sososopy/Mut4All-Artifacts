//source file
#include "../include/Mutator_Add_Parentheses_To_Enum_Bitwise_Or_83.h"

// ========================================================================================================
#define MUT83_OUTPUT 1

void MutatorFrontendAction_83::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BE = Result.Nodes.getNodeAs<clang::BinaryOperator>("enumBitwiseOr")) {
      //Filter nodes in header files
      if (!BE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BE->getBeginLoc()))
        return;
      
      // Check that the operator is bitwise OR
      if (BE->getOpcode() != BO_Or)
        return;
      
      // Get left and right subexpressions
      Expr *LHS = BE->getLHS()->IgnoreImpCasts();
      Expr *RHS = BE->getRHS()->IgnoreImpCasts();
      
      // Check if either side involves enum types
      bool hasEnumType = false;
      
      // Check LHS type
      QualType LHSType = LHS->getType();
      if (LHSType->isEnumeralType() || 
          (LHSType->isIntegerType() && LHS->getType()->getAs<EnumType>()))
        hasEnumType = true;
      
      // Check RHS type
      QualType RHSType = RHS->getType();
      if (RHSType->isEnumeralType() || 
          (RHSType->isIntegerType() && RHS->getType()->getAs<EnumType>()))
        hasEnumType = true;
      
      // Check for DeclRefExpr to enum constants
      if (!hasEnumType) {
        if (DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(LHS)) {
          if (EnumConstantDecl *ECD = dyn_cast<EnumConstantDecl>(DRE->getDecl()))
            hasEnumType = true;
        }
        if (DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(RHS)) {
          if (EnumConstantDecl *ECD = dyn_cast<EnumConstantDecl>(DRE->getDecl()))
            hasEnumType = true;
        }
      }
      
      if (!hasEnumType)
        return;
      
      //Get the source code text of target node
      SourceRange range = BE->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "(" + originalText + ")";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(range, mutatedText);
    }
}
  
void MutatorFrontendAction_83::MutatorASTConsumer_83::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasOperatorName("|")).bind("enumBitwiseOr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}