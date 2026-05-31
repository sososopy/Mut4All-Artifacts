//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Arg_With_Undefined_Call_247
 */ 
class MutatorFrontendAction_247 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(247)

private:
    class MutatorASTConsumer_247 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_247(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Default_Arg_With_Undefined_Call_247.h"

// ========================================================================================================
#define MUT247_OUTPUT 1

void MutatorFrontendAction_247::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PVD = Result.Nodes.getNodeAs<clang::ParmVarDecl>("defaultParam")) {
        // Filter nodes in header files
        if (!PVD || !Result.Context->getSourceManager().isWrittenInMainFile(PVD->getLocation()))
            return;

        // Check if the function is a template
        if (auto FD = PVD->getDeclContext()->getEnclosingFunctionOrMethod()) {
            if (!FD->isTemplated())
                return;

            if (!PVD->hasDefaultArg())
                return;

            const Expr *DefaultArgExpr = PVD->getDefaultArg();
            SourceRange ArgRange = DefaultArgExpr->getSourceRange();

            std::string ParamName = PVD->getNameAsString();
            std::string NewArg = "unknown(" + ParamName + ")()";

            Rewrite.ReplaceText(ArgRange, NewArg);
        }
    }
}
  
void MutatorFrontendAction_247::MutatorASTConsumer_247::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplate(), hasAnyParameter(parmVarDecl(hasDefaultArg()).bind("defaultParam"))).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}