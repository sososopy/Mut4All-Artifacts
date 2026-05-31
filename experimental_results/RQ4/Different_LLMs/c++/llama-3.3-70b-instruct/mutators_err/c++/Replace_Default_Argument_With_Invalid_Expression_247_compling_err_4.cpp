//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

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
        Callback(Rewriter &Rewrite, ASTContext &Context, clang::SourceManager &SrcManager) : Rewrite(Rewrite), Context(Context), SrcManager(SrcManager) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        ASTContext &Context;
        clang::SourceManager &SrcManager;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Default_Argument_With_Invalid_Expression_247.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Debug.h"

// ========================================================================================================
#define MUT247_OUTPUT 1

void MutatorFrontendAction_247::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD ||!SrcManager.isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(FD->getSourceRange()), 
          *SrcManager, 
          clang::LangOptions());
      std::string declStr = declaration.str();
      //Perform mutation on the source code text by applying string replacement
      std::string invalidExpression = "g(k)()";
      size_t pos = declStr.find("=");
      while (pos!= std::string::npos) {
        size_t endPos = declStr.find(",", pos);
        if (endPos == std::string::npos) {
          endPos = declStr.find(")", pos);
        }
        if (endPos!= std::string::npos) {
          std::string defaultValue = declStr.substr(pos + 1, endPos - pos - 1);
          declStr.replace(pos + 1, defaultValue.length(), invalidExpression);
        }
        pos = declStr.find("=", pos + 1);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(FD->getSourceRange()), declStr);
    }
}

void MutatorFrontendAction_247::MutatorASTConsumer_247::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasDefaultArg(0))).bind("Function");
    Callback callback(TheRewriter, Context, Context.getSourceManager());
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}