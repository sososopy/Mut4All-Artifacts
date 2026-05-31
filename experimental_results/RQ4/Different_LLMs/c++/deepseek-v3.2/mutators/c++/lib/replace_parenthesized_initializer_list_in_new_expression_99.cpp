//source file
#include "../include/Mutator_99.h"

// ========================================================================================================
#define MUT99_OUTPUT 1

void MutatorFrontendAction_99::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NE = Result.Nodes.getNodeAs<clang::CXXNewExpr>("newExpr")) {
      //Filter nodes in header files
      if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NE->getBeginLoc()))
        return;
      
      // Get the constructor arguments
      auto *constructExpr = NE->getConstructExpr();
      if (!constructExpr) return;
      
      unsigned numArgs = constructExpr->getNumArgs();
      if (numArgs == 0) return;
      
      // Find an argument that is a parenthesized initializer list
      for (unsigned i = 0; i < numArgs; ++i) {
        const Expr* arg = constructExpr->getArg(i);
        if (!arg) continue;
        
        // Check if the argument is a CXXConstructExpr (constructor call)
        if (auto* innerConstructExpr = dyn_cast<CXXConstructExpr>(arg)) {
          // Check if it has exactly one argument that is an InitListExpr
          if (innerConstructExpr->getNumArgs() == 1) {
            const Expr* innerArg = innerConstructExpr->getArg(0);
            if (isa<InitListExpr>(innerArg)) {
              // Found a parenthesized initializer list
              // Get the source range of this argument
              SourceRange argRange = arg->getSourceRange();
              if (!argRange.isValid()) continue;
              
              // Perform mutation: replace with empty braced initializer list
              // Replace the entire argument with "{}"
              Rewrite.ReplaceText(argRange, "{}");
              return; // Mutate only one argument per new expression
            }
          }
        }
        // Also check for explicit cast expressions containing InitListExpr
        else if (auto* castExpr = dyn_cast<ExplicitCastExpr>(arg)) {
          const Expr* subExpr = castExpr->getSubExpr();
          if (isa<InitListExpr>(subExpr)) {
            // Found a cast expression containing an init list
            SourceRange argRange = arg->getSourceRange();
            if (!argRange.isValid()) continue;
            
            Rewrite.ReplaceText(argRange, "{}");
            return;
          }
        }
        // Direct InitListExpr inside parentheses (parenExpr around initListExpr)
        else if (auto* parenExpr = dyn_cast<ParenExpr>(arg)) {
          const Expr* innerExpr = parenExpr->getSubExpr();
          if (isa<InitListExpr>(innerExpr)) {
            SourceRange argRange = arg->getSourceRange();
            if (!argRange.isValid()) continue;
            
            Rewrite.ReplaceText(argRange, "{}");
            return;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_99::MutatorASTConsumer_99::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxNewExpr().bind("newExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}