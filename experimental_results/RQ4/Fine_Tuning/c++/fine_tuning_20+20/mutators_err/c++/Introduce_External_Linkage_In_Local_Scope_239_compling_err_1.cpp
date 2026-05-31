//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_External_Linkage_In_Local_Scope_239
 */ 
class MutatorFrontendAction_239 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(239)

private:
    class MutatorASTConsumer_239 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_239(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> cur_vars;
    };
};

//source file
#include "../include/Introduce_External_Linkage_In_Local_Scope_239.h"

// ========================================================================================================
#define MUT239_OUTPUT 1

void MutatorFrontendAction_239::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      //Get the source code text of target node
      auto body = FD->getBody();
      auto decls = body->decls();
      for (auto decl : decls) {
        if (auto *VD = dyn_cast<VarDecl>(decl)) {
          cur_vars.push_back(VD);
        }
      }
    }
    //Check whether the matched AST node is the target node
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Variables")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          VD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      decl = "/*mut239*/extern " + decl;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), decl);
    }
}
  
void MutatorFrontendAction_239::MutatorASTConsumer_239::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto function_matcher = functionDecl().bind("Functions");
    auto var_matcher = varDecl().bind("Variables");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}