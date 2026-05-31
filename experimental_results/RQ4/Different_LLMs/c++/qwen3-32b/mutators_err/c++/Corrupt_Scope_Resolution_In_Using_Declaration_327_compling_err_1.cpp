//header file
#pragma once
#include "Mutator_base.h"

/**
 * Corrupt_Scope_Resolution_In_Using_Declaration_327
 */ 
class MutatorFrontendAction_327 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(327)

private:
    class MutatorASTConsumer_327 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_327(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Corrupt_Scope_Resolution_In_Using_Declaration_327.h"

// ========================================================================================================
#define MUT327_OUTPUT 1

void MutatorFrontendAction_327::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::UsingDecl>("usingDecl")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        
        if (!MT->getQualifier() || MT->getName().isEmpty())
            return;
        
        SourceLocation nameLoc = MT->getNameInfo().getBeginLoc();
        if (nameLoc.isInvalid())
            return;
        
        Rewrite.RemoveText(nameLoc, MT->getName().getLength());
    }
}
  
void MutatorFrontendAction_327::MutatorASTConsumer_327::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = usingDecl(hasDeclContext(cxxRecordDecl())).bind("usingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}