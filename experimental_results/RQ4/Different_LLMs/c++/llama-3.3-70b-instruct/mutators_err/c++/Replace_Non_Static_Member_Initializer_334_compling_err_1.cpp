```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Non_Static_Member_Initializer_334
 */ 
class MutatorFrontendAction_334 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(334)

private:
    class MutatorASTConsumer_334 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_334(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Non_Static_Member_Initializer_334.h"

// ========================================================================================================
#define MUT334_OUTPUT 1

void MutatorFrontendAction_334::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecls")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto initializerStart = declaration.find('=');
      if (initializerStart != string::npos) {
        auto typeStart = declaration.find_last_of(' ', initializerStart);
        if (typeStart != string::npos) {
          declaration.replace(typeStart + 1, initializerStart - typeStart - 1, "auto");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_334::MutatorASTConsumer_334::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(unless(isStatic())).bind("FieldDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}