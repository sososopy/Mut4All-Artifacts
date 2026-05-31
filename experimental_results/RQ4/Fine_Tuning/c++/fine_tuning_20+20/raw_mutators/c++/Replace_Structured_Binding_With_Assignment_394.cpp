//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Structured_Binding_With_Assignment_394
 */ 
class MutatorFrontendAction_394 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(394)

private:
    class MutatorASTConsumer_394 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_394(Rewriter &R) : TheRewriter(R) {}
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
        const clang::DeclRefExpr *target_expr;
        const clang::VarDecl *target_decl;
    };
};

//source file
#include "../include/Replace_Structured_Binding_With_Assignment_394.h"

// ========================================================================================================
#define MUT394_OUTPUT 1

void MutatorFrontendAction_394::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::DecompositionDecl>("Decompositions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      target_decl = DL;
      llvm::outs() << "match DecompositionDecl\n";
      auto DL_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            DL->getSourceRange());
      llvm::outs() << DL_str << '\n';
      DL_str = "/*mut394*/" + DL_str;
      Rewrite.ReplaceText(DL->getBeginLoc(), DL_str);
    } else if (auto *EX =
                   Result.Nodes.getNodeAs<clang::DeclRefExpr>("Exprs")) {
      if (!EX || !Result.Context->getSourceManager().isWrittenInMainFile(
                     EX->getBeginLoc()))
        return;
      if (EX->getDecl() != target_decl)
        return;
      target_expr = EX;
      llvm::outs() << "match DeclRefExpr\n";
      auto EX_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            EX->getSourceRange());
      llvm::outs() << EX_str << '\n';
      EX_str = "/*mut394*/" + EX_str;
      Rewrite.ReplaceText(EX->getBeginLoc(), EX_str);
    } else if (auto *ST = Result.Nodes.getNodeAs<clang::Stmt>("Stmts")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getBeginLoc()))
        return;
      if (target_expr == nullptr)
        return;
      if (ST->getBeginLoc() != target_expr->getBeginLoc())
        return;
      llvm::outs() << "match Stmt\n";
      auto DL_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            target_decl->getSourceRange());
      auto EX_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            target_expr->getSourceRange());
      auto ST_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            ST->getSourceRange());
      llvm::outs() << DL_str << '\n';
      llvm::outs() << EX_str << '\n';
      llvm::outs() << ST_str << '\n';
      DL_str = DL_str.substr(DL_str.find('[') + 1);
      DL_str = DL_str.substr(0, DL_str.rfind(']'));
      llvm::outs() << DL_str << '\n';
      DL_str += " = " + EX_str;
      DL_str = "/*mut394*/" + DL_str;
      Rewrite.ReplaceText(ST->getBeginLoc(), DL_str);
    }
}
  
void MutatorFrontendAction_394::MutatorASTConsumer_394::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = decompositionDecl().bind("Decompositions");
    auto expr_matcher = declRefExpr().bind("Exprs");
    auto stmt_matcher = stmt().bind("Stmts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(expr_matcher, &callback);
    matchFinder.addMatcher(stmt_matcher, &callback);
    matchFinder.matchAST(Context);
}