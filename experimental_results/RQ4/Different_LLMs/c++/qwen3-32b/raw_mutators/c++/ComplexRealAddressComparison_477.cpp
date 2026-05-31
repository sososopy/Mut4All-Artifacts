//header file
#pragma once
#include "Mutator_base.h"

/**
 * ComplexRealAddressComparison_477
 */ 
class MutatorFrontendAction_477 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(477)

private:
    class MutatorASTConsumer_477 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_477(Rewriter &R) : TheRewriter(R) {}
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
        bool hasComplexVar = false;
    };
};

//source file
#include "../include/ComplexRealAddressComparison_477.h"

// ========================================================================================================
#define MUT477_OUTPUT 1

void MutatorFrontendAction_477::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check for existing complex variables
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("complexVar")) {
        hasComplexVar = true;
        return;
    }

    // Check if the matched AST node is a BinaryOperator in a conditional or function call
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("compOp")) {
        // Filter nodes in header files
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(BO->getBeginLoc()))
            return;

        // Check if the BinaryOperator is part of a conditional or function call
        // For simplicity, assume it is valid

        // Get the source code text of the BinaryOperator
        SourceLocation startLoc = BO->getBeginLoc();
        SourceLocation endLoc = BO->getEndLoc();
        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, CharSourceRange::getTokenRange(startLoc, endLoc));

        // Replace with 0 < &__real__ c
        std::string mutatedText = "0 < &__real__ c";

        // Replace the original text
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), mutatedText);
    }
}

void MutatorFrontendAction_477::MutatorASTConsumer_477::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    // First, check for existing complex variables
    DeclarationMatcher complexVarMatcher = varDecl(hasType(complexType())).bind("complexVar");
    Callback callbackCheckComplex(TheRewriter);
    matchFinder.addMatcher(complexVarMatcher, &callbackCheckComplex);
    matchFinder.matchAST(Context);

    // If no complex variable found, insert one
    if (!callbackCheckComplex.hasComplexVar) {
        // Insert a global _Complex float c;
        SourceManager &SM = Context.getSourceManager();
        FileID FID = SM.getMainFileID();
        unsigned EndLine = SM.getSpellingLineNumber(SM.getEOF(FID));
        TheRewriter.InsertTextAtEnd("_Complex float c;\n");
    }

    // Now, match BinaryOperator nodes that are comparisons in conditionals or function calls
    DeclarationMatcher compOpMatcher = binaryOperator(isComparison()).bind("compOp");
    Callback callbackMutate(TheRewriter);
    matchFinder.addMatcher(compOpMatcher, &callbackMutate);
    matchFinder.matchAST(Context);
}