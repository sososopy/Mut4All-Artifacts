//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Pack_Expansion_In_Return_Type_204
 */ 
class MutatorFrontendAction_204 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(204)

private:
    class MutatorASTConsumer_204 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_204(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateParameterList *> parameter_lists;
    };
};

//source file
#include "../include/Template_Parameter_Pack_Expansion_In_Return_Type_204.h"

// ========================================================================================================
#define MUT204_OUTPUT 1

void MutatorFrontendAction_204::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isDependentContext())
        return;
      if (FD->isTemplated() == false) {
        llvm::outs() << "Mut204: Not a template function!\n";
        return;
      }
      auto ret_type =
          stringutils::rangetoStr(*(Result.SourceManager), FD->getReturnTypeSourceRange());
      llvm::outs() << "Mut204: Original return type: " << ret_type << '\n';
      if (ret_type.find("...") != string::npos)
        return;
      if (parameter_lists.empty()) {
        llvm::outs() << "Mut204: No template parameter list!\n";
        return;
      }
      const TemplateParameterList *target_list = nullptr;
      for (auto list : parameter_lists) {
        if (FD->getBeginLoc() < list->getBeginLoc())
          continue;
        if (FD->getEndLoc() > list->getEndLoc())
          continue;
        target_list = list;
      }
      if (target_list == nullptr) {
        llvm::outs() << "Mut204: No valid template parameter list!\n";
        return;
      }
      llvm::outs() << "Mut204: Target template parameter list: "
                   << target_list->size() << '\n';
      bool has_pack = false;
      for (size_t i = 0; i < target_list->size(); ++i) {
        auto param = target_list->getParam(i);
        if (param->isTemplateParameterPack()) {
          has_pack = true;
          break;
        }
      }
      if (has_pack == false) {
        llvm::outs() << "Mut204: No parameter pack!\n";
        return;
      }
      std::vector<int> pack_indices;
      for (size_t i = 0; i < target_list->size(); ++i) {
        auto param = target_list->getParam(i);
        if (param->isTemplateParameterPack()) {
          pack_indices.push_back(i);
        }
      }
      if (pack_indices.empty()) {
        llvm::outs() << "Mut204: No parameter pack!\n";
        return;
      }
      int pack_index = getrandom::getRandomIndex(pack_indices.size() - 1);
      llvm::outs() << "Mut204: Chosen parameter pack index: "
                   << pack_indices[pack_index] << '\n';
      auto pack_param = target_list->getParam(pack_indices[pack_index]);
      if (auto type_param = dyn_cast<TemplateTypeParmDecl>(pack_param)) {
        auto pack_name = type_param->getNameAsString();
        llvm::outs() << "Mut204: Chosen parameter pack name: " << pack_name
                     << '\n';
        ret_type = "/*mut204*/" + pack_name + "...";
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getReturnTypeSourceRange()),
            ret_type);
      } else if (auto non_type_param =
                     dyn_cast<NonTypeTemplateParmDecl>(pack_param)) {
        auto pack_name = non_type_param->getNameAsString();
        llvm::outs() << "Mut204: Chosen parameter pack name: " << pack_name
                     << '\n';
        int dim = getrandom::getRandomIndex(3);
        ret_type = "/*mut204*/decltype(" + pack_name + "[0])";
        for (int i = 0; i < dim; ++i)
          ret_type += "[]";
        llvm::outs() << "Mut204: Chosen return type: " << ret_type << '\n';
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getReturnTypeSourceRange()),
            ret_type);
      } else if (auto template_param =
                     dyn_cast<TemplateTemplateParmDecl>(pack_param)) {
        auto pack_name = template_param->getNameAsString();
        llvm::outs() << "Mut204: Chosen parameter pack name: " << pack_name
                     << '\n';
        ret_type = "/*mut204*/" + pack_name + "<>";
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getReturnTypeSourceRange()),
            ret_type);
      }
    } else if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateDecl>(
                   "Template")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      parameter_lists.push_back(TP->getTemplateParameters());
    }
}
  
void MutatorFrontendAction_204::MutatorASTConsumer_204::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Function");
    auto template_matcher = templateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}