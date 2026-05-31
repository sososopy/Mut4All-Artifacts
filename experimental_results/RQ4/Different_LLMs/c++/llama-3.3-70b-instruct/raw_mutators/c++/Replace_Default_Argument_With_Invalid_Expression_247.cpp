//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Argument_With_Invalid_Expression_247
 */ 
class MutatorFrontendAction_247 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(247)

private:
    class MutatorASTConsumer_247 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_247(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Default_Argument_With_Invalid_Expression_247.h"

// ========================================================================================================
#define MUT247_OUTPUT 1

void MutatorFrontendAction_247::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string invalidExpression = "g(k)()";
      size_t pos = declaration.find("=");
      while (pos != std::string::npos) {
        size_t endPos = declaration.find(",", pos);
        if (endPos == std::string::npos) {
          endPos = declaration.find(")", pos);
        }
        if (endPos != std::string::npos) {
          std::string defaultValue = declaration.substr(pos + 1, endPos - pos - 1);
          declaration.replace(pos + 1, defaultValue.length(), invalidExpression);
        }
        pos = declaration.find("=", pos + 1);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_247::MutatorASTConsumer_247::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDefaultArg()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}