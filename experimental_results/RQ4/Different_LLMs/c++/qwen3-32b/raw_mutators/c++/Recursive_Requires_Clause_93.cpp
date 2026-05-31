//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Requires_Clause_93
 */ 
class MutatorFrontendAction_93 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(93)

private:
    class MutatorASTConsumer_93 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_93(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Recursive_Requires_Clause_93.h"

// ========================================================================================================
#define MUT93_OUTPUT 1

void MutatorFrontendAction_93::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("concept")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        if (auto *RE = CD->getConstraintExpr()) {
            if (auto *RExpr = dyn_cast<clang::RequiresExpr>(RE)) {
                if (RExpr->getNumExplicitParams() == 0) {
                    SourceLocation requiresLoc = RExpr->getRequiresLoc();
                    std::string conceptName = CD->getNameAsString();
                    std::string paramList = " (" + conceptName + " auto x)";
                    Rewrite.InsertTextAfter(requiresLoc, paramList);

                    if (auto *body = RExpr->getBody()) {
                        SourceRange bodyRange = body->getSourceRange();
                        std::string newBody = "{ x; }";
                        Rewrite.ReplaceText(bodyRange, newBody);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_93::MutatorASTConsumer_93::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}