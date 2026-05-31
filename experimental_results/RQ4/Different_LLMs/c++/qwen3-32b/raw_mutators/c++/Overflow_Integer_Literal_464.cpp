//header file
#pragma once
#include "Mutator_base.h"

/**
 * Overflow_Integer_Literal_464
 */ 
class MutatorFrontendAction_464 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(464)

private:
    class MutatorASTConsumer_464 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_464(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Overflow_Integer_Literal_464.h"

// ========================================================================================================
#define MUT464_OUTPUT 1

void MutatorFrontendAction_464::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *intLit = Result.Nodes.getNodeAs<clang::IntegerLiteral>("intLit")) {
        //Filter nodes in header files
        if (!intLit || !Result.Context->getSourceManager().isWrittenInMainFile(
                       intLit->getLocation()))
            return;
        //Perform mutation on the source code text by applying string replacement
        int choice = getrandom::getRandomIndex(2);
        std::string replacement;
        if (choice == 0) {
            replacement = "0x8000000000000000LL";
        } else {
            replacement = "-0x8000000000000000LL-1";
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(intLit->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_464::MutatorASTConsumer_464::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = integerLiteral().bind("intLit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}