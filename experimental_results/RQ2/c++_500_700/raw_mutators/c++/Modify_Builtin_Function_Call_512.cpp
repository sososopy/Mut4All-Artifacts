//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_builtin_function_call_512
 */ 
class MutatorFrontendAction_512 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(512)

private:
    class MutatorASTConsumer_512 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_512(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> variables;
        std::vector<const clang::Expr *> constants;
    };
};

//source file
#include "../include/modify_builtin_function_call_512.h"

// ========================================================================================================
#define MUT512_OUTPUT 1

void MutatorFrontendAction_512::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinCall")) {
      //Filter nodes in header files
      if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Call->getExprLoc()))
        return;

      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto &LangOpts = Result.Context->getLangOpts();
      std::string originalCall = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(Call->getSourceRange()), SM, LangOpts).str();

      // Select a parameter that is a variable or a constant
      if (Call->getNumArgs() < 1)
        return;

      const clang::Expr *selectedArg = nullptr;
      for (unsigned i = 0; i < Call->getNumArgs(); ++i) {
        if (auto *DRE = llvm::dyn_cast<clang::DeclRefExpr>(Call->getArg(i))) {
          if (auto *VD = llvm::dyn_cast<clang::VarDecl>(DRE->getDecl())) {
            selectedArg = Call->getArg(i);
            variables.push_back(VD);
            break;
          }
        } else if (auto *IL = llvm::dyn_cast<clang::IntegerLiteral>(Call->getArg(i))) {
          selectedArg = Call->getArg(i);
          constants.push_back(IL);
          break;
        }
      }

      if (!selectedArg)
        return;

      // Create a complex arithmetic expression
      std::string complexExpr = "(";
      if (!variables.empty()) {
        complexExpr += variables.back()->getNameAsString();
      }
      if (!constants.empty()) {
        if (!variables.empty()) complexExpr += " + ";
        complexExpr += std::to_string(constants.back()->getValue().getSExtValue());
      }
      complexExpr += " * 3 - 1.5)";

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCall = originalCall;
      size_t argPos = mutatedCall.find(clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(selectedArg->getSourceRange()), SM, LangOpts).str());
      if (argPos != std::string::npos) {
        mutatedCall.replace(argPos, clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(selectedArg->getSourceRange()), SM, LangOpts).str().length(), complexExpr);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Call->getSourceRange()), mutatedCall);
    }
}
  
void MutatorFrontendAction_512::MutatorASTConsumer_512::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_")))).bind("BuiltinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}