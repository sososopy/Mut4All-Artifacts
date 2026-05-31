//header file
#pragma once
#include "Mutator_base.h"

/**
 * Append_Whitespace_At_EOF_192
 */ 
class MutatorFrontendAction_192 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(192)

private:
    class MutatorASTConsumer_192 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_192(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT192_OUTPUT 1

void MutatorFrontendAction_192::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      //We need to check if this is the last token in the file
      SourceManager &SM = *Result.SourceManager;
      SourceLocation endLoc = FD->getEndLoc();
      
      // Check if this function ends the file
      std::pair<FileID, unsigned> endLocInfo = SM.getDecomposedLoc(endLoc);
      std::pair<FileID, unsigned> fileEndInfo = SM.getDecomposedLoc(SM.getLocForEndOfFile(endLocInfo.first));
      
      // If this function ends at the end of the file
      if (endLocInfo.second == fileEndInfo.second - 1) {
        //Check if there's already whitespace at the end
        const char *buffer = SM.getCharacterData(endLoc);
        if (buffer && (*buffer == ' ' || *buffer == '\n' || *buffer == '\t' || *buffer == '\r')) {
          return; // Already has whitespace
        }
        
        //Perform mutation on the source code text by applying string replacement
        //Add either a space or newline randomly
        int choice = getrandom::getRandomIndex(1);
        string whitespace = (choice == 0) ? " " : "\n";
        
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(endLoc, whitespace);
      }
    }
}
  
void MutatorFrontendAction_192::MutatorASTConsumer_192::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}