//header file
#pragma once
#include "Mutator_base.h"

/**
 * Zero_initialized_Array_with_Non-zero_Initializer_466
 */ 
class MutatorFrontendAction_466 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(466)

private:
    class MutatorASTConsumer_466 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_466(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Zero_initialized_Array_with_Non-zero_Initializer_466.h"

// ========================================================================================================
#define MUT466_OUTPUT 1

void MutatorFrontendAction_466::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ZeroInitArray")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isConstexpr() || DL->isConstinit())
        return;
      if (DL->getType().isConstQualified() == false)
        return;
      if (DL->hasInit() == false)
        return;
      auto init = DL->getInit();
      if (init->getStmtClass() != Stmt::StmtClass::InitListExprClass)
        return;
      auto type = DL->getType();
      if (type->isArrayType() == false)
        return;
      auto arraytype = type->getAsArrayTypeUnsafe();
      if (arraytype->isSugared() == false)
        return;
      auto elementtype = arraytype->getElementType();
      if (elementtype->isStructureType() == false)
        return;
      auto initlist = dyn_cast<InitListExpr>(init);
      if (initlist->getNumInits() != 1)
        return;
      auto subinit = initlist->getInit(0);
      if (subinit->getStmtClass() != Stmt::StmtClass::CStyleCastExprClass)
        return;
      auto castsubinit = dyn_cast<CStyleCastExpr>(subinit);
      if (castsubinit->getSubExprAsWritten()->getStmtClass() !=
          Stmt::StmtClass::StringLiteralClass)
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          DL->getSourceRange());
      llvm::outs() << text << '\n';
      text = stringutils::replace(text, "(\"\")", "(0)");
      llvm::outs() << text << '\n';
      Rewrite.ReplaceText(DL->getSourceRange(), text);
    }
}
  
void MutatorFrontendAction_466::MutatorASTConsumer_466::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr(
                                      has(cStyleCastExpr(has(stringLiteral()))))))
                                      .bind("ZeroInitArray");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}