//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Function_Template_To_Concept_Constrained_Non_Type_Template_Parameter_Pack_In_Class_Template_Member_Function_106
 */ 
class MutatorFrontendAction_106 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(106)

private:
    class MutatorASTConsumer_106 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_106(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> cur_func_templates;
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<const clang::ConceptDecl *> cur_concepts;
        std::vector<const clang::ClassTemplateDecl *> cur_class_templates;
    };
};

//source file
#include "../include/transform_function_template_to_concept_constrained_non_type_template_parameter_pack_in_class_template_member_function_106.h"

// ========================================================================================================
#define MUT106_OUTPUT 1

void MutatorFrontendAction_106::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      cur_func_templates.push_back(FT);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      cur_classes.push_back(CL);
    } else if (auto *CP = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CP->getLocation()))
        return;
      cur_concepts.push_back(CP);
    } else if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplates")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      cur_class_templates.push_back(CT);
    }
  }

void MutatorFrontendAction_106::MutatorASTConsumer_106::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("FuncTemplates");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto concept_matcher = conceptDecl().bind("Concepts");
    auto class_template_matcher = classTemplateDecl().bind("ClassTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.addMatcher(class_template_matcher, &callback);
    matchFinder.matchAST(Context);

    if (cur_func_templates.empty())
      return;
    size_t index = getrandom::getRandomIndex(cur_func_templates.size() - 1);
    auto target = cur_func_templates[index];
    auto content = stringutils::rangetoStr(*(Context.getSourceManager()),
                                           target->getSourceRange());
    llvm::outs() << content << '\n';

    if (target->getDeclContext()->isRecord()) {
      auto DL = dyn_cast<CXXRecordDecl>(target->getDeclContext());
      if (DL == nullptr)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->getDescribedClassTemplate() == nullptr) {
        if (cur_classes.empty())
          return;
        size_t cindex = getrandom::getRandomIndex(cur_classes.size() - 1);
        auto target_class = cur_classes[cindex];
        llvm::outs() << target_class->getNameAsString() << '\n';
        auto class_content = stringutils::rangetoStr(
            *(Context.getSourceManager()), target_class->getSourceRange());
        llvm::outs() << class_content << '\n';
        if (class_content.find("template") == string::npos) {
          class_content = "template<typename T>\n" + class_content;
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(target_class->getSourceRange()),
              class_content);
        }
      }
    } else {
      if (cur_classes.empty())
        return;
      size_t cindex = getrandom::getRandomIndex(cur_classes.size() - 1);
      auto target_class = cur_classes[cindex];
      llvm::outs() << target_class->getNameAsString() << '\n';
      auto class_content = stringutils::rangetoStr(
          *(Context.getSourceManager()), target_class->getSourceRange());
      llvm::outs() << class_content << '\n';
      if (class_content.find("template") == string::npos) {
        class_content = "template<typename T>\n" + class_content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(target_class->getSourceRange()),
            class_content);
      }
      class_content.insert(class_content.rfind('}'), content);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(target_class->getSourceRange()),
          class_content);
    }

    if (content.find("concept") == string::npos) {
      if (!cur_concepts.empty()) {
        size_t cindex = getrandom::getRandomIndex(cur_concepts.size() - 1);
        auto target_concept = cur_concepts[cindex];
        auto concept_content = stringutils::rangetoStr(
            *(Context.getSourceManager()), target_concept->getSourceRange());
        llvm::outs() << concept_content << '\n';
        auto concept_name = target_concept->getNameAsString();
        llvm::outs() << concept_name << '\n';
        content = "template<" + concept_name + " auto... xs>" +
                  content.substr(content.find('>') + 1);
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(target->getSourceRange()), content);
      } else {
        content = "template<typename T> concept AlwaysTrue = true;\n" +
                  content.substr(0, content.find('>') + 1) +
                  "template<AlwaysTrue auto... xs>" +
                  content.substr(content.find('>') + 1);
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(target->getSourceRange()), content);
      }
    } else {
      content = content.substr(0, content.find('>') + 1) +
                "template<Concept auto... xs>" +
                content.substr(content.find('>') + 1);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(target->getSourceRange()), content);
    }
  }