//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Pointer_With_Direct_Instance_201
 */ 
class MutatorFrontendAction_201 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(201)

private:
    class MutatorASTConsumer_201 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_201(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Pointer_With_Direct_Instance_201.h"

// ========================================================================================================
#define MUT201_OUTPUT 1

void MutatorFrontendAction_201::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        auto *ClassDecl = FD->getParent();
        if (!ClassDecl || !ClassDecl->isCompleteDefinition())
            return;
        
        auto typeRange = FD->getTypeSourceInfo()->getTypeLoc().getSourceRange();
        if (typeRange.isInvalid())
            return;
        
        std::string className = ClassDecl->getNameAsString();
        if (className.empty())
            return;
        
        Rewrite.ReplaceText(typeRange, className);
    }
}

void MutatorFrontendAction_201::MutatorASTConsumer_201::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        has(fieldDecl(
            anyOf(
                hasType(pointsTo(qualType(isSameOrCompatibleType(declType())))),
                hasType(references(qualType(isSameOrCompatibleType(declType()))))
            )
        ).bind("field")
    ).bind("class");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}