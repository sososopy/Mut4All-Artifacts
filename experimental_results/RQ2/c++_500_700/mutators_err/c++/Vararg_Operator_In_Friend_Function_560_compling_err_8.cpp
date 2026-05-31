//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * vararg_operator_in_friend_function_560
 */ 
class MutatorFrontendAction_560 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(560)

private:
    class MutatorASTConsumer_560 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_560(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/vararg_operator_in_friend_function_560.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT560_OUTPUT 1

void MutatorFrontendAction_560::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getFriendObjectKind() != clang::Decl::FOK_None) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t paramStart = declaration.find('(');
        size_t paramEnd = declaration.find(')');
        if (paramStart != std::string::npos && paramEnd != std::string::npos) {
            declaration.replace(paramStart + 1, paramEnd - paramStart - 1, "...");
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_560::MutatorASTConsumer_560::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isFriend()).bind("FriendFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}