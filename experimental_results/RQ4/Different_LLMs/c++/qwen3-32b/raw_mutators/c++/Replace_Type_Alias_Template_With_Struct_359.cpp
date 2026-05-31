//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Type_Alias_Template_With_Struct_359
 */ 
class MutatorFrontendAction_359 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(359)

private:
    class MutatorASTConsumer_359 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_359(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Type_Alias_Template_With_Struct_359.h"

// ========================================================================================================
#define MUT359_OUTPUT 1

void MutatorFrontendAction_359::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TATD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("alias")) {
        //Filter nodes in header files
        if (!TATD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TATD->getLocation()))
            return;
        auto *TAD = TATD->getTemplatedDecl();
        if (!TAD)
            return;
        auto *TSI = TAD->getTypeSourceInfo();
        if (!TSI)
            return;
        SourceRange typeRange = TSI->getTypeLoc().getSourceRange();
        if (typeRange.isInvalid())
            return;
        std::string aliasName = TAD->getNameAsString();
        std::string structCode = "struct " + aliasName + " { int dummy; };";
        Rewrite.ReplaceText(typeRange, structCode);
    }
}
  
void MutatorFrontendAction_359::MutatorASTConsumer_359::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}