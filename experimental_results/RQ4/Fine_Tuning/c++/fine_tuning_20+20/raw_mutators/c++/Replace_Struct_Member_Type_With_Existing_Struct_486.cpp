//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Struct_Member_Type_With_Existing_Struct_486
 */ 
class MutatorFrontendAction_486 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(486)

private:
    class MutatorASTConsumer_486 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_486(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_struct_member_type_with_existing_struct_486.h"

// ========================================================================================================
#define MUT486_OUTPUT 1

void MutatorFrontendAction_486::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
    if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                   ST->getLocation()))
      return;
    if (!ST->isStruct())
      return;
    cur_structs.push_back(ST);
  } else if (auto *FD =
                 Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    llvm::outs() << "find field\n";
    if (cur_structs.empty())
      return;
    int index = getrandom::getRandomIndex(cur_structs.size() - 1);
    auto target = cur_structs[index];
    auto target_name = target->getNameAsString();
    llvm::outs() << target_name << '\n';
    if (target_name == "")
      return;
    if (FD->getType()->isStructureType() ||
        FD->getType()->isClassType()) // cannot replace struct or class members
      return;
    auto field_name = FD->getNameAsString();
    auto field_type = FD->getType().getAsString();
    auto field_decl =
        stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
    llvm::outs() << field_decl << '\n';
    llvm::outs() << field_type << '\n';
    llvm::outs() << field_name << '\n';
    if (field_decl.find(field_type) != string::npos) {
      field_decl.replace(field_decl.find(field_type), field_type.size(),
                         target_name);
      llvm::outs() << field_decl << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(FD->getSourceRange()), field_decl);
    }
  }
}
  
void MutatorFrontendAction_486::MutatorASTConsumer_486::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Structs");
  auto field_matcher = fieldDecl().bind("FieldDecl");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(struct_matcher, &callback);
  matchFinder.addMatcher(field_matcher, &callback);
  matchFinder.matchAST(Context);
}