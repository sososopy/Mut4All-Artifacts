//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_With_Thread_Local_325
 */ 
class MutatorFrontendAction_325 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(325)

private:
    class MutatorASTConsumer_325 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_325(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Static_With_Thread_Local_325.h"

// ========================================================================================================
#define MUT325_OUTPUT 1

void MutatorFrontendAction_325::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;
        //Perform mutation on the source code text by applying string replacement
        SourceLocation loc = VD->getBeginLoc();
        if (loc.isInvalid())
            return;
        //Replace 'static' with 'thread_local'
        Rewrite.ReplaceText(loc, strlen("static"), "thread_local");
    }
}
  
void MutatorFrontendAction_325::MutatorASTConsumer_325::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasLocalStorage(), hasAncestor(functionDecl(isConsteval()))).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}