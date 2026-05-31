```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Initialization_Modifier_184
 */ 
class MutatorFrontendAction_184 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(184)

private:
    class MutatorASTConsumer_184 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_184(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Recursive_Initialization_Modifier_184.h"

// ========================================================================================================
#define MUT184_OUTPUT 1

void MutatorFrontendAction_184::Callback::run(const MatchFinder::MatchResult &Result) {
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
      if (FD->getInClassInitStyle() == FieldDecl::ICIS_CInit) {
        auto initializer = FD->getInClassInitializer();
        if (initializer && initializer->isCallExpr()) {
          auto callExpr = initializer->toCallExpr();
          auto callee = callExpr->getDirectCallee();
          if (callee && callee->isFunction()) {
            auto funcDecl = callee->getFunctionDecl();
            auto funcName = funcDecl->getNameAsString();
            // Replace the initializer with a call to the same function, creating a new instance of the same struct
            declaration.replace(declaration.find("=") + 1, declaration.find(";"), funcName + "(" + FD->getParent()->getNameAsString() + "())");
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_184::MutatorASTConsumer_184::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasInitializer(callExpr())).bind("FieldDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}