//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Exception_Specification_Mutation_57
 */ 
class MutatorFrontendAction_57 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(57)

private:
    class MutatorASTConsumer_57 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_57(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_function_exception_specification_mutation_57.h"

// ========================================================================================================
#define MUT57_OUTPUT 1

void MutatorFrontendAction_57::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || !FD->getDescribedFunctionTemplate())
            return;

        if (FD->hasAttr<clang::NoThrowAttr>()) {
            auto startLoc = FD->getExceptionSpecSourceRange().getBegin();
            auto endLoc = FD->getExceptionSpecSourceRange().getEnd();
            if (startLoc.isValid() && endLoc.isValid()) {
                std::string newSpec = "noexcept(T())";
                Rewrite.ReplaceText(clang::SourceRange(startLoc, endLoc), newSpec);
            }
        }
    }
}

void MutatorFrontendAction_57::MutatorASTConsumer_57::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasAttr<clang::NoThrowAttr>()).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}