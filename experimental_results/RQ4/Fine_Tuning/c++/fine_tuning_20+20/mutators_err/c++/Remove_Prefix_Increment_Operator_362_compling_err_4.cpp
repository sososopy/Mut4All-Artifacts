//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Prefix_Increment_Operator_362
 */ 
class MutatorFrontendAction_362 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(362)

private:
    class MutatorASTConsumer_362 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_362(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/mutator_362.h"

// ========================================================================================================
#define MUT362_OUTPUT 1

void MutatorFrontendAction_362::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto decl =
          stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      if (decl.find("operator++()") == string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      decl = stringutils::removeStrBetween(decl, "operator++()", "{", "}");
      decl = stringutils::removeStrBetween(decl, "operator++()", "(", ")");
      decl = stringutils::removeStrBetween(decl, "operator++()", "(", ")");
      decl = stringutils::removeStrBetween(decl, "operator++()", "(", ")");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), decl);
    }
}
  
void MutatorFrontendAction_362::MutatorASTConsumer_362::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}