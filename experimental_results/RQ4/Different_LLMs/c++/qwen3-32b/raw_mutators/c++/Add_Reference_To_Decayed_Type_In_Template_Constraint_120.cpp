//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Reference_To_Decayed_Type_In_Template_Constraint_120
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
#include "../include/Add_Reference_To_Decayed_Type_In_Template_Constraint_120.h"

// ========================================================================================================
#define MUT120_OUTPUT 1

void MutatorFrontendAction_120::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("targetConstructor")) {
        if (!ctor || !Result.Context->getSourceManager().isWrittenInMainFile(ctor->getLocation()))
            return;

        if (auto *req = ctor->getRequiresClause()) {
            if (auto *cond = req->getCondition()) {
                SourceManager &SM = *Result.SourceManager;
                auto condRange = cond->getSourceRange();
                std::string condText = stringutils::rangetoStr(SM, condRange);
                if (condText.find("std::is_constructible_v<std::decay_t<") != std::string::npos) {
                    size_t decayPos = condText.find("std::decay_t<");
                    if (decayPos != std::string::npos) {
                        size_t endDecay = condText.find('>', decayPos);
                        if (endDecay != std::string::npos) {
                            std::string newText = condText.substr(0, endDecay) + "&" + condText.substr(endDecay);
                            Rewrite.ReplaceText(condRange, newText);
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_120::MutatorASTConsumer_120::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(
        isExplicitlyCreatedByUser(),
        hasRequiresClause(anything())
    ).bind("targetConstructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}