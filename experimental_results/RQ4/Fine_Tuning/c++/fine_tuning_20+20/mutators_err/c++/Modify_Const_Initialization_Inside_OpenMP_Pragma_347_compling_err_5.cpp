//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Const_Initialization_Inside_OpenMP_Pragma_347
 */ 
class MutatorFrontendAction_347 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(347)

private:
    class MutatorASTConsumer_347 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_347(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::VarDecl *> const_vars;
        std::vector<const clang::Expr *> init_exprs;
        std::vector<const clang::Stmt *> omp_exprs;
    };
};

//source file
#include "../include/modify_const_initialization_inside_openmp_pragma_347.h"

// ========================================================================================================
#define MUT347_OUTPUT 1

void MutatorFrontendAction_347::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstVars")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->getType().isConstQualified())
        return;
      auto init = VD->getInit();
      if (!init)
        return;
      const_vars.push_back(VD);
      init_exprs.push_back(init);
    } else if (auto *OD = Result.Nodes.getNodeAs<clang::OMPExecutableDirective>(
                   "OmpDirectives")) {
      if (!OD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     OD->getBeginLoc()))
        return;
      auto exprs = OD->getAssociatedStmt()->children();
      for (auto expr : exprs) {
        if (expr) {
          omp_exprs.push_back(expr);
        }
      }
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (const_vars.empty() || omp_exprs.empty())
        return;
      auto choice = getrandom::getRandomIndex(const_vars.size() - 1);
      auto target_var = const_vars[choice];
      auto target_init = init_exprs[choice];
      auto target_stmt = stringutils::rangetoStr(
          *(Result.SourceManager), target_var->getSourceRange());
      auto target_expr = stringutils::rangetoStr(
          *(Result.SourceManager), target_init->getSourceRange());
      auto target_omp = omp_exprs[getrandom::getRandomIndex(omp_exprs.size() - 1)];
      auto omp_stmt = stringutils::rangetoStr(
          *(Result.SourceManager), target_omp->getSourceRange());
      auto new_stmt = target_stmt;
      if (target_expr.find("mut") != string::npos)
        return;
      if (omp_stmt.find("mut") != string::npos)
        return;
      if (target_expr.find("0") != string::npos) {
        new_stmt = stringutils::strReplace(new_stmt, target_expr, "mut347()");
      } else {
        new_stmt = stringutils::strReplace(new_stmt, target_expr,
                                           omp_stmt + " + mut347()");
      }
      new_stmt = "/*mut347*/" + new_stmt;
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(target_var->getSourceRange()),
          new_stmt);
      llvm::outs() << target_stmt << '\n';
      llvm::outs() << target_expr << '\n';
      llvm::outs() << omp_stmt << '\n';
    }
}
  
void MutatorFrontendAction_347::MutatorASTConsumer_347::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto constvar_matcher = varDecl().bind("ConstVars");
    auto omp_matcher = ompExecutableDirective().bind("OmpDirectives");
    auto func_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constvar_matcher, &callback);
    matchFinder.addMatcher(omp_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}