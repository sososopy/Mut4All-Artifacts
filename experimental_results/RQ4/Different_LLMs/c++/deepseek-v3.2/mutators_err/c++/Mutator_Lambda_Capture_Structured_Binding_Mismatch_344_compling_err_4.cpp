//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Lambda_Capture_Structured_Binding_Mismatch_344
 */ 
class MutatorFrontendAction_344 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(344)

private:
    class MutatorASTConsumer_344 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_344(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::DecompositionDecl*> decompositionDecls;
    };
};

//source file
#include "../include/Mutator_Lambda_Capture_Structured_Binding_Mismatch_344.h"

// ========================================================================================================
#define MUT344_OUTPUT 1

void MutatorFrontendAction_344::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DD = Result.Nodes.getNodeAs<clang::DecompositionDecl>("DecompositionDecl")) {
      //Filter nodes in header files
      if (!DD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      decompositionDecls.push_back(DD);
    }
    else if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = LE->getSourceRange();
      std::string lambdaText = stringutils::rangetoStr(*Result.SourceManager, lambdaRange);
      auto captureList = LE->captures();
      if (captureList.empty()) return;
      //Find nearby structured binding declarations
      for (auto* DD : decompositionDecls) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(DD->getLocation())) continue;
        //Check if lambda captures any of the structured binding variables
        bool capturesAny = false;
        std::vector<std::string> varNames;
        for (auto binding : DD->bindings()) {
          std::string varName = binding->getNameAsString();
          for (auto capture : captureList) {
            if (capture.getCapturedVar() && capture.getCapturedVar()->getNameAsString() == varName) {
              capturesAny = true;
              varNames.push_back(varName);
              break;
            }
          }
        }
        if (!capturesAny) continue;
        //Determine reference qualifier of structured binding
        auto type = DD->getType();
        bool isReference = type->isReferenceType();
        //Modify lambda capture list
        std::string newCaptureList = "[";
        size_t i = 0;
        for (auto capture : captureList) {
          std::string captureVarName = capture.getCapturedVar() ? capture.getCapturedVar()->getNameAsString() : "";
          bool isStructuredBindingVar = false;
          for (auto varName : varNames) {
            if (varName == captureVarName) {
              isStructuredBindingVar = true;
              break;
            }
          }
          if (isStructuredBindingVar) {
            //Mismatch reference qualifier
            if (isReference) {
              //Original is reference, remove reference
              newCaptureList += captureVarName;
            } else {
              //Original is non-reference, add reference
              newCaptureList += "&" + captureVarName;
            }
          } else {
            //Keep original capture
            if (capture.getCaptureKind() == clang::LCK_ByRef) {
              newCaptureList += "&" + captureVarName;
            } else if (capture.getCaptureKind() == clang::LCK_ByCopy) {
              newCaptureList += captureVar1Name;
            } else {
              newCaptureList += captureVarName;
            }
          }
          if (i != captureList.size() - 1) {
            newCaptureList += ", ";
          }
          ++i;
        }
        newCaptureList += "]";
        //Find the original capture list in the lambda text
        size_t captureStart = lambdaText.find("[");
        size_t captureEnd = lambdaText.find("]", captureStart);
        if (captureStart == std::string::npos || captureEnd == std::string::npos) continue;
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedLambda = lambdaText.substr(0, captureStart) + newCaptureList + lambdaText.substr(captureEnd + 1);
        mutatedLambda = "/*mut344*/" + mutatedLambda;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(lambdaRange, mutatedLambda);
        break;
      }
    }
}
  
void MutatorFrontendAction_344::MutatorASTConsumer_344::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher decompositionMatcher = decompositionDecl().bind("DecompositionDecl");
    StatementMatcher lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(decompositionMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}