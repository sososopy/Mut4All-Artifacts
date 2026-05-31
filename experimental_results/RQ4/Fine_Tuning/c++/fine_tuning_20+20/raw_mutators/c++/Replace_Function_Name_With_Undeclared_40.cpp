//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Name_With_Undeclared_40
 */ 
class MutatorFrontendAction_40 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(40)

private:
    class MutatorASTConsumer_40 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_40(Rewriter &R) : TheRewriter(R) {}
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
        std::set<string> declared;
    };
};

//source file
#include "../include/replace_function_name_with_undeclared_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto funcname = DL->getNameAsString();
      declared.insert(funcname);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto funcname = DL->getNameAsString();
      if (declared.find(funcname) == declared.end())
        return;
      string newname = "mut40_";
      int index = 0;
      while (true) {
        auto validname = newname + std::to_string(index);
        if (declared.find(validname) == declared.end()) {
          newname = validname;
          break;
        }
        ++index;
      }
      Rewrite.ReplaceText(DL->getNameInfo().getSourceRange(), newname);
    }
}
  
void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto matcher2 = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
    MatchFinder matchFinder2;
    matchFinder2.addMatcher(matcher2, &callback);
    matchFinder2.matchAST(Context);
}