//header file
#pragma once
#include "Mutator_base.h"

/**
 * External_Redefinition_In_Function_Scope_44
 */ 
class MutatorFrontendAction_44 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(44)

private:
    class MutatorASTConsumer_44 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_44(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/External_Redefinition_In_Function_Scope_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        auto params = FD->parameters();
        if (!params.empty()) {
          // Choose the first parameter for mutation
          auto param = params[0];
          auto paramName = param->getNameAsString();
          auto paramType = param->getType().getAsString();
          std::string externalDecl = "extern " + paramType + " " + paramName + ";\n";
          //Perform mutation on the source code text by applying string replacement
          Rewrite.InsertTextAfterToken(body->getBeginLoc(), "/*mut44*/" + externalDecl);
        }
      }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody(stmt()), hasParameterCount(1)).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}