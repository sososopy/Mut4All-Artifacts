//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Standard_Constraint_With_Custom_Concept_In_Partial_Specialization_441
 */ 
class MutatorFrontendAction_441 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(441)

private:
    class MutatorASTConsumer_441 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_441(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Standard_Constraint_With_Custom_Concept_In_Partial_Specialization_441.h"

// ========================================================================================================
#define MUT441_OUTPUT 1

void MutatorFrontendAction_441::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PartialSpec = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("partialSpec")) {
        if (!PartialSpec || !Result.Context->getSourceManager().isWrittenInMainFile(PartialSpec->getLocation()))
            return;

        if (auto *RequiresClause = PartialSpec->getRequiresClause()) {
            if (auto *Condition = RequiresClause->getCondition()) {
                const TemplateParameterList *TPL = PartialSpec->getTemplateParameters();
                if (!TPL || TPL->size() != 1)
                    return;

                const TemplateTypeParmDecl *TTP = dyn_cast<TemplateTypeParmDecl>(TPL->getParam(0));
                if (!TTP)
                    return;

                std::string ParamName = TTP->getNameAsString();
                std::string Replacement = "[]<typename U>(U& u) { return requires { typename Checker<U>(); }; }(declval<" + ParamName + "&>())";
                Rewrite.ReplaceText(Condition->getSourceRange(), Replacement);
            }
        }
    }
}
  
void MutatorFrontendAction_441::MutatorASTConsumer_441::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl(hasRequiresClause()).bind("partialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}