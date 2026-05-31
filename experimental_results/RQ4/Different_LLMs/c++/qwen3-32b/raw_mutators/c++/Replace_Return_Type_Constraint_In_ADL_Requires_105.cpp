//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Return_Type_Constraint_In_ADL_Requires_105
 */ 
class MutatorFrontendAction_105 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(105)

private:
    class MutatorASTConsumer_105 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_105(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Return_Type_Constraint_In_ADL_Requires_105.h"

// ========================================================================================================
#define MUT105_OUTPUT 1

void MutatorFrontendAction_105::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("requiresExpr")) {
        if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(RE->getLocation()))
            return;

        if (auto *CSE = RE->getReturnTypeConstraint()) {
            std::string currentConcept = Rewrite.getRewrittenText(CSE->getSourceRange());
            std::vector<std::string> replacements = {
                "std::forward_iterator",
                "std::bidirectional_iterator",
                "std::random_access_iterator",
                "std::contiguous_iterator"
            };
            auto it = std::find(replacements.begin(), replacements.end(), currentConcept);
            if (it != replacements.end()) {
                replacements.erase(it);
            }
            if (!replacements.empty()) {
                int index = getrandom::getRandomIndex(replacements.size());
                std::string newConcept = replacements[index];
                Rewrite.ReplaceText(CSE->getSourceRange(), newConcept);
            }
        }
    }
}
  
void MutatorFrontendAction_105::MutatorASTConsumer_105::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = requiresExpr().bind("requiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}