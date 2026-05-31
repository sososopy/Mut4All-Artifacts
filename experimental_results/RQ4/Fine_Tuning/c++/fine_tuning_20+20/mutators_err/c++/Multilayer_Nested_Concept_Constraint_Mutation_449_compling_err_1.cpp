//header file
#pragma once
#include "Mutator_base.h"

/**
 * Multilayer_Nested_Concept_Constraint_Mutation_449
 */ 
class MutatorFrontendAction_449 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(449)

private:
    class MutatorASTConsumer_449 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_449(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl *> cur_concepts;
    };
};

//source file
#include "../include/Multilayer_Nested_Concept_Constraint_Mutation_449.h"

// ========================================================================================================
#define MUT449_OUTPUT 1

void MutatorFrontendAction_449::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithRequires")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithTemplate")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambda")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplate")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "ConceptsWithLambdaTemplateConstraint")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    }
  }

void MutatorFrontendAction_449::MutatorASTConsumer_449::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conceptDecl().bind("Concepts");
    auto matcher_with_requires = conceptDecl(hasRequiresClause()).bind("ConceptsWithRequires");
    auto matcher_with_constraint = conceptDecl(hasConstraintExpression()).bind("ConceptsWithConstraint");
    auto matcher_with_template = conceptDecl(hasTemplateParameterList()).bind("ConceptsWithTemplate");
    auto matcher_with_lambda = conceptDecl(hasRequiresClause(hasDescendant(lambdaExpr()))).bind("ConceptsWithLambda");
    auto matcher_with_lambda_template = conceptDecl(hasRequiresClause(hasDescendant(lambdaExpr(hasTemplateParameterList())))).bind("ConceptsWithLambdaTemplate");
    auto matcher_with_lambda_template_constraint = conceptDecl(hasRequiresClause(hasDescendant(lambdaExpr(hasTemplateParameterList(hasDescendant(conceptSpecializationExpr())))))).bind("ConceptsWithLambdaTemplateConstraint");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher_with_requires, &callback);
    matchFinder.addMatcher(matcher_with_constraint, &callback);
    matchFinder.addMatcher(matcher_with_template, &callback);
    matchFinder.addMatcher(matcher_with_lambda, &callback);
    matchFinder.addMatcher(matcher_with_lambda_template, &callback);
    matchFinder.addMatcher(matcher_with_lambda_template_constraint, &callback);
    matchFinder.matchAST(Context);
}