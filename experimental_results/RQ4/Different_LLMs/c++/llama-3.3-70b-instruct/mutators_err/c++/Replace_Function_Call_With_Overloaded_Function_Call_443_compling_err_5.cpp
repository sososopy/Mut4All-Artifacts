//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"

/**
 * Replace_Function_Call_With_Overloaded_Function_Call_443
 */ 
class MutatorFrontendAction_443 : public MutatorFrontendAction {
public:
    MutatorFrontendAction_443() {}
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_443>(TheRewriter);
    }

private:
    class MutatorASTConsumer_443 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_443(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, ASTContext &Context) : Rewrite(Rewrite), Context(Context) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        ASTContext &Context;
        //Necessary node information record used in the mutation process
    };

    Rewriter TheRewriter;
};

//source file
#include "../include/Replace_Function_Call_With_Overloaded_Function_Call_443.h"
#include "clang/AST/DeclBase.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CallExpr->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   CallExpr->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Find overloaded functions with the same name
      auto funcDecl = CallExpr->getDirectCallee();
      if (!funcDecl)
        return;
      auto funcName = funcDecl->getNameAsString();
      auto overloadedFuncs = Result.Context->getTranslationUnitDecl()->lookupSingleName(
          clang::DeclarationName(funcDecl->getName()), 
          clang::LookupNameKind::LookupOrdinaryName);
      if (!overloadedFuncs)
        return;
      for (auto overloadedFunc : *overloadedFuncs) {
        if (auto overloadedFuncDecl = dyn_cast<FunctionDecl>(overloadedFunc)) {
          if (overloadedFuncDecl->getNumParams() != funcDecl->getNumParams())
            continue;
          // Replace the original function call with the overloaded function call
          auto newCallExpr = "/*mut443*/" + funcName + "(";
          for (auto arg : CallExpr->arguments()) {
            newCallExpr += stringutils::rangetoStr(*(Result.SourceManager),
                                                    arg->getSourceRange()) + ", ";
          }
          newCallExpr.pop_back();
          newCallExpr.pop_back();
          newCallExpr += ")";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(CallExpr->getSourceRange()), newCallExpr);
        }
      }
    }
}
  
void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    auto matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter, Context);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}