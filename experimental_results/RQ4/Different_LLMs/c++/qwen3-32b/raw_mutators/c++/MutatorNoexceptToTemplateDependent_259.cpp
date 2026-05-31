//header file
#pragma once
#include "Mutator_base.h"

/**
 * MutatorNoexceptToTemplateDependent_259
 */ 
class MutatorFrontendAction_259 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(259)

private:
    class MutatorASTConsumer_259 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_259(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/MutatorNoexceptToTemplateDependent_259.h"

// ========================================================================================================
#define MUT259_OUTPUT 1

void MutatorFrontendAction_259::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::CXXFunctionDecl>("functionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        const CXXNoexceptExpr *NoexceptExpr = FD->getNoexceptExpr();
        if (!NoexceptExpr)
            return;

        const Expr *InnerExpr = NoexceptExpr->getExpr();
        if (!InnerExpr)
            return;

        InnerExpr = InnerExpr->IgnoreParenImpCasts();
        if (!isa<clang::CXXBoolLiteralExpr>(InnerExpr))
            return;

        TemplateParameterList *TPL = nullptr;
        if (FD->getParent() && FD->getParent()->isTemplateInstantiation()) {
            const CXXRecordDecl *RD = FD->getParent();
            TPL = RD->getTemplateParameterList();
        } else if (FD->getDescribedFunctionTemplate()) {
            const FunctionTemplateDecl *FTD = FD->getDescribedFunctionTemplate();
            TPL = FTD->getTemplateParameters();
        }

        if (!TPL || TPL->size() == 0)
            return;

        const TemplateTypeParmDecl *TTP = dyn_cast<TemplateTypeParmDecl>(TPL->getParam(0));
        if (!TTP)
            return;

        std::string ParamName = TTP->getNameAsString();
        SourceLocation BoolLoc = InnerExpr->getBeginLoc();
        SourceLocation EndLoc = InnerExpr->getEndLoc();

        std::string Replacement = ParamName + "::value";
        Rewrite.ReplaceText(SourceRange(BoolLoc, EndLoc), Replacement);
    }
}
  
void MutatorFrontendAction_259::MutatorASTConsumer_259::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxFunctionDecl(hasDescendant(cxxNoexceptExpr(has(ignoringParenImpCasts(cxxBoolLiteral()))))).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}