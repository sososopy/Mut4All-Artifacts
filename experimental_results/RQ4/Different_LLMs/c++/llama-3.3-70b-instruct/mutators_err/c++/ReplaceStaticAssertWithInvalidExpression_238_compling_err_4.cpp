//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceStaticAssertWithInvalidExpression_238
 */ 
class MutatorFrontendAction_238 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_238>(TheRewriter);
    }

private:
    class MutatorASTConsumer_238 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_238(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, SourceManager &SourceManager) : Rewrite(Rewrite), SourceManager(SourceManager) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        SourceManager &SourceManager;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/ReplaceStaticAssertWithInvalidExpression_238.h"
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/Tooling.h"

// ========================================================================================================
#define MUT238_OUTPUT 1

void MutatorFrontendAction_238::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = Lexer::getSourceText(
          CharSourceRange::getTokenRange(SA->getSourceRange()), 
          SourceManager, 
          LangOptions());
      //Perform mutation on the source code text by applying string replacement
      // Replace the condition with an invalid expression
      size_t pos = declaration.find("(");
      size_t endPos = declaration.find(")");
      std::string condition = declaration.substr(pos + 1, endPos - pos - 1);
      std::string invalidExpression = "sizeof(m)";
      declaration.replace(pos + 1, endPos - pos - 1, invalidExpression);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(SA->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_238::MutatorASTConsumer_238::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter, Context.getSourceManager());
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}