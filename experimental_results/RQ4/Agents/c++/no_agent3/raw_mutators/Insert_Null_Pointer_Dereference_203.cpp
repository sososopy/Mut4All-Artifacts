```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Null_Pointer_Dereference_203
 */ 
class MutatorFrontendAction_203 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(203)

private:
    class MutatorASTConsumer_203 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_203(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/insert_null_pointer_dereference_203.h"

// ========================================================================================================
#define MUT203_OUTPUT 1

void MutatorFrontendAction_203::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string mutation = "\n    int* ptr = 0; /*mut203*/\n    int val = *ptr;\n";
        functionBody.insert(functionBody.find("{") + 1, mutation);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_203::MutatorASTConsumer_203::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```