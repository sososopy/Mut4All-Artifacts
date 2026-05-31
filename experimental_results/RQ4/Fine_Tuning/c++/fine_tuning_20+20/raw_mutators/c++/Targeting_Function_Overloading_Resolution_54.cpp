//header file
#pragma once
#include "Mutator_base.h"

/**
 * Targeting_Function_Overloading_Resolution_54
 */ 
class MutatorFrontendAction_54 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(54)

private:
    class MutatorASTConsumer_54 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_54(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> cur_funcs;
    };
};

//source file
#include "../include/targeting_function_overloading_resolution_54.h"

// ========================================================================================================
#define MUT54_OUTPUT 1

void MutatorFrontendAction_54::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      auto name = FD->getNameAsString();
      if (name == "")
        return;
      if (FD->getNumParams() != 1)
        return;
      if (FD->isOverloadedOperator())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isTemplated())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      llvm::outs() << name << '\n';
      cur_funcs.push_back(FD);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::Decl>("Decls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (cur_funcs.size() < 2)
        return;
      std::map<string, int> name_count;
      for (auto func : cur_funcs) {
        auto name = func->getNameAsString();
        if (name_count.find(name) == name_count.end()) {
          name_count[name] = 1;
        } else {
          name_count[name]++;
        }
      }
      string target = "";
      for (auto [key, value] : name_count) {
        if (value >= 2) {
          target = key;
          break;
        }
      }
      if (target == "")
        return;
      std::vector<const clang::FunctionDecl *> target_funcs;
      for (auto func : cur_funcs) {
        if (func->getNameAsString() == target)
          target_funcs.push_back(func);
      }
      if (target_funcs.size() < 2)
        return;
      llvm::outs() << target << '\n';
      auto first_type = target_funcs[0]->getParamDecl(0)->getType();
      auto second_type = target_funcs[1]->getParamDecl(0)->getType();
      llvm::outs() << first_type.getAsString() << '\n';
      llvm::outs() << second_type.getAsString() << '\n';
      string call = "";
      if (first_type->isIntegerType() && second_type->isFloatingType()) {
        call = target + "(1)";
      } else if (first_type->isFloatingType() && second_type->isIntegerType()) {
        call = target + "(1)";
      } else if (first_type->isPointerType() && second_type->isArrayType()) {
        call = target + "(nullptr)";
      } else if (first_type->isArrayType() && second_type->isPointerType()) {
        call = target + "(nullptr)";
      } else if (first_type->isPointerType() && second_type->isPointerType()) {
        call = target + "(nullptr)";
      } else if (first_type->isArrayType() && second_type->isArrayType()) {
        call = target + "(nullptr)";
      } else if (first_type->isPointerType() && second_type->isIntegerType()) {
        call = target + "(0)";
      } else if (first_type->isIntegerType() && second_type->isPointerType()) {
        call = target + "(0)";
      } else if (first_type->isPointerType() && second_type->isFloatingType()) {
        call = target + "(0)";
      } else if (first_type->isFloatingType() && second_type->isPointerType()) {
        call = target + "(0)";
      } else if (first_type->isArrayType() && second_type->isIntegerType()) {
        call = target + "(0)";
      } else if (first_type->isIntegerType() && second_type->isArrayType()) {
        call = target + "(0)";
      } else if (first_type->isArrayType() && second_type->isFloatingType()) {
        call = target + "(0)";
      } else if (first_type->isFloatingType() && second_type->isArrayType()) {
        call = target + "(0)";
      }
      llvm::outs() << call << '\n';
      Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut54*/" + call + ";\n");
    }
  }

void MutatorFrontendAction_54::MutatorASTConsumer_54::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto decl_matcher = decl().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}