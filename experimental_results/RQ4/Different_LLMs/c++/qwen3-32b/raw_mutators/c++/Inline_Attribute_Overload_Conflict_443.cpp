//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"

/**
 * Inline_Attribute_Overload_Conflict_443
 */ 
class MutatorFrontendAction_443 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(443)

private:
    class MutatorASTConsumer_443 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_443(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/inline_attribute_overload_conflict_443.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
        return;
      clang::FunctionDecl *FD = FTD->getTemplatedDecl();
      if (!FD || !FD->hasBody())
        return;
      clang::CompoundStmt *Body = FD->getBody();
      if (!Body)
        return;
      for (const auto *Stmt : Body->children()) {
        if (const auto *CE = clang::dyn_cast<clang::CallExpr>(Stmt)) {
          clang::FunctionDecl *CalledFD = CE->getDirectCallee();
          if (!CalledFD || CalledFD->isTemplateInstantiation())
            continue;
          if (FD->getNameAsString() != CalledFD->getNameAsString())
            continue;
          if (FD->getNumParams() != CalledFD->getNumParams())
            continue;
          bool typesDiffer = false;
          for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            if (FD->getParamDecl(i)->getType() != CalledFD->getParamDecl(i)->getType()) {
              typesDiffer = true;
              break;
            }
          }
          if (!typesDiffer)
            continue;
          clang::SourceLocation StartLoc = CE->getBeginLoc();
          clang::SourceLocation EndLoc = CE->getEndLoc();
          if (StartLoc.isInvalid() || EndLoc.isInvalid())
            continue;
          std::string InlineAttr = "[[clang::always_inline]] ";
          clang::ParmVarDecl *CalledParam = CalledFD->getParamDecl(0);
          std::string CastType = CalledParam->getType().getAsString();
          clang::Expr *Arg = CE->getArg(0);
          std::string ArgText = stringutils::rangetoStr(*Result.SourceManager, Arg->getSourceRange());
          std::string NewCall = InlineAttr + "f(reinterpret_cast<" + CastType + ">(" + ArgText + "))";
          Rewrite.ReplaceText(CE->getSourceRange(), NewCall);
        }
      }
    }
}
  
void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(functionDecl(hasBody(compoundStmt(hasDescendant(callExpr())))))).bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}