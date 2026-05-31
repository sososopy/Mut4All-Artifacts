//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * modify_friend_function_default_parameter_24
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
#include "../include/modify_friend_function_default_parameter_24.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getFriendObjectKind() == clang::Decl::Friend) {
        std::string functionDecl = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(FD->getSourceRange()), 
                                                        *Result.SourceManager, 
                                                        Result.Context->getLangOpts()));

        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
          if (FD->getParamDecl(i)->hasDefaultArg()) {
            size_t defaultArgPos = functionDecl.find("=");
            if (defaultArgPos != std::string::npos) {
              size_t commaPos = functionDecl.find(",", defaultArgPos);
              size_t closeParenPos = functionDecl.find(")", defaultArgPos);
              size_t endPos = (commaPos != std::string::npos && commaPos < closeParenPos) ? commaPos : closeParenPos;
              functionDecl.erase(defaultArgPos, endPos - defaultArgPos);
            }
          }
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionDecl);
      }
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasFriend(), hasParent(cxxRecordDecl(isTemplateInstantiation()))).bind("FriendFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}