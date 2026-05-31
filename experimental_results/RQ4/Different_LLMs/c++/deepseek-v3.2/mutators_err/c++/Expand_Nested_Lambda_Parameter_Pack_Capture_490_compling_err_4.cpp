//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Nested_Lambda_Parameter_Pack_Capture_490
 */ 
class MutatorFrontendAction_490 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(490)

private:
    class MutatorASTConsumer_490 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_490(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Expand_Nested_Lambda_Parameter_Pack_Capture_490.h"

// ========================================================================================================
#define MUT490_OUTPUT 1

void MutatorFrontendAction_490::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("InnerLambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto *OuterLambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("OuterLambda");
      auto *FunctionDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl");
      if (!OuterLambda || !FunctionDecl)
        return;
      
      // Check if the outer lambda captures a parameter pack implicitly
      auto OuterCapture = OuterLambda->getCaptureDefault();
      if (OuterCapture != clang::LCD_ByRef && OuterCapture != clang::LCD_ByCopy)
        return;
      
      // Find the parameter pack in the enclosing function
      bool hasParameterPack = false;
      std::string packName;
      for (auto *P : FunctionDecl->parameters()) {
        if (P->getType()->getAs<clang::PackExpansionType>()) {
          hasParameterPack = true;
          packName = P->getNameAsString();
          break;
        }
      }
      if (!hasParameterPack || packName.empty())
        return;
      
      // Ensure the inner lambda is nested within the outer lambda
      auto InnerLoc = MT->getBeginLoc();
      auto OuterLoc = OuterLambda->getBeginLoc();
      if (!Result.SourceManager->isBeforeInTranslationUnit(OuterLoc, InnerLoc))
        return;
      
      // Get the inner lambda's capture list source range
      auto CaptureRange = MT->getCaptureDefaultLoc();
      if (CaptureRange.isInvalid())
        return;
      
      // Perform mutation on the source code text by applying string replacement
      std::string NewCapture = "[&" + packName + "...]";
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CaptureRange, NewCapture);
    }
}
  
void MutatorFrontendAction_490::MutatorASTConsumer_490::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(
      hasDescendant(
        lambdaExpr(
          hasCaptureDefault(anyOf(LCD_ByRef, LCD_ByCopy)).bind("OuterLambda"),
          hasDescendant(
            lambdaExpr(
              hasCaptureDefault(anyOf(LCD_ByRef, LCD_ByCopy)).bind("InnerLambda")
            )
          )
        )
      )
    ).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}