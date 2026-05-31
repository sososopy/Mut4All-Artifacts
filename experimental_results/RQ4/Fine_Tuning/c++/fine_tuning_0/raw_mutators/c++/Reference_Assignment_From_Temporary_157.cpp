//header file
#pragma once
#include "Mutator_base.h"

/**
 * Reference_Assignment_From_Temporary_157
 */ 
class MutatorFrontendAction_157 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(157)

private:
    class MutatorASTConsumer_157 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_157(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Reference_Assignment_From_Temporary_157.h"

// ========================================================================================================
#define MUT157_OUTPUT 1

void MutatorFrontendAction_157::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("RefVar")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType()->isReferenceType() == false)
        return;
      auto init = DL->getInit();
      if (init == nullptr)
        return;
      auto init_type = init->getType();
      if (init_type->isPointerType() || init_type->isReferenceType())
        return;
      auto init_expr = stringutils::rangetoStr(*(Result.SourceManager),
                                               init->getSourceRange());
      auto type = DL->getType();
      if (type->isPointerType() || type->isReferenceType())
        return;
      auto methods = init_type->getAsCXXRecordDecl()->methods();
      for (auto method : methods) {
        if (method->isImplicit())
          continue;
        if (method->getReturnType() == init_type) {
          string method_name = method->getNameAsString();
          init_expr += "." + method_name + "()";
          llvm::outs() << init_expr;
          Rewrite.ReplaceText(init->getSourceRange(), init_expr);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_157::MutatorASTConsumer_157::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("RefVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}