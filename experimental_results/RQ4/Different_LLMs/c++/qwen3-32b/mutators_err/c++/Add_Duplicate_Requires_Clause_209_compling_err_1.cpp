//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Duplicate_Requires_Clause_209
 */ 
class MutatorFrontendAction_209 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(209)

private:
    class MutatorASTConsumer_209 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_209(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Duplicate_Requires_Clause_209.h"

// ========================================================================================================
#define MUT209_OUTPUT 1

void MutatorFrontendAction_209::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("concept")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        if (auto *RE = CD->getDefinition()) {
            if (auto *REExpr = dyn_cast<clang::RequiresExpr>(RE)) {
                SourceLocation StartLoc = REExpr->getBeginLoc();
                SourceLocation EndLoc = REExpr->getEndLoc();
                if (StartLoc.isInvalid() || EndLoc.isInvalid())
                    return;

                std::string requiresClause = stringutils::rangetoStr(*Result.SourceManager, StartLoc, EndLoc);
                std::string mutatedClause = "requires " + requiresClause;
                Rewrite.ReplaceText(StartLoc, requiresClause.size(), mutatedClause);
            }
        }
    }
}

void MutatorFrontendAction_209::MutatorASTConsumer_209::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl(hasDefinition(requiresExpr())).bind("concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}