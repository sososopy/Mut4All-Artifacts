//header file
#pragma once
#include "Mutator_base.h"

/**
 * Switch_To_Type_Alias_In_Template_Parameter_427
 */ 
class MutatorFrontendAction_427 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(427)

private:
    class MutatorASTConsumer_427 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_427(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TypeAliasDecl *alias_decl = nullptr;
    };
};

//source file
#include "../include/Switch_To_Type_Alias_In_Template_Parameter_427.h"

// ========================================================================================================
#define MUT427_OUTPUT 1

void MutatorFrontendAction_427::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("AliasDecl")) {
    if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   AL->getBeginLoc()))
      return;
    alias_decl = AL;
  } else if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateDecl>(
                 "TemplateDecl")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getBeginLoc()))
      return;
    if (alias_decl == nullptr)
      return;
    auto alias_name = alias_decl->getNameAsString();
    if (auto *CT = dyn_cast<ClassTemplateDecl>(TP)) {
      auto params = CT->getTemplateParameters();
      for (auto param : *params) {
        if (auto *TP = dyn_cast<TemplateTypeParmDecl>(param)) {
          if (TP->wasDeclaredWithTypename()) {
            auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TP->getSourceRange());
            llvm::outs() << content << '\n';
            auto new_content = content;
            stringutils::replaceFirst(new_content, "typename", alias_name);
            llvm::outs() << new_content << '\n';
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(TP->getSourceRange()),
                new_content);
          }
        }
      }
    }
  } else if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>(
                 "TemplateTypeParm")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getBeginLoc()))
      return;
    llvm::outs() << TP->getNameAsString() << '\n';
    if (TP->wasDeclaredWithTypename()) {
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TP->getSourceRange());
      llvm::outs() << content << '\n';
      if (alias_decl == nullptr) {
        auto alias_name = "Mut427";
        auto alias_content = "using " + alias_name + " = int;\n";
        Rewrite.ReplaceText(TP->getBeginLoc(), 0, "/*mut427*/" + alias_content);
        auto new_content = content;
        stringutils::replaceFirst(new_content, "typename", alias_name);
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(TP->getSourceRange()), new_content);
      } else {
        auto alias_name = alias_decl->getNameAsString();
        auto new_content = content;
        stringutils::replaceFirst(new_content, "typename", alias_name);
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(TP->getSourceRange()), new_content);
      }
    }
  }
}
  
void MutatorFrontendAction_427::MutatorASTConsumer_427::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto alias_matcher = typeAliasDecl().bind("AliasDecl");
    auto template_matcher = templateDecl().bind("TemplateDecl");
    auto type_param_matcher = templateTypeParmDecl().bind("TemplateTypeParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(alias_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(type_param_matcher, &callback);
    matchFinder.matchAST(Context);
}