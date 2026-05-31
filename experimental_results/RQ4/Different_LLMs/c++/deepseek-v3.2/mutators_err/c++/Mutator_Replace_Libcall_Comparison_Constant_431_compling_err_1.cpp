//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Replace_Libcall_Comparison_Constant_431
 */ 
class MutatorFrontendAction_431 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(431)

private:
    class MutatorASTConsumer_431 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_431(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Libcall_Comparison_Constant_431.h"

// ========================================================================================================
#define MUT431_OUTPUT 1

void MutatorFrontendAction_431::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinOp")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      clang::Expr *LHS = MT->getLHS()->IgnoreParenImpCasts();
      clang::Expr *RHS = MT->getRHS()->IgnoreParenImpCasts();
      
      // Check if one side is a call to a library function
      bool lhsIsLibCall = false;
      bool rhsIsLibCall = false;
      clang::CallExpr *libCall = nullptr;
      clang::IntegerLiteral *intLiteral = nullptr;
      
      if (clang::CallExpr *call = clang::dyn_cast<clang::CallExpr>(LHS)) {
        if (clang::FunctionDecl *funcDecl = call->getDirectCallee()) {
          std::string funcName = funcDecl->getNameAsString();
          if (funcName == "memcpy" || funcName == "memmove" || funcName == "memset" || funcName == "strlen") {
            lhsIsLibCall = true;
            libCall = call;
          }
        }
      }
      if (clang::CallExpr *call = clang::dyn_cast<clang::CallExpr>(RHS)) {
        if (clang::FunctionDecl *funcDecl = call->getDirectCallee()) {
          std::string funcName = funcDecl->getNameAsString();
          if (funcName == "memcpy" || funcName == "memmove" || funcName == "memset" || funcName == "strlen") {
            rhsIsLibCall = true;
            libCall = call;
          }
        }
      }
      
      // Check if the other side is an integer literal
      if (lhsIsLibCall) {
        intLiteral = clang::dyn_cast<clang::IntegerLiteral>(RHS);
      } else if (rhsIsLibCall) {
        intLiteral = clang::dyn_cast<clang::IntegerLiteral>(LHS);
      }
      
      if (!libCall || !intLiteral) return;
      
      // If integer literal is already zero, skip mutation
      if (intLiteral->getValue().isZero()) return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "0";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(intLiteral->getSourceRange(), mutatedText);
    }
}
  
void MutatorFrontendAction_431::MutatorASTConsumer_431::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasAnyOperatorName("<", ">", "==", "!=", "<=", ">="),
                                               anyOf(hasLHS(callExpr(callee(functionDecl(hasAnyName("memcpy", "memmove", "memset", "strlen")))).bind("LibCall")),
                                                     hasRHS(callExpr(callee(functionDecl(hasAnyName("memcpy", "memmove", "memset", "strlen")))).bind("LibCall"))),
                                               anyOf(hasLHS(integerLiteral().bind("IntLiteral")),
                                                     hasRHS(integerLiteral().bind("IntLiteral")))).bind("BinOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}