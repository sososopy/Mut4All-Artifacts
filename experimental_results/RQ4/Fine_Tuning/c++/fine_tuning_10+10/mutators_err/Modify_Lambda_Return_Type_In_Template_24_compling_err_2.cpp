//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Return_Type_In_Template_24
 */ 
class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)

private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionTemplateDecl *CurFuncTemplate = nullptr;
    };
};

//source file
#include "../include/Modify_Lambda_Return_Type_In_Template_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        if (!CurFuncTemplate)
            return;

        const FunctionDecl *FuncDecl = CurFuncTemplate->getTemplatedDecl();
        const ParmVarDecl *FirstParam = nullptr;

        for (const ParmVarDecl *Param : FuncDecl->parameters()) {
            if (!LambdaExpr->capturesVariable(Param)) {
                FirstParam = Param;
                break;
            }
        }

        if (!FirstParam)
            return;

        std::string LambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LambdaExpr->getSourceRange());
        std::string ParamName = FirstParam->getNameAsString();
        std::string MutatedLambda = LambdaSource;
        size_t ArrowPos = MutatedLambda.find("->");

        if (ArrowPos == std::string::npos) {
            size_t BracePos = MutatedLambda.find("{");
            if (BracePos != std::string::npos) {
                MutatedLambda.insert(BracePos, " -> decltype(" + ParamName + ")");
            }
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), MutatedLambda);
    }
}

void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr(hasAncestor(functionTemplateDecl().bind("funcTemplate"))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}