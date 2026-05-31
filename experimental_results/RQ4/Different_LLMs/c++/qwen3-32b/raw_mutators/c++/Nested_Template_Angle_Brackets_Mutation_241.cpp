//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Template_Angle_Brackets_Mutation_241
 */ 
class MutatorFrontendAction_241 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(241)

private:
    class MutatorASTConsumer_241 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_241(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Nested_Template_Angle_Brackets_Mutation_241.h"

// ========================================================================================================
#define MUT241_OUTPUT 1

void MutatorFrontendAction_241::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VariableDecl>("varDecl")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        // Get the type's TypeLoc
        TypeSourceInfo *TSI = VD->getTypeSourceInfo();
        if (!TSI)
            return;

        TypeLoc typeLoc = TSI->getTypeLoc();
        if (!typeLoc.getType()->isTemplateSpecializationType())
            return;

        SourceLocation endLoc = typeLoc.getEndLoc();
        if (endLoc.isInvalid())
            return;

        // Replace the '>' with '>>'
        Rewrite.ReplaceText(endLoc, 1, ">>");
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        // Get the return type's TypeLoc
        TypeSourceInfo *TSI = FD->getReturnTypeSourceInfo();
        if (!TSI)
            return;

        TypeLoc typeLoc = TSI->getTypeLoc();
        if (!typeLoc.getType()->isTemplateSpecializationType())
            return;

        SourceLocation endLoc = typeLoc.getEndLoc();
        if (endLoc.isInvalid())
            return;

        // Replace the '>' with '>>'
        Rewrite.ReplaceText(endLoc, 1, ">>");
    }
}

void MutatorFrontendAction_241::MutatorASTConsumer_241::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = 
        variableDecl(hasType(typeLoc(templateSpecializationType()))).bind("varDecl") ||
        functionDecl(hasReturnType(typeLoc(templateSpecializationType()))).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}