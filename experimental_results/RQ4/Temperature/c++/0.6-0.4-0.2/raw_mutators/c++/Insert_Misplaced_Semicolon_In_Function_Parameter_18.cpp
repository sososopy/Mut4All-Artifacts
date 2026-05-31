//header file
#pragma once
#include "Mutator_base.h"

/**
 * insert_misplaced_semicolon_in_function_parameter_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_misplaced_semicolon_in_function_parameter_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      // Ensure the function is within a class
      if (auto *RD = dyn_cast<CXXRecordDecl>(FD->getParent())) {
        // Get the source code text of target node
        auto paramList = FD->parameters();
        if (paramList.size() > 1) {
          //Perform mutation on the source code text by applying string replacement
          auto param = paramList[0];
          SourceLocation insertLoc = param->getEndLoc().getLocWithOffset(1);
          //Insert a semicolon after the first parameter
          Rewrite.InsertText(insertLoc, ";", true, true);
        }
      }
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParent(cxxRecordDecl())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}