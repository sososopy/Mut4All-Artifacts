//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Volatile_In_Atomic_Template_408
 */ 
class MutatorFrontendAction_408 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(408)

private:
    class MutatorASTConsumer_408 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_408(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> candidateVars;
    };
};

//source file
#include "../include/Mutator_Introduce_Volatile_In_Atomic_Template_408.h"

// ========================================================================================================
#define MUT408_OUTPUT 1

void MutatorFrontendAction_408::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if variable is already volatile
      if (VD->getType().isVolatileQualified())
        return;
      //Check if variable is used in atomic operation (record for later processing)
      candidateVars.push_back(VD);
    }
    else if (auto *AE = Result.Nodes.getNodeAs<clang::CallExpr>("atomicExpr")) {
      //Filter nodes in header files
      if (!AE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AE->getBeginLoc()))
        return;
      //Get the source code text of target node
      //Check if this is an atomic built-in function
      const FunctionDecl* FD = AE->getDirectCallee();
      if (!FD) return;
      std::string funcName = FD->getNameAsString();
      if (!funcName.compare(0, 8, "__atomic")) return;
      
      //Find the variable argument (first argument)
      if (AE->getNumArgs() < 1) return;
      const Expr* firstArg = AE->getArg(0);
      //Try to get the VarDecl from the argument
      VarDecl* targetVar = nullptr;
      if (auto* UE = dyn_cast<UnaryOperator>(firstArg)) {
        if (UE->getOpcode() == UO_AddrOf) {
          if (auto* DRE = dyn_cast<DeclRefExpr>(UE->getSubExpr())) {
            targetVar = dyn_cast<VarDecl>(DRE->getDecl());
          }
        }
      } else if (auto* DRE = dyn_cast<DeclRefExpr>(firstArg)) {
        targetVar = dyn_cast<VarDecl>(DRE->getDecl());
      }
      
      if (!targetVar) return;
      //Check if this variable is in our candidate list
      bool found = false;
      for (auto* cand : candidateVars) {
        if (cand == targetVar) {
          found = true;
          break;
        }
      }
      if (!found) return;
      
      //Check if variable is already volatile
      if (targetVar->getType().isVolatileQualified())
        return;
      
      //(optional)Record the node information to be used in the mutation process
      SourceLocation declLoc = targetVar->getBeginLoc();
      //Perform mutation on the source code text by applying string replacement
      //Add volatile qualifier before the type
      std::string newText = "volatile ";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(declLoc, newText);
    }
}
  
void MutatorFrontendAction_408::MutatorASTConsumer_408::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //Match variable declarations in template functions
    DeclarationMatcher varMatcher = varDecl(
        hasParent(functionDecl(templateFunctionDecl())),
        unless(hasType(hasCanonicalType(isVolatile())))
    ).bind("varDecl");
    
    //Match atomic built-in function calls
    StatementMatcher atomicMatcher = callExpr(
        callee(functionDecl(hasAnyName("__atomic_fetch_add", "__atomic_load", "__atomic_store", "__atomic_exchange", "__atomic_compare_exchange")))
    ).bind("atomicExpr");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(atomicMatcher, &callback);
    matchFinder.matchAST(Context);
}