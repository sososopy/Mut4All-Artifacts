//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_Clause_Implementation_56
 */ 
class MutatorFrontendAction_56 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(56)

private:
    class MutatorASTConsumer_56 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_56(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/replace_requires_clause_implementation_56.h"

// ========================================================================================================
#define MUT56_OUTPUT 1

void MutatorFrontendAction_56::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (FT->getTemplatedDecl()->getBody() == nullptr)
        return;
      if (FT->getTemplatedDecl()->getType()->isDependentType() == false)
        return;
      if (FT->getTemplatedDecl()->getType()->isFunctionProtoType() == false)
        return;
      if (FT->getTemplatedDecl()
              ->getFunctionType()
              ->getAs<FunctionProtoType>()
              ->getTypeConstraint()
              ->isDependentType() == false)
        return;
      cur_templates.push_back(FT);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "Functions")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      if (FD->getBody() == nullptr)
        return;
      if (FD->getType()->isDependentType() == false)
        return;
      if (FD->getType()->isFunctionProtoType() == false)
        return;
      if (FD->getFunctionType()
              ->getAs<FunctionProtoType>()
              ->getTypeConstraint()
              ->isDependentType() == false)
        return;
      size_t index = getrandom::getRandomIndex(cur_templates.size() - 1);
      auto target = cur_templates[index];
      auto target_decl = target->getTemplatedDecl();
      auto target_type = target_decl->getFunctionType();
      auto target_constraint = target_type->getAs<FunctionProtoType>();
      auto target_constraint_type = target_constraint->getTypeConstraint();
      auto target_type_str = target_constraint_type.getAsString();
      llvm::outs() << target_type_str << '\n';
      auto FD_type = FD->getFunctionType();
      auto FD_constraint = FD_type->getAs<FunctionProtoType>();
      auto FD_constraint_type = FD_constraint->getTypeConstraint();
      auto FD_type_str = FD_constraint_type.getAsString();
      llvm::outs() << FD_type_str << '\n';
      if (target_type_str != FD_type_str)
        return;
      auto target_body = stringutils::rangetoStr(*(Result.SourceManager),
                                                 target_decl->getBody());
      llvm::outs() << target_body << '\n';
      Rewrite.ReplaceText(FD->getBody()->getSourceRange(), target_body);
    }
}
  
void MutatorFrontendAction_56::MutatorASTConsumer_56::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Templates");
    auto function_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.matchAST(Context);
}