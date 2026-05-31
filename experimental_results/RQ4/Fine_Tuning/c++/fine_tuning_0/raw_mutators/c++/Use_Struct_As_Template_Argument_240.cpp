//header file
#pragma once
#include "Mutator_base.h"

/**
 * Use_Struct_As_Template_Argument_240
 */ 
class MutatorFrontendAction_240 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(240)

private:
    class MutatorASTConsumer_240 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_240(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *struct_decl = nullptr;
        const clang::TemplateDecl *template_decl = nullptr;
    };
};

//source file
#include "../include/Use_Struct_As_Template_Argument_240.h"

// ========================================================================================================
#define MUT240_OUTPUT 1

void MutatorFrontendAction_240::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      if (!SD->isStruct())
        return;
      struct_decl = SD;
      llvm::outs() << "Struct: " << struct_decl->getNameAsString() << "\n";
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>(
                   "Template")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      template_decl = TD;
      llvm::outs() << "Template: "
                   << template_decl->getNameAsString() << "\n";
    } else if (auto *TI = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>(
                   "TemplateInstance")) {
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getBeginLoc()))
        return;
      if (!struct_decl || !template_decl)
        return;
      auto struct_name = struct_decl->getNameAsString();
      auto template_name = template_decl->getNameAsString();
      auto TI_name = stringutils::rangetoStr(*(Result.SourceManager),
                                             TI->getSourceRange());
      llvm::outs() << "TemplateInstance: " << TI_name << "\n";
      if (TI_name.find(template_name) == string::npos)
        return;
      auto new_TI_name = template_name + "<" + struct_name + ">";
      Rewrite.ReplaceText(TI->getBeginLoc(), TI_name.length(), new_TI_name);
    }
}
  
void MutatorFrontendAction_240::MutatorASTConsumer_240::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Struct");
    auto template_matcher = templateDecl().bind("Template");
    auto template_instance_matcher =
        templateSpecializationType().bind("TemplateInstance");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(template_instance_matcher, &callback);
    matchFinder.matchAST(Context);
}