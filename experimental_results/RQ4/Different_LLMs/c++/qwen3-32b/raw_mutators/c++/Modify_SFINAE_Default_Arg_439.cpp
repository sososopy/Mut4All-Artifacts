//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_SFINAE_Default_Arg_439
 */ 
class MutatorFrontendAction_439 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(439)
private:
    class MutatorASTConsumer_439 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_439(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_SFINAE_Default_Arg_439.h"

// ========================================================================================================
#define MUT439_OUTPUT 1

void MutatorFrontendAction_439::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("function_template")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        if (auto *RE = FT->getRequiresClause()) {
            auto *ConstraintExpr = RE->getConstraintExpr();
            if (auto *DE = dyn_cast<clang::DecltypeExpr>(ConstraintExpr)) {
                SourceLocation Start = DE->getBeginLoc();
                SourceLocation End = DE->getEndLoc();
                if (Start.isInvalid() || End.isInvalid())
                    return;

                std::string originalExpr = stringutils::rangetoStr(*Result.SourceManager, SourceRange(Start, End));
                std::string mutatedExpr = "decltype(" + std::string("T::MemberFunction(") + 
                    originalExpr.substr(8, originalExpr.size()-9) + "+2))";

                Rewrite.ReplaceText(SourceRange(Start, End), mutatedExpr);
            }
        }
    }
}
  
void MutatorFrontendAction_439::MutatorASTConsumer_439::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasRequiresClause(
            requiresExpr(
                hasConstraint(
                    decltypeExpr().bind("decltype_expr")
                )
            )
        )
    ).bind("function_template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}