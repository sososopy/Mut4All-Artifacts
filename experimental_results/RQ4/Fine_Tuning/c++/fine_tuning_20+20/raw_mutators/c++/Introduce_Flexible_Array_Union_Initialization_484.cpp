//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Flexible_Array_Union_Initialization_484
 */ 
class MutatorFrontendAction_484 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(484)

private:
    class MutatorASTConsumer_484 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_484(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::RecordDecl *> cur_structs;
        std::vector<const clang::RecordDecl *> cur_unions;
    };
};

//source file
#include "../include/Introduce_Flexible_Array_Union_Initialization_484.h"

// ========================================================================================================
#define MUT484_OUTPUT 1

void MutatorFrontendAction_484::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::RecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (ST->isStruct() == false)
        return;
      if (ST->isCompleteDefinition() == false)
        return;
      auto fields = ST->fields();
      for (auto field : fields) {
        if (field->getType()->isIncompleteArrayType()) {
          cur_structs.push_back(ST);
          break;
        }
      }
    } else if (auto *UN = Result.Nodes.getNodeAs<clang::RecordDecl>("Unions")) {
      if (!UN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UN->getLocation()))
        return;
      if (UN->isUnion() == false)
        return;
      if (UN->isCompleteDefinition() == false)
        return;
      cur_unions.push_back(UN);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("Main")) {
      if (!DL)
        return;
      if (cur_structs.empty())
        return;
      if (cur_unions.empty()) {
        auto target = cur_structs[0];
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               target->getSourceRange());
        auto union_def = "union {\n" + content + " mut484;\n} mut484_u={.mut484={0}};\n";
        llvm::outs() << union_def;
        Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut484*/" + union_def);
      } else {
        auto target_struct = cur_structs[getrandom::getRandomIndex(
            cur_structs.size() - 1)];
        auto target_union =
            cur_unions[getrandom::getRandomIndex(cur_unions.size() - 1)];
        auto union_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                     target_union->getSourceRange());
        auto struct_name = target_struct->getNameAsString();
        auto union_name = target_union->getNameAsString();
        auto mutated_union_content = union_content;
        if (union_content.rfind('}') != string::npos) {
          mutated_union_content.insert(union_content.rfind('}'),
                                       struct_name + " mut484;\n");
        }
        llvm::outs() << mutated_union_content;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                target_union->getSourceRange()),
                            mutated_union_content);
        Rewrite.ReplaceText(target_union->getEndLoc(), 0,
                            "/*mut484*/ " + union_name + " mut484_u={.mut484={0}};\n");
      }
    }
  }

void MutatorFrontendAction_484::MutatorASTConsumer_484::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = recordDecl().bind("Structs");
    auto union_matcher = recordDecl().bind("Unions");
    auto main_matcher = translationUnitDecl().bind("Main");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(union_matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}