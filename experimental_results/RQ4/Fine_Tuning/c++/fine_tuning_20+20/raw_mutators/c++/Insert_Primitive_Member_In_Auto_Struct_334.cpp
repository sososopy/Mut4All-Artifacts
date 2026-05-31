//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Primitive_Member_In_Auto_Struct_334
 */ 
class MutatorFrontendAction_334 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(334)

private:
    class MutatorASTConsumer_334 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_334(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_structs;
    };
};

//source file
#include "../include/insert_primitive_member_in_auto_struct_334.h"

// ========================================================================================================
#define MUT334_OUTPUT 1

void MutatorFrontendAction_334::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct() || ST->isLambda())
        return;
      if (!ST->isCompleteDefinition())
        return;
      auto fields = ST->fields();
      bool has_auto = false;
      for (auto field : fields) {
        if (field->getType()->isUndeducedType()) {
          has_auto = true;
        }
      }
      if (has_auto)
        cur_structs.push_back(ST);
    } else if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "TargetStructs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct() || ST->isLambda())
        return;
      if (!ST->isCompleteDefinition())
        return;
      auto name = ST->getNameAsString();
      if (name == "")
        return;
      for (auto target : cur_structs) {
        if (target->getNameAsString() == name) {
          auto fields = ST->fields();
          bool has_auto = false;
          for (auto field : fields) {
            if (field->getType()->isUndeducedType()) {
              has_auto = true;
            }
          }
          if (!has_auto)
            return;
          auto ins = "\n/*mut334*/int mut_334=0;\n";
          Rewrite.ReplaceText(ST->getEndLoc(), 0, ins);
        }
      }
    }
}
  
void MutatorFrontendAction_334::MutatorASTConsumer_334::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    auto target_struct_matcher = cxxRecordDecl().bind("TargetStructs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
    MatchFinder finder2;
    finder2.addMatcher(target_struct_matcher, &callback);
    finder2.matchAST(Context);
}