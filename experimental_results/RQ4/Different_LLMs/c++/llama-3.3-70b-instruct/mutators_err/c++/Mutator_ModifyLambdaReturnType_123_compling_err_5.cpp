//header file
#pragma once
#include "Mutator_base.h"

/**
 * ModifyLambdaReturnType_123
 */ 
class MutatorFrontendAction_123 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(123)

private:
    class MutatorASTConsumer_123 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_123(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/ModifyLambdaReturnType_123.h"

// ========================================================================================================
#define MUT123_OUTPUT 1

void MutatorFrontendAction_123::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isInMainFile(LT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto returnType = LT->getReturnTypeInfoAsWritten()->getType()->getAsString();
      if (returnType == "void") {
        declaration.replace(declaration.find("-> void"), 7, "-> int");
        declaration.insert(declaration.rfind("}"), "\nreturn 0;");
      } else if (returnType == "int") {
        declaration.replace(declaration.find("-> int"), 6, "-> void");
        declaration.erase(declaration.rfind("return 0;"), 9);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_123::MutatorASTConsumer_123::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}