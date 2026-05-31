//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Template_With_Inheritance_467
 */ 
class MutatorFrontendAction_467 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(467)
    
    private:
      class MutatorASTConsumer_467 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_467(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<string> base_classes;
      };
    };

//source file
#include "../include/variadic_template_with_inheritance_467.h"

// ========================================================================================================
#define MUT467_OUTPUT 1

void MutatorFrontendAction_467::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isLambda())
      return;
    auto bases = DL->bases();
    for (auto base : bases) {
      auto base_type = base.getType();
      auto base_decl = base_type->getAsCXXRecordDecl();
      if (base_decl == nullptr)
        continue;
      if (base_decl->isCompleteDefinition() == false)
        continue;
      auto base_name = base_decl->getNameAsString();
      base_classes.push_back(base_name);
    }
  } else if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                 "Templates")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), TP->getSourceRange());
    if (content.find("...") == string::npos)
      return;
    if (content.find("using") == string::npos)
      return;
    if (content.find("::") == string::npos)
      return;
    if (content.find("...") == content.rfind("...")) {
      llvm::outs() << content << '\n';
      return;
    }
    if (base_classes.empty())
      return;
    auto base_name = base_classes[getrandom::getRandomIndex(base_classes.size() - 1)];
    llvm::outs() << base_name << '\n';
    string new_content = "/*mut467*/" + content + base_name;
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TP->getSourceRange()),
                        new_content);
  }
}

void MutatorFrontendAction_467::MutatorASTConsumer_467::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto template_matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}