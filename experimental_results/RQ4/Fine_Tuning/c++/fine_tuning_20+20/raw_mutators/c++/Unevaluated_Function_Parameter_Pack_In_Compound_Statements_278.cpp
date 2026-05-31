//header file
#pragma once
#include "Mutator_base.h"

/**
 * Unevaluated_Function_Parameter_Pack_In_Compound_Statements_278
 */ 
class MutatorFrontendAction_278 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(278)

private:
    class MutatorASTConsumer_278 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_278(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FunctionDecl *cur_func;
    };
};

//source file
#include "../include/unevaluated_function_parameter_pack_in_compound_statements_278.h"

// ========================================================================================================
#define MUT278_OUTPUT 1

void MutatorFrontendAction_278::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CS = Result.Nodes.getNodeAs<clang::CompoundStmt>("CompoundStmt")) {
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getBeginLoc()))
        return;
      if (cur_func == nullptr)
        return;
      auto params = cur_func->parameters();
      bool has_pack = false;
      for (auto param : params) {
        if (param->isParameterPack())
          has_pack = true;
      }
      if (!has_pack) {
        auto func_decl = stringutils::rangetoStr(*(Result.SourceManager),
                                                 cur_func->getSourceRange());
        if (cur_func->getNumParams() == 0) {
          if (cur_func->isVariadic())
            func_decl.insert(func_decl.rfind(")"), "int... mut_278");
          else
            func_decl.insert(func_decl.rfind(")"), "int... mut_278,");
        } else {
          if (cur_func->isVariadic())
            func_decl.insert(func_decl.rfind(")"), ", int... mut_278");
          else
            func_decl.insert(func_decl.rfind(")"), ", int... mut_278,");
        }
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(cur_func->getSourceRange()),
            func_decl);
        Rewrite.ReplaceText(CS->getBeginLoc(), 0, "/*mut278*/sizeof(mut_278);");
      } else {
        string pack_name = "";
        for (auto param : params) {
          if (param->isParameterPack()) {
            pack_name = param->getNameAsString();
            break;
          }
        }
        Rewrite.ReplaceText(CS->getBeginLoc(), 0,
                            "/*mut278*/sizeof(" + pack_name + ");");
      }
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      cur_func = FD;
    }
  }

void MutatorFrontendAction_278::MutatorASTConsumer_278::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto compound_matcher = compoundStmt().bind("CompoundStmt");
    auto func_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(compound_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}