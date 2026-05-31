//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Function_Call_Mutation_205
 */ 
class MutatorFrontendAction_205 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(205)

private:
    class MutatorASTConsumer_205 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_205(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        const clang::EnumDecl * cur_enum = nullptr;
    };
};

//source file
#include "../include/Enum_Function_Call_Mutation_205.h"

// ========================================================================================================
#define MUT205_OUTPUT 1

void MutatorFrontendAction_205::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_enum = DL;
    } else if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("FuncCall")) {
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getBeginLoc()))
        return;
      if (cur_enum == nullptr)
        return;
      auto callee = FC->getCalleeDecl();
      if (callee == nullptr)
        return;
      auto callee_name = callee->getNameInfo().getAsString();
      llvm::outs() << callee_name << '\n';
      auto args = FC->arguments();
      auto enum_name = cur_enum->getNameAsString();
      auto enums = cur_enum->enumerators();
      for (auto arg : args) {
        if (arg->getType().getAsString() == enum_name) {
          auto arg_content = stringutils::rangetoStr(
              *(Result.SourceManager), arg->getSourceRange());
          llvm::outs() << arg_content << '\n';
          for (auto en : enums) {
            auto en_name = en->getNameAsString();
            if (arg_content.find(en_name) != string::npos) {
              auto replace_content = enum_name + "::mut205";
              replace_content = "/*mut205*/" + replace_content;
              Rewrite.ReplaceText(
                  CharSourceRange::getTokenRange(arg->getSourceRange()),
                  replace_content);
            }
          }
        }
      }
    }
}

void MutatorFrontendAction_205::MutatorASTConsumer_205::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = enumDecl().bind("EnumDecl");
    auto func_matcher = callExpr().bind("FuncCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}