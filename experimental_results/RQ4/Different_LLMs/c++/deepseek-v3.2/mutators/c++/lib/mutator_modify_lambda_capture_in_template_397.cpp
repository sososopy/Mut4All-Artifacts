//source file
#include "../include/Mutator_Modify_Lambda_Capture_In_Template_397.h"

// ========================================================================================================
#define MUT397_OUTPUT 1

void MutatorFrontendAction_397::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      
      // Check that the lambda is inside a template member function
      const auto *FD = dyn_cast_or_null<FunctionDecl>(Result.Context->getParents(*L)[0].get<FunctionDecl>());
      if (!FD || !FD->isTemplateInstantiation())
        return;
      
      // Get the capture list
      auto Capture = L->getLambdaClass()->getLambdaCaptureDefault();
      if (!Capture)
        return;
      
      // Determine if 'this' is captured explicitly
      bool hasExplicitThis = false;
      bool hasImplicitCapture = false;
      for (const auto &C : L->captures()) {
        if (C.getCaptureKind() == LambdaCaptureKind::LCK_This) {
          hasExplicitThis = true;
        }
        if (C.getCaptureKind() == LambdaCaptureKind::LCK_StarThis || 
            C.getCaptureKind() == LambdaCaptureKind::LCK_ByCopy) {
          hasImplicitCapture = true;
        }
      }
      
      // Get the source code text of target node
      std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), L->getSourceRange());
      
      // Find the capture clause part
      size_t captureStart = lambdaText.find('[');
      size_t captureEnd = lambdaText.find(']', captureStart);
      if (captureStart == std::string::npos || captureEnd == std::string::npos)
        return;
      
      std::string captureClause = lambdaText.substr(captureStart, captureEnd - captureStart + 1);
      
      // Perform mutation: change explicit 'this' to implicit '[&]' or vice versa
      std::string newCaptureClause;
      if (hasExplicitThis) {
        // Change [this] to [&]
        newCaptureClause = "[&]";
      } else if (hasImplicitCapture) {
        // Change [&] or [=] to [this]
        newCaptureClause = "[this]";
      } else {
        // No capture change needed
        return;
      }
      
      // Replace the capture clause in the lambda text
      lambdaText.replace(captureStart, captureEnd - captureStart + 1, newCaptureClause);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(L->getSourceRange()), lambdaText);
    }
}
  
void MutatorFrontendAction_397::MutatorASTConsumer_397::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}