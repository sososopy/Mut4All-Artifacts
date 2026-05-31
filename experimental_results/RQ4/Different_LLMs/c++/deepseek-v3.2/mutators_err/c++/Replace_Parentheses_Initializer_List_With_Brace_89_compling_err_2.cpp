//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Parentheses_Initializer_List_With_Brace_89
 */ 
class MutatorFrontendAction_89 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(89)

private:
    class MutatorASTConsumer_89 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_89(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Parentheses_Initializer_List_With_Brace_89.h"

// ========================================================================================================
#define MUT89_OUTPUT 1

void MutatorFrontendAction_89::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParenListExpr>("ParenListInit")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      SourceRange range = MT->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
      //Perform mutation on the source code text by applying string replacement
      if (originalText.size() >= 2 && originalText.front() == '(' && originalText.back() == ')') {
        std::string mutatedText = "{" + originalText.substr(1, originalText.size() - 2) + "}";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(range, mutatedText);
      }
    }
}
  
void MutatorFrontendAction_89::MutatorASTConsumer_89::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = parenListExpr(hasAncestor(cxxRecordDecl())).bind("ParenListInit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}