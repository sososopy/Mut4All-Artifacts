//source file
#include "../include/Mutator_Introduce_Lambda_Capture_In_Requires_469.h"

// ========================================================================================================
#define MUT469_OUTPUT 1

void MutatorFrontendAction_469::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto varName = VD->getNameAsString();
      auto varRange = VD->getSourceRange();
      auto varSource = stringutils::rangetoStr(*(Result.SourceManager), varRange);
      
      //Find requires clause or constraint expression using this variable
      if (auto *CE = Result.Nodes.getNodeAs<clang::Expr>("constraintExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
          return;
        
        //Check if variable is used in this expression
        auto constraintSource = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
        if (constraintSource.find(varName) == std::string::npos)
          return;
        
        //Perform mutation on the source code text by applying string replacement
        std::string lambdaCapture = "[&" + varName + "]() -> auto& { return " + varName + "; }()";
        std::string mutatedConstraint = constraintSource;
        
        //Replace all occurrences of varName with lambda capture
        size_t pos = 0;
        while ((pos = mutatedConstraint.find(varName, pos)) != std::string::npos) {
          //Check if this is a standalone variable usage (not part of larger identifier)
          bool isStandalone = true;
          if (pos > 0 && (isalnum(mutatedConstraint[pos-1]) || mutatedConstraint[pos-1] == '_'))
            isStandalone = false;
          size_t endPos = pos + varName.length();
          if (endPos < mutatedConstraint.length() && (isalnum(mutatedConstraint[endPos]) || mutatedConstraint[endPos] == '_'))
            isStandalone = false;
          
          if (isStandalone) {
            mutatedConstraint.replace(pos, varName.length(), lambdaCapture);
            pos += lambdaCapture.length();
          } else {
            pos += varName.length();
          }
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedConstraint);
      }
    }
}
  
void MutatorFrontendAction_469::MutatorASTConsumer_469::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(
      hasParent(functionDecl()),
      unless(isExpansionInSystemHeader())
    ).bind("varDecl");
    
    StatementMatcher constraintMatcher = expr(
      unless(isExpansionInSystemHeader())
    ).bind("constraintExpr");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(constraintMatcher, &callback);
    matchFinder.matchAST(Context);
}