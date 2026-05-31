//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Array_Initialization_Loop_Expression_Substitution_With_Lambda_Capture_42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)

private:
    class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;
      
      // Check if lambda has capture and body
      if (!LE->hasCaptures() || !LE->getLambdaClass()->getLambdaCallOperator()->hasBody())
        return;
      
      // Get the lambda body
      const Stmt *Body = LE->getLambdaClass()->getLambdaCallOperator()->getBody();
      if (!Body)
        return;
      
      // Look for nested array initialization or array subscript expressions inside the lambda
      // We'll use a recursive visitor to find suitable patterns
      class NestedArrayVisitor : public RecursiveASTVisitor<NestedArrayVisitor> {
      public:
        bool foundNestedArray = false;
        const ArraySubscriptExpr *targetSubscript = nullptr;
        const DeclRefExpr *arrayDeclRef = nullptr;
        
        bool VisitArraySubscriptExpr(const ArraySubscriptExpr *ASE) {
          // Check if this is a nested array access (e.g., array[0][0])
          const Expr *Base = ASE->getBase()->IgnoreParenImpCasts();
          if (const ArraySubscriptExpr *InnerASE = dyn_cast<ArraySubscriptExpr>(Base)) {
            foundNestedArray = true;
            targetSubscript = ASE;
            // Try to find the array declaration reference
            const Expr *InnerBase = InnerASE->getBase()->IgnoreParenImpCasts();
            if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(InnerBase)) {
              arrayDeclRef = DRE;
            }
          }
          return true;
        }
      };
      
      NestedArrayVisitor Visitor;
      Visitor.TraverseStmt(const_cast<Stmt*>(Body));
      
      if (!Visitor.foundNestedArray || !Visitor.targetSubscript || !Visitor.arrayDeclRef)
        return;
      
      // Get the source range of the lambda expression
      SourceRange LambdaRange = LE->getSourceRange();
      if (LambdaRange.isInvalid())
        return;
      
      // Get the original lambda text
      std::string LambdaText = stringutils::rangetoStr(*(Result.SourceManager), LambdaRange);
      
      // Find the capture list
      size_t captureStart = LambdaText.find('[');
      size_t captureEnd = LambdaText.find(']');
      if (captureStart == std::string::npos || captureEnd == std::string::npos)
        return;
      
      // Get the array name
      std::string ArrayName = Visitor.arrayDeclRef->getDecl()->getNameAsString();
      
      // Modify the capture to be by reference if not already
      std::string CaptureList = LambdaText.substr(captureStart, captureEnd - captureStart + 1);
      if (CaptureList.find('&') == std::string::npos) {
        // Insert & before the array name in capture
        size_t arrayPos = CaptureList.find(ArrayName);
        if (arrayPos != std::string::npos) {
          CaptureList.insert(arrayPos, "&");
        }
      }
      
      // Get the source range of the nested array subscript expression
      SourceRange SubscriptRange = Visitor.targetSubscript->getSourceRange();
      if (SubscriptRange.isInvalid())
        return;
      
      // Get the text of the subscript expression
      std::string SubscriptText = stringutils::rangetoStr(*(Result.SourceManager), SubscriptRange);
      
      // Parse the indices from the nested subscript
      // Example: array[0][0] -> indices [0][0]
      // We'll extract the inner and outer indices
      size_t firstBracket = SubscriptText.find('[');
      size_t secondBracket = SubscriptText.find('[', firstBracket + 1);
      if (firstBracket == std::string::npos || secondBracket == std::string::npos)
        return;
      
      // Extract outer index (e.g., [0] from array[0][0])
      size_t firstBracketClose = SubscriptText.find(']', firstBracket);
      std::string OuterIndex = SubscriptText.substr(firstBracket, firstBracketClose - firstBracket + 1);
      
      // Extract inner index (e.g., [0] from array[0][0])
      size_t secondBracketClose = SubscriptText.find(']', secondBracket);
      std::string InnerIndex = SubscriptText.substr(secondBracket, secondBracketClose - secondBracket + 1);
      
      // Construct the new lambda body with decomposed array access
      // Find the lambda body start
      size_t bodyStart = LambdaText.find('{');
      if (bodyStart == std::string::npos)
        return;
      
      // Create the mutated lambda text
      std::string MutatedLambda = LambdaText.substr(0, captureStart) + 
                                  CaptureList + 
                                  LambdaText.substr(captureEnd + 1, bodyStart - captureEnd - 1) + 
                                  "{\nauto& innerArray = " + ArrayName + OuterIndex + ";\nreturn innerArray" + InnerIndex + ";\n}";
      
      // Replace the original lambda with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaRange), MutatedLambda);
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasAnyCapture()).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}