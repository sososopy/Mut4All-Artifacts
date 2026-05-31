//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Noexcept_With_Requires_482
 */ 
class MutatorFrontendAction_482 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(482)

private:
    class MutatorASTConsumer_482 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_482(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Lambda_Noexcept_With_Requires_482.h"

// ========================================================================================================
#define MUT482_OUTPUT 1

void MutatorFrontendAction_482::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        clang::DeclContext *DC = lambda->getDeclContext();
        clang::FunctionDecl *FD = dyn_cast<clang::FunctionDecl>(DC);
        if (!FD)
            return;
        clang::FunctionTemplateDecl *FTD = FD->getDescribedFunctionTemplate();
        if (!FTD)
            return;

        clang::TemplateParameterList *TPL = FTD->getTemplateParameters();
        if (!TPL || TPL->size() < 1)
            return;

        clang::TemplateTypeParmDecl *TTP = dyn_cast<clang::TemplateTypeParmDecl>(TPL->getParam(0));
        if (!TTP)
            return;

        std::string TName = TTP->getNameAsString();

        SourceManager &SM = Result.Context->getSourceManager();
        const char *start = SM.getCharacterData(lambda->getBeginLoc());
        const char *end = SM.getCharacterData(lambda->getEndLoc());
        std::string lambdaCode(start, end - start);

        size_t captureEndPos = lambdaCode.find(']') + 1;
        if (captureEndPos == std::string::npos)
            return;

        std::string newLambdaCode = lambdaCode.substr(0, captureEndPos) + "<typename U>" + lambdaCode.substr(captureEndPos);

        size_t noexceptPos = newLambdaCode.find("noexcept");
        if (noexceptPos == std::string::npos)
            return;

        size_t openParenPos = noexceptPos + 8;
        size_t closeParenPos = newLambdaCode.find(')', openParenPos);
        if (openParenPos == std::string::npos || closeParenPos == std::string::npos)
            return;

        std::string requiresClause = "requires { typename " + TName + "::X; }";
        newLambdaCode.replace(openParenPos + 1, closeParenPos - openParenPos - 1, requiresClause);

        Rewrite.ReplaceText(lambda->getSourceRange(), newLambdaCode);
    }
}

void MutatorFrontendAction_482::MutatorASTConsumer_482::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(
        hasAncestor(functionTemplateDecl(
            hasTemplateParameters(1, anything())
        ))
    ).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}