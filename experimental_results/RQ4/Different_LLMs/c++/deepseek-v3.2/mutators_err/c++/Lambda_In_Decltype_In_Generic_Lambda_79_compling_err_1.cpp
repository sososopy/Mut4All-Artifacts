//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_In_Decltype_In_Generic_Lambda_79
 */ 
class MutatorFrontendAction_79 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(79)

private:
    class MutatorASTConsumer_79 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_79(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr*> genericLambdas;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT79_OUTPUT 1

void MutatorFrontendAction_79::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("genericLambda")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;
      
      // Check if it's a generic lambda (has auto parameter)
      bool hasAutoParam = false;
      if (auto *MD = LE->getCallOperator()) {
        for (auto *P : MD->parameters()) {
          if (P->getType()->isAutoType()) {
            hasAutoParam = true;
            break;
          }
        }
      }
      if (!hasAutoParam) return;
      
      // Record generic lambda for later processing
      genericLambdas.push_back(LE);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Find if this variable declaration uses any of our recorded generic lambdas
      const clang::Expr* initExpr = VD->getInit();
      if (!initExpr) return;
      
      // Check if initExpr contains any of our generic lambdas
      const clang::LambdaExpr* targetLambda = nullptr;
      for (auto* lambda : genericLambdas) {
        // Check if lambda is within initExpr's subtree
        if (initExpr->IgnoreImplicit() == lambda || 
            (initExpr->IgnoreImplicit()->getStmtClass() == clang::Stmt::ImplicitCastExprClass &&
             static_cast<const clang::ImplicitCastExpr*>(initExpr->IgnoreImplicit())->getSubExpr() == lambda)) {
          targetLambda = lambda;
          break;
        }
      }
      
      if (!targetLambda) return;
      
      //Get the source code text of target node
      auto lambdaRange = targetLambda->getSourceRange();
      std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      // Check if lambda already has trailing return type
      bool hasTrailingReturn = false;
      size_t arrowPos = lambdaText.find("->");
      if (arrowPos != std::string::npos) {
        // Check if arrow is before body
        size_t bodyStart = lambdaText.find('{');
        if (bodyStart != std::string::npos && arrowPos < bodyStart) {
          hasTrailingReturn = true;
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      if (hasTrailingReturn) {
        // Replace existing trailing return type
        size_t arrowEnd = lambdaText.find('>', arrowPos);
        if (arrowEnd == std::string::npos) return;
        
        // Find where the return type ends (before '{' or before noexcept/specifiers)
        size_t returnTypeEnd = lambdaText.find('{', arrowEnd);
        if (returnTypeEnd == std::string::npos) {
          // Try to find noexcept or other specifiers
          returnTypeEnd = lambdaText.find("noexcept", arrowEnd);
          if (returnTypeEnd == std::string::npos) {
            returnTypeEnd = lambdaText.length();
          }
        }
        
        // Create new decltype with inner lambda
        std::string newReturnType = "-> decltype([] { return 0; }())";
        mutatedText = lambdaText.substr(0, arrowPos) + newReturnType + 
                     lambdaText.substr(returnTypeEnd);
      } else {
        // Insert trailing return type before body
        size_t bodyStart = lambdaText.find('{');
        if (bodyStart == std::string::npos) return;
        
        std::string newReturnType = " -> decltype([] { return 0; }()) ";
        mutatedText = lambdaText.substr(0, bodyStart) + newReturnType + 
                     lambdaText.substr(bodyStart);
      }
      
      mutatedText = "/*mut79*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedText);
    }
}
  
void MutatorFrontendAction_79::MutatorASTConsumer_79::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher lambdaMatcher = lambdaExpr().bind("genericLambda");
    DeclarationMatcher varDeclMatcher = varDecl(hasInitializer(anything())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(varDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}