//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Builtin_Va_Arg_With_Global_Va_List_330
 */ 
class MutatorFrontendAction_330 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(330)

private:
    class MutatorASTConsumer_330 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_330(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> global_va_lists;
    };
};

//source file
#include "../include/replace_builtin_va_arg_with_global_va_list_330.h"

// ========================================================================================================
#define MUT330_OUTPUT 1

void MutatorFrontendAction_330::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VL = Result.Nodes.getNodeAs<clang::VarDecl>("GlobalVaList")) {
      if (!VL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VL->getBeginLoc()))
        return;
      if (VL->getType().getAsString() != "va_list")
        return;
      global_va_lists.push_back(VL);
    } else if (auto *BA = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinVaArg")) {
      if (!BA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BA->getBeginLoc()))
        return;
      if (global_va_lists.empty()) {
        Rewrite.ReplaceText(BA->getBeginLoc(), 0, "/*mut330*/va_list g_default_global;\n");
        Rewrite.ReplaceText(BA->getBeginLoc(), 0, "g_default_global = va;");
        Rewrite.ReplaceText(BA->getArg(0)->getSourceRange(), "g_default_global");
      } else {
        size_t index = getrandom::getRandomIndex(global_va_lists.size() - 1);
        auto target = global_va_lists[index];
        Rewrite.ReplaceText(BA->getArg(0)->getSourceRange(),
                            target->getNameAsString());
      }
    }
}
  
void MutatorFrontendAction_330::MutatorASTConsumer_330::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto va_list_matcher = varDecl().bind("GlobalVaList");
    auto builtin_va_arg_matcher = callExpr(callee(builtinDecl())).bind("BuiltinVaArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(va_list_matcher, &callback);
    matchFinder.addMatcher(builtin_va_arg_matcher, &callback);
    matchFinder.matchAST(Context);
}