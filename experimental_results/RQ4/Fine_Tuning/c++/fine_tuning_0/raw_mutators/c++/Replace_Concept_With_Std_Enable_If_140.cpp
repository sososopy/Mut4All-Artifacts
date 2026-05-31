//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_With_Std_Enable_If_140
 */ 
class MutatorFrontendAction_140 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(140)

private:
    class MutatorASTConsumer_140 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_140(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Concept_With_Std_Enable_If_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Func")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isFunctionTemplateSpecialization())
        return;
      if (!FD->isTemplated())
        return;
      if (!FD->isTemplateInstantiation())
        return;
      if (FD->getTemplatedKind() != FunctionDecl::TK_FunctionTemplate)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TSK_ImplicitInstantiation)
        return;
      if (FD->isDependentContext() == false)
        return;
      llvm::outs() << "Mut140: " << FD->getNameAsString() << '\n';
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FD->getSourceRange());
      auto template_paras = FD->getTemplateSpecializationArgs();
      for (auto para : template_paras->asArray()) {
        llvm::outs() << para.getAsType().getAsString() << '\n';
      }
      auto template_decl = FD->getPrimaryTemplate();
      auto template_decl_content = stringutils::rangetoStr(
          *(Result.SourceManager), template_decl->getSourceRange());
      llvm::outs() << template_decl_content << '\n';
      auto concept_pos = template_decl_content.find("requires");
      if (concept_pos == string::npos)
        return;
      auto concept_end = template_decl_content.find('>');
      if (concept_end == string::npos)
        return;
      auto concept_name = template_decl_content.substr(
          concept_pos + 9, concept_end - concept_pos - 9);
      llvm::outs() << concept_name << '\n';
      string new_template_decl = "template <typename ";
      for (auto para : template_paras->asArray()) {
        new_template_decl += para.getAsType().getAsString() + ",";
      }
      new_template_decl.pop_back();
      new_template_decl +=
          "> using = std::enable_if_t<std::is_integral_v<T>>";
      new_template_decl = "/*mut140*/" + new_template_decl;
      llvm::outs() << new_template_decl << '\n';
      Rewrite.ReplaceText(
          FD->getBeginLoc(),
          template_decl->getEndLoc().getRawEncoding() -
              FD->getBeginLoc().getRawEncoding() + 1,
          new_template_decl);
    }
}
  
void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}