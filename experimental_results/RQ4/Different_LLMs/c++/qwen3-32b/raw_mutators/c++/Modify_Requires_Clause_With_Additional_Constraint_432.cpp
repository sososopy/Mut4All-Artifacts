//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Requires_Clause_With_Additional_Constraint_432
 */ 
class MutatorFrontendAction_432 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(432)

private:
    class MutatorASTConsumer_432 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_432(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableConcepts;
    };
};

//source file
#include "../include/Modify_Requires_Clause_With_Additional_Constraint_432.h"

// ========================================================================================================
#define MUT432_OUTPUT 1

void MutatorFrontendAction_432::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("concept")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        availableConcepts.push_back(CD->getNameAsString());
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("funcTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;
        auto *RE = FTD->getRequiresClause();
        if (!RE)
            return;
        auto *constraintExpr = RE->getConstraintExpr();
        if (!constraintExpr)
            return;
        SourceManager &SM = *Result.SourceManager;
        if (!SM.isInMainFile(constraintExpr->getBeginLoc()))
            return;
        std::string originalConstraint = stringutils::rangetoStr(SM, constraintExpr->getSourceRange());
        std::string newConstraint;
        if (!availableConcepts.empty()) {
            int index = getrandom::getRandomIndex(availableConcepts.size());
            std::string newConcept = availableConcepts[index];
            newConstraint = originalConstraint + " && " + newConcept + "<T>";
        } else {
            newConstraint = originalConstraint + " && (sizeof(T) > 0)";
        }
        Rewrite.ReplaceText(constraintExpr->getSourceRange(), newConstraint);
    }
}
  
void MutatorFrontendAction_432::MutatorASTConsumer_432::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = functionTemplateDecl(hasRequiresClause()).bind("funcTemplate");
    DeclarationMatcher matcher2 = conceptDecl().bind("concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}