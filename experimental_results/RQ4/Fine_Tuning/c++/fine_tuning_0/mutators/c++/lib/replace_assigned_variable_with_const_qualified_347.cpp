//source file
#include "../include/Replace_Assigned_Variable_With_Const_Qualified_347.h"

// ========================================================================================================
#define MUT347_OUTPUT 1

void MutatorFrontendAction_347::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto vars = FD->decls();
      for (auto var : vars) {
        if (llvm::isa<VarDecl>(var)) {
          const VarDecl *vardecl = llvm::cast<VarDecl>(var);
          if (vardecl->getType().isConstQualified()) {
            func_var_map[FD].push_back(vardecl);
          }
        }
      }
    } else if (auto *CS = Result.Nodes.getNodeAs<clang::CompoundStmt>(
                   "CompoundStmt")) {
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto parents = Result.Context->getParents(*CS);
      if (parents.size() != 1)
        return;
      const Decl *decl = parents[0].get<Decl>();
      if (decl == nullptr)
        return;
      if (!llvm::isa<FunctionDecl>(decl))
        return;
      const FunctionDecl *FD = llvm::cast<FunctionDecl>(decl);
      auto vars = func_var_map[FD];
      if (vars.size() == 0) {
        auto decl = "const int mut_347 = 0;\n";
        Rewrite.ReplaceText(CS->getBeginLoc(), 0, std::string("/*mut347*/") + decl);
        return;
      }
      auto stmts = CS->body();
      for (auto stmt : stmts) {
        if (llvm::isa<BinaryOperator>(stmt)) {
          const BinaryOperator *bop = llvm::cast<BinaryOperator>(stmt);
          if (bop->isAssignmentOp()) {
            auto lhs = bop->getLHS();
            if (llvm::isa<DeclRefExpr>(lhs)) {
              const DeclRefExpr *declref = llvm::cast<DeclRefExpr>(lhs);
              const ValueDecl *vardecl = declref->getDecl();
              if (llvm::isa<VarDecl>(vardecl)) {
                const VarDecl *var = llvm::cast<VarDecl>(vardecl);
                for (auto v : vars) {
                  if (v == var)
                    continue;
                  auto name = v->getNameAsString();
                  Rewrite.ReplaceText(lhs->getSourceRange(), std::string("/*mut347*/") + name);
                  return;
                }
                auto decl = "const int mut_347 = 0;\n";
                Rewrite.ReplaceText(FD->getBody()->getBeginLoc(), 0,
                                    std::string("/*mut347*/") + decl);
                Rewrite.ReplaceText(lhs->getSourceRange(), std::string("/*mut347*/mut_347"));
                return;
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_347::MutatorASTConsumer_347::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto func_matcher = functionDecl().bind("Functions");
    auto compound_stmt_matcher = compoundStmt(hasParent(functionDecl())).bind("CompoundStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.addMatcher(compound_stmt_matcher, &callback);
    matchFinder.matchAST(Context);
}