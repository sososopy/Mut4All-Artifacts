//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Atomic_Qualifier_In_Assignment_237
 */ 
class MutatorFrontendAction_237 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(237)
private:
    class MutatorASTConsumer_237 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_237(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Atomic_Qualifier_In_Assignment_237.h"

// ========================================================================================================
#define MUT237_OUTPUT 1

void MutatorFrontendAction_237::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        
        if (!VD->getType()->isAtomicType())
            return;
        
        ASTContext &Ctx = *Result.Context;
        QualType declType = VD->getType();
        QualType innerType = declType->getAs<clang::AtomicType>()->getValueType();
        std::string innerTypeName = innerType.getAsString(Ctx.getPrintingPolicy());
        
        const clang::TypeSourceInfo *TInfo = VD->getTypeSourceInfo();
        SourceRange typeRange = TInfo->getTypeLoc().getSourceRange();
        
        Rewrite.ReplaceText(typeRange, innerTypeName);
    }
}
  
void MutatorFrontendAction_237::MutatorASTConsumer_237::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(atomicType()), hasInitializer(expr(hasType(atomicType())))).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}