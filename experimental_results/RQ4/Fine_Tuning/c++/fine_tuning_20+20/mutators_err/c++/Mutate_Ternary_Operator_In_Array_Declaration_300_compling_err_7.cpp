//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Ternary_Operator_In_Array_Declaration_300
 */ 
class MutatorFrontendAction_300 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(300)

private:
    class MutatorASTConsumer_300 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_300(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/mutate_ternary_operator_in_array_declaration_300.h"

// ========================================================================================================
#define MUT300_OUTPUT 1

void MutatorFrontendAction_300::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ArrayType>("Arrays")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isSugared() == false)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('?') == string::npos ||
          content.find(':') == string::npos)
        return;
      if (content.find("sizeof") == string::npos)
        return;
      if (content.find('(') == string::npos ||
          content.find(')') == string::npos)
        return;
      if (content.find(',') == string::npos)
        return;
      auto left_bracket = content.find('(');
      auto right_bracket = content.rfind(')');
      if (left_bracket > right_bracket)
        return;
      auto comma = content.find(',', left_bracket);
      if (comma > right_bracket)
        return;
      content[comma] = '=';
      content.insert(left_bracket, 1, '(');
      content.insert(right_bracket + 1, 1, ')');
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_300::MutatorASTConsumer_300::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = arrayType().bind("Arrays");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}