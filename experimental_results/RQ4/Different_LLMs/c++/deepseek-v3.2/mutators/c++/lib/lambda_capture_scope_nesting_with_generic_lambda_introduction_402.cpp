//source file
#include "../include/Lambda_Capture_Scope_Nesting_With_Generic_Lambda_Introduction_402.h"

// ========================================================================================================
#define MUT402_OUTPUT 1

void MutatorFrontendAction_402::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Check if lambda is generic (has auto parameter)
      if (MT->getCallOperator()->getNumParams() > 0) {
        for (auto param : MT->getCallOperator()->parameters()) {
          if (param->getType()->getContainedAutoType()) {
            return; // Skip generic lambdas
          }
        }
      }
      //Check if lambda captures variables
      if (MT->capture_size() == 0) return;
      
      //Get the source code text of target node
      std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      
      //Record capture information
      capturedVars.clear();
      captureTypes.clear();
      for (auto capture : MT->captures()) {
        if (capture.capturesVariable()) {
          std::string varName = capture.getCapturedVar()->getNameAsString();
          capturedVars.push_back(varName);
          if (capture.getCaptureKind() == clang::LCK_ByRef) {
            captureTypes.push_back("&");
          } else {
            captureTypes.push_back("");
          }
        }
      }
      
      //Construct outer generic lambda capture list
      std::string outerCaptureList = "";
      for (size_t i = 0; i < capturedVars.size(); ++i) {
        if (outerCaptureList != "") outerCaptureList += ", ";
        outerCaptureList += capturedVars[i] + captureTypes[i];
      }
      
      //Determine default argument for outer lambda invocation
      std::string defaultArg = "0";
      //Look for suitable types in context
      if (MT->getCallOperator()->getNumParams() > 0) {
        auto paramType = MT->getCallOperator()->getParamDecl(0)->getType();
        if (paramType->isPointerType()) {
          defaultArg = "nullptr";
        } else if (paramType->isRecordType()) {
          defaultArg = "{}";
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedLambda = "[" + outerCaptureList + "](auto) { return " + lambdaText + "; }(" + defaultArg + ")";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedLambda);
    }
}
  
void MutatorFrontendAction_402::MutatorASTConsumer_402::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}