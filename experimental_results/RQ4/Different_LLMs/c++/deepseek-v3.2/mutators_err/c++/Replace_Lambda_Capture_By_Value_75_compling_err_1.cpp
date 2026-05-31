//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Capture_By_Value_75
 */ 
class MutatorFrontendAction_75 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(75)

private:
    class MutatorASTConsumer_75 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_75(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::LambdaExpr*> lambdaExprs;
    };
};

//source file
#include "../include/Replace_Lambda_Capture_By_Value_75.h"

// ========================================================================================================
#define MUT75_OUTPUT 1

void MutatorFrontendAction_75::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      // Check if the lambda is in a consteval or constexpr context
      auto parentFunction = Result.Context->getParents(*MT)[0];
      if (parentFunction.get<clang::FunctionDecl>()) {
        auto funcDecl = parentFunction.get<clang::FunctionDecl>();
        if (!funcDecl->isConsteval() && !funcDecl->isConstexpr()) {
          return;
        }
      } else {
        // Check for constexpr variable context
        bool inConstexprContext = false;
        auto parents = Result.Context->getParents(*MT);
        for (auto parent : parents) {
          if (parent.get<clang::VarDecl>()) {
            auto varDecl = parent.get<clang::VarDecl>();
            if (varDecl->isConstexpr()) {
              inConstexprContext = true;
              break;
            }
          }
        }
        if (!inConstexprContext) return;
      }

      // Check if lambda is used in pack expansion or nested in another lambda
      bool inPackExpansion = false;
      bool isNested = false;
      auto parents = Result.Context->getParents(*MT);
      for (auto parent : parents) {
        if (parent.get<clang::LambdaExpr>()) {
          isNested = true;
          break;
        }
        if (parent.get<clang::PackExpansionExpr>()) {
          inPackExpansion = true;
          break;
        }
      }

      if (!inPackExpansion && !isNested) return;

      // Record lambda for potential mutation
      lambdaExprs.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaForMutation")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      // Find this lambda in recorded list
      auto it = std::find(lambdaExprs.begin(), lambdaExprs.end(), MT);
      if (it == lambdaExprs.end()) return;

      //Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      // Find capture clause
      auto captureDefault = MT->getCaptureDefault();
      auto explicitCaptures = MT->captures();
      
      // Only mutate if capture default is by reference (&)
      if (captureDefault != clang::LCK_ByRef) return;
      
      // Check if we can capture nothing
      bool canCaptureNothing = true;
      for (auto capture : explicitCaptures) {
        if (capture.capturesVariable()) {
          auto varDecl = capture.getCapturedVar();
          // Check if variable is accessible without capture (e.g., parameter)
          if (!varDecl->isLocalVarDecl() && !varDecl->isParameter()) {
            canCaptureNothing = false;
            break;
          }
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string newCapture;
      if (canCaptureNothing && explicitCaptures.empty()) {
        newCapture = "[]";
      } else {
        // Capture by value
        newCapture = "[=";
        if (!explicitCaptures.empty()) {
          newCapture += " ";
          for (size_t i = 0; i < explicitCaptures.size(); ++i) {
            if (i > 0) newCapture += ", ";
            auto capture = explicitCaptures[i];
            if (capture.capturesVariable()) {
              auto varDecl = capture.getCapturedVar();
              newCapture += varDecl->getNameAsString();
            }
          }
        }
        newCapture += "]";
      }
      
      // Replace the capture clause
      auto introRange = MT->getIntroducerRange();
      if (introRange.isValid()) {
        auto introText = stringutils::rangetoStr(*(Result.SourceManager), introRange);
        size_t pos = lambdaText.find(introText);
        if (pos != std::string::npos) {
          lambdaText.replace(pos, introText.length(), newCapture);
        }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
    }
}
  
void MutatorFrontendAction_75::MutatorASTConsumer_75::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto lambdaMatcher = lambdaExpr().bind("lambdaExpr");
    auto lambdaForMutationMatcher = lambdaExpr().bind("lambdaForMutation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(lambdaForMutationMatcher, &callback);
    matchFinder.matchAST(Context);
}