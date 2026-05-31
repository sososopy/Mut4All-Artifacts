//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_constraint_satisfaction_587
 */ 
class MutatorFrontendAction_587 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(587)

private:
    class MutatorASTConsumer_587 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_587(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_constraint_satisfaction_587.h"

// ========================================================================================================
#define MUT587_OUTPUT 1

void MutatorFrontendAction_587::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (auto *RC = FD->getRequiresClause()) {
            std::string newConcept = "new_concept";
            std::string condition = "/* some condition on T */";

            std::string newConceptDecl = "template<typename T>\nconcept " + newConcept + " = " + condition + ";\n";
            Rewrite.InsertText(FD->getBeginLoc(), newConceptDecl);

            std::string newConstraint = "(" + RC->getSourceRange().getAsString() + " && " + newConcept + "<U>)";
            Rewrite.ReplaceText(RC->getSourceRange(), newConstraint);
        }
    }
}

void MutatorFrontendAction_587::MutatorASTConsumer_587::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasRequiresClause()).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}