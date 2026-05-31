//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Inheriting_Constructor_With_Delegate_414
 */ 
class MutatorFrontendAction_414 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(414)

private:
    class MutatorASTConsumer_414 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_414(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Inheriting_Constructor_With_Delegate_414.h"

// ========================================================================================================
#define MUT414_OUTPUT 1

void MutatorFrontendAction_414::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("usingDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        const DeclContext *baseDC = UD->getNominatedNamespace();
        const CXXRecordDecl *baseClass = dyn_cast<CXXRecordDecl>(baseDC);
        if (!baseClass)
            return;

        const Decl *targetDecl = UD->getTargetDecl();
        const CXXConstructorDecl *ctor = dyn_cast<CXXConstructorDecl>(targetDecl);
        if (!ctor)
            return;

        const CXXRecordDecl *derivedClass = UD->getDeclContext()->getRecordDecl();
        if (!derivedClass)
            return;

        SmallString<100> paramStr;
        for (const auto *param : ctor->parameters()) {
            if (!paramStr.empty())
                paramStr += ", ";
            paramStr += param->getType().getAsString();
        }

        std::string newCtor = derivedClass->getNameAsString() + "(" + paramStr.c_str() + ") : " + baseClass->getNameAsString() + "::" + baseClass->getNameAsString() + "(" + paramStr.c_str() + ") {}";
        Rewrite.ReplaceText(UD->getSourceRange(), newCtor);
    }
}

void MutatorFrontendAction_414::MutatorASTConsumer_414::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = usingDecl(hasTargetDecl(cxxConstructorDecl())).bind("usingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}