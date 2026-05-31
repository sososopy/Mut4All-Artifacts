//header file
#pragma once
#include "Mutator_base.h"

/**
 * ArraySizeMutator_350
 */ 
class MutatorFrontendAction_350 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(350)

private:
    class MutatorASTConsumer_350 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_350(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/ArraySizeMutator_350.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT350_OUTPUT 1

void MutatorFrontendAction_350::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AD = Result.Nodes.getNodeAs<clang::ArraySubscriptExpr>("ArrayDecl")) {
      //Filter nodes in header files
      if (!AD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AD->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(AD->getBeginLoc(), AD->getEndLoc()),
          *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find('[');
      size_t endPos = declaration.find(']');
      if (pos != std::string::npos && endPos != std::string::npos) {
        std::string sizeStr = declaration.substr(pos + 1, endPos - pos - 1);
        int size = atoi(sizeStr.c_str());
        std::string mutatedSizeStr = std::to_string(-size);
        declaration.replace(pos + 1, endPos - pos - 1, mutatedSizeStr);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(AD->getBeginLoc(), AD->getEndLoc()), declaration);
    }
}

void MutatorFrontendAction_350::MutatorASTConsumer_350::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = arraySubscriptExpr().bind("ArrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}