//header file
#pragma once
#include "Mutator_base.h"

/**
 * unusual_pointer_declaration_611
 */ 
class MutatorFrontendAction_611 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(611)

private:
    class MutatorASTConsumer_611 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_611(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/unusual_pointer_declaration_611.h"

// ========================================================================================================
#define MUT611_OUTPUT 1

void MutatorFrontendAction_611::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("pointerDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        
        if (VD->getType()->isPointerType()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
            std::string mutatedDeclaration = declaration + ", **ptr2, ***ptr3;";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), mutatedDeclaration);
        }
    }
}
  
void MutatorFrontendAction_611::MutatorASTConsumer_611::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(pointerType())).bind("pointerDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}