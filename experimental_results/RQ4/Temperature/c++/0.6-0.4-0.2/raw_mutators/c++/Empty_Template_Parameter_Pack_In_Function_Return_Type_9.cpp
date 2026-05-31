//header file
#pragma once
#include "Mutator_base.h"

/**
 * empty_template_parameter_pack_in_function_return_type_9
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)

private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/empty_template_parameter_pack_in_function_return_type_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        const auto *TemplateArgs = FD->getTemplateSpecializationArgs();
        if (TemplateArgs && TemplateArgs->size() == 0) {
            std::string funcCall = FD->getQualifiedNameAsString() + "<>::func<0>();";
            Rewrite.InsertTextAfterToken(FD->getEndLoc(), "\n" + funcCall + "\n");
        }
    }
}

void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(isTemplateInstantiation()).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}