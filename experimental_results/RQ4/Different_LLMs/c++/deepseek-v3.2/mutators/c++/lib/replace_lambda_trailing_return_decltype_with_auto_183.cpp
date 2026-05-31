//source file
#include "../include/Mutator_Replace_Lambda_Trailing_Return_Decltype_With_Auto_183.h"

// ========================================================================================================
#define MUT183_OUTPUT 1

void MutatorFrontendAction_183::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc()))
        return;
      
      // Get the lambda's call operator
      const clang::CXXMethodDecl *callOperator = LT->getCallOperator();
      if (!callOperator) return;
      
      // Check if it has a trailing return type
      const clang::TypeSourceInfo *trailingReturn = callOperator->getTypeSourceInfo();
      if (!trailingReturn) return;
      
      // Get the trailing return type location
      clang::SourceLocation trailingLoc = trailingReturn->getTypeLoc().getBeginLoc();
      if (trailingLoc.isInvalid()) return;
      
      // Check if it's a decltype type
      const clang::DecltypeType *decltypeType = 
          trailingReturn->getType()->getAs<clang::DecltypeType>();
      if (!decltypeType) return;
      
      // Get the decltype expression
      const clang::Expr *decltypeExpr = decltypeType->getUnderlyingExpr();
      if (!decltypeExpr) return;
      
      // Check if it's a decltype expression referencing a single identifier
      if (const clang::DeclRefExpr *declRef = 
          clang::dyn_cast<clang::DeclRefExpr>(decltypeExpr->IgnoreParenImpCasts())) {
        
        // Get the referenced declaration
        const clang::ValueDecl *valueDecl = declRef->getDecl();
        if (!valueDecl) return;
        
        // Check if it's a variable (not a type or function)
        if (!clang::isa<clang::VarDecl>(valueDecl)) return;
        
        // Check if the variable is declared in the immediately enclosing function scope
        const clang::DeclContext *enclosingFunc = callOperator->getParent();
        const clang::DeclContext *varContext = valueDecl->getDeclContext();
        
        // Walk up to find the nearest function scope
        while (varContext && !clang::isa<clang::FunctionDecl>(varContext)) {
          varContext = varContext->getParent();
        }
        
        if (!varContext || varContext != enclosingFunc) return;
        
        //Get the source code text of target node
        clang::SourceRange decltypeRange = trailingReturn->getTypeLoc().getSourceRange();
        std::string decltypeText = stringutils::rangetoStr(*(Result.SourceManager), decltypeRange);
        
        // Check if it's a simple decltype(var) pattern
        if (decltypeText.find("decltype(") == 0 && decltypeText.back() == ')') {
          //Perform mutation on the source code text by applying string replacement
          std::string replacement = "decltype(auto)";
          
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(decltypeRange, replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_183::MutatorASTConsumer_183::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}