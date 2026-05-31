//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Invalid_Member_Type_360
 */ 
class MutatorFrontendAction_360 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(360)

private:
    class MutatorASTConsumer_360 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_360(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> varNames;
    };
};

//source file
#include "../include/Mutator_Invalid_Member_Type_360.h"

// ========================================================================================================
#define MUT360_OUTPUT 1

void MutatorFrontendAction_360::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        varNames.push_back(VD->getNameAsString());
        return;
    }
    
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("member")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        clang::TypeSourceInfo *TSI = FD->getTypeSourceInfo();
        if (!TSI)
            return;
        
        clang::SourceRange typeRange = TSI->getTypeLoc().getSourceRange();
        if (typeRange.isInvalid())
            return;
        
        if (varNames.empty())
            return;
        
        std::string varName = varNames[getrandom::getRandomIndex(varNames.size())];
        Rewrite.ReplaceText(typeRange, varName);
    }
}

void MutatorFrontendAction_360::MutatorASTConsumer_360::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher varMatcher = varDecl().bind("varDecl");
    DeclarationMatcher fieldMatcher = fieldDecl().bind("member");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.matchAST(Context);
}