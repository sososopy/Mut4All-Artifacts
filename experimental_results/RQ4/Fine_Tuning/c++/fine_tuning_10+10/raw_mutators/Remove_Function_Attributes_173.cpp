```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Function_Attributes_173
 */ 
class MutatorFrontendAction_173 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(173)

private:
    class MutatorASTConsumer_173 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_173(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Remove_Function_Attributes_173.h"

// ========================================================================================================
#define MUT173_OUTPUT 1

void MutatorFrontendAction_173::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithAttributes")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      // Check if the function has attributes
      if (FD->hasAttrs()) {
        //Get the source code text of target node
        auto startLoc = FD->getBeginLoc();
        auto endLoc = FD->getAttr(FD->getAttrs().back())->getRange().getEnd();
        //Perform mutation on the source code text by applying string replacement
        Rewrite.RemoveText(SourceRange(startLoc, endLoc));
      }
    }
}
  
void MutatorFrontendAction_173::MutatorASTConsumer_173::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr(attr::AttrKind::NoReturn)).bind("FunctionWithAttributes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```