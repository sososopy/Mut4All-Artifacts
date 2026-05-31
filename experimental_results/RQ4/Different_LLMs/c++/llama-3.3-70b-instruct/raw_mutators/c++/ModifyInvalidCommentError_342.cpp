//header file
#pragma once
#include "Mutator_base.h"

/**
 * ModifyInvalidCommentError_342
 */ 
class MutatorFrontendAction_342 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(342)

private:
    class MutatorASTConsumer_342 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_342(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ModifyInvalidCommentError_342.h"

// ========================================================================================================
#define MUT342_OUTPUT 1

void MutatorFrontendAction_342::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CM = Result.Nodes.getNodeAs<clang::Comment>(("Comment"))) {
      //Filter nodes in header files
      if (!CM || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CM->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto commentText = stringutils::rangetoStr(*(Result.SourceManager), CM->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace '//' with '/*' and '/' or introduce a '@' symbol to mimic a directive
      std::string mutatedComment = commentText;
      mutatedComment.replace(mutatedComment.find("//"), 2, "/*");
      mutatedComment.insert(mutatedComment.find_first_not_of(" \t"), "/");
      mutatedComment.insert(mutatedComment.find_first_not_of(" \t"), "@");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CM->getSourceRange()), mutatedComment);
    }
}
  
void MutatorFrontendAction_342::MutatorASTConsumer_342::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = comment().bind("Comment");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}