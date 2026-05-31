//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Underlying_Incomplete_Type_180
 */ 
class MutatorFrontendAction_180 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(180)

private:
    class MutatorASTConsumer_180 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_180(Rewriter &R) : TheRewriter(R) {}
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
        static int counter;
    };
};

//source file
#include "../include/Enum_Underlying_Incomplete_Type_180.h"

// ========================================================================================================
#define MUT180_OUTPUT 1

int MutatorFrontendAction_180::Callback::counter = 0;

void MutatorFrontendAction_180::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;
        if (!ED->getIntegerType().getTypePtr())
            return;
        std::string newTypeName = "IncompleteType_" + std::to_string(counter++);
        Rewrite.InsertTextBefore(ED->getLocation(), "struct " + newTypeName + ";\n");
        SourceRange typeRange = ED->getIntegerTypeSourceInfo()->getTypeLoc().getSourceRange();
        Rewrite.ReplaceText(typeRange, newTypeName);
    }
}
  
void MutatorFrontendAction_180::MutatorASTConsumer_180::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(hasType(type())).bind("enumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}