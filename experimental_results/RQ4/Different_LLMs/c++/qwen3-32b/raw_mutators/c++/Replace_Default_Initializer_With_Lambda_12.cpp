//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Initializer_With_Lambda_12
 */ 
class MutatorFrontendAction_12 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(12)

private:
    class MutatorASTConsumer_12 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_12(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Default_Initializer_With_Lambda_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        // Check if in main file
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Check if parent is a class template
        const auto *record = FD->getDeclContext()->getRecordDecl();
        if (!record || !record->getDescribedClassTemplate())
            return;

        // Get the initializer
        const Expr *initializer = FD->getInClassInitializer();
        if (!initializer)
            return;

        // Get the source range of the initializer
        SourceRange initRange = initializer->getSourceRange();
        if (initRange.isInvalid())
            return;

        // Get the original initializer text
        std::string initText = stringutils::rangetoStr(*Result.SourceManager, initRange);
        if (initText.empty())
            return;

        // Create the lambda string
        std::string lambdaText = "[[]{ return " + initText + "; }();";

        // Replace the initializer with the lambda
        Rewrite.ReplaceText(initRange, lambdaText);
    }
}
  
void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer(expr())).bind("field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}