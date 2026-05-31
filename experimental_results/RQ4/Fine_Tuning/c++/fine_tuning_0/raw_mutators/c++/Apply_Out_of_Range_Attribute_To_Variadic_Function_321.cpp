//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Out_of_Range_Attribute_To_Variadic_Function_321
 */ 
class MutatorFrontendAction_321 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(321)

private:
    class MutatorASTConsumer_321 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_321(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Apply_Out_of_Range_Attribute_To_Variadic_Function_321.h"

// ========================================================================================================
#define MUT321_OUTPUT 1

void MutatorFrontendAction_321::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("VarFunc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto func_name = MT->getNameAsString();
      auto params = MT->parameters();
      if (!MT->isVariadic())
        return;
      //Perform mutation on the source code text by applying string replacement
      int param_num = params.size();
      int choice = getrandom::getRandomIndex(1);
      if (choice == 0) {
        auto attr = "__attribute__((format_arg(" + std::to_string(param_num) +
                    ")))";
        llvm::outs() << "/*mut321*/" << attr << '\n';
        Rewrite.ReplaceText(MT->getBeginLoc(), 0, "/*mut321*/" + attr + " ");
      }
    }
}
  
void MutatorFrontendAction_321::MutatorASTConsumer_321::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("VarFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}