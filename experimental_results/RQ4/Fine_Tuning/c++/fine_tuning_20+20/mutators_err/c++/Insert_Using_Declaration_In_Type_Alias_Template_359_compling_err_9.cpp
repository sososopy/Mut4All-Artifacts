//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Using_Declaration_In_Type_Alias_Template_359
 */ 
class MutatorFrontendAction_359 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(359)
    
    private:
      class MutatorASTConsumer_359 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_359(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::TypeAliasTemplateDecl *> talist;
          std::vector<const clang::VarDecl *> valist;
      };
    };

//source file
#include "../include/insert_using_declaration_in_type_alias_template_359"

// ========================================================================================================
#define MUT359_OUTPUT 1

void MutatorFrontendAction_359::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TypeAlias")) {
    if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TA->getLocation()))
      return;
    talist.push_back(TA);
  } else if (auto *VA = Result.Nodes.getNodeAs<clang::VarDecl>("Var")) {
    if (!VA || !Result.Context->getSourceManager().isWrittenInMainFile(
                   VA->getLocation()))
      return;
    valist.push_back(VA);
  } else if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>(
                 "TypeAliasTemplate")) {
    if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TA->getLocation()))
      return;
    auto name = TA->getNameAsString();
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), TA->getSourceRange());
    llvm::outs() << content << '\n';
    if (content.rfind('}') != string::npos) {
      string ins = "\n\t/*mut359*/struct foo{int ";
      if (!valist.empty()) {
        auto varname = valist[0]->getNameAsString();
        ins += varname + " = 0;};\n";
      } else {
        ins += "foo = 0;};\n";
      }
      content.insert(content.rfind('}'), ins);
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TA->getSourceRange()),
                        content);
  } else if (auto *VA = Result.Nodes.getNodeAs<clang::VarDecl>("VarAlias")) {
    if (!VA || !Result.Context->getSourceManager().isWrittenInMainFile(
                   VA->getLocation()))
      return;
    auto name = VA->getNameAsString();
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), VA->getSourceRange());
    llvm::outs() << content << '\n';
    if (content.rfind('(') != string::npos) {
      content.insert(content.rfind('('), name);
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(VA->getSourceRange()),
                        content);
  } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                 "MainFunction")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (!FD->hasBody())
      return;
    if (FD->getNameAsString() != "main")
      return;
    if (!talist.empty()) {
      auto aliasname = talist[0]->getNameAsString();
      string ins = "/*mut359*/" + aliasname + " mut_359;\n";
      Rewrite.ReplaceText(FD->getBeginLoc(), 0, ins);
    }
  }
}

void MutatorFrontendAction_359::MutatorASTConsumer_359::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = typeAliasTemplateDecl().bind("TypeAliasTemplate");
    auto var_matcher = varDecl().bind("VarAlias");
    auto typealias_matcher = typeAliasTemplateDecl().bind("TypeAlias");
    auto variable_matcher = varDecl().bind("Var");
    auto main_matcher = functionDecl().bind("MainFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.addMatcher(typealias_matcher, &callback);
    matchFinder.addMatcher(variable_matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}