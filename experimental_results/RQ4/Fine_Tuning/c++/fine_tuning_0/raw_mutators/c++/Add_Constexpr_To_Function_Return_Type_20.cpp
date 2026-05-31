//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Constexpr_To_Function_Return_Type_20
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(20)

private:
    class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_20(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Constexpr_To_Function_Return_Type_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isConstexpr())
        return;
      if (!FD->hasBody())
        return;
      auto returnType = FD->getReturnType();
      if (!returnType->isBuiltinType() && !returnType->isEnumeralType() &&
          !returnType->isRecordType())
        return;
      auto body = FD->getBody();
      if (body == nullptr)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FD->getSourceRange());
      content = "constexpr " + content;
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}