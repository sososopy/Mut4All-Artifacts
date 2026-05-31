//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Constructor_Constraint_120
 */ 
class MutatorFrontendAction_120 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(120)

private:
    class MutatorASTConsumer_120 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_120(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_constructor_constraint_120.h"

// ========================================================================================================
#define MUT120_OUTPUT 1

void MutatorFrontendAction_120::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateConstructor")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation()) {
        //Get the source code text of target node
        auto requiresClause = FD->getTrailingRequiresClause();
        if (!requiresClause)
          return;

        auto constraintExpr = requiresClause->getConstraintExpr();
        if (!constraintExpr)
          return;

        //Perform mutation on the source code text by applying string replacement
        std::string originalConstraint = stringutils::rangetoStr(*(Result.SourceManager), constraintExpr->getSourceRange());
        std::string mutatedConstraint = "std::is_constructible_v<std::decay_t<T>, T>";
        std::string newConstraint = "requires " + mutatedConstraint;

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(requiresClause->getSourceRange(), newConstraint);
      }
    }
}
  
void MutatorFrontendAction_120::MutatorASTConsumer_120::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasRequiresClause()).bind("TemplateConstructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}