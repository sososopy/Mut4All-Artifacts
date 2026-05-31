//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Member_Init_With_Function_Call_465
 */ 
class MutatorFrontendAction_465 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(465)

private:
    class MutatorASTConsumer_465 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_465(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Static_Member_Init_With_Function_Call_465.h"

// ========================================================================================================
#define MUT465_OUTPUT 1

void MutatorFrontendAction_465::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VT = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplates")) {
      if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VT->getLocation()))
        return;
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                VT->getSourceRange());
      auto init_expr = VT->getTemplatedDecl()->getInit();
      auto init_expr_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                   init_expr->getSourceRange());
      llvm::outs() << init_expr_str << '\n';
      auto pos = init_expr_str.find("::");
      if (pos == std::string::npos)
        return;
      auto class_name = init_expr_str.substr(0, pos);
      auto member_name = init_expr_str.substr(pos + 2);
      llvm::outs() << class_name << ' ' << member_name << '\n';
      auto new_init_expr_str = class_name + "::get" +
                               stringutils::capitalize(member_name) + "()";
      llvm::outs() << new_init_expr_str << '\n';
      stringutils::replace_all(definition, init_expr_str, new_init_expr_str);
      llvm::outs() << definition << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VT->getSourceRange()),
                          definition);
    }
}
  
void MutatorFrontendAction_465::MutatorASTConsumer_465::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::varTemplateDecl().bind("VarTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}