//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Template_Instantiation_472
 */ 
class MutatorFrontendAction_472 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(472)

private:
    class MutatorASTConsumer_472 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_472(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> func_templates;
    };
};

//source file
#include "../include/Modify_Function_Template_Instantiation_472.h"

// ========================================================================================================
#define MUT472_OUTPUT 1

void MutatorFrontendAction_472::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      func_templates.push_back(FT);
    } else if (auto *ST = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "SpecializedTemplates")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      const clang::TemplateArgumentList *args = ST->getTemplateSpecializationArgs();
      if (args == nullptr)
        return;
      auto arg_str = stringutils::rangetoStr(*(Result.SourceManager),
                                             args->getSourceRange());
      llvm::outs() << arg_str << '\n';
      if (arg_str.find("int") != string::npos) {
        string new_arg_str = stringutils::replace(arg_str, "int", "double");
        llvm::outs() << new_arg_str << '\n';
        Rewrite.ReplaceText(args->getSourceRange(), new_arg_str);
      }
    }
  }

void MutatorFrontendAction_472::MutatorASTConsumer_472::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto func_template_matcher =
        functionTemplateDecl().bind("FuncTemplates");
    auto specialized_template_matcher =
        functionDecl(hasAncestor(functionTemplateDecl()))
            .bind("SpecializedTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(func_template_matcher, &callback);
    matchFinder.addMatcher(specialized_template_matcher, &callback);
    matchFinder.matchAST(Context);
}