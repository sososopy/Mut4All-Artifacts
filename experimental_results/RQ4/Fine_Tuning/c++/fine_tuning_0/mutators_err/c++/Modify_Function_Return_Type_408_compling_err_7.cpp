//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Return_Type_408
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
    };
};

//source file
#include "../include/Modify_Function_Return_Type_408.h"

// ========================================================================================================
#define MUT408_OUTPUT 1

void MutatorFrontendAction_408::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isMain())
        return;
      if (FD->getReturnType()->isVoidType())
        return;
      auto return_type = FD->getReturnType();
      auto return_type_str = return_type.getAsString();
      llvm::outs() << return_type_str << '\n';
      auto all_types = FD->getASTContext().getTranslationUnitDecl()->decls();
      vector<string> type_names;
      for (auto type : all_types) {
        if (type->getKind() == Decl::Kind::Typedef) {
          auto typedef_decl = dyn_cast<TypedefDecl>(type);
          auto typedef_type = typedef_decl->getUnderlyingType();
          auto typedef_type_str = typedef_type.getAsString();
          type_names.push_back(typedef_type_str);
        }
      }
      if (type_names.size() == 0)
        return;
      int index = getrandom::getRandomIndex(type_names.size() - 1);
      auto new_return_type = type_names[index];
      llvm::outs() << new_return_type << '\n';
      auto body = FD->getBody();
      auto return_stmts = body->children();
      for (auto stmt : return_stmts) {
        if (stmt->getStmtClass() == Stmt::StmtClass::ReturnStmtClass) {
          auto return_stmt = dyn_cast<ReturnStmt>(stmt);
          auto return_value = return_stmt->getRetValue();
          auto return_value_str =
              stringutils::rangetoStr(*(Result.SourceManager),
                                      return_value->getSourceRange());
          llvm::outs() << return_value_str << '\n';
          auto all_vars = body->decls();
          vector<string> var_names;
          for (auto var : all_vars) {
            if (var->getKind() == Decl::Kind::Var) {
              auto var_decl = dyn_cast<VarDecl>(var);
              auto var_type = var_decl->getType();
              auto var_type_str = var_type.getAsString();
              if (var_type_str == new_return_type) {
                var_names.push_back(var_decl->getNameAsString());
              }
            }
          }
          if (var_names.size() == 0) {
            auto new_var = new_return_type + " mut_var;";
            Rewrite.ReplaceText(body->getBeginLoc(), 0, new_var);
            var_names.push_back("mut_var");
          }
          int var_index = getrandom::getRandomIndex(var_names.size() - 1);
          auto new_return_value = var_names[var_index];
          Rewrite.ReplaceText(return_value->getSourceRange(), new_return_value);
        }
      }
      Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), new_return_type);
    }
}

void MutatorFrontendAction_408::MutatorASTConsumer_408::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}