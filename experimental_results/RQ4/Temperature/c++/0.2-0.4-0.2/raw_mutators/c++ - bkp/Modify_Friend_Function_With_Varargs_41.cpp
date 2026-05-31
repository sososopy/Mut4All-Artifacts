//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_friend_function_with_varargs_41
 */ 
class MutatorFrontendAction_41 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(41)

private:
    class MutatorASTConsumer_41 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_41(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FriendDecl *TargetFriendDecl = nullptr;
    };
};

//source file
#include "../include/modify_friend_function_with_varargs_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FriendDecl>("FriendDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (const FunctionDecl *FunDecl = FD->getFriendDecl()->getAsFunction()) {
        if (FunDecl->isVariadic()) {
          //Get the source code text of target node
          auto declText = stringutils::rangetoStr(*(Result.SourceManager),
                                                  FunDecl->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          std::string modifiedDecl = declText;
          size_t pos = modifiedDecl.find("...");
          if (pos != std::string::npos) {
            modifiedDecl.insert(pos, "int, ");
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FunDecl->getSourceRange()), modifiedDecl);

          // Use the modified friend function in an expression
          SourceLocation insertLoc = FD->getEndLoc().getLocWithOffset(1);
          std::string usageCode = "/*mut41*/const char *result = (ExampleStruct(), 42, \"test\");\n";
          Rewrite.InsertTextAfter(insertLoc, usageCode);
        }
      }
    }
}
  
void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = friendDecl(has(friendDecl(hasType(functionType(isVariadic()))))).bind("FriendDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}