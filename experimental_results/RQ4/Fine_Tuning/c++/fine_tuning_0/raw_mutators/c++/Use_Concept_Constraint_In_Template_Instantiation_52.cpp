//header file
#pragma once
#include "Mutator_base.h"

/**
 * Use_Concept_Constraint_In_Template_Instantiation_52
 */ 
class MutatorFrontendAction_52 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(52)

private:
    class MutatorASTConsumer_52 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_52(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Use_Concept_Constraint_In_Template_Instantiation_52.h"

// ========================================================================================================
#define MUT52_OUTPUT 1

void MutatorFrontendAction_52::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      if (!FD->isThisDeclarationADefinition())
        return;
      auto body = FD->getBody();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             body->getSourceRange());
      auto pos = content.find('{');
      if (pos == string::npos)
        return;
      content.insert(pos + 1, "\n/*mut52*/using alias = T<int>;\n");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          content);
    } else if (auto *TD =
                   Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TD->getSourceRange());
      auto pos = content.find('<');
      if (pos == string::npos)
        return;
      content.insert(pos + 1, "Constraint<T> ");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_52::MutatorASTConsumer_52::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto function_matcher = functionDecl().bind("Function");
    auto typealias_matcher = typeAliasDecl().bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(typealias_matcher, &callback);
    matchFinder.matchAST(Context);
}