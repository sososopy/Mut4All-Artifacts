//source file
#include "../include/Toggle_Lambda_Capture_104.h"

// ========================================================================================================
#define MUT104_OUTPUT 1

void MutatorFrontendAction_104::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;

      // Get the lambda's capture list
      auto captures = L->captures();
      if (captures.empty()) return; // Only mutate lambdas that capture at least one variable

      // Get the source range for the capture list
      auto captureRange = L->getCaptureDefaultLoc();
      auto introducerRange = L->getIntroducerRange();
      SourceLocation captureEnd;
      
      // Determine the end of the capture list
      if (captureRange.isValid()) {
        captureEnd = Lexer::getLocForEndOfToken(captureRange, 0, Rewrite.getSourceMgr(), Rewrite.getLangOpts());
      } else {
        // If no default capture, find the end of the explicit capture list
        for (const auto &capture : captures) {
          SourceLocation loc = capture.getLocation();
          if (loc.isValid() && (loc < captureEnd || !captureEnd.isValid())) {
            captureEnd = loc;
          }
        }
        if (captureEnd.isValid()) {
          captureEnd = Lexer::getLocForEndOfToken(captureEnd, 0, Rewrite.getSourceMgr(), Rewrite.getLangOpts());
        }
      }

      if (!captureEnd.isValid()) return;

      // Get the full capture text
      SourceManager &SM = Rewrite.getSourceMgr();
      auto startLoc = introducerRange.getEnd();
      auto endLoc = captureEnd;
      if (!startLoc.isValid() || !endLoc.isValid()) return;

      CharSourceRange range = CharSourceRange::getCharRange(startLoc, endLoc);
      std::string captureText = stringutils::rangetoStr(SM, SourceRange(startLoc, endLoc));
      
      // Perform mutation: toggle capture method
      std::string mutatedCapture;
      if (captureText.find("[&]") != std::string::npos) {
        // Default reference capture -> change to value capture
        mutatedCapture = "[=]";
      } else if (captureText.find("[=]") != std::string::npos) {
        // Default value capture -> change to reference capture
        mutatedCapture = "[&]";
      } else if (captureText.find("[&") != std::string::npos && captureText.find("]") != std::string::npos) {
        // Explicit reference capture -> change to value capture
        size_t start = captureText.find("[&");
        size_t end = captureText.find("]");
        if (start != std::string::npos && end != std::string::npos) {
          std::string inside = captureText.substr(start + 2, end - start - 2);
          mutatedCapture = "[" + inside + "]";
        }
      } else if (captureText.find("[") != std::string::npos && captureText.find("&") == std::string::npos && captureText.find("]") != std::string::npos) {
        // Explicit value capture -> change to reference capture
        size_t start = captureText.find("[");
        size_t end = captureText.find("]");
        if (start != std::string::npos && end != std::string::npos) {
          std::string inside = captureText.substr(start + 1, end - start - 1);
          mutatedCapture = "[&" + inside + "]";
        }
      }

      if (!mutatedCapture.empty()) {
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(range, mutatedCapture);
      }
    }
}
  
void MutatorFrontendAction_104::MutatorASTConsumer_104::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}