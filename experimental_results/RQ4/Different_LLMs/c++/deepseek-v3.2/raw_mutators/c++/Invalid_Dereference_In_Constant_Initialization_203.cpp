//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Dereference_In_Constant_Initialization_203
 */ 
class MutatorFrontendAction_203 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(203)

private:
    class MutatorASTConsumer_203 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_203(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Dereference_In_Constant_Initialization_203.h"

// ========================================================================================================
#define MUT203_OUTPUT 1

void MutatorFrontendAction_203::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UE = Result.Nodes.getNodeAs<clang::UnaryOperator>("DerefExpr")) {
      //Filter nodes in header files
      if (!UE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UE->getLocation()))
        return;
      //Check that this is a dereference operator
      if (UE->getOpcode() != UO_Deref)
        return;
      //Check that this dereference is within a constant initialization context
      //We need to find the parent VarDecl and check if it has constant initialization
      const clang::Expr *SubExpr = UE->getSubExpr()->IgnoreParenImpCasts();
      if (!SubExpr)
        return;
      //Get the parent variable declaration
      clang::ASTContext *Ctx = Result.Context;
      clang::SourceManager &SM = Ctx->getSourceManager();
      clang::ParentMapContext &Parents = Ctx->getParentMapContext();
      auto ParentsList = Parents.getParents(*UE);
      if (ParentsList.empty())
        return;
      clang::DynTypedNode Parent = ParentsList[0];
      const clang::VarDecl *VarD = nullptr;
      //Traverse up to find the VarDecl
      while (true) {
        if (Parent.get<clang::VarDecl>()) {
          VarD = Parent.get<clang::VarDecl>();
          break;
        }
        else if (Parent.get<clang::Expr>()) {
          auto NextParents = Parents.getParents(Parent);
          if (NextParents.empty())
            break;
          Parent = NextParents[0];
        }
        else {
          break;
        }
      }
      if (!VarD)
        return;
      //Check if the variable is const-qualified or has static storage
      if (!VarD->getType().isConstQualified() && !VarD->hasGlobalStorage())
        return;
      //Check if this is part of the variable's initializer
      if (!VarD->hasInit())
        return;
      //Get the source code text of target node
      clang::SourceRange SubExprRange = SubExpr->getSourceRange();
      std::string SubExprText = stringutils::rangetoStr(SM, SubExprRange);
      //Perform mutation on the source code text by applying string replacement
      //Replace the subexpression with integer literal 0
      std::string Replacement = "0";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(SubExprRange, Replacement);
    }
}
  
void MutatorFrontendAction_203::MutatorASTConsumer_203::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = unaryOperator(hasOperatorName("*")).bind("DerefExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}