//header file
#pragma once
#include "Mutator_base.h"

/**
 * ForwardDeclaredLambdaDefaultArg_384
 */ 
class MutatorFrontendAction_384 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(384)

private:
    class MutatorASTConsumer_384 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_384(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/ForwardDeclaredLambdaDefaultArg_384.h"

// ========================================================================================================
#define MUT384_OUTPUT 1

void MutatorFrontendAction_384::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FuncDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FuncDecl || !Result.Context->getSourceManager().isWrittenInMainFile(FuncDecl->getLocation()))
            return;

        for (auto *Param : FuncDecl->parameters()) {
            QualType ParamType = Param->getType();
            if (ParamType->isRecordType() && ParamType->isIncompleteType()) {
                std::string ParamText = stringutils::rangetoStr(*Result.SourceManager, Param->getSourceRange());
                std::string NewParamText = ParamText + " = [] { return " + Param->getNameAsString() + "; }";
                Rewrite.ReplaceText(Param->getSourceRange(), NewParamText);
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_384::MutatorASTConsumer_384::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasType(type(isIncomplete())))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}