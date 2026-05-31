//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Type.h"

/**
 * Replace_Va_List_With_Incompatible_Type_330
 */ 
class MutatorFrontendAction_330 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_330>(TheRewriter);
    }

private:
    class MutatorASTConsumer_330 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_330(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Replace_Va_List_With_Incompatible_Type_330.h"
#include "clang/AST/Type.h"

// ========================================================================================================
#define MUT330_OUTPUT 1

void MutatorFrontendAction_330::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto params = FD->parameters();
      for (auto param : params) {
        if (auto *VD = dyn_cast<clang::VarDecl>(param)) {
          if (VD->getType()->isVariadic() == true) {
            //Perform mutation on the source code text by applying string replacement
            std::string replacement = "0"; // Replace va_list with an integer literal
            Rewrite.ReplaceText(VD->getSourceRange(), replacement);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_330::MutatorASTConsumer_330::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(0, anyOf(hasType(cxxRecordDecl(hasName("va_list"))), hasType(isInteger())))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}