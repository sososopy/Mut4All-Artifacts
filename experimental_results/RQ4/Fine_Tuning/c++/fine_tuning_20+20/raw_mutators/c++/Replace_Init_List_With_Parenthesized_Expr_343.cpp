//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Init_List_With_Parenthesized_Expr_343
 */ 
class MutatorFrontendAction_343 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(343)

private:
    class MutatorASTConsumer_343 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_343(Rewriter &R) : TheRewriter(R) {}
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
        const clang::InitListExpr *target_expr;
    };
};

//source file
#include "../include/replace_init_list_with_parenthesized_expr_343.h"

// ========================================================================================================
#define MUT343_OUTPUT 1

void MutatorFrontendAction_343::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isStaticDataMember())
        return;
      if (MT->isLocalVarDeclOrParm() == false)
        return;
      if (MT->hasInit() == false)
        return;
      if (MT->getInitStyle() != VarDecl::InitializationStyle::CInit)
        return;
      auto init = MT->getInit();
      if (init->getStmtClass() != Stmt::InitListExprClass)
        return;
      auto init_expr = cast<InitListExpr>(init);
      if (init_expr->getNumInits() != 1)
        return;
      target_expr = init_expr;
      auto init_code = stringutils::rangetoStr(*(Result.SourceManager),
                                               init_expr->getSourceRange());
      llvm::outs() << init_code << '\n';
      if (init_code.front() == '{')
        init_code.front() = '(';
      if (init_code.back() == '}')
        init_code.back() = ')';
      init_code = "/*mut343*/" + init_code + ",";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              target_expr->getSourceRange()),
                          init_code);
    }
}

void MutatorFrontendAction_343::MutatorASTConsumer_343::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}