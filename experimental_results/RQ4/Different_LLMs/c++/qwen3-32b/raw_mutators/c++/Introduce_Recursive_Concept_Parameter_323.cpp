//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"

/**
 * Introduce_Recursive_Concept_Parameter_323
 */ 
class MutatorFrontendAction_Introduce_Recursive_Concept_Parameter_323 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Introduce_Recursive_Concept_Parameter_323)
private:
    class MutatorASTConsumer_Introduce_Recursive_Concept_Parameter_323 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Introduce_Recursive_Concept_Parameter_323(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/introduce_recursive_concept_parameter_323.h"

// ========================================================================================================
#define MUTIntroduce_Recursive_Concept_Parameter_323_OUTPUT 1

void MutatorFrontendAction_Introduce_Recursive_Concept_Parameter_323::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("concept")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        auto *constraint = CD->getConstraint();
        if (auto *RE = dyn_cast<clang::RequiresExpr>(constraint)) {
            for (auto *param : RE->parameters()) {
                auto *typeLoc = param->getTypeSourceInfo()->getTypeLoc();
                SourceLocation start = typeLoc.getBeginLoc();
                SourceLocation end = typeLoc.getEndLoc();
                if (start.isInvalid() || end.isInvalid())
                    continue;

                std::string conceptName = CD->getNameAsString();
                std::string newType = conceptName + " auto";
                Rewrite.ReplaceText(SourceRange(start, end), newType);
            }
        }
    }
}
  
void MutatorFrontendAction_Introduce_Recursive_Concept_Parameter_323::MutatorASTConsumer_Introduce_Recursive_Concept_Parameter_323::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl(has(requiresExpr())).bind("concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}