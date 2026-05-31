//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_With_Semicolon_380
 */ 
class MutatorFrontendAction_Replace_Member_With_Semicolon_380 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Member_With_Semicolon_380)

private:
    class MutatorASTConsumer_Replace_Member_With_Semicolon_380 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Member_With_Semicolon_380(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Member_With_Semicolon_380.h"

// ========================================================================================================
#define MUTReplace_Member_With_Semicolon_380_OUTPUT 1

void MutatorFrontendAction_Replace_Member_With_Semicolon_380::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("member")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Perform mutation on the source code text by replacing the field with a semicolon
      Rewrite.ReplaceText(FD->getSourceRange(), ";");
    }
}
  
void MutatorFrontendAction_Replace_Member_With_Semicolon_380::MutatorASTConsumer_Replace_Member_With_Semicolon_380::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(isInCXXRecordDecl()).bind("member");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}