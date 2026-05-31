//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Return_Type_With_Auto_Variable_Type_27
 */ 
class MutatorFrontendAction_27 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(27)

private:
    class MutatorASTConsumer_27 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_27(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::VarDecl *> cur_autos;
    };
};

//source file
#include "../include/replace_function_return_type_with_auto_variable_type_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (FD->isDefaulted() || FD->isDeleted())
        return;
      if (!FD->doesThisDeclarationHaveABody())
        return;
      auto rettype = FD->getReturnType();
      if (rettype->isUndeducedType())
        return;
      if (rettype->isDependentType())
        return;
      llvm::outs() << rettype.getAsString() << '\n';
      if (cur_autos.empty())
        return;
      auto index = getrandom::getRandomIndex(cur_autos.size() - 1);
      auto target = cur_autos[index];
      auto target_type = target->getType();
      if (target_type->isUndeducedType())
        return;
      if (target_type->isDependentType())
        return;
      if (target_type.getAsString() == rettype.getAsString())
        return;
      auto retstr = target_type.getAsString();
      llvm::outs() << retstr << '\n';
      Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), retstr);
    } else if (auto *VD =
                   Result.Nodes.getNodeAs<clang::VarDecl>("AutoVariables")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType()->isUndeducedType())
        return;
      if (VD->getType()->isDependentType())
        return;
      cur_autos.push_back(VD);
    }
}
  
void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto function_matcher = functionDecl().bind("Functions");
    auto auto_variable_matcher =
        varDecl(hasType(autoType())).bind("AutoVariables");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(auto_variable_matcher, &callback);
    matchFinder.matchAST(Context);
}