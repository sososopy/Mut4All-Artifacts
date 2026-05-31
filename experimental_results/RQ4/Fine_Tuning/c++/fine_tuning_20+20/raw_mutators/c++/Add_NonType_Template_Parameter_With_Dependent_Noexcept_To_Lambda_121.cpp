//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_NonType_Template_Parameter_With_Dependent_Noexcept_To_Lambda_121
 */ 
class MutatorFrontendAction_121 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(121)

private:
    class MutatorASTConsumer_121 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_121(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionTemplateDecl *cur_function_template = nullptr;
    };
};

//source file
#include "../include/add_nontype_template_parameter_with_dependent_noexcept_to_lambda_121.h"

// ========================================================================================================
#define MUT121_OUTPUT 1

void MutatorFrontendAction_121::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      llvm::outs() << "FT\n";
      cur_function_template = FT;
    } else if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      if (cur_function_template == nullptr)
        return;
      if (!cur_function_template->getSourceRange().fullyContains(
              LE->getSourceRange()))
        return;
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 LE->getSourceRange());
      if (lambda_text.find("noexcept") != string::npos)
        return;
      auto tpl_pos = lambda_text.find('<');
      if (tpl_pos == string::npos)
        return;
      auto tpl_end_pos = lambda_text.find('>', tpl_pos);
      if (tpl_end_pos == string::npos)
        return;
      auto new_lambda_text = lambda_text;
      new_lambda_text.insert(tpl_end_pos, ", int x");
      new_lambda_text.insert(tpl_end_pos + 6,
                             " noexcept(requires { requires requires { typename "
                             "b::x; }; }) ");
      Rewrite.ReplaceText(LE->getSourceRange(), new_lambda_text);
    }
}
  
void MutatorFrontendAction_121::MutatorASTConsumer_121::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto function_template_matcher = functionTemplateDecl().bind("FunctionTemplate");
    auto lambda_matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(function_template_matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}