//header file
#pragma once
#include "Mutator_base.h"

/**
 * malformed_comment_adjustment_698
 */ 
class MutatorFrontendAction_698 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(698)

private:
    class MutatorASTConsumer_698 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_698(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/malformed_comment_adjustment_698.h"

// ========================================================================================================
#define MUT698_OUTPUT 1

void MutatorFrontendAction_698::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      //Get the source code text of target node
      if (const RawComment *RC = FD->getASTContext().getRawCommentForDeclNoCache(FD)) {
        auto commentRange = RC->getSourceRange();
        auto commentText = Lexer::getSourceText(CharSourceRange::getTokenRange(commentRange), *Result.SourceManager, Result.Context->getLangOpts());
        
        //Perform mutation on the source code text by applying string replacement
        std::string commentStr = commentText.str();
        std::string mutatedComment = "/// Satisfies the [`Add<@doc.self>`]($some::concept::Add) concept.\n";
        commentStr.replace(commentStr.find("///"), 3, mutatedComment);
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(commentRange), commentStr);
      }
    }
}
  
void MutatorFrontendAction_698::MutatorASTConsumer_698::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr<AnnotateAttr>()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}