//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_Capture_In_Variadic_Template_41
 */ 
class MutatorFrontendAction_41 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(41)

private:
    class MutatorASTConsumer_41 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_41(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> outerVars;
        std::vector<const clang::LambdaExpr*> candidateLambdas;
    };
};

//source file
#include "../include/Mutator_Insert_Lambda_Capture_In_Variadic_Template_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("outerVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record outer template function local variables
      if (VD->isLocalVarDecl() && !VD->isStaticLocal()) {
        outerVars.push_back(VD);
      }
    }
    //Check whether the matched AST node is the target node
    else if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;
      //Record lambda expressions inside template functions
      candidateLambdas.push_back(LE);
    }
    //Check whether the matched AST node is the target node
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("variadicCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocation()))
        return;
      
      //Find if any lambda argument is used in this variadic call
      for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
        const Expr* arg = CE->getArg(i)->IgnoreImplicit();
        if (const LambdaExpr* LE = dyn_cast<LambdaExpr>(arg)) {
          //Check if this lambda is in our candidate list
          auto it = std::find(candidateLambdas.begin(), candidateLambdas.end(), LE);
          if (it != candidateLambdas.end()) {
            //Get the source code text of target node
            std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                           LE->getSourceRange());
            
            //Find a suitable outer variable to capture
            if (!outerVars.empty()) {
              size_t varIndex = getrandom::getRandomIndex(outerVars.size() - 1);
              const VarDecl* targetVar = outerVars[varIndex];
              
              //Check if variable is in scope for this lambda
              if (targetVar->getDeclContext()->getParent() == LE->getLambdaClass()->getParent()) {
                std::string varName = targetVar->getNameAsString();
                
                //Determine capture type (by reference or by value)
                bool captureByRef = getrandom::getRandomIndex(1);
                std::string captureStr = captureByRef ? "&" + varName : varName;
                
                //Perform mutation on the source code text by applying string replacement
                size_t captureStart = lambdaText.find('[');
                size_t captureEnd = lambdaText.find(']', captureStart);
                if (captureStart != std::string::npos && captureEnd != std::string::npos) {
                  std::string existingCapture = lambdaText.substr(captureStart + 1, 
                                                                 captureEnd - captureStart - 1);
                  
                  std::string newCapture;
                  if (existingCapture.empty() || existingCapture == "=" || existingCapture == "&") {
                    newCapture = "[" + captureStr + "]";
                  } else {
                    newCapture = "[" + existingCapture + ", " + captureStr + "]";
                  }
                  
                  lambdaText.replace(captureStart, captureEnd - captureStart + 1, newCapture);
                  
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), 
                                    lambdaText);
                  
                  //Remove this lambda from candidates to avoid multiple mutations
                  candidateLambdas.erase(it);
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //Match local variables in template functions
    DeclarationMatcher varMatcher = varDecl(hasAncestor(functionDecl(isTemplateInstantiation()))).bind("outerVar");
    //Match lambda expressions
    DeclarationMatcher lambdaMatcher = lambdaExpr(hasAncestor(functionDecl(isTemplateInstantiation()))).bind("lambdaExpr");
    //Match calls to variadic template functions
    DeclarationMatcher callMatcher = callExpr(callee(functionDecl(isVariadic()))).bind("variadicCall");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}