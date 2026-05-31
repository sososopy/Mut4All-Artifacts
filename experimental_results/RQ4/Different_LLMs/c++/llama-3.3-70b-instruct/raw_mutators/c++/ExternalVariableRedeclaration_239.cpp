//header file
#pragma once
#include "Mutator_base.h"

/**
 * ExternalVariableRedeclaration_239
 */ 
class MutatorFrontendAction_239 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(239)

private:
    class MutatorASTConsumer_239 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_239(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_ExternalVariableRedeclaration_239.h"

// ========================================================================================================
#define MUT239_OUTPUT 1

void MutatorFrontendAction_239::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto params = FD->parameters();
      for (auto param : params) {
        // Check if there is an external variable with the same name as the parameter
        auto externalVars = Result.Context->getTranslationUnitDecl()->decls();
        for (auto var : externalVars) {
          if (auto varDecl = dyn_cast<VarDecl>(var)) {
            if (varDecl->getNameAsString() == param->getNameAsString()) {
              // Perform mutation on the source code text by applying string replacement
              auto paramType = param->getType().getAsString();
              auto varType = varDecl->getType().getAsString();
              if (paramType != varType) {
                std::string mutatedParam = "extern " + varType + " " + param->getNameAsString();
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(param->getSourceRange()), mutatedParam);
              }
            }
          }
        }
      }
    }
}

void MutatorFrontendAction_239::MutatorASTConsumer_239::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}