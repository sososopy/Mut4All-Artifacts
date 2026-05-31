//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Enum_Assignment_Statement_279
 */ 
class MutatorFrontendAction_279 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(279)

private:
    class MutatorASTConsumer_279 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_279(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::EnumDecl *> cur_enums;
    };
};

//source file
#include "../include/insert_enum_assignment_statement_279.h"

// ========================================================================================================
#define MUT279_OUTPUT 1

void MutatorFrontendAction_279::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
    if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                   ED->getLocation()))
      return;
    cur_enums.push_back(ED);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (!DL->isCompleteDefinition())
      return;
    if (cur_enums.empty())
      return;
    size_t index = getrandom::getRandomIndex(cur_enums.size() - 1);
    auto target = cur_enums[index];
    auto targetname = target->getNameAsString();
    auto enums = target->enumerators();
    std::set<string> enum_names;
    for (auto en : enums)
      enum_names.insert(en->getNameAsString());
    std::vector<string> all_names;
    for (auto name : stringutils::all_enum_names) {
      if (enum_names.find(name) == enum_names.end())
        all_names.push_back(name);
    }
    if (all_names.empty())
      return;
    size_t choice = getrandom::getRandomIndex(all_names.size() - 1);
    string ins = "/*mut279*/" + targetname + "=" + all_names[choice] + ";";
    Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
  }
}
  
void MutatorFrontendAction_279::MutatorASTConsumer_279::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto enum_matcher = enumDecl().bind("Enums");
  auto class_matcher = cxxRecordDecl().bind("Classes");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(enum_matcher, &callback);
  matchFinder.addMatcher(class_matcher, &callback);
  matchFinder.matchAST(Context);
}