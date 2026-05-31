//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Atomic_Type_And_Add_Invalid_Comparison_237
 */ 
class MutatorFrontendAction_237 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(237)

private:
    class MutatorASTConsumer_237 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_237(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Atomic_Type_And_Add_Invalid_Comparison_237.h"

// ========================================================================================================
#define MUT237_OUTPUT 1

void MutatorFrontendAction_237::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("atomicFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto func = stringutils::rangetoStr(*(Result.SourceManager),
                                          FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (func.find("std::atomic") == string::npos)
        return;
      llvm::outs() << func << '\n';
      auto pos = func.find("std::atomic");
      auto end = func.find('>', pos);
      func.replace(pos, end - pos + 1, "int");
      auto body_pos = func.find('{');
      auto body_end = func.rfind('}');
      string body = func.substr(body_pos, body_end - body_pos + 1);
      llvm::outs() << body << '\n';
      auto var_pos = body.find("std::atomic");
      auto var_end = body.find('=', var_pos);
      auto var = body.substr(var_pos, var_end - var_pos);
      llvm::outs() << var << '\n';
      string cmp = "if(" + var + "<5){}";
      func.insert(body_end, cmp);
      llvm::outs() << func << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(FD->getSourceRange()), func);
    }
}
  
void MutatorFrontendAction_237::MutatorASTConsumer_237::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody()).bind("atomicFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}