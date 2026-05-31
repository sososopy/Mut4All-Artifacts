//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Recursive_Member_Function_Call_In_Template_13
 */ 
class MutatorFrontendAction_13 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(13)

private:
    class MutatorASTConsumer_13 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_13(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Transform_Recursive_Member_Function_Call_In_Template_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("RecursiveMethod")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isTemplated() || !MT->isRecursive())
        return;
      auto methodName = MT->getNameAsString();
      auto methodBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getBody()->getSourceRange());
      llvm::outs() << "Original method body: " << methodBody << '\n';
      string target = "(*this)(";
      string replacement = "this->operator()(";
      llvm::outs() << "Replacing " << target << " with " << replacement << '\n';
      size_t pos = 0;
      while ((pos = methodBody.find(target, pos)) != string::npos) {
        methodBody.replace(pos, target.length(), replacement);
        pos += replacement.length();
      }
      llvm::outs() << "Mutated method body: " << methodBody << '\n';
      Rewrite.ReplaceText(MT->getBody()->getSourceRange(), methodBody);
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto recursiveMethodMatcher =
        cxxMethodDecl(isRecursive()).bind("RecursiveMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(recursiveMethodMatcher, &callback);
    matchFinder.matchAST(Context);
}