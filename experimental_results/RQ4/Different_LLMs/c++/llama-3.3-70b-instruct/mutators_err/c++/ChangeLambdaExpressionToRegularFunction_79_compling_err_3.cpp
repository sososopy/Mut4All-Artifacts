//header file
#pragma once
#include "Mutator_base.h"

/**
 * ChangeLambdaExpressionToRegularFunction_79
 */ 
class MutatorFrontendAction_79 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(79)

private:
    class MutatorASTConsumer_79 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_79(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, ASTContext &Context) : Rewrite(Rewrite), Context(Context) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        ASTContext &Context;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/ChangeLambdaExpressionToRegularFunction_79.h"

// ========================================================================================================
#define MUT79_OUTPUT 1

void MutatorFrontendAction_79::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(LE->getSourceRange()), 
          *Result.Context->getSourceManager(), 
          Context.getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string funcName = "regularFunc";
      std::string returnType = "int"; // assuming int return type, may need to be adjusted based on actual lambda return type
      std::string params = "";
      for (auto param : LE->capture_init_list()) {
        params += param->getCapturedVar()->getNameAsString() + ", ";
      }
      if (!params.empty()) {
        params.pop_back();
        params.pop_back();
      }
      std::string funcText = returnType + " " + funcName + "(" + params + ") {\n";
      funcText += lambdaText.substr(lambdaText.find("{") + 1, lambdaText.find("}") - lambdaText.find("{") - 1) + "\n";
      funcText += "}\n";
      funcText += "auto funcPtr = " + funcName + ";\n";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), funcText);
    }
}

void MutatorFrontendAction_79::MutatorASTConsumer_79::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter, Context);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}