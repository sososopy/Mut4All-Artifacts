//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Enum_Cross_Mutation_320
 */ 
class MutatorFrontendAction_320 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(320)

private:
    class MutatorASTConsumer_320 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_320(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Struct_Enum_Cross_Mutation_320.h"

// ========================================================================================================
#define MUT320_OUTPUT 1

void MutatorFrontendAction_320::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStruct() == false)
        return;
      cur_structs.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (cur_structs.size() == 0)
        return;
      auto target = cur_structs.back();
      if (DL->getParent() != target)
        return;
      auto enums = DL->enumerators();
      std::vector<std::string> enum_names;
      for (auto e : enums) {
        enum_names.push_back(e->getNameAsString());
      }
      auto fields = target->fields();
      std::vector<std::string> field_names;
      for (auto f : fields) {
        field_names.push_back(f->getNameAsString());
      }
      std::string replacement = "/*mut320*/";
      if (field_names.size() == 0) {
        replacement += "mut320_undefined";
      } else {
        replacement += field_names[getrandom::getRandomIndex(field_names.size() - 1)];
      }
      replacement += ", ";
      replacement += "mut320_undefined";
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_320::MutatorASTConsumer_320::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    auto enum_matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.matchAST(Context);
}