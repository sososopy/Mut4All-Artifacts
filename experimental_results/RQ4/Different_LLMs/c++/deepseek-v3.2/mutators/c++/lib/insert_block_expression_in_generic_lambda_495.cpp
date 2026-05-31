//source file
#include "../include/Mutator_495.h"

// ========================================================================================================
#define MUT495_OUTPUT 1

void MutatorFrontendAction_495::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaDecl = MT->getLambdaClass();
      if (!lambdaDecl) return;
      
      // Check if lambda has generic parameter (auto)
      bool hasAutoParam = false;
      auto callOperator = lambdaDecl->getLambdaCallOperator();
      if (callOperator) {
        for (auto param : callOperator->parameters()) {
          if (param->getType()->getContainedAutoType()) {
            hasAutoParam = true;
            break;
          }
        }
      }
      if (!hasAutoParam) return;
      
      // Check capture list
      if (MT->capture_size() == 0) return;
      
      // Choose a captured variable
      std::string captureVar;
      for (unsigned i = 0; i < MT->capture_size(); ++i) {
        auto capture = MT->capture_begin()[i];
        if (capture.capturesVariable()) {
          auto varDecl = capture.getCapturedVar();
          if (varDecl) {
            captureVar = varDecl->getNameAsString();
            break;
          }
        }
      }
      if (captureVar.empty()) return;
      
      // Get lambda body
      auto body = MT->getBody();
      if (!body) return;
      
      // Get source range of body
      SourceRange bodyRange = body->getSourceRange();
      SourceLocation insertLoc = bodyRange.getBegin().getLocWithOffset(1);
      
      //Perform mutation on the source code text by applying string replacement
      std::string blockExpr = "^{ (void)" + captureVar + "; };\n";
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertText(insertLoc, blockExpr);
    }
}
  
void MutatorFrontendAction_495::MutatorASTConsumer_495::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}