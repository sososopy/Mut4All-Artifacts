//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Constraint_Circular_Dependency_481
 */ 
class MutatorFrontendAction_481 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(481)

private:
    class MutatorASTConsumer_481 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_481(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Template_Constraint_Circular_Dependency_481.h"

// ========================================================================================================
#define MUT481_OUTPUT 1

void MutatorFrontendAction_481::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunctions")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (TD->isFunctionTemplateSpecialization())
        return;
      if (TD->isMemberSpecialization())
        return;
      if (!TD->isCompleteDefinition())
        return;
      if (!TD->hasAssociatedConstraints())
        return;
      llvm::outs() << TD->getNameAsString() << '\n';
      auto constraints = TD->getAssociatedConstraints();
      if (!constraints)
        return;
      auto constraint_expr = stringutils::rangetoStr(
          *(Result.SourceManager), constraints->getSourceRange());
      llvm::outs() << constraint_expr << '\n';
      auto new_constraint_expr =
          "requires std::is_constructible_v<std::decay_t<T>, T>";
      auto new_constraint = constraint_expr.replace(
          constraint_expr.find("requires"), new_constraint_expr.length(),
          new_constraint_expr);
      llvm::outs() << new_constraint << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(constraints->getSourceRange()),
          new_constraint);
    }
}
  
void MutatorFrontendAction_481::MutatorASTConsumer_481::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}