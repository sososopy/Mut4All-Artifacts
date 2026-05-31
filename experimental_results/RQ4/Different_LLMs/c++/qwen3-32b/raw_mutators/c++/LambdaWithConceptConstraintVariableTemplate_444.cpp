//header file
#pragma once
#include "Mutator_base.h"

/**
 * LambdaWithConceptConstraintVariableTemplate_444
 */ 
class MutatorFrontendAction_444 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(444)

private:
    class MutatorASTConsumer_444 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_444(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl* TargetFuncDecl = nullptr;
        std::string TargetFuncName;
        bool ConceptInserted = false;
    };
};

//source file
#include "../include/LambdaWithConceptConstraintVariableTemplate_444.h"

// ========================================================================================================
#define MUT444_OUTPUT 1

void MutatorFrontendAction_444::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FuncDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FuncDecl || !Result.Context->getSourceManager().isWrittenInMainFile(FuncDecl->getLocation()))
            return;

        if (FuncDecl->isTemplateInstantiation())
            return;

        TargetFuncDecl = FuncDecl;
        TargetFuncName = FuncDecl->getNameAsString();

        if (!ConceptInserted) {
            SourceLocation loc = FuncDecl->getLocation();
            std::string conceptDecl = "template<typename T>\nconcept MyConcept = true;\n";
            Rewrite.InsertText(loc, conceptDecl, true, true);
            ConceptInserted = true;
        }

        std::string lambdaParams;
        for (const auto *Param : FuncDecl->parameters()) {
            std::string paramType = Param->getType().getAsString();
            std::string paramName = Param->getNameAsString();
            lambdaParams += paramType + " " + paramName;
            if (Param != FuncDecl->parameters().back()) {
                lambdaParams += ", ";
            }
        }

        std::string originalBody;
        if (auto *Body = FuncDecl->getBody()) {
            originalBody = stringutils::rangetoStr(*Result.SourceManager, Body->getSourceRange());
        } else {
            originalBody = ";";
        }

        std::string newDecl = "template<typename T>\nauto " + TargetFuncName + " = [" + lambdaParams + "] requires MyConcept<T> {\n" + originalBody + "};\n";
        Rewrite.ReplaceText(FuncDecl->getSourceRange(), newDecl);
    }
    else if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getLocation()))
            return;

        if (auto *Callee = CallExpr->getDirectCallee()) {
            if (Callee == TargetFuncDecl) {
                std::string originalCall = stringutils::rangetoStr(*Result.SourceManager, CallExpr->getSourceRange());
                std::string newCall = TargetFuncName + "<int>()";
                Rewrite.ReplaceText(CallExpr->getSourceRange(), newCall);
            }
        }
    }
}

void MutatorFrontendAction_444::MutatorASTConsumer_444::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher funcMatcher = functionDecl().bind("funcDecl");
    DeclarationMatcher callMatcher = callExpr(callee(functionDecl())).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}