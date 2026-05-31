//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Struct_To_Anonymous_Struct_With_Incomplete_Alias_Template_371
 */ 
class MutatorFrontendAction_371 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(371)
    
    private:
      class MutatorASTConsumer_371 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_371(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::TypeAliasTemplateDecl *> alias_templates;
      };
    };

//source file
#include "../include/modify_struct_to_anonymous_struct_with_incomplete_alias_template_371.h"

// ========================================================================================================
#define MUT371_OUTPUT 1

void MutatorFrontendAction_371::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isLambda() || DL->isUnion() || DL->isClass())
      return;
    if (DL->isAnonymousStructOrUnion())
      return;
    if (DL->isNested())
      return;
    if (alias_templates.empty())
      return;
    auto alias_index = getrandom::getRandomIndex(alias_templates.size() - 1);
    auto alias_name = alias_templates[alias_index]->getNameAsString();
    auto struct_content = "/*mut371*/using " + alias_name + ";\n";
    llvm::outs() << struct_content;
    Rewrite.ReplaceText(DL->getLocation(), DL->getNameAsString().length(), "");
    Rewrite.ReplaceText(DL->getEndLoc(), 0, struct_content);
  } else if (auto *AL = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>(
                 "AliasTemplates")) {
    if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   AL->getLocation()))
      return;
    alias_templates.push_back(AL);
  }
}

void MutatorFrontendAction_371::MutatorASTConsumer_371::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Structs");
    auto alias_matcher = typeAliasTemplateDecl().bind("AliasTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(alias_matcher, &callback);
    matchFinder.matchAST(Context);
}