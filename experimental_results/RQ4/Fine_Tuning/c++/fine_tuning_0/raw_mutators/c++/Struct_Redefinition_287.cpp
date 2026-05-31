//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Redefinition_287
 */ 
class MutatorFrontendAction_287 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(287)

private:
    class MutatorASTConsumer_287 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_287(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FunctionDecl *cur_func;
    };
};

//source file
#include "../include/Struct_Redefinition_287.h"

// ========================================================================================================
#define MUT287_OUTPUT 1

void MutatorFrontendAction_287::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      cur_func = FD;
    } else if (auto *SD =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      if (SD->isCompleteDefinition() == false)
        return;
      if (SD->isStruct() == false)
        return;
      if (SD->isLambda())
        return;
      if (SD->getParent() != cur_func)
        return;
      auto name = SD->getNameAsString();
      auto def =
          stringutils::rangetoStr(*(Result.SourceManager), SD->getSourceRange());
      llvm::outs() << def << '\n';
      string ins = "/*mut287*/struct " + name + "; ";
      def = ins + def;
      Rewrite.ReplaceText(SD->getBeginLoc(), 0, def);
    }
}
  
void MutatorFrontendAction_287::MutatorASTConsumer_287::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
}