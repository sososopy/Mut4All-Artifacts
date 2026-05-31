//header file
#pragma once
#include "Mutator_base.h"

/**
 * misplaced_preprocessor_directive_parameters_32
 */ 
class MutatorFrontendAction_32 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(32)

private:
    class MutatorASTConsumer_32 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_32(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misplaced_preprocessor_directive_parameters_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PPD = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("PreprocessorDirective")) {
        if (!PPD || !Result.SourceManager->isWrittenInMainFile(PPD->getLocation()))
            return;

        auto directiveText = stringutils::rangetoStr(*(Result.SourceManager), PPD->getSourceRange());
        
        // Perform mutation: Insert an unknown parameter before known ones
        std::string mutatedDirective = "#embed unknown_param(2) " + directiveText;
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(PPD->getSourceRange()), mutatedDirective);
    }
}

void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = preprocessingDirective().bind("PreprocessorDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}