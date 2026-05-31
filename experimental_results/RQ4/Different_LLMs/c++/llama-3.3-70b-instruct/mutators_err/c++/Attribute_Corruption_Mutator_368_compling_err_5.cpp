//header file
#pragma once
#include "Mutator_base.h"

/**
 * Attribute_Corruption_Mutator_368
 */ 
class MutatorFrontendAction_368 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(368)

private:
    class MutatorASTConsumer_368 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_368(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, ASTContext *Context, clang::SourceManager *SourceMgr) 
            : Rewrite(Rewrite), Context(Context), SourceMgr(SourceMgr) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        ASTContext *Context;
        clang::SourceManager *SourceMgr;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Attribute_Corruption_Mutator_368.h"

// ========================================================================================================
#define MUT368_OUTPUT 1

void MutatorFrontendAction_368::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Attr = Result.Nodes.getNodeAs<clang::Attr>(("Attribute"))) {
      //Filter nodes in header files
      if (!Attr || !SourceMgr->isInMainFile(Attr->getLocStart().getLocWithOffset(0)))
        return;
      //Get the source code text of target node
      auto attrText = Lexer::getSourceText(
          CharSourceRange::getTokenRange(Attr->getLocStart(), Attr->getLocEnd()),
          *SourceMgr, Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Replace the attribute name with a corrupted version
      std::string corruptedAttr = attrText;
      corruptedAttr.replace(corruptedAttr.find("nodiscard"), 9, "nodiscard2");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Attr->getLocStart(), Attr->getLocEnd()), corruptedAttr);
    }
}
  
void MutatorFrontendAction_368::MutatorASTConsumer_368::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    AttrMatcher matcher = clang::ast_matchers::attr().bind("Attribute");
    Callback callback(TheRewriter, &Context, &Context.getSourceManager());
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}