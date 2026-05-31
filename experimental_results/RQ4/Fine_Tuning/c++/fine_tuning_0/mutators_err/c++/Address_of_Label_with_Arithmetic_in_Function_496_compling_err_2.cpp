//header file
#pragma once
#include "Mutator_base.h"

/**
 * Address_of_Label_with_Arithmetic_in_Function_496
 */ 
class MutatorFrontendAction_496 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(496)

private:
    class MutatorASTConsumer_496 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_496(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Address_of_Label_with_Arithmetic_in_Function_496.h"

// ========================================================================================================
#define MUT496_OUTPUT 1

void MutatorFrontendAction_496::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (!FD->hasBody())
        return;
      auto name = FD->getNameAsString();
      auto body = FD->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      llvm::outs() << body_str << '\n';
      auto label1 = "label1";
      auto label2 = "label2";
      auto ins = string("&&") + label1 + "-&&" + label2 + "<" + name + ";";
      llvm::outs() << ins << '\n';
      body_str.insert(body_str.find('{') + 1, ins);
      body_str.insert(body_str.rfind('}'), label1 + ": ;" + label2 + ": ;");
      llvm::outs() << body_str << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          body_str);
    }
}
  
void MutatorFrontendAction_496::MutatorASTConsumer_496::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}