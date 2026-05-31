//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Scoped_Static_Argument_Mutation_448
 */ 
class MutatorFrontendAction_448 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(448)

private:
    class MutatorASTConsumer_448 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_448(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *target_decl;
    };
};

//source file
#include "../include/Template_Scoped_Static_Argument_Mutation_448.h"

// ========================================================================================================
#define MUT448_OUTPUT 1

void MutatorFrontendAction_448::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("Template")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      llvm::outs() << "mut448: " << MT->getTemplateName().getAsTemplateDecl()
                          ->getNameAsString()
                   << '\n';
      auto args = MT->template_arguments();
      for (auto arg : args) {
        auto kind = arg.getKind();
        if (kind == TemplateArgument::ArgKind::Expression) {
          auto expr = arg.getAsExpr();
          if (auto declref = dyn_cast<DeclRefExpr>(expr)) {
            auto decl = declref->getDecl();
            if (auto var = dyn_cast<VarDecl>(decl)) {
              if (var->isStaticLocal())
                target_decl = var;
            }
          }
        }
      }
    }
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL != target_decl)
        return;
      auto DL_str = DL->getNameAsString();
      DL_str = "/*mut448*/" + DL_str;
      Rewrite.ReplaceText(DL->getBeginLoc(), DL_str);
    }
  }
  
void MutatorFrontendAction_448::MutatorASTConsumer_448::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = type().bind("Template");
    auto var_matcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}