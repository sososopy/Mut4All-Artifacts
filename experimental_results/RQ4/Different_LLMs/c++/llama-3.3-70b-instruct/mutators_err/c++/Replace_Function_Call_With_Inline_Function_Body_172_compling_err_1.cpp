```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Call_With_Inline_Function_Body_172
 */ 
class MutatorFrontendAction_172 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(172)

private:
    class MutatorASTConsumer_172 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_172(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Function_Call_With_Inline_Function_Body_172.h"

// ========================================================================================================
#define MUT172_OUTPUT 1

void MutatorFrontendAction_172::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      // Check if the method is a destructor
      if (MT->isDestructor()) {
        // Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        // Check if the destructor has an inline body
        if (MT->hasBody()) {
          // Get the inline body of the destructor
          auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                              MT->getBody()->getSourceRange());
          // Perform mutation on the source code text by applying string replacement
          // Replace the destructor call with its inline body
          auto mutatedCode = declaration + "\n" + body;
          // Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedCode);
        }
      }
    }
}
  
void MutatorFrontendAction_172::MutatorASTConsumer_172::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isDestructor()).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}