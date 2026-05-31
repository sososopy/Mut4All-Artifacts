//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Comment.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * misleading_block_comment_650
 */ 
class MutatorFrontendAction_650 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(650)

private:
    class MutatorASTConsumer_650 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_650(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misleading_block_comment_650.h"

// ========================================================================================================
#define MUT650_OUTPUT 1

void MutatorFrontendAction_650::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BC = Result.Nodes.getNodeAs<comments::Comment>("BlockComment")) {
      //Filter nodes in header files
      if (!BC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BC->getLocation()))
        return;
      //Get the source code text of target node
      auto commentText = Lexer::getSourceText(CharSourceRange::getTokenRange(BC->getSourceRange()), 
                                              *Result.SourceManager, 
                                              Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      if (commentText.find("/*") == 0 && commentText.rfind("*/") == commentText.size() - 2) {
        std::string mutatedComment = (llvm::Twine("/*\\ ") + commentText.substr(2, commentText.size() - 4) + " */").str();
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BC->getSourceRange()), mutatedComment);
      }
    }
}
  
void MutatorFrontendAction_650::MutatorASTConsumer_650::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = comments().bind("BlockComment");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}