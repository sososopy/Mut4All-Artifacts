//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Variadic_Friend_Operator_154
 */ 
class MutatorFrontendAction_154 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(154)

private:
    class MutatorASTConsumer_154 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_154(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FriendDecl*> friendDecls;
    };
};

//source file
#include "../include/Mutator_154.h"

// ========================================================================================================
#define MUT154_OUTPUT 1

void MutatorFrontendAction_154::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FriendDecl>("friendDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record candidate friend declarations
      friendDecls.push_back(FD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FriendDecl>("targetFriend")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto friendDeclText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Check if it's an operator friend declaration
      auto friendDeclaredFunction = FD->getFriendDecl();
      if (!friendDeclaredFunction) return;
      if (auto *FDDecl = dyn_cast<FunctionDecl>(friendDeclaredFunction)) {
        if (!FDDecl->isOverloadedOperator()) return;
        //Check for comma operator specifically
        if (FDDecl->getOverloadedOperator() != clang::OO_Comma) return;
        //Determine if variadic or non-variadic
        bool isVariadic = FDDecl->isVariadic();
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedDecl;
        if (isVariadic) {
          //Replace variadic with non-variadic: change ... to void*
          size_t ellipsisPos = friendDeclText.find("...");
          if (ellipsisPos != std::string::npos) {
            mutatedDecl = friendDeclText.substr(0, ellipsisPos) + "void*" + friendDeclText.substr(ellipsisPos + 3);
          } else {
            //If no explicit ellipsis found, try to reconstruct
            mutatedDecl = friendDeclText;
            size_t paramEnd = mutatedDecl.find_last_of(')');
            if (paramEnd != std::string::npos) {
              mutatedDecl.insert(paramEnd, ", void*");
            }
          }
        } else {
          //Replace non-variadic with variadic: add ... parameter
          size_t paramEnd = friendDeclText.find_last_of(')');
          if (paramEnd != std::string::npos) {
            mutatedDecl = friendDeclText.substr(0, paramEnd) + ", ..." + friendDeclText.substr(paramEnd);
          } else {
            mutatedDecl = friendDeclText;
          }
        }
        mutatedDecl = "\n/*mut154*/" + mutatedDecl;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedDecl);
      }
    }
}
  
void MutatorFrontendAction_154::MutatorASTConsumer_154::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //First matcher to collect all friend declarations
    DeclarationMatcher collectorMatcher = friendDecl().bind("friendDecl");
    //Second matcher to target operator friend declarations for mutation
    DeclarationMatcher targetMat2 = friendDecl(hasFriend(hasDescendant(functionDecl(isOverloadedOperator(), hasOverloadedOperatorName(","))))).bind("targetFriend");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(collectorMatcher, &callback);
    matchFinder.addMatcher(targetMat2, &callback);
    matchFinder.matchAST(Context);
}