//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Exception_Specifier_In_Function_Def_259
 */ 
class MutatorFrontendAction_259 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(259)

private:
    class MutatorASTConsumer_259 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_259(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Exception_Specifier_In_Function_Def_259.h"

// ========================================================================================================
#define MUT259_OUTPUT 1

void MutatorFrontendAction_259::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Func")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->hasAttr<CUDADeviceAttr>() || FD->hasAttr<CUDAGlobalAttr>() ||
          FD->hasAttr<CUDAHostAttr>())
        return;
      if (FD->isMain())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->getExceptionSpecType() == EST_None)
        return;
      auto exception = FD->getExceptionSpecType();
      llvm::outs() << exception << '\n';
      if (exception == 3) { // noexcept
        auto noexceptexpr = FD->getNoexceptExpr();
        if (noexceptexpr == nullptr)
          return;
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               noexceptexpr->getSourceRange());
        llvm::outs() << content << '\n';
        auto funcs = FD->getTranslationUnitDecl()->decls();
        for (auto func : funcs) {
          if (func->getKind() == clang::Decl::Kind::Function) {
            auto funcdecl = dyn_cast<FunctionDecl>(func);
            if (funcdecl->isConstexpr() == false) {
              auto name = funcdecl->getNameAsString();
              content = name + "()";
              break;
            }
          }
        }
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(noexceptexpr->getSourceRange()),
            content);
      } else if (exception == 2) { // throw()
        auto content = "mut_259";
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getEndLoc(), FD->getEndLoc()),
            content);
      }
    }
}
  
void MutatorFrontendAction_259::MutatorASTConsumer_259::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}