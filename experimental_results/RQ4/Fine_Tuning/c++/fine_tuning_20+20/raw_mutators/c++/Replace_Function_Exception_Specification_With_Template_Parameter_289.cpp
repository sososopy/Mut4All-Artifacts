//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Exception_Specification_With_Template_Parameter_289
 */ 
class MutatorFrontendAction_289 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(289)

private:
    class MutatorASTConsumer_289 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_289(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> functions;
    };
};

//source file
#include "../include/replace_function_exception_specification_with_template_parameter_289.h"

// ========================================================================================================
#define MUT289_OUTPUT 1

void MutatorFrontendAction_289::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->isDefaulted())
      return;
    if (FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplate)
      return;
    if (FD->getTemplatedKind() == FunctionDecl::TK_MemberSpecialization)
      return;
    if (FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplateSpecialization)
      return;
    functions.push_back(FD);
  } else if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateDecl>(
                 "TemplateDecl")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getBeginLoc()))
      return;
    if (TP->getTemplatedKind() != TemplateDecl::TK_FunctionTemplate)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_MemberSpecialization)
      return;
    if (TP->getTemplatedKind() ==
        TemplateDecl::TK_FunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunctionTemplateSpecialization)
      return;
    if (TP->getTemplatedKind() == TemplateDecl::TK_DependentFunction