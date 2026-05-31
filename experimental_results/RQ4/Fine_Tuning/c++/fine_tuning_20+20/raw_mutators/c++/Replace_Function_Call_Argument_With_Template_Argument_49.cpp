//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Call_Argument_With_Template_Argument_49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
    class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FunctionDecl *target_func;
    };
};

//source file
#include "../include/replace_function_call_argument_with_template_argument_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      if (MT->getNumArgs() <= 1)
        return;
      auto callee = MT->getDirectCallee();
      if (!callee)
        return;
      if (callee->getTemplatedKind() !=
          FunctionDecl::TemplatedKind::TK_FunctionTemplate)
        return;
      target_func = callee;
      llvm::outs() << target_func->getNameAsString() << '\n';
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      auto args = MT->getTemplateArgs();
      if (args.size() == 0)
        return;
      int dice = getrandom::getRandomIndex(args.size() - 1);
      llvm::outs() << "dice: " << dice << '\n';
      auto replace_content =
          stringutils::rangetoStr(*(Result.SourceManager), args[dice]);
      llvm::outs() << replace_content << '\n';
      auto arg_contents = MT->getArgs();
      int arg_dice = getrandom::getRandomIndex(arg_contents.size() - 1);
      llvm::outs() << "arg_dice: " << arg_dice << '\n';
      auto arg_content =
          stringutils::rangetoStr(*(Result.SourceManager), arg_contents[arg_dice]->getSourceRange());
      llvm::outs() << arg_content << '\n';
      stringutils::strrpl(content, arg_content, replace_content);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}