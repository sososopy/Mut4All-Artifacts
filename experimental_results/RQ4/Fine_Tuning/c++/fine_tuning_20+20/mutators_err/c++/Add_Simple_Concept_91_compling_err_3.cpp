//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Simple_Concept_91
 */ 
class MutatorFrontendAction_91 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(91)

private:
    class MutatorASTConsumer_91 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_91(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::ClassTemplateDecl *> class_templates;
        std::vector<const clang::FunctionTemplateDecl *> function_templates;
        std::vector<string> class_template_names;
        std::vector<string> function_template_names;
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Add_Simple_Concept_91.h"

// ========================================================================================================
#define MUT91_OUTPUT 1

void MutatorFrontendAction_91::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      if (CT->isThisDeclarationADefinition() == false)
        return;
      class_templates.push_back(CT);
      class_template_names.push_back(CT->getNameAsString());
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (FT->isThisDeclarationADefinition() == false)
        return;
      function_templates.push_back(FT);
      function_template_names.push_back(FT->getNameAsString());
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes")) { // If there is no template, try to find the
                                 // class that can be transformed into a
                                 // template
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      cur_classes.push_back(DL);
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "Functions")) { // If there is no template, try to find the
                                   // function that can be transformed into a
                                   // template
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (FT->isThisDeclarationADefinition() == false)
        return;
      if (FT->isTemplateInstantiation() || FT->isTemplated())
        return;
      if (FT->isLambda())
        return;
      if (FT->getNameAsString() == "")
        return;
      if (FT->getDescribedFunctionTemplate() != nullptr)
        return;
      if (FT->isOverloadedOperator())
        return;
      if (FT->getType().getAsString().find("decltype") != string::npos)
        return;
      if (FT->isVariadic())
        return;
      if (FT->isMain())
        return;
      if (FT->getKind() != Decl::Kind::Function)
        return;
      cur_classes.clear();
    } else if (auto *CT = Result.Nodes.getNodeAs<clang::ConceptDecl>(
                   "Concepts")) { // If there is already a concept, try to
                                  // insert it into the existing template
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      return;
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::Decl>("Decls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!class_templates.empty()) {
        llvm::outs() << "class template\n";
        size_t index = getrandom::getRandomIndex(class_templates.size() - 1);
        auto target = class_templates[index];
        auto params = target->getTemplateParameters();
        if (params->size() == 0)
          return;
        auto param = params->getParam(0);
        auto param_name = param->getName();
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               target->getSourceRange());
        auto insert_concept = "/*mut91*/concept mut_91=std::is_integral_v<" +
                              param_name.str() + ">;\n";
        content = insert_concept + content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(target->getSourceRange()), content);
        for (auto name : class_template_names) {
          auto matcher = cxxRecordDecl(hasName(name)).bind("Classes");
          auto cur_classes =
              getrandom::getRandomMatcher(matchFinder, matcher, *(Result.Context));
          for (auto cur_class : cur_classes) {
            auto cur_content = stringutils::rangetoStr(
                *(Result.SourceManager), cur_class->getSourceRange());
            llvm::outs() << cur_content << '\n';
            if (cur_content.find("mut_91") != string::npos)
              continue;
            auto new_content = stringutils::strReplace(
                cur_content, param_name.str(), "mut_91");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                    cur_class->getSourceRange()),
                                new_content);
          }
        }
      } else if (!function_templates.empty()) {
        llvm::outs() << "function template\n";
        size_t index = getrandom::getRandomIndex(function_templates.size() - 1);
        auto target = function_templates[index];
        auto params = target->getTemplateParameters();
        if (params->size() == 0)
          return;
        auto param = params->getParam(0);
        auto param_name = param->getName();
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               target->getSourceRange());
        auto insert_concept = "/*mut91*/concept mut_91=std::is_integral_v<" +
                              param_name.str() + ">;\n";
        content = insert_concept + content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(target->getSourceRange()), content);
        for (auto name : function_template_names) {
          auto matcher = functionDecl(hasName(name)).bind("Functions");
          auto cur_functions =
              getrandom::getRandomMatcher(matchFinder, matcher, *(Result.Context));
          for (auto cur_function : cur_functions) {
            auto cur_content = stringutils::rangetoStr(
                *(Result.SourceManager), cur_function->getSourceRange());
            if (cur_content.find("mut_91") != string::npos)
              continue;
            auto new_content = stringutils::strReplace(
                cur_content, param_name.str(), "mut_91");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                    cur_function->getSourceRange()),
                                new_content);
          }
        }
      } else if (!cur_classes.empty()) {
        llvm::outs() << "class\n";
        size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
        auto target = cur_classes[index];
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               target->getSourceRange());
        if (content.find("template") != string::npos)
          return;
        if (content.find("typename") != string::npos)
          return;
        if (content.find("class") != string::npos)
          return;
        auto new_content = "/*mut91*/template<typename T>\nconcept mut_91=std::is_integral_v<T>;\n"
                           "template<mut_91 T>\n" +
                           content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(target->getSourceRange()),
            new_content);
      } else {
        llvm::outs() << "function\n";
        auto matcher = functionDecl().bind("Functions");
        auto cur_functions =
            getrandom::getRandomMatcher(matchFinder, matcher, *(Result.Context));
        if (cur_functions.empty())
          return;
        size_t index = getrandom::getRandomIndex(cur_functions.size() - 1);
        auto target = cur_functions[index];
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               target->getSourceRange());
        if (content.find("template") != string::npos)
          return;
        if (content.find("typename") != string::npos)
          return;
        if (content.find("class") != string::npos)
          return;
        auto new_content =
            "/*mut91*/template<typename T>\nconcept mut_91=std::is_integral_v<T>;\n"
            "template<mut_91 T>\n" +
            content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(target->getSourceRange()),
            new_content);
      }
    }
}

void MutatorFrontendAction_91::MutatorASTConsumer_91::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_template_matcher = classTemplateDecl().bind("ClassTemplate");
    auto function_template_matcher =
        functionTemplateDecl().bind("FunctionTemplate");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto function_matcher = functionDecl().bind("Functions");
    auto concept_matcher = conceptDecl().bind("Concepts");
    auto decl_matcher = decl().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_template_matcher, &callback);
    matchFinder.addMatcher(function_template_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}