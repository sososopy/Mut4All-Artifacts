//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Extern_Declaration_With_Parameter_Shadowing_239
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
#include "../include/Duplicate_Extern_Declaration_With_Parameter_Shadowing_239.h"

// ========================================================================================================
#define MUT239_OUTPUT 1

void MutatorFrontendAction_239::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      //Get the source code text of target node
      auto params = FD->parameters();
      if (params.empty())
        return;
      //Select a random parameter
      size_t paramIndex = getrandom::getRandomIndex(params.size() - 1);
      auto param = params[paramIndex];
      if (!param->getIdentifier())
        return;
      string paramName = param->getNameAsString();
      //Determine type for extern declaration
      string externType;
      auto paramType = param->getType();
      if (paramType->isFundamentalType() || paramType->isPointerType()) {
        externType = paramType.getAsString();
      } else {
        externType = "int"; // placeholder for complex types
      }
      //Construct extern declaration
      string externDecl = "extern " + externType + " " + paramName + ";";
      //Insert at start of function body
      auto body = FD->getBody();
      if (!body)
        return;
      SourceLocation insertLoc = body->getBeginLoc().getLocWithOffset(1);
      //Perform mutation on the source code text by applying string replacement
      string insertion = "\n/*mut239*/" + externDecl + "\n";
      //Replace the original AST node with the mutated one
      Rewrite.InsertText(insertLoc, insertion);
    }
}
  
void MutatorFrontendAction_239::MutatorASTConsumer_239::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}