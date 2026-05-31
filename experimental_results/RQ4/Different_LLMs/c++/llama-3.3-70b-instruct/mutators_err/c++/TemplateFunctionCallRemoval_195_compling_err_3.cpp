//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * TemplateFunctionCallRemoval_195
 */ 
class MutatorFrontendAction_195 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(195)

private:
    class MutatorASTConsumer_195 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_195(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateFunctionCallRemoval_195.h"

// ========================================================================================================
#define MUT195_OUTPUT 1

void MutatorFrontendAction_195::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find(")");
      if (pos != string::npos) {
        declaration.erase(pos);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_195::MutatorASTConsumer_195::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::callExpr(clang::ast_matchers::hasDescendant(clang::ast_matchers::templateArgumentList())).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}