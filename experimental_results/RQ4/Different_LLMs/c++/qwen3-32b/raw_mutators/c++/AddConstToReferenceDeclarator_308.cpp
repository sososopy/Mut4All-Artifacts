//header file
#pragma once
#include "Mutator_base.h"

/**
 * AddConstToReferenceDeclarator_308
 */ 
class MutatorFrontendAction_308 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(308)

private:
    class MutatorASTConsumer_308 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_308(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/AddConstToReferenceDeclarator_308.h"

// ========================================================================================================
#define MUT308_OUTPUT 1

void MutatorFrontendAction_308::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *parm = Result.Nodes.getNodeAs<clang::ParmVarDecl>("refParm")) {
        if (!parm || !Result.Context->getSourceManager().isWrittenInMainFile(parm->getLocation()))
            return;
        auto typeRange = parm->getTypeSourceInfo()->getTypeLoc().getSourceRange();
        Rewrite.InsertTextAfter(typeRange.getEnd(), " const");
    }
    else if (auto *var = Result.Nodes.getNodeAs<clang::VarDecl>("refVar")) {
        if (!var || !Result.Context->getSourceManager().isWrittenInMainFile(var->getLocation()))
            return;
        auto typeRange = var->getTypeSourceInfo()->getTypeLoc().getSourceRange();
        Rewrite.InsertTextAfter(typeRange.getEnd(), " const");
    }
    else if (auto *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("refReturn")) {
        if (!func || !Result.Context->getSourceManager().isWrittenInMainFile(func->getLocation()))
            return;
        auto typeLoc = func->getTypeSourceInfo()->getTypeLoc();
        if (auto funcTypeLoc = typeLoc.getAs<FunctionTypeLoc>()) {
            auto returnTypeRange = funcTypeLoc->getReturnSourceRange();
            Rewrite.InsertTextAfter(returnTypeRange.getEnd(), " const");
        }
    }
}
  
void MutatorFrontendAction_308::MutatorASTConsumer_308::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcherParm = parmVarDecl(hasType(referenceType())).bind("refParm");
    DeclarationMatcher matcherVar = varDecl(hasType(referenceType())).bind("refVar");
    DeclarationMatcher matcherFunc = functionDecl(hasAnyReturnType(referenceType())).bind("refReturn");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcherParm, &callback);
    matchFinder.addMatcher(matcherVar, &callback);
    matchFinder.addMatcher(matcherFunc, &callback);
    matchFinder.matchAST(Context);
}