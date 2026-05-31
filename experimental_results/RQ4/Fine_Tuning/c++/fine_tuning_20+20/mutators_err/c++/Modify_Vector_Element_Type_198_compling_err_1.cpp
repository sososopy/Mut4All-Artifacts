//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Vector_Element_Type_198
 */ 
class MutatorFrontendAction_198 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(198)

private:
    class MutatorASTConsumer_198 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_198(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeAliasDecl *> alias_decls;
        std::vector<const clang::TypeAliasTemplateDecl *> alias_template_decls;
        std::vector<const clang::TypedefDecl *> typedef_decls;
        std::vector<const clang::VarDecl *> vector_decls;
    };
};

//source file
#include "../include/modify_vector_element_type_198.h"

// ========================================================================================================
#define MUT198_OUTPUT 1

void MutatorFrontendAction_198::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AL->getLocation()))
        return;
      alias_decls.push_back(AL);
    } else if (auto *ALT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>(
                   "AliasTemplate")) {
      if (!ALT || !Result.Context->getSourceManager().isWrittenInMainFile(
                      ALT->getLocation()))
        return;
      alias_template_decls.push_back(ALT);
    } else if (auto *TD =
                   Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedef")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      typedef_decls.push_back(TD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vectors")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back(VD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vector")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType().getAsString().find("vector") == string::npos)
        return;
      auto type = VD->getType();
      if (type->isVectorType() == false)
        return;
      if (type->isInstantiationDependentType())
        return;
      for (auto alias : alias_decls) {
        if (alias->getUnderlyingType() == type)
          return;
      }
      for (auto alias_template : alias_template_decls) {
        if (alias_template->getTemplatedDecl()->getUnderlyingType() == type)
          return;
      }
      for (auto td : typedef_decls) {
        if (td->getUnderlyingType() == type)
          return;
      }
      vector_decls.push_back