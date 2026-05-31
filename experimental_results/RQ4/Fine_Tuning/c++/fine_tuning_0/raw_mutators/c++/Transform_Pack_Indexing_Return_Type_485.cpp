//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Pack_Indexing_Return_Type_485
 */ 
class MutatorFrontendAction_485 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(485)

private:
    class MutatorASTConsumer_485 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_485(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Transform_Pack_Indexing_Return_Type_485.h"

// ========================================================================================================
#define MUT485_OUTPUT 1

void MutatorFrontendAction_485::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("PackIndexingFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplated()) {
        auto body = FD->getBody();
        if (!body)
          return;
        auto return_stmt = dyn_cast<ReturnStmt>(body->child_begin());
        if (!return_stmt)
          return;
        auto return_expr = return_stmt->getRetValue();
        if (!return_expr)
          return;
        auto return_type = return_expr->getType();
        if (!return_type->isDependentType())
          return;
        auto return_type_name = return_type.getAsString();
        llvm::outs() << return_type_name << '\n';
        auto new_return_expr =
            stringutils::rangetoStr(*(Result.SourceManager),
                                    return_expr->getSourceRange()) +
            ".__x0";
        llvm::outs() << new_return_expr << '\n';
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(return_expr->getSourceRange()),
            new_return_expr);
      }
    }
}
  
void MutatorFrontendAction_485::MutatorASTConsumer_485::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("PackIndexingFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}