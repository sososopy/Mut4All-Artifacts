//header file
#pragma once
#include "Mutator_base.h"
#include <vector>
#include <string>

/**
 * Replace_Delete_Param_With_Class_385
 */ 
class MutatorFrontendAction_385 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(385)

private:
    class MutatorASTConsumer_385 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_385(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableClasses;
    };
};

//source file
#include "../include/Replace_Delete_Param_With_Class_385.h"

// ========================================================================================================
#define MUT385_OUTPUT 1

void MutatorFrontendAction_385::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        availableClasses.push_back(CD->getNameAsString());
    } else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("opDelete")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;

        if (MD->parameters().empty())
            return;

        ParmVarDecl *firstParam = *MD->parameters().begin();
        TypeLoc typeLoc = firstParam->getTypeSourceInfo()->getTypeLoc();
        if (typeLoc.isNull())
            return;

        SourceRange typeRange = typeLoc.getSourceRange();
        if (typeRange.isInvalid())
            return;

        if (availableClasses.empty())
            return;

        std::string className = availableClasses[getrandom::getRandomIndex(availableClasses.size())];

        Rewrite.ReplaceText(typeRange, className);
    }
}

void MutatorFrontendAction_385::MutatorASTConsumer_385::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("classDecl");
    DeclarationMatcher opDeleteMatcher = cxxMethodDecl(
        hasName("operator delete"),
        isStatic(),
        hasParameter(0, hasType(not(isClass())))
    ).bind("opDelete");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(opDeleteMatcher, &callback);
    matchFinder.matchAST(Context);
}