//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Structured_Binding_Capture_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Replace_Structured_Binding_Capture_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LS = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LS->getLocation()))
        return;
      
      // Get the structured binding variable declarations that are captured
      auto *SBD = Result.Nodes.getNodeAs<clang::DecompositionDecl>("decompDecl");
      if (!SBD)
        return;
      
      // Ensure the structured binding is a reference to a temporary
      if (!SBD->getType()->isReferenceType())
        return;
      
      // Get the lambda's capture list
      auto *LambdaClass = LS->getLambdaClass();
      if (!LambdaClass)
        return;
      
      // Check if the lambda captures the structured binding identifiers
      bool capturesDecompVars = false;
      for (auto *Field : LambdaClass->fields()) {
        if (Field->getCaptureKind() == clang::LCK_ByCopy || Field->getCaptureKind() == clang::LCK_ByRef) {
          // Check if this field corresponds to one of the decomposed variables
          for (auto *Binding : SBD->bindings()) {
            if (Binding->getID() && Field->getName() == Binding->getID()->getName()) {
              capturesDecompVars = true;
              break;
            }
          }
        }
        if (capturesDecompVars)
          break;
      }
      
      if (!capturesDecompVars)
        return;
      
      //Get the source code text of target node
      auto LambdaRange = LS->getSourceRange();
      if (LambdaRange.isInvalid())
        return;
      
      std::string LambdaText = stringutils::rangetoStr(*(Result.SourceManager), LambdaRange);
      
      // Find the capture list part
      auto CaptureStart = LambdaText.find('[');
      auto CaptureEnd = LambdaText.find(']');
      if (CaptureStart == std::string::npos || CaptureEnd == std::string::npos)
        return;
      
      std::string CaptureList = LambdaText.substr(CaptureStart + 1, CaptureEnd - CaptureStart - 1);
      
      // Build new capture list with references to decomposed variables
      std::string NewCaptureList = "";
      bool First = true;
      for (auto *Binding : SBD->bindings()) {
        if (Binding->getID()) {
          std::string VarName = Binding->getID()->getName().str();
          if (!First)
            NewCaptureList += ", ";
          NewCaptureList += "&" + VarName;
          First = false;
        }
      }
      
      if (NewCaptureList.empty())
        return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string NewLambdaText = LambdaText;
      NewLambdaText.replace(CaptureStart + 1, CaptureEnd - CaptureStart - 1, NewCaptureList);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaRange), NewLambdaText);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(
      hasAncestor(
        varDecl(
          hasDescendant(
            decompositionDecl(
              hasType(referenceType())
            ).bind("decompDecl")
          )
        )
      )
    ).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}