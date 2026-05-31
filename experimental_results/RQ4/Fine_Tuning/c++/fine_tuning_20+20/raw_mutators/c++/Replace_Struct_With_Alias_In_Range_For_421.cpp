//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Struct_With_Alias_In_Range_For_421
 */ 
class MutatorFrontendAction_421 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(421)

private:
    class MutatorASTConsumer_421 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_421(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeAliasDecl *> alias_names;
        std::vector<const clang::TypedefNameDecl *> typedef_names;
    };
};

//source file
#include "../include/replace_struct_with_alias_in_range_for_421.h"

// ========================================================================================================
#define MUT421_OUTPUT 1

void MutatorFrontendAction_421::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      alias_names.push_back(DL);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::TypedefNameDecl>("Typedef")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      typedef_names.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "StructInFor")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isStruct())
        return;
      if (DL->getNameAsString() == "")
        return;
      if (alias_names.empty() && typedef_names.empty())
        return;
      int dice = getrandom::getRandomIndex(1);
      if (!alias_names.empty() && (dice == 0 || typedef_names.empty())) {
        int index = getrandom::getRandomIndex(alias_names.size() - 1);
        auto name = alias_names[index]->getNameAsString();
        llvm::outs() << name << '\n';
        Rewrite.ReplaceText(DL->getLocation(), name);
      } else if (!typedef_names.empty() &&
                 (dice == 1 || alias_names.empty())) {
        int index = getrandom::getRandomIndex(typedef_names.size() - 1);
        auto name = typedef_names[index]->getNameAsString();
        llvm::outs() << name << '\n';
        Rewrite.ReplaceText(DL->getLocation(), name);
      }
    }
}
  
void MutatorFrontendAction_421::MutatorASTConsumer_421::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto alias_matcher = typeAliasDecl().bind("Alias");
    auto typedef_matcher = typedefNameDecl().bind("Typedef");
    auto struct_matcher = cxxRecordDecl(hasParent(cxxForRangeStmt()))
                              .bind("StructInFor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(alias_matcher, &callback);
    matchFinder.addMatcher(typedef_matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
}