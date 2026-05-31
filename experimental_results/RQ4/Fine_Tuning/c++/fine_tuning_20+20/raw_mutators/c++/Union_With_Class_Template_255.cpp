//header file
#pragma once
#include "Mutator_base.h"

/**
 * Union_With_Class_Template_255
 */ 
class MutatorFrontendAction_255 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(255)

private:
    class MutatorASTConsumer_255 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_255(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> cur_templates;
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<const clang::CXXRecordDecl *> cur_unions;
        std::vector<const clang::FunctionDecl *> cur_functions;
        std::vector<const clang::VarDecl *> cur_vars;
    };
};

//source file
#include "../include/union_with_class_template_255.h"

// ========================================================================================================
#define MUT255_OUTPUT 1

void MutatorFrontendAction_255::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    cur_templates.push_back(DL);
  } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if (!CL->isCompleteDefinition())
      return;
    if (CL->isUnion())
      cur_unions.push_back(CL);
    else
      cur_classes.push_back(CL);
  } else if (auto *FL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FL->getLocation()))
      return;
    cur_functions.push_back(FL);
  } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vars")) {
    if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   VD->getLocation()))
      return;
    cur_vars.push_back(VD);
  } else if (auto *TL = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                 "TranslationUnit")) {
    if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TL->getBeginLoc()))
      return;
    for (auto DL : TL->decls()) {
      if (DL->getIdentifierNamespace() ==
          clang::Decl::IdentifierNamespace::IDNS_Tag)
        if (DL->getKind() == clang::Decl::Kind::CXXRecord) {
          auto CL = dyn_cast<clang::CXXRecordDecl>(DL);
          if (!CL->isCompleteDefinition())
            continue;
          if (CL->isUnion())
            cur_unions.push_back(CL);
          else
            cur_classes.push_back(CL);
        }
      if (DL->getIdentifierNamespace() ==
          clang::Decl::IdentifierNamespace::IDNS_Ordinary)
        if (DL->getKind() == clang::Decl::Kind::Function) {
          auto FL = dyn_cast<clang::FunctionDecl>(DL);
          cur_functions.push_back(FL);
        } else if (DL->getKind() == clang::Decl::Kind::Var) {
          auto VL = dyn_cast<clang::VarDecl>(DL);
          cur_vars.push_back(VL);
        }
      if (DL->getIdentifierNamespace() ==
          clang::Decl::IdentifierNamespace::IDNS_OrdinaryFriend) {
        if (DL->getKind() == clang::Decl::Kind::ClassTemplate) {
          auto TL = dyn_cast<clang::ClassTemplateDecl>(DL);
          cur_templates.push_back(TL);
        }
      }
    }
  }
  if (!cur_templates.empty()) {
    auto index = getrandom::getRandomIndex(cur_templates.size() - 1);
    auto target_template = cur_templates[index];
    auto temp_name = target_template->getNameAsString();
    auto temp_params = target_template->getTemplateParameters();
    string ins_params = "";
    for (auto param : *temp_params) {
      if (param->getKind() == clang::Decl::Kind::TemplateTypeParm) {
        ins_params += "int";
      } else if (param->getKind() == clang::Decl::Kind::NonTypeTemplateParm) {
        auto NTP = dyn_cast<clang::NonTypeTemplateParmDecl>(param);
        auto type = NTP->getType();
        ins_params += type.getAsString();
      } else if (param->getKind() == clang::Decl::Kind::TemplateTemplateParm) {
        auto TTP = dyn_cast<clang::TemplateTemplateParmDecl>(param);
        auto temp = TTP->getTemplateParameters();
        string inner_params = "";
        for (auto inner : *temp) {
          if (inner->getKind() == clang::Decl::Kind::TemplateTypeParm) {
            inner_params += "typename";
          } else if (inner->getKind() == clang::Decl::Kind::NonTypeTemplateParm) {
            auto NTP = dyn_cast<clang::NonTypeTemplateParmDecl>(inner);
            auto type = NTP->getType();
            inner_params += type.getAsString();
          } else if (inner->getKind() ==
                     clang::Decl::Kind::TemplateTemplateParm) {
            inner_params += "template<typename> typename";
          }
          inner_params += ",";
        }
        if (!inner_params.empty())
          inner_params.pop_back();
        ins_params += "template<" + inner_params + "> typename";
      }
      ins_params += ",";
    }
    if (!ins_params.empty())
      ins_params.pop_back();
    llvm::outs() << ins_params << '\n';
    if (!cur_unions.empty()) {
      auto index = getrandom::getRandomIndex(cur_unions.size() - 1);
      auto target_union = cur_unions[index];
      auto union_name = target_union->getNameAsString();
      auto union_content = stringutils::rangetoStr(
          *(Result.SourceManager), target_union->getSourceRange());
      if (union_content.find(temp_name) != string::npos)
        return;
      auto ins = temp_name + "<" + ins_params + "> mut255;";
      union_content.insert(union_content.rfind('}'), ins);
      llvm::outs() << union_content << '\n';
      Rewrite.ReplaceText(
          clang::CharSourceRange::getTokenRange(target_union->getSourceRange()),
          union_content);
      if (!cur_functions.empty()) {
        auto index = getrandom::getRandomIndex(cur_functions.size() - 1);
        auto target_function = cur_functions[index];
        if (target_function->isDefined() == false)
          return;
        auto func_content = stringutils::rangetoStr(
            *(Result.SourceManager), target_function->getSourceRange());
        func_content.insert(func_content.find('{') + 1,
                            "\n/*mut255*/" + union_name + " mut255;\n");
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(
                                target_function->getSourceRange()),
                            func_content);
      } else if (!cur_vars.empty()) {
        auto index = getrandom::getRandomIndex(cur_vars.size() - 1);
        auto target_var = cur_vars[index];
        auto var_content = stringutils::rangetoStr(
            *(Result.SourceManager), target_var->getSourceRange());
        var_content += "\n/*mut255*/" + union_name + " mut255;\n";
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(
                                target_var->getSourceRange()),
                            var_content);
      }
    } else {
      if (!cur_classes.empty()) {
        auto index = getrandom::getRandomIndex(cur_classes.size() - 1);
        auto target_class = cur_classes[index];
        auto class_content = stringutils::rangetoStr(
            *(Result.SourceManager), target_class->getSourceRange());
        class_content.insert(class_content.rfind('}'),
                             "\n/*mut255*/union mut255{\n" + temp_name + "<" +
                                 ins_params + "> mut255;\n};\n");
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(
                                target_class->getSourceRange()),
                            class_content);
      } else if (!cur_functions.empty()) {
        auto index = getrandom::getRandomIndex(cur_functions.size() - 1);
        auto target_function = cur_functions[index];
        if (target_function->isDefined() == false)
          return;
        auto func_content = stringutils::rangetoStr(
            *(Result.SourceManager), target_function->getSourceRange());
        func_content.insert(func_content.find('{') + 1,
                            "\n/*mut255*/union mut255{\n" + temp_name + "<" +
                                ins_params + "> mut255;\n};\n");
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(
                                target_function->getSourceRange()),
                            func_content);
      } else if (!cur_vars.empty()) {
        auto index = getrandom::getRandomIndex(cur_vars.size() - 1);
        auto target_var = cur_vars[index];
        auto var_content = stringutils::rangetoStr(
            *(Result.SourceManager), target_var->getSourceRange());
        var_content += "\n/*mut255*/union mut255{\n" + temp_name + "<" +
                       ins_params + "> mut255;\n};\n";
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(
                                target_var->getSourceRange()),
                            var_content);
      }
    }
  }
}

void MutatorFrontendAction_255::MutatorASTConsumer_255::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto func_matcher = functionDecl().bind("Functions");
    auto var_matcher = varDecl().bind("Vars");
    auto translationunit_matcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.addMatcher(translationunit_matcher, &callback);
    matchFinder.matchAST(Context);
}