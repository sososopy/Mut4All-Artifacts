//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_AliasTemplate_To_FunctionTemplate_473
 */ 
class MutatorFrontendAction_473 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(473)

private:
    class MutatorASTConsumer_473 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_473(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeAliasTemplateDecl *> alias_templates;
        std::vector<const clang::TypeAliasTemplateDecl *> target_templates;
    };
};

//source file
#include "../include/Replace_AliasTemplate_To_FunctionTemplate_473.h"

// ========================================================================================================
#define MUT473_OUTPUT 1

void MutatorFrontendAction_473::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplates")) {
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      alias_templates.push_back(AT);
    } else if (auto *TT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>(
                   "TargetTemplates")) {
      if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TT->getLocation()))
        return;
      target_templates.push_back(TT);
    } else if (auto *ST = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>(
                   "SelectedTemplate")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      auto alias_name = ST->getNameAsString();
      auto alias_type = ST->getTemplatedDecl()->getUnderlyingType();
      auto template_params = ST->getTemplateParameters();
      std::string template_str = "";
      llvm::raw_string_ostream stream(template_str);
      template_params->print(stream, *(Result.Context->getPrintingPolicy()), 0,
                             true);
      auto params_str = stream.str();
      auto type_str = alias_type.getAsString();
      auto func_str = "template " + params_str + " using " + alias_name +
                      " = " + type_str + ";\n";
      llvm::outs() << func_str << '\n';
      Rewrite.ReplaceText(ST->getSourceRange(), func_str);
    }
}
  
void MutatorFrontendAction_473::MutatorASTConsumer_473::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = typeAliasTemplateDecl().bind("AliasTemplates");
    auto target_matcher = typeAliasTemplateDecl(has(typeAliasDecl(hasType(
                                    hasDescendant(typeAliasTemplateDecl())))))
                              .bind("TargetTemplates");
    auto selected_matcher = typeAliasTemplateDecl(has(typeAliasDecl(hasType(
                                        hasDescendant(typeAliasTemplateDecl(
                                            hasAnyName("t", "u", "v"))))))
                              .bind("SelectedTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(target_matcher, &callback);
    matchFinder.addMatcher(selected_matcher, &callback);
    matchFinder.matchAST(Context);
}