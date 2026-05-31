//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Arbitrary_Identifier_Before_Constructor_354
 */ 
class MutatorFrontendAction_354 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(354)

private:
    class MutatorASTConsumer_354 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_354(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Insert_Arbitrary_Identifier_Before_Constructor_354.h"

// ========================================================================================================
#define MUT354_OUTPUT 1

void MutatorFrontendAction_354::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructors")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (!MT->hasBody())
            return;
        std::string identifier = getrandom::getRandomIndex(10);
        SourceLocation loc = MT->getLocation();
        Rewrite.InsertText(loc, identifier + " ", false);
    }
}
  
void MutatorFrontendAction_354::MutatorASTConsumer_354::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(hasBody()).bind("Constructors");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}