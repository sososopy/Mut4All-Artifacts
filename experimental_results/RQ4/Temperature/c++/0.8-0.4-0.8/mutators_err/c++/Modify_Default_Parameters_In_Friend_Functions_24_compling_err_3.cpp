//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * modify_default_parameters_in_friend_functions_24
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
#include "../include/modify_default_parameters_in_friend_functions_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (FD->getFriendObjectKind() != clang::Decl::FOK_None) {
        bool hasDefaultArgs = false;
        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
          if (FD->getParamDecl(i)->hasDefaultArg()) {
            hasDefaultArgs = true;
            break;
          }
        }
        
        if (hasDefaultArgs) {
          //Get the source code text of target node
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                     FD->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            if (FD->getParamDecl(i)->hasDefaultArg()) {
              auto defaultArgRange = FD->getParamDecl(i)->getDefaultArgRange();
              declaration.erase(defaultArgRange.getBegin().getRawEncoding(),
                                defaultArgRange.getEnd().getRawEncoding() -
                                defaultArgRange.getBegin().getRawEncoding());
            }
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    using namespace clang::ast_matchers;
    DeclarationMatcher matcher = functionDecl(isFriend()).bind("FriendFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}