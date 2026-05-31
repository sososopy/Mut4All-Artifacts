//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceInitializerListBegin_10
 */ 
class MutatorFrontendAction_10 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)

private:
    class MutatorASTConsumer_10 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_ReplaceInitializerListBegin_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (declaration.find("begin()") != std::string::npos) {
        declaration.replace(declaration.find("begin()"), 6, "end()");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    CallExprMatcher matcher = callExpr(hasArgument(0, hasDescendant(recordDecl(hasName("std::initializer_list"))))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}