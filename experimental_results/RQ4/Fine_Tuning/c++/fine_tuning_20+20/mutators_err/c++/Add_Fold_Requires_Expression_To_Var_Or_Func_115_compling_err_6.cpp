//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Fold_Requires_Expression_To_Var_Or_Func_115
 */ 
class MutatorFrontendAction_115 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(115)

private:
    class MutatorASTConsumer_115 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_115(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::VarDecl *> cur_vars;
        std::vector<const clang::FunctionDecl *> cur_funcs;
    };
};

//source file
#include "../include/mutator_115.h"

// ========================================================================================================
#define MUT115_OUTPUT 1

void MutatorFrontendAction_115::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType()->isDependentType())
        return;
      if (VD->getType()->isUndeducedType())
        return;
      if (VD->getType()->isInstantiationDependentType())
        return;
      if (VD->getType()->isFoldableType())
        return;
      cur_vars.push_back(VD);
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      cur_funcs.push_back(FD);
    } else if (auto *PD = Result.Nodes.getNodeAs<clang::ParmVarDecl>(
                   "ParamVarDecl")) {
      if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PD->getLocation()))
        return;
      if (PD->getType()->isDependentType())
        return;
      if (PD->getType()->isUndeducedType())
        return;
      if (PD->getType()->isInstantiationDependentType())
        return;
      if (PD->getType()->isFoldableType())
        return;
      if (PD->getType()->isPackExpansionType())
        return;
      return;
    } else if (auto *RD =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Record")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition())
        return;
      if (!RD->isStruct() && !RD->isClass())
        return;
      if (RD->isLambda())
        return;
      return;
    } else if (auto *ED =
                   Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      if (!ED->isCompleteDefinition())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isInstantiationDependentType())
        return;
      if (FD->getReturnType()->isFoldableType())
        return;
      return;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD