//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Return_In_Generic_Lambda_253
 */ 
class MutatorFrontendAction_253 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(253)

private:
    class MutatorASTConsumer_253 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_253(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Return_In_Generic_Lambda_253.h"

// ========================================================================================================
#define MUT253_OUTPUT 1

void MutatorFrontendAction_253::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LS = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LS->getLocation()))
        return;
      
      // Check if lambda is generic (has auto parameters or template)
      auto LambdaDecl = LS->getLambdaClass();
      if (!LambdaDecl) return;
      auto CallOperator = LambdaDecl->getLambdaCallOperator();
      if (!CallOperator) return;
      
      // Check if call operator is generic (has auto/template parameters)
      bool IsGeneric = false;
      if (CallOperator->getNumParams() > 0) {
        for (auto Param : CallOperator->parameters()) {
          if (Param->getType()->isAutoType()) {
            IsGeneric = true;
            break;
          }
        }
      }
      if (!IsGeneric) return;
      
      // Get lambda body and look for conditional statements with return
      auto Body = CallOperator->getBody();
      if (!Body) return;
      
      // Traverse body to find if/else with return statements
      std::vector<clang::IfStmt*> IfStatements;
      for (auto Child : Body->children()) {
        if (auto If = dyn_cast<clang::IfStmt>(Child)) {
          IfStatements.push_back(If);
        }
      }
      
      for (auto If : IfStatements) {
        auto Then = If->getThen();
        auto Else = If->getElse();
        
        // Check both branches have return statements
        bool ThenHasReturn = false;
        bool ElseHasReturn = false;
        
        if (Then) {
          ThenHasReturn = hasReturnStatement(Then);
        }
        if (Else) {
          ElseHasReturn = hasReturnStatement(Else);
        }
        
        if (ThenHasReturn && ElseHasReturn) {
          // Found target - mutate the else branch return
          if (Else) {
            // Find return statement in else branch
            clang::ReturnStmt* ElseReturn = findReturnStatement(Else);
            if (ElseReturn) {
              //Get the source code text of target node
              auto ReturnRange = ElseReturn->getSourceRange();
              //Perform mutation on the source code text by applying string replacement
              std::string Mutation = "return undef_var;";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(ReturnRange), Mutation);
              return; // Mutate only one lambda
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_253::MutatorASTConsumer_253::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}