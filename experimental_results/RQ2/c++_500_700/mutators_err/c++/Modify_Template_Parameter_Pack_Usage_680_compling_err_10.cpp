//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/StmtCXX.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ExprConcepts.h"

/**
 * modify_template_parameter_pack_usage_680
 */ 
class MutatorFrontendAction_680 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(680)

private:
    class MutatorASTConsumer_680 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_680(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_parameter_pack_usage_680.h"

// ========================================================================================================
#define MUT680_OUTPUT 1

void MutatorFrontendAction_680::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      for (auto *ReqClause : FT->getTemplateParameters()->asArray()) {
        if (auto *ReqExpr = llvm::dyn_cast<clang::TemplateTypeParmDecl>(ReqClause)) {
          if (ReqExpr->hasDefaultArgument()) {
            auto DefaultArg = ReqExpr->getDefaultArgument();
            if (auto *RequiresExpr = llvm::dyn_cast<clang::RequiresExpr>(DefaultArg->getTypePtr())) {
              auto clauseText = stringutils::rangetoStr(*(Result.SourceManager),
                                                        RequiresExpr->getSourceRange());
              std::string modifiedClause = "(true && ... && " + clauseText + ")";
              Rewrite.ReplaceText(RequiresExpr->getSourceRange(), modifiedClause);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_680::MutatorASTConsumer_680::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}