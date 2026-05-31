//header file
#pragma once
#include "Mutator_base.h"

/**
 * Attribute_Injection_In_Unexpected_Contexts_434
 */ 
class MutatorFrontendAction_434 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(434)

private:
    class MutatorASTConsumer_434 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_434(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Attribute_Injection_In_Unexpected_Contexts_434.h"

// ========================================================================================================
#define MUT434_OUTPUT 1

void MutatorFrontendAction_434::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;

      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          ST->getSourceRange());
      llvm::outs() << decl << '\n';
      decl = "/*mut434*/[[nodiscard]] " + decl;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()),
                          decl);
    } else if (auto *FN =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FN->getLocation()))
        return;
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          FN->getSourceRange());
      llvm::outs() << decl << '\n';
      decl = "/*mut434*/[[nodiscard]] " + decl;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FN->getSourceRange()),
                          decl);
    } else if (auto *EN = Result.Nodes.getNodeAs<clang::EnumDecl>("Enum")) {
      if (!EN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     EN->getLocation()))
        return;
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          EN->getSourceRange());
      llvm::outs() << decl << '\n';
      decl = "/*mut434*/[[nodiscard]] " + decl;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(EN->getSourceRange()),
                          decl);
    }
}
  
void MutatorFrontendAction_434::MutatorASTConsumer_434::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl(isStruct()).bind("Struct");
    auto function_matcher = functionDecl().bind("Function");
    auto enum_matcher = enumDecl().bind("Enum");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.matchAST(Context);
}