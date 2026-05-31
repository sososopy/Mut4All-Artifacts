//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Friend_Class_Constraint_Generalization_491
 */ 
class MutatorFrontendAction_491 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(491)

private:
    class MutatorASTConsumer_491 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_491(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Friend_Class_Constraint_Generalization_491.h"

// ========================================================================================================
#define MUT491_OUTPUT 1

void MutatorFrontendAction_491::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FriendTemplateDecl>("Mutator")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      llvm::outs() << decl << '\n';
      auto tpl = MT->getFriendDecl();
      if (tpl == nullptr)
        return;
      if (tpl->getKind() != Decl::Kind::ClassTemplate)
        return;
      auto class_tpl = dyn_cast<ClassTemplateDecl>(tpl);
      auto params = class_tpl->getTemplateParameters();
      if (params->size() != 1)
        return;
      auto param = params->getParam(0);
      if (param->getKind() != TemplateParameter::ParameterKind::NonTypeTemplate)
        return;
      auto non_type_param = dyn_cast<NonTypeTemplateParmDecl>(param);
      auto type = non_type_param->getType();
      if (!type->isDependentType())
        return;
      auto type_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              type->getSourceRange());
      llvm::outs() << type_str << '\n';
      decl = std::regex_replace(
          decl, std::regex(type_str),
          "typename " + type_str + ", typename = std::enable_if_t<" +
              type_str + "<U>>");
      llvm::outs() << decl << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), decl);
    }
}
  
void MutatorFrontendAction_491::MutatorASTConsumer_491::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = friendTemplateDecl().bind("Mutator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}