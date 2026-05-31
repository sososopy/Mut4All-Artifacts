//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Parameterized_Template_Function_Call_Without_Parentheses_195
 */ 
class MutatorFrontendAction_195 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(195)

private:
    class MutatorASTConsumer_195 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_195(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/transform_parameterized_template_function_call_without_parentheses_195.h"

// ========================================================================================================
#define MUT195_OUTPUT 1

void MutatorFrontendAction_195::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXDependentScopeMemberExpr>("TemplateMember")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.find('(') == string::npos ||
          content.find(')') == string::npos)
        return;
      content.erase(content.find('('), 1);
      content.erase(content.rfind(')'), 1);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_195::MutatorASTConsumer_195::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxDependentScopeMemberExpr(
                       hasTemplateKeyword(), hasTemplateArgs(),
                       unless(hasAncestor(expr(anyOf(callExpr(), memberExpr())))
                                  .bind("Call")))
                       .bind("TemplateMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}