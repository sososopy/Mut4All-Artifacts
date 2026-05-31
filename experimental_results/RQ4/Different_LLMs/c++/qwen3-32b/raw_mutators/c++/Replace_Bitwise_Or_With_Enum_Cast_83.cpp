//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Bitwise_Or_With_Enum_Cast_83
 */ 
class MutatorFrontendAction_83 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(83)

private:
    class MutatorASTConsumer_83 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_83(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Bitwise_Or_With_Enum_Cast_83.h"

// ========================================================================================================
#define MUT83_OUTPUT 1

void MutatorFrontendAction_83::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        auto Init = VD->getInit();
        if (!Init)
            return;

        auto *BO = dyn_cast<clang::BinaryOperator>(Init);
        if (!BO || BO->getOpcode() != clang::BO_OR)
            return;

        auto *LHS = BO->getLHS()->IgnoreImplicit();
        auto *RHS = BO->getRHS()->IgnoreImplicit();

        auto *LHS_ECD = dyn_cast<clang::EnumConstantDecl>(LHS);
        auto *RHS_ECD = dyn_cast<clang::EnumConstantDecl>(RHS);

        if (!LHS_ECD || !RHS_ECD)
            return;

        auto *LHS_ED = dyn_cast<clang::EnumDecl>(LHS_ECD->getDeclContext());
        auto *RHS_ED = dyn_cast<clang::EnumDecl>(RHS_ECD->getDeclContext());

        if (!LHS_ED || !RHS_ED || LHS_ED != RHS_ED)
            return;

        std::string enumQualifiedName = LHS_ED->getQualifiedNameAsString();

        // Replace the type
        auto *TypeLoc = VD->getTypeSourceInfo()->getTypeLoc();
        SourceRange typeRange = TypeLoc->getSourceRange();
        Rewrite.ReplaceText(typeRange, enumQualifiedName);

        // Replace the initializer with a cast
        SourceRange boRange = BO->getSourceRange();
        std::string castedInit = "(" + enumQualifiedName + ")(" + 
                                 Rewrite.getRewrittenText(boRange) + ")";
        Rewrite.ReplaceText(boRange, castedInit);
    }
}

void MutatorFrontendAction_83::MutatorASTConsumer_83::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        varDecl(
            hasType(asString("int")),
            hasInitializer(
                binaryOperator(
                    hasOperatorName("|"),
                    hasLHS(enumConstantDecl()),
                    hasRHS(enumConstantDecl())
                )
            )
        ).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}