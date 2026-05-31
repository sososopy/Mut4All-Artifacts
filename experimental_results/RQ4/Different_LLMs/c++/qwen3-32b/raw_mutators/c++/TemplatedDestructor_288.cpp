//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplatedDestructor_288
 */ 
class MutatorFrontendAction_288 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(288)

private:
    class MutatorASTConsumer_288 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_288(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/TemplatedDestructor_288.h"

// ========================================================================================================
#define MUT288_OUTPUT 1

void MutatorFrontendAction_288::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Dtor = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Dtor")) {
        if (!Dtor || !Result.Context->getSourceManager().isWrittenInMainFile(Dtor->getLocation()))
            return;
        
        if (!Dtor->isUserProvided())
            return;
            
        std::string original = stringutils::rangetoStr(*Result.SourceManager, Dtor->getSourceRange());
        std::string mutated = "template<typename T> " + original;
        
        Rewrite.ReplaceText(Dtor->getSourceRange(), mutated);
    }
}
  
void MutatorFrontendAction_288::MutatorASTConsumer_288::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDestructorDecl(isUserProvided()).bind("Dtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}