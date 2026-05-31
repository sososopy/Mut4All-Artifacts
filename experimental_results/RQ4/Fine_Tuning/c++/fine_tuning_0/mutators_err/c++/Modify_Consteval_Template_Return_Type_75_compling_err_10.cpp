//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Consteval_Template_Return_Type_75
 */ 
class MutatorFrontendAction_75 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(75)

private:
    class MutatorASTConsumer_75 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_75(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Consteval_Template_Return_Type_75.h"

// ========================================================================================================
#define MUT75_OUTPUT 1

void MutatorFrontendAction_75::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstevalTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (!FD->isTemplateInstantiation() && !FD->isTemplated())
        return;
      if (!FD->isConsteval())
        return;
      auto returnType = FD->getReturnType();
      if (!returnType->isDependentType())
        return;
      llvm::outs() << returnType.getAsString() << '\n';
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                FD->getSourceRange());
      llvm::outs() << definition << '\n';
      stringutils::removeSubStr(definition, "consteval");
      stringutils::removeSubStr(definition, "constexpr");
      stringutils::replaceSubStr(definition, "auto", "int");
      definition = "consteval " + definition;
      llvm::outs() << definition << '\n';
      Rewrite.ReplaceText(FD->getSourceRange(), definition);
    } else if (auto *IL = Result.Nodes.getNodeAs<clang::VarDecl>("Invoke")) {
      if (!IL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IL->getLocation()))
        return;
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                IL->getSourceRange());
      llvm::outs() << definition << '\n';
      stringutils::replaceSubStr(definition, "auto", "int");
      stringutils::replaceSubStr(definition, "constexpr", "consteval");
      llvm::outs() << definition << '\n';
      Rewrite.ReplaceText(IL->getSourceRange(), definition);
    }
}
  
void MutatorFrontendAction_75::MutatorASTConsumer_75::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("ConstevalTemplate");
    auto invoke_matcher = varDecl(hasInitializer(callExpr())).bind("Invoke");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(invoke_matcher, &callback);
    matchFinder.matchAST(Context);
}