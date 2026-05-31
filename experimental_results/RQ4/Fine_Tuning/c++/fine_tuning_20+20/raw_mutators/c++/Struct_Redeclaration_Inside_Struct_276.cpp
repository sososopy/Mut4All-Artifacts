//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Redeclaration_Inside_Struct_276
 */ 
class MutatorFrontendAction_276 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(276)
    
    private:
      class MutatorASTConsumer_276 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_276(Rewriter &R) : TheRewriter(R) {}
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
          const clang::CXXRecordDecl *cur_struct = nullptr;
          const clang::CXXRecordDecl *inner_struct = nullptr;
      };
    };

//source file
#include "../include/struct_redeclaration_inside_struct_276.h"

// ========================================================================================================
#define MUT276_OUTPUT 1

void MutatorFrontendAction_276::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isStruct())
      return;
    if (!DL->isCompleteDefinition())
      return;
    cur_struct = DL;
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "InnerStruct")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isStruct())
      return;
    if (!DL->isCompleteDefinition())
      return;
    if (cur_struct == nullptr)
      return;
    if (DL->getBeginLoc() < cur_struct->getBeginLoc() ||
        DL->getEndLoc() > cur_struct->getEndLoc())
      return;
    inner_struct = DL;
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "StructToReplace")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isStruct())
      return;
    if (!DL->isCompleteDefinition())
      return;
    if (inner_struct == nullptr)
      return;
    if (DL->getBeginLoc() != inner_struct->getBeginLoc())
      return;
    if (DL->getEndLoc() != inner_struct->getEndLoc())
      return;
    auto name = DL->getNameAsString();
    // llvm::outs()<<"name:"<<name<<'\n';
    auto rep = "/*mut276*/struct " + name + ";";
    Rewrite.ReplaceText(DL->getBeginLoc(), DL->getEndLoc(), rep);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "StructToInsert")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isStruct())
      return;
    if (!DL->isCompleteDefinition())
      return;
    if (inner_struct == nullptr)
      return;
    if (DL->getBeginLoc() != inner_struct->getBeginLoc())
      return;
    if (DL->getEndLoc() != inner_struct->getEndLoc())
      return;
    auto def =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    Rewrite.ReplaceText(cur_struct->getEndLoc(), 0, "/*mut276*/" + def);
  }
}

void MutatorFrontendAction_276::MutatorASTConsumer_276::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl(isStruct()).bind("Struct");
    auto inner_struct_matcher = cxxRecordDecl(isStruct()).bind("InnerStruct");
    auto replace_matcher = cxxRecordDecl(isStruct()).bind("StructToReplace");
    auto insert_matcher = cxxRecordDecl(isStruct()).bind("StructToInsert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(inner_struct_matcher, &callback);
    matchFinder.addMatcher(replace_matcher, &callback);
    matchFinder.addMatcher(insert_matcher, &callback);
    matchFinder.matchAST(Context);
}