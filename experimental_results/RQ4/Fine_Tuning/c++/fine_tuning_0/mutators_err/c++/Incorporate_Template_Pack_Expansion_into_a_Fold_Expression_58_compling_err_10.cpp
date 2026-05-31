//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incorporate_Template_Pack_Expansion_into_a_Fold_Expression_58
 */ 
class MutatorFrontendAction_58 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(58)

private:
    class MutatorASTConsumer_58 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_58(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/incorporate_template_pack_expansion_into_a_fold_expression_58.h"

// ========================================================================================================
#define MUT58_OUTPUT 1

void MutatorFrontendAction_58::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FE = Result.Nodes.getNodeAs<clang::CXXFoldExpr>("FoldExpr")) {
      if (!FE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FE->getBeginLoc()))
        return;
      if (!FE->isInstantiationDependent())
        return;
      llvm::outs() << "FoldExpr: "
                   << stringutils::rangetoStr(*(Result.SourceManager),
                                              FE->getSourceRange())
                   << "\n";
      auto LHS = FE->getLHS();
      auto RHS = FE->getRHS();
      if (LHS == nullptr || RHS == nullptr)
        return;
      if (!RHS->isInstantiationDependent() && !LHS->isInstantiationDependent())
        return;
      auto LHS_str = stringutils::rangetoStr(*(Result.SourceManager),
                                             LHS->getSourceRange());
      auto RHS_str = stringutils::rangetoStr(*(Result.SourceManager),
                                             RHS->getSourceRange());
      auto op = FE->getOperator();
      string op_str = "";
      switch (op) {
      case clang::BO_Add:
        op_str = "+";
        break;
      case clang::BO_Mul:
        op_str = "*";
        break;
      case clang::BO_Or:
        op_str = "|";
        break;
      case clang::BO_And:
        op_str = "&";
        break;
      case clang::BO_LAnd:
        op_str = "&&";
        break;
      case clang::BO_LOr:
        op_str = "||";
        break;
      case clang::BO_Sub:
        op_str = "-";
        break;
      case clang::BO_Div:
        op_str = "/";
        break;
      default:
        llvm::outs() << "Unknown operator\n";
        return;
      }
      llvm::outs() << "Operator: " << op_str << "\n";
      // if (op_str == "+" || op_str == "*" || op_str == "|") {
      //   op_str = "&";
      // } else {
      //   op_str = "|";
      // }
      int choice = getrandom::getRandomIndex(1);
      if (choice == 0) {
        op_str = "&&";
        RHS_str = "true";
      } else {
        op_str = "||";
        RHS_str = "false";
      }
      auto new_fold_expr = "(" + LHS_str + " " + op_str + " ...) " + op_str +
                           " " + RHS_str;
      llvm::outs() << "New fold expr: " << new_fold_expr << "\n";
      Rewrite.ReplaceText(FE->getSourceRange(), new_fold_expr);
    }
}
  
void MutatorFrontendAction_58::MutatorASTConsumer_58::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxFoldExpr().bind("FoldExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}