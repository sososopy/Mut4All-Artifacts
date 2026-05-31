//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Annotation_Token_281
 */ 
class MutatorFrontendAction_281 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_281>(TheRewriter);
    }

private:
    class MutatorASTConsumer_281 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_281(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Insert_Annotation_Token_281.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT281_OUTPUT 1

void MutatorFrontendAction_281::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::AnnotateAttr>("Annotations")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getLocStart().getLocWithOffset(0)))
        return;
      //Get the source code text of target node
      auto annotation = Lexer::getSourceText(
          CharSourceRange::getCharRange(MT->getLocStart(), MT->getLocEnd()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string newAnnotation = " [[deprecated]]";
      Rewrite.InsertTextAfterToken(MT->getLocEnd().getLocWithOffset(0), newAnnotation);
    }
}
  
void MutatorFrontendAction_281::MutatorASTConsumer_281::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    AttrMatcher matcher = clang::ast_matchers::attr().bind("Annotations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}