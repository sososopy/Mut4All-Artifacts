//header file
#pragma once
#include "Mutator_base.h"

/**
 * Pointer_Dereference_And_Subtraction_In_Vector_Context_203
 */ 
class MutatorFrontendAction_203 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(203)

private:
    class MutatorASTConsumer_203 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_203(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Pointer_Dereference_And_Subtraction_In_Vector_Context_203.h"

// ========================================================================================================
#define MUT203_OUTPUT 1

void MutatorFrontendAction_203::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VectorDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (!VD->hasInit())
        return;

      auto type = VD->getType();
      if (!type->isVectorType())
        return;

      auto attr = VD->getAttrs();
      bool has_vector_size_attr = false;
      for (auto at : attr) {
        if (at->getKind() == clang::attr::Kind::VectorSize) {
          has_vector_size_attr = true;
          break;
        }
      }
      if (!has_vector_size_attr)
        return;

      auto ele_type = type->getAs<clang::VectorType>()->getElementType();
      if (!ele_type->isIntegerType())
        return;

      auto init = VD->getInit();
      auto init_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              init->getSourceRange());
      llvm::outs() << init_str << '\n';
      auto varname = VD->getNameAsString();
      llvm::outs() << varname << '\n';
      auto ele_type_str = ele_type.getAsString();
      llvm::outs() << ele_type_str << '\n';
      auto decl_str = ele_type_str + " t0 = *0;";
      auto new_init_str = init_str + " - t0";
      llvm::outs() << decl_str << '\n';
      llvm::outs() << new_init_str << '\n';
      Rewrite.ReplaceText(VD->getBeginLoc(), 0, "/*mut203*/" + decl_str);
      Rewrite.ReplaceText(init->getBeginLoc(), init_str.length(), new_init_str);
    }
}
  
void MutatorFrontendAction_203::MutatorASTConsumer_203::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl().bind("VectorDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}