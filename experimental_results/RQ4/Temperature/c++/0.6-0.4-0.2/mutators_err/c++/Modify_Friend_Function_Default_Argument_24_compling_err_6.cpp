//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_friend_function_default_argument_24
 */ 
class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)

private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_friend_function_default_argument_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
          auto *param = FD->getParamDecl(i);
          if (param->hasDefaultArg()) {
              //Perform mutation on the source code text by applying string replacement
              std::string newDefaultArg = "10"; // Example new default argument
              std::string newParamText = param->getType().getAsString() + " = " + newDefaultArg;
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(param->getSourceRange(), newParamText);
              break; // Modify only one default argument
          }
      }
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isFriendDecl(), hasParent(cxxRecordDecl(isTemplateInstantiation()))).bind("FriendFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}