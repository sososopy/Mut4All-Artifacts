//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Variable_Template_Instantiation_With_Template_Class_Member_In_Struct_Or_Class_465
 */ 
class MutatorFrontendAction_465 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(465)

private:
    class MutatorASTConsumer_465 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_465(Rewriter &R) : TheRewriter(R) {}
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
        const clang::ClassTemplateDecl *target = nullptr;
    };
};

//source file
#include "../include/modify_variable_template_instantiation_with_template_class_member_in_struct_or_class_465.h"

// ========================================================================================================
#define MUT465_OUTPUT 1

void MutatorFrontendAction_465::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplate")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto name = MT->getNameAsString();
      llvm::outs() << "var template: " << name << '\n';
      if (target == nullptr)
        return;
      auto target_name = target->getNameAsString();
      auto target_params = target->getTemplateParameters();
      auto target_param0 = target_params->getParam(0);
      auto target_param0_name = target_param0->getNameAsString();
      auto target_param1 = target_params->getParam(1);
      auto target_param1_name = target_param1->getNameAsString();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find(target_name) != string::npos)
        return;
      if (content.find(target_param0_name) != string::npos) {
        llvm::outs() << target_param0_name << '\n';
        stringutils::strReplace(content, target_param0_name, target_name);
        content = "/*mut465*/" + content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(MT->getSourceRange()), content);
      } else if (content.find(target_param1_name) != string::npos) {
        llvm::outs() << target_param1_name << '\n';
        stringutils::strReplace(content, target_param1_name, target_name);
        content = "/*mut465*/" + content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(MT->getSourceRange()), content);
      }
    }
    else if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                 "ClassTemplate")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      if (target != nullptr)
        return;
      auto name = CT->getNameAsString();
      llvm::outs() << "class template: " << name << '\n';
      auto params = CT->getTemplateParameters();
      if (params->size() != 2)
        return;
      target = CT;
    }
}
  
void MutatorFrontendAction_465::MutatorASTConsumer_465::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varTemplateDecl().bind("VarTemplate");
    auto class_template_matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_template_matcher, &callback);
    matchFinder.matchAST(Context);
}