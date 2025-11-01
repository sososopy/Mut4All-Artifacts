//header file
#pragma once
#include "Mutator_base.h"

/**
 * Auto_Type_In_Struct_Member_101
 */ 
class MutatorFrontendAction_101 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(101)

private:
    class MutatorASTConsumer_101 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_101(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Auto_Type_In_Struct_Member_101.h"

// ========================================================================================================
#define MUT101_OUTPUT 1

void MutatorFrontendAction_101::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("fieldDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasInClassInitializer()) {
            QualType fieldType = FD->getType();
            if (!fieldType->isDependentType() && !fieldType->isAutoType()) {
                std::string initExpr = stringutils::rangetoStr(*(Result.SourceManager), FD->getInClassInitializer()->getSourceRange());
                std::string newDecl = "auto " + FD->getNameAsString() + " = " + initExpr + ";";
                Rewrite.ReplaceText(FD->getSourceRange(), newDecl);
            }
        }
    }
}

void MutatorFrontendAction_101::MutatorASTConsumer_101::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer()).bind("fieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}