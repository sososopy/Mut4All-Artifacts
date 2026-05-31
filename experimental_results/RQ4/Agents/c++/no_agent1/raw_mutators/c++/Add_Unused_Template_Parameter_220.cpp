//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Unused_Template_Parameter_220
 */ 

class MutatorFrontendAction_220 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(220)
private:
    class MutatorASTConsumer_220 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_220(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionDecl *> processedFunctions;
    };
};

//source file
#include "../include/add_unused_template_parameter_220.h"

// ========================================================================================================
#define MUT220_OUTPUT 1

void MutatorFrontendAction_220::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        
        if (processedFunctions.find(FD) != processedFunctions.end())
            return;

        processedFunctions.insert(FD);

        if (FD->isTemplateInstantiation() || FD->getTemplatedKind() != clang::FunctionDecl::TK_NonTemplate)
            return;

        SourceLocation loc = FD->getLocation();
        std::string templateParam = "\n/*mut220*/template<typename T = int> ";
        
        if (FD->isThisDeclarationADefinition()) {
            Rewrite.InsertTextBefore(loc, templateParam);
        }
    }
}

void MutatorFrontendAction_220::MutatorASTConsumer_220::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}