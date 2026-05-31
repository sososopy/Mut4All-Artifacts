//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_With_Base_And_Reference_Member_To_Variant_82
 */ 
class MutatorFrontendAction_82 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(82)

private:
    class MutatorASTConsumer_82 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_82(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Struct_With_Base_And_Reference_Member_To_Variant_82.h"

// ========================================================================================================
#define MUT82_OUTPUT 1

void MutatorFrontendAction_82::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if ((!ST->isStruct() && !ST->isClass()) || ST->isLambda())
        return;
      if (!ST->isCompleteDefinition())
        return;
      if (ST->getNumBases() == 0)
        return;
      auto fields = ST->fields();
      bool has_ref = false;
      for (auto field : fields) {
        if (field->getType()->isReferenceType())
          has_ref = true;
      }
      if (has_ref == false)
        return;
      cur_structs.push_back(ST);
    } else if (auto *VT =
                   Result.Nodes.getNodeAs<clang::VarDecl>("Variants")) {
      if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VT->getLocation()))
        return;
      auto vartype = VT->getType();
      if (vartype->isStructureOrClassType() == false)
        return;
      auto decl = vartype->getAsCXXRecordDecl();
      if (decl->getNameAsString() != "variant")
        return;
      auto init = VT->getInit();
      if (init == nullptr)
        return;
      auto init_type = init->getType();
      llvm::outs() << init_type.getAsString() << '\n';
      for (std::size_t j = 0; j < cur_structs.size(); ++j) {
        if (init_type == cur_structs[j]->getType())
          return;
      }
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             VT->getSourceRange());
      content = "/*mut82*/" + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_82::MutatorASTConsumer_82::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    auto variant_matcher = varDecl().bind("Variants");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(variant_matcher, &callback);
    matchFinder.matchAST(Context);
}