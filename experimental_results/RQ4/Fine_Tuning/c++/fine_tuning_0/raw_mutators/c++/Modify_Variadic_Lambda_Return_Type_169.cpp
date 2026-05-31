//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Variadic_Lambda_Return_Type_169
 */ 
class MutatorFrontendAction_169 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(169)

private:
    class MutatorASTConsumer_169 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_169(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Modify_Variadic_Lambda_Return_Type_169.h"

// ========================================================================================================
#define MUT169_OUTPUT 1

void MutatorFrontendAction_169::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << lambda_text << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (MT->hasExplicitTemplateParameters()) {
        if (MT->hasExplicitParameters()) {
          if (MT->hasExplicitResultType()) {
            auto tpl_pos = lambda_text.find("template");
            auto param_pos = lambda_text.find("(", tpl_pos);
            auto ret_pos = lambda_text.find("->", param_pos);
            auto lbrace_pos = lambda_text.find("{", ret_pos);
            auto tpl_str = lambda_text.substr(tpl_pos, param_pos - tpl_pos);
            auto param_str =
                lambda_text.substr(param_pos, ret_pos - param_pos);
            auto ret_str =
                lambda_text.substr(ret_pos, lbrace_pos - ret_pos);
            auto lbrace_str =
                lambda_text.substr(lbrace_pos, lambda_text.size());
            llvm::outs() << tpl_str << '\n';
            llvm::outs() << param_str << '\n';
            llvm::outs() << ret_str << '\n';
            llvm::outs() << lbrace_str << '\n';
            //Replace the original AST node with the mutated one
            auto new_ret_str =
                "->decltype(std::is_invocable_v<decltype(f),decltype(std::forward"
                "<Args>(args))...>)";
            auto new_lambda_text =
                tpl_str + param_str + new_ret_str + lbrace_str;
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(MT->getSourceRange()),
                new_lambda_text);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_169::MutatorASTConsumer_169::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}