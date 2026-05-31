//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_With_Incomplete_Type_180
 */ 
class MutatorFrontendAction_180 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(180)

private:
    class MutatorASTConsumer_180 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_180(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::EnumDecl *> enums;
    };
};

//source file
#include "../include/Enum_With_Incomplete_Type_180.h"

// ========================================================================================================
#define MUT180_OUTPUT 1

void MutatorFrontendAction_180::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
    if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                   ED->getLocation()))
      return;
    enums.push_back(ED);
  } else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("Types")) {
    if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TD->getLocation()))
      return;
    if (!TD->isCompleteDefinition())
      return;
    auto type_name = TD->getNameAsString();
    if (type_name == "")
      return;
    for (auto ed : enums) {
      auto ed_type = ed->getIntegerType();
      if (!ed_type->isIncompleteType())
        continue;
      auto ed_type_name = ed_type.getAsString();
      if (ed_type_name != type_name)
        continue;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TD->getSourceRange());
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()),
                          "/*mut180*/" + content);
    }
  }
}
  
void MutatorFrontendAction_180::MutatorASTConsumer_180::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto enum_matcher = enumDecl().bind("Enums");
  auto type_matcher =
      typeDecl(anyOf(classTemplateSpecializationDecl(), cxxRecordDecl(),
                     enumDecl(), typedefDecl(), typeAliasDecl()))
          .bind("Types");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(enum_matcher, &callback);
  matchFinder.addMatcher(type_matcher, &callback);
  matchFinder.matchAST(Context);
}