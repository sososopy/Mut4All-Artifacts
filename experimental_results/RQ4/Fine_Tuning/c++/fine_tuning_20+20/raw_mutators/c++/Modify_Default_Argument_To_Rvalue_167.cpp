//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Default_Argument_To_Rvalue_167
 */ 
class MutatorFrontendAction_167 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(167)

private:
    class MutatorASTConsumer_167 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_167(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> candidate_funcs;
    };
};

//source file
#include "../include/modify_default_argument_to_rvalue_167.h"

// ========================================================================================================
#define MUT167_OUTPUT 1

void MutatorFrontendAction_167::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (FD->isDependentContext())
        return;
      if (FD->isOverloadedOperator())
        return;
      if (FD->getDescribedFunctionTemplate() != nullptr)
        return;
      if (FD->getNumParams() == 0)
        return;
      if (FD->hasBody() == false)
        return;
      candidate_funcs.push_back(FD);
    } else if (auto *PD = Result.Nodes.getNodeAs<clang::ParmVarDecl>(
                   "DefaultParam")) {
      if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PD->getLocation()))
        return;
      if (PD->hasDefaultArg() == false)
        return;
      auto defarg = PD->getDefaultArg();
      if (defarg == nullptr)
        return;
      auto defarg_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                defarg->getSourceRange());
      llvm::outs() << defarg_str << '\n';
      auto type = PD->getType();
      if (type->isReferenceType() == false)
        return;
      auto ref_type = type->getAs<clang::ReferenceType>();
      if (ref_type->isRValueReferenceType() == false)
        return;
      auto new_type = ref_type->getPointeeType();
      std::string new_type_str = new_type.getAsString();
      llvm::outs() << new_type_str << '\n';
      auto new_defarg_str = std::move(defarg_str) + "/*mut167*/";
      Rewrite.ReplaceText(defarg->getBeginLoc(), 0,
                          "std::move(" + new_type_str + "(");
      Rewrite.ReplaceText(defarg->getEndLoc(), 0, "))");
    }
  }
  
void MutatorFrontendAction_167::MutatorASTConsumer_167::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Function");
    auto default_param_matcher =
        parmVarDecl(hasType(referenceType()), hasDefaultArgument())
            .bind("DefaultParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(default_param_matcher, &callback);
    matchFinder.matchAST(Context);
}