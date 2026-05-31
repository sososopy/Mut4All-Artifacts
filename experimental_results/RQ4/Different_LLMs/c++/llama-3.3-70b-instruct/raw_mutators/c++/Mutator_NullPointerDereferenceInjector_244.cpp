```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * NullPointerDereferenceInjector_244
 */ 
class MutatorFrontendAction_NullPointerDereferenceInjector_244 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(NullPointerDereferenceInjector_244)

private:
    class MutatorASTConsumer_NullPointerDereferenceInjector_244 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_NullPointerDereferenceInjector_244(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_NullPointerDereferenceInjector_244.h"

// ========================================================================================================
#define MUT_NullPointerDereferenceInjector_244_OUTPUT 1

void MutatorFrontendAction_NullPointerDereferenceInjector_244::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecls")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode = "if (ptr == nullptr) { ptr->memberFunction(); }\n" + declaration;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_NullPointerDereferenceInjector_244::MutatorASTConsumer_NullPointerDereferenceInjector_244::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}