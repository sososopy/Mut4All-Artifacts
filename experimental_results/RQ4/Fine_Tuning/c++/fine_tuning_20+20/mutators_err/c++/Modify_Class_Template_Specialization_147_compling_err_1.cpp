//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Class_Template_Specialization_147
 */ 
class MutatorFrontendAction_147 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(147)

private:
    class MutatorASTConsumer_147 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_147(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplatePartialSpecializationDecl *>
            cur_partials;
    };
};

//source file
#include "../include/modify_class_template_specialization_147.h"

// ========================================================================================================
#define MUT147_OUTPUT 1

void MutatorFrontendAction_147::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto specs = MT->specializations();
      for (auto spec : specs) {
        if (spec->isPartialSpecialization()) {
          llvm::outs() << "Partial specialization detected.\n";
          cur_partials.push_back(
              llvm::dyn_cast<ClassTemplatePartialSpecializationDecl>(spec));
        }
      }
      cur_templates.push_back(MT);
    } else if (auto *PS = Result.Nodes.getNodeAs<
                           clang::ClassTemplatePartialSpecializationDecl>(
                   "Partials")) {
      if (!PS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PS->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             PS->getSourceRange());
      llvm::outs() << content << '\n';
      auto params = PS->getTemplateParameters();
      std::vector<string> param_names;
      for (auto param : *params) {
        param_names.push_back(param->getNameAsString());
      }
      llvm::outs() << "param size: " << param_names.size() << '\n';
      int dice = getrandom::getRandomIndex(2);
      if (dice == 0) {
        llvm::outs() << "Mut147: Change the partial specialization.\n";
        int param_num = getrandom::getRandomIndex(param_names.size() - 1) + 1;
        llvm::outs() << "param num: " << param_num << '\n';
        std::vector<int> param_used(param_names.size(), 0);
        for (int i = 0; i < param_num; ++i) {
          int index = getrandom::getRandomIndex(param_names.size() - 1);
          param_used[index] = 1;
        }
        string new_partial = "template<";
        bool first = true;
        for (int i = 0; i < param_names.size(); ++i) {
          if (param_used[i]) {
            if (first)
              first = false;
            else
              new_partial += ',';
            new_partial += "class " + param_names[i];
          }
        }
        new_partial += ">\n";
        llvm::outs() << new_partial << '\n';
        auto pos = content.find('>');
        if (pos != string::npos) {
          content.replace(0, pos + 1, new_partial);
        }
      } else if (dice == 1) {
        llvm::outs() << "Mut147: Remove the partial specialization.\n";
        content = "";
      } else
        assert(false && "Mut147: Dice should have 2 possible values!");
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(PS->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::Decl>("Decls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isImplicit())
        return;
      if (DL->getDeclKind() != Decl::Kind::CXXRecord &&
          DL->getDeclKind() != Decl::Kind::Function &&
          DL->getDeclKind() != Decl::Kind::Var)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (DL->getDeclKind() == Decl::Kind::CXXRecord) {
        if (content.find("template") == string::npos)
          return;
      } else if (DL->getDeclKind() == Decl::Kind::Function) {
        if (content.find('<') == string::npos)
          return;
      } else if (DL->getDeclKind() == Decl::Kind::Var) {
        if (content.find('<') == string::npos)
          return;
      }
      if (content.find("mut147") != string::npos)
        return;
      int dice = getrandom::getRandomIndex(2);
      if (dice == 0) {
        llvm::outs() << "Mut147: Add a new partial specialization.\n";
        if (cur_templates.empty())
          return;
        auto target = cur_templates[getrandom::getRandomIndex(
            cur_templates.size() - 1)];
        auto params = target->getTemplateParameters();
        std::vector<string> param_names;
        for (auto param : *params) {
          param_names.push_back(param->getNameAsString());
        }
        llvm::outs() << "param size: " << param_names.size() << '\n';
        int param_num = getrandom::getRandomIndex(param_names.size() - 1) + 1;
        llvm::outs() << "param num: " << param_num << '\n';
        std::vector<int> param_used(param_names.size(), 0);
        for (int i = 0; i < param_num; ++i) {
          int index = getrandom::getRandomIndex(param_names.size() - 1);
          param_used[index] = 1;
        }
        string new_partial = "template<";
        bool first = true;
        for (int i = 0; i < param_names.size(); ++i) {
          if (param_used[i]) {
            if (first)
              first = false;
            else
              new_partial += ',';
            new_partial += "class " + param_names[i];
          }
        }
        new_partial += ">\n";
        new_partial += "struct " + target->getNameAsString() + '<';
        first = true;
        for (int i = 0; i < param_names.size(); ++i) {
          if (param_used[i]) {
            if (first)
              first = false;
            else
              new_partial += ',';
            new_partial += param_names[i];
          } else {
            if (first)
              first = false;
            else
              new_partial += ',';
            new_partial += "int";
          }
        }
        new_partial += "> {};\n";
        llvm::outs() << new_partial << '\n';
        Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut147*/" + new_partial);
      } else if (dice == 1) {
        llvm::outs() << "Mut147: Add a new partial specialization.\n";
        if (cur_partials.empty())
          return;
        auto target =
            cur_partials[getrandom::getRandomIndex(cur_partials.size() - 1)];
        auto params = target->getTemplateParameters();
        std::vector<string> param_names;
        for (auto param : *params) {
          param_names.push_back(param->getNameAsString());
        }
        llvm::outs() << "param size: " << param_names.size() << '\n';
        int param_num = getrandom::getRandomIndex(param_names.size() - 1) + 1;
        llvm::outs() << "param num: " << param_num << '\n';
        std::vector<int> param_used(param_names.size(), 0);
        for (int i = 0; i < param_num; ++i) {
          int index = getrandom::getRandomIndex(param_names.size() - 1);
          param_used[index] = 1;
        }
        string new_partial = "template<";
        bool first = true;
        for (int i = 0; i < param_names.size(); ++i) {
          if (param_used[i]) {
            if (first)
              first = false;
            else
              new_partial += ',';
            new_partial += "class " + param_names[i];
          }
        }
        new_partial += ">\n";
        new_partial += "struct " + target->getNameAsString() + '<';
        first = true;
        for (int i = 0; i < param_names.size(); ++i) {
          if (param_used[i]) {
            if (first)
              first = false;
            else
              new_partial += ',';
            new_partial += param_names[i];
          } else {
            if (first)
              first = false;
            else
              new_partial += ',';
            new_partial += "int";
          }
        }
        new_partial += "> {};\n";
        llvm::outs() << new_partial << '\n';
        Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut147*/" + new_partial);
      } else
        assert(false && "Mut147: Dice should have 2 possible values!");
    }
}
  
void MutatorFrontendAction_147::MutatorASTConsumer_147::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto template_matcher = classTemplateDecl().bind("Templates");
    auto partial_matcher =
        classTemplatePartialSpecializationDecl().bind("Partials");
    auto decl_matcher = decl().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(partial_matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}