//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_structured_binding_with_non_class_type_31
 */ 
class MutatorFrontendAction_31 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(31)

private:
    class MutatorASTConsumer_31 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_31(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/invalid_structured_binding_with_non_class_type_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DS = Result.Nodes.getNodeAs<clang::DeclStmt>("declStmt")) {
        if (!DS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       DS->getBeginLoc()))
            return;

        if (auto *VD = dyn_cast<VarDecl>(DS->getSingleDecl())) {
            if (VD->isStructuredBinding()) {
                auto initExpr = VD->getInit();
                if (initExpr && !initExpr->getType()->isAggregateType()) {
                    std::string replacement = "char myChar = 'a';";
                    Rewrite.ReplaceText(DS->getSourceRange(), replacement);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = declStmt(hasSingleDecl(varDecl(hasInitializer(expr())).bind("varDecl"))).bind("declStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}