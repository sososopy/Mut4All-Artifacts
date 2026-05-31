//source file
#include "../include/Lambda_Capture_Replace_This_497.h"

// ========================================================================================================
#define MUT497_OUTPUT 1

void MutatorFrontendAction_497::MutatorASTConsumer_497::HandleTranslationUnit(ASTContext &Context) {
    // Implement the required method
}

void MutatorFrontendAction_497::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      //Record the node information to be used in the mutation process
      candidateLambdas.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("CXXMethodDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->hasBody())
        return;
      //Perform mutation on the source code text by applying string replacement
      for (auto lambda : candidateLambdas) {
        if (lambda->getCaptureDefault() != clang::LambdaCaptureDefault::LCD_None) {
          continue;
        }
        auto captures = lambda->explicit_captures();
        if (captures.empty()) {
          continue;
        }
        bool allCapturesAreMembers = true;
        for (auto capture : captures) {
          if (capture.capturesVariable()) {
            auto var = capture.getCapturedVar();
            if (!var->isCXXClassMember()) {
              allCapturesAreMembers = false;
              break;
            }
          }
        }
        if (!allCapturesAreMembers) {
          continue;
        }
        //Replace the original AST node with the mutated one
        std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambda->getSourceRange());
        std::string captureText = "[this]";
        size_t captureStart = lambdaText.find('[');
        size_t captureEnd = lambdaText.find(']');
        if (captureStart != std::string::npos && captureEnd != std::string::npos) {
          lambdaText.replace(captureStart, captureEnd - captureStart + 1, captureText);
          for (auto capture : captures) {
            if (capture.capturesVariable()) {
              auto var = capture.getCapturedVar();
              std::string varName = var->getNameAsString();
              std::string replacement = "this->" + varName;
              size_t pos = lambdaText.find(varName);
              while (pos != std::string::npos) {
                lambdaText.replace(pos, varName.length(), replacement);
                pos = lambdaText.find(varName, pos + replacement.length());
              }
            }
          }
          lambdaText = "/*mut497*/" + lambdaText;
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambda->getSourceRange()), lambdaText);
        }
      }
    }
}