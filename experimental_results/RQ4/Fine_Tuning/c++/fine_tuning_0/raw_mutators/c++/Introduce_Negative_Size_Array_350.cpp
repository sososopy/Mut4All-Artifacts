//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Negative_Size_Array_350
 */ 
class MutatorFrontendAction_350 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(350)

private:
    class MutatorASTConsumer_350 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_350(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> cur_arrays;
    };
};

//source file
#include "../include/Introduce_Negative_Size_Array_350.h"

// ========================================================================================================
#define MUT350_OUTPUT 1

void MutatorFrontendAction_350::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isMain())
      return;
    if (DL->hasBody() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    auto vars = DL->decls();
    for (auto var : vars) {
      if (var->getKind() == Decl::Kind::Var) {
        auto vardecl = dyn_cast<VarDecl>(var);
        if (vardecl->getType()->isArrayType()) {
          cur_arrays.push_back(vardecl);
        }
      }
    }
    if (cur_arrays.empty()) {
      content.insert(content.rfind('}'), "/*mut350*/int mut_350[-1]={};");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    } else {
      size_t index = getrandom::getRandomIndex(cur_arrays.size() - 1);
      auto target = cur_arrays[index];
      auto arraytype = target->getType()->getAsArrayTypeUnsafe();
      auto size = arraytype->getSizeExpr();
      auto size_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              size->getSourceRange());
      size_str = "/*mut350*/-" + size_str;
      Rewrite.ReplaceText(size->getSourceRange(), size_str);
    }
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TU")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    auto decls = DL->decls();
    for (auto decl : decls) {
      if (decl->getKind() == Decl::Kind::Var) {
        auto vardecl = dyn_cast<VarDecl>(decl);
        if (vardecl->getType()->isArrayType()) {
          cur_arrays.push_back(vardecl);
        }
      }
    }
    if (cur_arrays.empty()) {
      Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut350*/int mut_350[-1]={};");
    } else {
      size_t index = getrandom::getRandomIndex(cur_arrays.size() - 1);
      auto target = cur_arrays[index];
      auto arraytype = target->getType()->getAsArrayTypeUnsafe();
      auto size = arraytype->getSizeExpr();
      auto size_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              size->getSourceRange());
      size_str = "/*mut350*/-" + size_str;
      Rewrite.ReplaceText(size->getSourceRange(), size_str);
    }
  }
}
  
void MutatorFrontendAction_350::MutatorASTConsumer_350::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto TU_matcher = translationUnitDecl().bind("TU");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(TU_matcher, &callback);
    matchFinder.matchAST(Context);
}