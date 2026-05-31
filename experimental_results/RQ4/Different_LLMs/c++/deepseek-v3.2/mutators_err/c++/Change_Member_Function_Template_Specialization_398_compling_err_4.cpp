//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Member_Function_Template_Specialization_398
 */ 
class MutatorFrontendAction_398 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(398)

private:
    class MutatorASTConsumer_398 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_398(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> specializations;
    };
};

//source file
#include "../include/Change_Member_Function_Template_Specialization_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Specialization")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        
        if (!FD->isFunctionTemplateSpecialization() || !FD->getInstantiatedFromMemberFunction())
            return;
        
        const clang::DeclContext* DC = FD->getParent();
        const clang::CXXRecordDecl* RD = clang::dyn_cast<clang::CXXRecordDecl>(DC);
        if (!RD || !RD->isCompleteDefinition())
            return;
        
        SourceLocation ClassEnd = RD->getEndLoc();
        SourceLocation SpecLoc = FD->getBeginLoc();
        if (!Result.SourceManager->isBeforeIn