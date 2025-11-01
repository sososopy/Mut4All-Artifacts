//header file
#pragma once
#include "Mutator_base.h"

/**
 * Define_Undefined_Type_Pointer_337
 */ 
class MutatorFrontendAction_337 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(337)

private:
    class MutatorASTConsumer_337 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_337(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Define_Undefined_Type_Pointer_337.h"

// ========================================================================================================
#define MUT337_OUTPUT 1

void MutatorFrontendAction_337::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("UndefinedStruct")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        if (!UD->isCompleteDefinition()) {
            std::string structName = UD->getNameAsString();
            std::string definition = "struct " + structName + " { int dummy; };";
            Rewrite.ReplaceText(UD->getSourceRange(), definition);
        }
    }
}

void MutatorFrontendAction_337::MutatorASTConsumer_337::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isStruct(), unless(isDefinition())).bind("UndefinedStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}