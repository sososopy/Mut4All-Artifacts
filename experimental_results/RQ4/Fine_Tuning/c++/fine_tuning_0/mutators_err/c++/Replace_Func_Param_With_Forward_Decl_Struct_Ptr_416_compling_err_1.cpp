//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Func_Param_With_Forward_Decl_Struct_Ptr_416
 */ 
class MutatorFrontendAction_416 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(416)

private:
    class MutatorASTConsumer_416 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_416(Rewriter &R) : TheRewriter(R) {}
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
        std::set<string> struct_names;
    };
};

//source file
#include "../include/Replace_Func_Param_With_Forward_Decl_Struct_Ptr_416.h"

// ========================================================================================================
#define MUT416_OUTPUT 1

void MutatorFrontendAction_416::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isMain())
        return;

      if (FD->isThisDeclarationADefinition() == false)
        return;

      if (FD->getNumParams() == 0)
        return;

      auto params = FD->parameters();
      int param_index = getrandom::getRandomIndex(params.size() - 1);
      auto param = params[param_index];
      auto param_type = param->getType();
      auto param_name = param->getNameAsString();
      llvm::outs() << "param_type: " << param_type.getAsString() << '\n';
      llvm::outs() << "param_name: " << param_name << '\n';

      auto struct_name = "Mut416_" + param_name;
      if (struct_names.find(struct_name) != struct_names.end()) {
        struct_name += std::to_string(getrandom::getRandomIndex(100));
      }
      struct_names.insert(struct_name);
      auto struct_forward_decl = "/*mut416*/struct " + struct_name + ";";
      auto new_param_type = struct_name + " *";
      llvm::outs() << "new_param_type: " << new_param_type << '\n';

      Rewrite.ReplaceText(param->getTypeSourceInfo()->getTypeLoc(),
                          new_param_type);
      Rewrite.ReplaceText(FD->getBeginLoc(), 0, struct_forward_decl);
    }
}
  
void MutatorFrontendAction_416::MutatorASTConsumer_416::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}