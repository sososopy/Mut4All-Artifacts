//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Constrained_Template_Class_459
 */ 
class MutatorFrontendAction_459 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(459)

private:
    class MutatorASTConsumer_459 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_459(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<const clang::TemplateDecl *> cur_templates;
        std::vector<const clang::TypeAliasDecl *> cur_alias;
    };
};

//source file
#include "../include/insert_constrained_template_class_459.h"

// ========================================================================================================
#define MUT459_OUTPUT 1

void MutatorFrontendAction_459::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isLambda())
      return;
    cur_classes.push_back(DL);
  } else if (auto *TP =
                 Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    cur_templates.push_back(TP);
  } else if (auto *AL =
                 Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
    if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   AL->getLocation()))
      return;
    cur_alias.push_back(AL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Insert")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isLambda())
      return;
    if (cur_alias.empty() && cur_templates.empty())
      return;
    int dice = getrandom::getRandomIndex(2);
    if (dice == 0 && cur_alias.empty())
      dice = 1;
    else if (dice == 1 && cur_templates.empty())
      dice = 0;
    string target_type = "";
    if (dice == 0) {
      size_t index = getrandom::getRandomIndex(cur_alias.size() - 1);
      target_type = cur_alias[index]->getNameAsString();
    } else if (dice == 1) {
      size_t index = getrandom::getRandomIndex(cur_templates.size() - 1);
      target_type = cur_templates[index]->getNameAsString();
    } else
      assert(false && "Mut459: Dice should have 2 possible values!");

    size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
    auto source_type = cur_classes[index]->getNameAsString();
    std::string ins = "template<typename T> class mut_459 {\n";
    ins += "public:\n";
    ins += "mut_459() requires std::is_same_v<T," + target_type + "> =default;\n";
    ins += "};\n";
    ins = "/*mut459*/" + ins;
    Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
  }
}
  
void MutatorFrontendAction_459::MutatorASTConsumer_459::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto template_matcher = templateDecl().bind("Templates");
  auto alias_matcher = typeAliasDecl().bind("TypeAlias");
  auto insert_matcher = cxxRecordDecl().bind("Insert");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(template_matcher, &callback);
  matchFinder.addMatcher(alias_matcher, &callback);
  matchFinder.addMatcher(insert_matcher, &callback);
  matchFinder.matchAST(Context);
}